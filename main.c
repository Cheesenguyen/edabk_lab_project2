#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_TITLE 50
#define MAX_TASK  100
#define LAST_OPTION 5

#include <limits.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

/* Mỗi Task gói 3 thuộc tính với TÊN GIỮ NGUYÊN: id, list (tiêu đề), progress */
typedef struct {
    int  id;                   // ID
    char list[MAX_TITLE];      // Tiêu đề (giữ tên "list" như trong code cũ)
    int  progress;             // Mức độ hoàn thành
} Task;

/* Khai báo hàm */
/* ======== Function Declarations ======== */

/* Public API */
int  inputGetOption(void);
int  inputGetID(int totalTasks);
int  inputGetProgress(void);

void inputNewTask(Task tasks[], int *taskCount);
int  systemDeleteTask(Task tasks[], int *taskCount, int index1based);
void systemEditTask(const char *filePath, Task tasks[], int *taskCount);
void outputViewTasks(const Task tasks[], int taskCount);
void systemSortListOfTask(Task tasks[], int taskCount);
void systemSearchTask(const Task tasks[], int taskCount);
void systemResponse(int choice, Task tasks[], int *taskCount, const char* filePath);

void inputReadFile(const char *filePath, Task tasks[], int *taskCount);
void outputWriteFile(const char *filePath, const Task tasks[], int taskCount);
static int  file_exists(const char *path);
static void ensure_parent_dir(const char *filePath);
static void to_abs_path(const char *in, char *out, size_t outsz);
static void safe_copy(char *dst, size_t dstsz, const char *src);

/* Internal helpers (static) */
static void flush_line(void);  /* <-- cần có vì được gọi trước khi định nghĩa */
static void rstrip(char *s);
static void csv_read_field(const char *line, size_t *pos, char *out, size_t outsz);
static int  parse_progress_safe(const char *s);
static int  is_numeric_id(const char *s);
static void csv_write_quoted(FILE *fp, const char *s);

/* ---- MAIN ---- */
int main(void)
{
    Task tasks[MAX_TASK];
    const char *file = "./docs/task.csv";
    int taskCount = 0;

    /* 1) Nếu chưa có file -> tạo mới file trống với 2 dòng mô tả */
    int created = 0;
    if (!file_exists(file)) {
        ensure_parent_dir(file);          /* bảo đảm có thư mục docs/ */
        outputWriteFile(file, tasks, 0);  /* ghi header 2 dòng */
        created = 1;
    }

    /* 2) Đọc dữ liệu */
    inputReadFile(file, tasks, &taskCount);

    /* 3) Log đường dẫn tuyệt đối + trạng thái */
    char absfile[PATH_MAX];
    to_abs_path(file, absfile, sizeof(absfile));
    if (created) {
        printf("Created new data file: %s\n", absfile);
    } else {
        printf("Loaded %d tasks from: %s\n", taskCount, absfile);
    }

    /* 4) Vòng lặp menu */
    while (1) {
        int userOption;
        printf("\nTask Manager\n");
        printf("1. Add Task\n");
        printf("2. Delete Task\n");
        printf("3. Edit Task\n");
        printf("4. Sort List of Tasks\n");
        printf("5. Search Task\n");
        printf("0. Exit\n");

        userOption = inputGetOption();
        systemResponse(userOption, tasks, &taskCount, file);
        outputWriteFile(file, tasks, taskCount);
    }
}


/* ---- INPUT ---- */
int inputGetOption(void)
{
    int userOption;
    while (1)
    {
        printf("Your option: ");
        if (scanf("%d", &userOption) == 1 && userOption >= 0 && userOption <= LAST_OPTION)
            return userOption;
        printf("Invalid option. Please try again!\n");
        flush_line();
    }
}

int inputGetID(int totalTasks)
{
    int taskIdx;
    while (1)
    {
        printf("Task ID: ");
        if (scanf("%d", &taskIdx) == 1 && taskIdx >= 1 && taskIdx <= totalTasks)
            return taskIdx;
        printf("Invalid ID. Please try again!\n");
        flush_line();
    }
}

int inputGetProgress(void)
{
    int taskProgress;
    while (1)
    {
        printf("Progress (1-100): ");
        if (scanf("%d", &taskProgress) == 1 && taskProgress >= 0 && taskProgress <= 100)
            return taskProgress;
        printf("Invalid progress. Please try again!\n");
        flush_line();
    }
}

/* ---- FEATURE: ADD ---- */
void inputNewTask(Task tasks[], int *taskCount)
{
    if (*taskCount == MAX_TASK)
    {
        printf("Task list is full!\n");
        return;
    }

    printf("Enter task title: ");
    flush_line();
    if (!fgets(tasks[*taskCount].list, MAX_TITLE, stdin)) {
    fprintf(stderr, "Input error (title).\n");
    return;  // hoặc xử lý phù hợp luồng của bạn
    }
    tasks[*taskCount].list[strcspn(tasks[*taskCount].list, "\n")] = '\0';

    tasks[*taskCount].id       = *taskCount + 1;     /* giữ cách đánh số như code cũ */
    tasks[*taskCount].progress = inputGetProgress();
    (*taskCount)++;

    printf("Task added successfully!\n");
}

/* ---- FEATURE: DELETE ---- */
int systemDeleteTask(Task tasks[], int *taskCount, int index1based)
{
    if (*taskCount == 0)
    {
        printf("No task available to delete.\n");
        return 0;
    }
    if (index1based < 1 || index1based > *taskCount)
    {
        printf("Invalid ID!\n");
        return 0;
    }

    int idx = index1based - 1;
    for (int i = idx; i < *taskCount - 1; ++i)
    {
        tasks[i] = tasks[i + 1]; /* dịch cả id/list/progress cùng nhau */
    }
    (*taskCount)--;
    return 0;
}

/* ---- FEATURE: EDIT ---- */
void systemEditTask(const char *filePath, Task tasks[], int *taskCount)
{
    /* nạp lại dữ liệu từ file (giữ nguyên hành vi cũ) */
    inputReadFile(filePath, tasks, taskCount);
    if (*taskCount == 0)
    {
        printf("No tasks available to edit.\n");
        return;
    }

    int idx = inputGetID(*taskCount) - 1;
    char newTitle[MAX_TITLE];
    char buf[16];

    printf("Editing [%d] %s - %d%%\n",
           idx + 1, tasks[idx].list, tasks[idx].progress);

    /* nhập tiêu đề mới (Enter = giữ nguyên) */
    printf("New title (Enter to skip): ");
    flush_line(); /* flush */
    if (!fgets(newTitle, MAX_TITLE, stdin)) {
    fprintf(stderr, "Input error (new title).\n");
    return;
    }
    if (newTitle[0] != '\n')
    {
        newTitle[strcspn(newTitle, "\n")] = '\0';
        safe_copy(tasks[idx].list, sizeof tasks[idx].list, newTitle);
        tasks[idx].list[MAX_TITLE - 1] = '\0';
    }

    /* nhập tiến độ mới (Enter = giữ nguyên) */
    printf("New progress (Enter to skip): ");
    if (!fgets(buf, sizeof(buf), stdin)) {
    fprintf(stderr, "Input error (progress).\n");
    return;
    }
    if (buf[0] != '\n')
    {
        int p = atoi(buf);
        if (p >= 1 && p <= 100) tasks[idx].progress = p;
        else printf("Invalid progress, keeping previous value.\n");
    }

    outputWriteFile(filePath, tasks, *taskCount);
    printf("Task updated successfully!\n");
}

/* ---- FEATURE: VIEW ---- */
void outputViewTasks(const Task tasks[], int taskCount)
{
    if (taskCount == 0)
    {
        printf("No tasks available.\n");
        return;
    }
    for (int i = 0; i < taskCount; i++)
    {
        printf("[%d] %s - Progress: %d%%\n",
               i + 1, tasks[i].list, tasks[i].progress);
    }
}

/* ---- FEATURE: SORT (by progress asc) ---- */
void systemSortListOfTask(Task tasks[], int taskCount)
{
    if (taskCount == 0)
    {
        printf("No tasks available to sort.\n");
        return;
    }

    /* Shell sort theo progress tăng dần */
    for (int gap = taskCount / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < taskCount; i++)
        {
            Task tmp = tasks[i];
            int j = i;
            while (j >= gap && tasks[j - gap].progress > tmp.progress)
            {
                tasks[j] = tasks[j - gap];
                j -= gap;
            }
            tasks[j] = tmp;
        }
    }
    printf("Tasks sorted by progress.\n");
    outputViewTasks(tasks, taskCount);
}

/* ---- FEATURE: SEARCH (by title substring) ---- */
void systemSearchTask(const Task tasks[], int taskCount)
{
    char keyword[MAX_TITLE];
    int found = 0;
    if (taskCount == 0)
    {
        printf("No tasks available to search.\n");
        return;
    }

    printf("Enter task title to search: ");
    flush_line(); /* flush */
    if (!fgets(keyword, MAX_TITLE, stdin)) {
    fprintf(stderr, "Input error (keyword).\n");
    return;
    }
    keyword[strcspn(keyword, "\n")] = '\0';

    printf("Search results:\n");
    for (int i = 0; i < taskCount; i++)
    {
        if (strstr(tasks[i].list, keyword) != NULL)
        {
            printf("[%d] %s\n", i + 1, tasks[i].list);
            found = 1;
        }
    }
    if (!found) printf("No matching tasks found.\n");
}

/* ---- CONTROLLER ---- */
void systemResponse(int choice, Task tasks[], int *taskCount, const char* filePath)
{
    switch (choice)
    {
        case 1:
            inputNewTask(tasks, taskCount);
            break;
        case 2:
        {
            int delete_idx = inputGetID(*taskCount);
            if (systemDeleteTask(tasks, taskCount, delete_idx))
                printf("Task deleted successfully!\n");
            break;
        }
        case 3:
            systemEditTask(filePath, tasks, taskCount);
            break;
        case 4:
            systemSortListOfTask(tasks, *taskCount);
            break;
        case 5:
            systemSearchTask(tasks, *taskCount);
            break;
        case 0:
            printf("You exit successfully!\n");
            fflush(stdout);
            exit(0);
    }
}

/* Cắt ký tự xuống dòng và khoảng trắng đuôi */
static void rstrip(char *s) {
    if (!s) return;
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r' || isspace((unsigned char)s[n-1]))) {
        s[--n] = '\0';
    }
}

/* Đọc 1 field CSV (hỗ trợ dấu ngoặc kép & dấu phẩy trong dữ liệu), không cấp phát động */
static void csv_read_field(const char *line, size_t *pos, char *out, size_t outsz) {
    size_t i = *pos, o = 0;
    int quoted = 0;

    if (!line || !out || outsz == 0) return;
    out[0] = '\0';

    if (line[i] == '"') { quoted = 1; i++; }

    while (line[i] != '\0') {
        char c = line[i];

        if (quoted) {
            if (c == '"') {
                if (line[i+1] == '"') {                 /* "" -> một dấu " trong dữ liệu */
                    if (o + 1 < outsz) out[o++] = '"';
                    i += 2;
                } else {                                 /* kết thúc field */
                    i++;
                    if (line[i] == ',') i++;             /* bỏ dấu phẩy nếu có */
                    break;
                }
            } else {
                if (o + 1 < outsz) out[o++] = c;
                i++;
            }
        } else {
            if (c == ',') { i++; break; }               /* hết field thường */
            if (o + 1 < outsz) out[o++] = c;
            i++;
        }
    }

    out[o] = '\0';
    *pos = i;
}

/* Lấy số tiến độ từ chuỗi status; chịu các format "75", "75%", "  75  % " */
static int parse_progress_safe(const char *s) {
    if (!s) return 0;
    while (*s && !isdigit((unsigned char)*s)) s++;
    long val = 0;
    while (*s && isdigit((unsigned char)*s)) {
        val = val * 10 + (*s - '0');
        s++;
    }
    if (val < 0) val = 0;
    if (val > 100) val = 100;
    return (int)val;
}

/* Kiểm tra ID dạng số (bỏ BOM, khoảng trắng đầu & cuối, cho phép dấu +) */
static int is_numeric_id(const char *s) {
    if (!s) return 0;
    /* Bỏ BOM UTF-8 nếu có */
    if ((unsigned char)s[0] == 0xEF && (unsigned char)s[1] == 0xBB && (unsigned char)s[2] == 0xBF) s += 3;
    while (*s && isspace((unsigned char)*s)) s++;
    if (!*s) return 0;
    if (*s == '+') s++;
    int seen = 0;
    while (*s && isdigit((unsigned char)*s)) { seen = 1; s++; }
    while (*s && isspace((unsigned char)*s)) s++;  /* mới thêm: bỏ khoảng trắng cuối */
    return seen && *s == '\0';
}
/* Ghi 1 field CSV luôn đặt trong dấu ngoặc kép, escape " thành "" */
static void csv_write_quoted(FILE *fp, const char *s) {
    fputc('"', fp);
    if (s) {
        const char *p = s;
        while (*p) {
            if (*p == '"') { fputc('"', fp); fputc('"', fp); }  /* "" */
            else            fputc(*p, fp);
            p++;
        }
    }
    fputc('"', fp);
}
/* Flush tới hết dòng hoặc EOF (an toàn hơn while(getchar()!='\n')) */
static void flush_line(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { /* no-op */ }
}
static int file_exists(const char *path) {
    FILE *f = fopen(path, "r");
    if (f) { fclose(f); return 1; }
    return 0;
}

static void ensure_parent_dir(const char *filePath) {
    char buf[PATH_MAX];
    safe_copy(buf, sizeof buf, filePath);
    buf[sizeof(buf)-1] = '\0';
    char *slash = strrchr(buf, '/');
    if (slash) {
        *slash = '\0';
        mkdir(buf, 0755); // idempotent: nếu đã tồn tại sẽ fail nhẹ, không sao
    }
}

static void to_abs_path(const char *in, char *out, size_t outsz) {
    if (!realpath(in, out)) {
        // realpath thất bại khi file chưa tồn tại → ghi lại đường dẫn gốc
        safe_copy(out, outsz, in);
        out[outsz-1] = '\0';
    }
}
static void safe_copy(char *dst, size_t dstsz, const char *src) {
    if (!dst || dstsz == 0) return;
    if (!src) { dst[0] = '\0'; return; }
     size_t i = 0;
    /* copy tối đa dstsz-1 ký tự, dừng khi gặp '\0' */
    for (; i + 1 < dstsz && src[i] != '\0'; ++i) {
        dst[i] = src[i];
    }
    dst[i] = '\0';
}

/* ---- FILE I/O ---- */

/* Đọc task từ file CSV vào mảng tasks (không cấp phát động, chịu ô trống & lệch cột) */
void inputReadFile(const char *filePath, Task tasks[], int *taskCount)
{
    if (!filePath || !tasks || !taskCount) return;

    *taskCount = 0;
    FILE *fp = fopen(filePath, "r");
    if (!fp) {
        perror("Cannot open task file");
        return;
    }

    enum { MAX_LINE = 4096 };
    char line[MAX_LINE];
    //int line_no = 0;

    while (fgets(line, sizeof(line), fp)) {
        //line_no++;
        rstrip(line);
        if (line[0] == '\0') continue;                  /* bỏ dòng rỗng */

        /* Tách 4 field đầu: ID, Title, Detail, Status (các cột sau có hay không cũng không sao) */
        size_t pos = 0;
        char idbuf[32] = {0};
        char titlebuf[MAX_TITLE] = {0};
        char tmpbuf[128] = {0};                          /* để bỏ qua Detail */
        char statusbuf[32] = {0};

        csv_read_field(line, &pos, idbuf, sizeof(idbuf));
        csv_read_field(line, &pos, titlebuf, sizeof(titlebuf));
        csv_read_field(line, &pos, tmpbuf, sizeof(tmpbuf));       /* Detail - có thể rỗng */
        csv_read_field(line, &pos, statusbuf, sizeof(statusbuf)); /* Status - có thể rỗng */

        /* BỎ HEADER: nếu field đầu không phải số -> coi là header/miêu tả, bỏ qua */
        if (!is_numeric_id(idbuf)) {
            continue;
        }

        /* Không tràn mảng tĩnh */
        if (*taskCount >= MAX_TASK) {
            break;
        }

        /* ID: nếu rỗng/không hợp lệ -> gán tuần tự; nếu có -> dùng số trong file */
        int id = (idbuf[0] != '\0') ? atoi(idbuf) : (*taskCount + 1);
        if (id <= 0) id = *taskCount + 1;

        /* Title: có thể rỗng -> để "" (an toàn) */
        /* Progress: chịu "x%", " x  %" hoặc rỗng -> mặc định 0..100 */
        int progress = parse_progress_safe(statusbuf);

        /* Ghi vào mảng tĩnh */
        tasks[*taskCount].id = id;
        safe_copy(tasks[*taskCount].list, sizeof tasks[*taskCount].list, titlebuf);
        tasks[*taskCount].list[MAX_TITLE - 1] = '\0';
        tasks[*taskCount].progress = progress;

        (*taskCount)++;
    }

    fclose(fp);
    /* Không re-index ID để giữ đúng ID từ file; các thao tác UI vẫn dùng index 1..N như trước */
}


/* Ghi tasks ra CSV đầy đủ 6 cột: ID,Title,Detail,Status,Priority,Deadline */
void outputWriteFile(const char *filePath, const Task tasks[], int taskCount)
{
    FILE *fp;
    int i;

    if (!filePath || !tasks) return;

    fp = fopen(filePath, "w");
    if (!fp) {
        perror("Cannot open task file for write");
        return;
    }

    /* Header 2 dòng (đọc file sẽ tự bỏ qua vì cột 1 không phải số) */
    fputs("ID,Title,Detail,Status,Priority,Deadline\n", fp);
    fputs("\"Unique task identifier\",\"Brief task name\",\"Task description\",\"Completion percentage\",\"Priority level\",\"Task due date\"\n", fp);

    /* Ghi từng dòng. Luôn đủ 6 trường, kể cả trống */
    for (i = 0; i < taskCount && i < MAX_TASK; ++i) {
        int id = tasks[i].id > 0 ? tasks[i].id : (i + 1);
        int p  = tasks[i].progress;
        char statusbuf[16];

        if (p < 0)   p = 0;
        if (p > 100) p = 100;

        /* ID (không quote) */
        fprintf(fp, "%d,", id);

        /* Title */
        csv_write_quoted(fp, tasks[i].list);
        fputc(',', fp);

        /* Detail: hiện không quản lý -> để trống nhưng vẫn có cột */
        csv_write_quoted(fp, "");
        fputc(',', fp);

        /* Status: dạng N% (không quote để giữ kiểu trước đây; parser vẫn đọc OK) */
        snprintf(statusbuf, sizeof(statusbuf), "%d%%", p);
        fputs(statusbuf, fp);
        fputc(',', fp);

        /* Priority (trống) */
        csv_write_quoted(fp, "");
        fputc(',', fp);

        /* Deadline (trống) */
        csv_write_quoted(fp, "");
        fputc('\n', fp);
    }

    fclose(fp);
}
