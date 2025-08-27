#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 50
#define MAX_TASK  10
#define LAST_OPTION 5

/* Mỗi Task gói 3 thuộc tính với TÊN GIỮ NGUYÊN: id, list (tiêu đề), progress */
typedef struct {
    int  id;                   // ID
    char list[MAX_TITLE];      // Tiêu đề (giữ tên "list" như trong code cũ)
    int  progress;             // Mức độ hoàn thành
} Task;

/* Khai báo hàm */
int  inputGetOption(void);
int  inputGetID(int listLength);
int  inputGetProgress(void);

void inputNewTask(Task tasks[], int *taskCount);
int  systemDeleteTask(Task tasks[], int *taskCount, int index1based);
void systemEditTask(const char *filePath, Task tasks[], int *taskCount);
void outputViewTasks(const Task tasks[], int taskCount);
void systemSortListOfTask(Task tasks[], int taskCount);
void systemSearchTask(const Task tasks[], int taskCount);
void systemResponse(int choice, Task tasks[], int *taskCount, const char* filePath);

void inputReadFile(const char* filePath, Task tasks[], int *taskCount);
void outputWriteFile(const char *filePath, const Task tasks[], int taskCount);

/* ---- MAIN ---- */
int main(void)
{
    Task tasks[MAX_TASK];
    const char* file = "./data/task.csv";
    int taskCount = 0;

    inputReadFile(file, tasks, &taskCount);
    printf("Loaded %d tasks from file.\n", taskCount);

    while (1)
    {
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
        while (getchar() != '\n');
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
        while (getchar() != '\n');
    }
}

int inputGetProgress(void)
{
    int taskProgress;
    while (1)
    {
        printf("Progress (1-100): ");
        if (scanf("%d", &taskProgress) == 1 && taskProgress >= 1 && taskProgress <= 100)
            return taskProgress;
        printf("Invalid progress. Please try again!\n");
        while (getchar() != '\n');
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
    while (getchar() != '\n'); /* flush */
    fgets(tasks[*taskCount].list, MAX_TITLE, stdin);
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

    /* Cập nhật lại id tuần tự (giống hành vi cũ: id = vị trí + 1) */
    for (int i = 0; i < *taskCount; ++i)
        tasks[i].id = i + 1;

    return 1;
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
    while (getchar() != '\n'); /* flush */
    fgets(newTitle, MAX_TITLE, stdin);
    if (newTitle[0] != '\n')
    {
        newTitle[strcspn(newTitle, "\n")] = '\0';
        strncpy(tasks[idx].list, newTitle, MAX_TITLE - 1);
        tasks[idx].list[MAX_TITLE - 1] = '\0';
    }

    /* nhập tiến độ mới (Enter = giữ nguyên) */
    printf("New progress (Enter to skip): ");
    fgets(buf, sizeof(buf), stdin);
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

    /* Sau khi sắp xếp, cập nhật lại id tuần tự giống hành vi cũ */
    for (int i = 0; i < taskCount; ++i)
        tasks[i].id = i + 1;

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
    while (getchar() != '\n'); /* flush */
    fgets(keyword, MAX_TITLE, stdin);
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
    int line_no = 0;

    while (fgets(line, sizeof(line), fp)) {
        line_no++;
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
        strncpy(tasks[*taskCount].list, titlebuf, MAX_TITLE - 1);
        tasks[*taskCount].list[MAX_TITLE - 1] = '\0';
        tasks[*taskCount].progress = progress;

        (*taskCount)++;
    }

    fclose(fp);
    /* Không re-index ID để giữ đúng ID từ file; các thao tác UI vẫn dùng index 1..N như trước */
}


void outputWriteFile(const char *filePath, const Task tasks[], int taskCount)
{
    if (!filePath || !tasks) return;

    FILE *fp = fopen(filePath, "w");
    if (!fp)
    {
        perror("Cannot open task file");
        return;
    }

    /* Header 2 dòng như cũ */
    fprintf(fp, "ID,Title,Detail,Status,Priority,Deadline\n");
    fprintf(fp, "\"Unique task identifier\",\"Brief task name\",\"Task description\",\"Completion percentage\",\"Priority level\",\"Task due date\"\n");

    for (int i = 0; i < taskCount && i < MAX_TASK; ++i)
    {
        fprintf(fp, "%d,%s,Description,%d%%,3,01/01/2025\n",
                tasks[i].id,
                tasks[i].list,
                tasks[i].progress);
    }

    fclose(fp);
}
