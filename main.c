#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE   700

// 1) System headers
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// 2) Macros and constants
#define MAX_TITLE   50
#define MAX_TASK    100
#define LAST_OPTION 5

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

// 3) Types
/* 
 * Task data model required by the assignment:
 * - Exactly 3 attributes: id, title, status
 * - No dynamic allocation (fixed-size buffers)
 * - Field names match "ID, title, status" semantics
 */
typedef struct
{
    int  id;                   // Unique (enforced while reading)
    char title[MAX_TITLE];     // Task title
    int  status;               // Completion percentage [0..100]
} Task;

// 4) Function prototypes

int inputGetOption(void);
int inputGetId(int totalTasks);
int inputGetProgress(void);

void inputNewTask(Task tasks[], int* taskCount);
int  systemDeleteTask(Task tasks[], int* taskCount, int index1Based);
void systemEditTask(Task tasks[], int* taskCount);
void outputViewTasks(const Task tasks[], int taskCount);
void systemSortListOfTask(Task tasks[], int taskCount);
void systemSearchTask(const Task tasks[], int taskCount);
void systemResponse(int         choice,
                    Task        tasks[],
                    int*        taskCount);

void inputReadFile(const char* filePath, Task tasks[], int* taskCount);
void outputWriteFile(const char* filePath, const Task tasks[], int taskCount);

/* Internal helpers (static) */
static void flushLine(void);
static void stripRight(char* s);
static void
csvReadField(const char* line, size_t* pos, char* out, size_t outSize);
static int  parseProgressSafe(const char* s);
static int  isNumericId(const char* s);
static void csvWriteQuoted(FILE* fp, const char* s);
static int  fileExists(const char* path);
static void toAbsPath(const char* in, char* out, size_t outSize);
static void safeCopy(char* dst, size_t dstSize, const char* src);
static int  isIdUsed(const Task tasks[], int count, int id);
static int  nextFreePositiveId(const Task tasks[], int count, int startFrom);

// 5) Function implementations

int inputGetOption(void)
{
    int userOption;

    while (1)
    {
        printf("Your option: ");
        if (scanf("%d", &userOption) == 1 && userOption >= 0 &&
            userOption <= LAST_OPTION)
        {
            return userOption;
        }

        printf("Invalid option. Please try again!\n");
        flushLine();
    }
}

int inputGetId(int totalTasks)
{
    int taskId;

    while (1)
    {
        printf("Task ID: ");
        if (scanf("%d", &taskId) == 1 && taskId >= 1 && taskId <= totalTasks)
        {
            return taskId;
        }

        printf("Invalid ID. Please try again!\n");
        flushLine();
    }
}

int inputGetProgress(void)
{
    int taskProgress;

    while (1)
    {
        printf("Progress (0-100): ");
        if (scanf("%d", &taskProgress) == 1 && taskProgress >= 0 &&
            taskProgress <= 100)
        {
            return taskProgress;
        }

        printf("Invalid progress. Please try again!\n");
        flushLine();
    }
}

void inputNewTask(Task tasks[], int* taskCount)
{
    if (*taskCount == MAX_TASK)
    {
        printf("Task list is full!\n");
        return;
    }

    printf("Enter task title: ");
    flushLine();

    if (!fgets(tasks[*taskCount].title, MAX_TITLE, stdin))
    {
        fprintf(stderr, "Input error (title).\n");
        return;
    }

    tasks[*taskCount].title[strcspn(tasks[*taskCount].title, "\n")] = '\0';

    // Assign a unique ID (next free positive integer starting from count+1)
    tasks[*taskCount].id = nextFreePositiveId(tasks, *taskCount, *taskCount + 1);

    tasks[*taskCount].status = inputGetProgress();
    (*taskCount)++;

    printf("Task added successfully!\n");
}

int systemDeleteTask(Task tasks[], int* taskCount, int index1Based)
{
    int index0 = index1Based - 1;
    int i;

    if (*taskCount == 0)
    {
        printf("No task available to delete.\n");
        return 0;
    }

    if (index1Based < 1 || index1Based > *taskCount)
    {
        printf("Invalid ID!\n");
        return 0;
    }

    for (i = index0; i < *taskCount - 1; ++i)
    {
        tasks[i] = tasks[i + 1];
    }

    (*taskCount)--;
    return 1;
}

void systemEditTask(Task tasks[], int* taskCount)
{
    int  index0;
    char newTitle[MAX_TITLE];
    char buf[16];
    int  p;

    if (*taskCount == 0)
    {
        printf("No tasks available to edit.\n");
        return;
    }

    index0 = inputGetId(*taskCount) - 1;

    printf("Editing [%d] %s - %d%%\n",
       tasks[index0].id, tasks[index0].title, tasks[index0].status);

    // New title (Enter to skip)
    printf("New title (Enter to skip): ");
    flushLine();

    if (!fgets(newTitle, MAX_TITLE, stdin))
    {
        fprintf(stderr, "Input error (new title).\n");
        return;
    }

    if (newTitle[0] != '\n')
    {
        newTitle[strcspn(newTitle, "\n")] = '\0';
        safeCopy(tasks[index0].title, sizeof tasks[index0].title, newTitle);
        tasks[index0].title[MAX_TITLE - 1] = '\0';
    }

    // New status (Enter to skip)
    printf("New status (Enter to skip): ");
    if (!fgets(buf, sizeof buf, stdin))
    {
        fprintf(stderr, "Input error (status).\n");
        return;
    }

    if (buf[0] != '\n')
    {
        p = atoi(buf);
        if (p >= 0 && p <= 100)
        {
            tasks[index0].status = p;
        }
        else
        {
            printf("Invalid status, keeping previous value.\n");
        }
    }

    printf("Task updated successfully (in-memory)!\n");
}

void outputViewTasks(const Task tasks[], int taskCount)
{
    int i;

    if (taskCount == 0)
    {
        printf("No tasks available.\n");
        return;
    }

    for (i = 0; i < taskCount; i++)
    {
        printf("[%d]  \"%s\"  - Status: %d%%\n",
       tasks[i].id, tasks[i].title, tasks[i].status);

    }
}

void systemSortListOfTask(Task tasks[], int taskCount)
{
    int gap, i, j;

    if (taskCount == 0)
    {
        printf("No tasks available to sort.\n");
        return;
    }

    for (gap = taskCount / 2; gap > 0; gap /= 2)
    {
        for (i = gap; i < taskCount; i++)
        {
            Task tmp = tasks[i];
            j        = i;

            while (j >= gap && tasks[j - gap].status > tmp.status)
            {
                tasks[j] = tasks[j - gap];
                j -= gap;
            }

            tasks[j] = tmp;
        }
    }

    printf("Tasks sorted by status.\n");
    outputViewTasks(tasks, taskCount);
}

void systemSearchTask(const Task tasks[], int taskCount)
{
    char keyword[MAX_TITLE];
    int  found = 0;
    int  i;

    if (taskCount == 0)
    {
        printf("No tasks available to search.\n");
        return;
    }

    printf("Enter task title to search: ");
    flushLine();

    if (!fgets(keyword, MAX_TITLE, stdin))
    {
        fprintf(stderr, "Input error (keyword).\n");
        return;
    }

    keyword[strcspn(keyword, "\n")] = '\0';

    printf("Search results:\n");
    for (i = 0; i < taskCount; i++)
    {
        if (strstr(tasks[i].title, keyword) != NULL)
        {
            printf("[%d] %s\n", tasks[i].id, tasks[i].title);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No matching tasks found.\n");
    }
}

void systemResponse(int         choice,
                    Task        tasks[],
                    int*        taskCount)
{
    switch (choice)
    {
        case 1:
        {
            inputNewTask(tasks, taskCount);
            break;
        }
        case 2:
        {
            int deleteIdx = inputGetId(*taskCount);
            if (systemDeleteTask(tasks, taskCount, deleteIdx))
            {
                printf("Task deleted successfully!\n");
            }
            break;
        }
        case 3:
        {
            systemEditTask(tasks, taskCount);
            break;
        }
        case 4:
        {
            systemSortListOfTask(tasks, *taskCount);
            break;
        }
        case 5:
        {
            systemSearchTask(tasks, *taskCount);
            break;
        }
        case 0:
        {
            printf("You exit successfully!\n");
            fflush(stdout);
            exit(0);
        }
        default:
        {
            printf("Unknown option.\n");
            break;
        }
    }
}

// Helpers
static void stripRight(char* s)
{
    size_t n;

    if (!s)
    {
        return;
    }

    n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r' ||
                     isspace((unsigned char) s[n - 1])))
    {
        s[--n] = '\0';
    }
}

static void
csvReadField(const char* line, size_t* pos, char* out, size_t outSize)
{
    size_t i = *pos, o = 0;
    int    quoted = 0;

    if (!line || !out || outSize == 0)
    {
        return;
    }

    out[0] = '\0';

    if (line[i] == '"')
    {
        quoted = 1;
        i++;
    }

    while (line[i] != '\0')
    {
        char c = line[i];

        if (quoted)
        {
            if (c == '"')
            {
                if (line[i + 1] == '"')
                {
                    if (o + 1 < outSize)
                    {
                        out[o++] = '"';
                    }
                    i += 2;
                }
                else
                {
                    i++;
                    if (line[i] == ',')
                    {
                        i++;
                    }
                    break;
                }
            }
            else
            {
                if (o + 1 < outSize)
                {
                    out[o++] = c;
                }
                i++;
            }
        }
        else
        {
            if (c == ',')
            {
                i++;
                break;
            }
            if (o + 1 < outSize)
            {
                out[o++] = c;
            }
            i++;
        }
    }

    out[o] = '\0';
    *pos   = i;
}

static int parseProgressSafe(const char* s)
{
    long val = 0;

    if (!s)
    {
        return 0;
    }

    while (*s && !isdigit((unsigned char) *s))
    {
        s++;
    }

    while (*s && isdigit((unsigned char) *s))
    {
        val = val * 10 + (*s - '0');
        s++;
    }

    if (val < 0)  val = 0;
    if (val > 100) val = 100;

    return (int) val;
}

static int isNumericId(const char* s)
{
    int seen = 0;

    if (!s)
    {
        return 0;
    }

    if ((unsigned char) s[0] == 0xEF && (unsigned char) s[1] == 0xBB &&
        (unsigned char) s[2] == 0xBF)
    {
        s += 3;
    }

    while (*s && isspace((unsigned char) *s))
    {
        s++;
    }

    if (!*s)
    {
        return 0;
    }

    if (*s == '+')
    {
        s++;
    }

    while (*s && isdigit((unsigned char) *s))
    {
        seen = 1;
        s++;
    }

    while (*s && isspace((unsigned char) *s))
    {
        s++;
    }

    return seen && *s == '\0';
}

static void csvWriteQuoted(FILE* fp, const char* s)
{
    const char* p;

    fputc('"', fp);
    if (s)
    {
        for (p = s; *p; ++p)
        {
            if (*p == '"')
            {
                fputc('"', fp);
                fputc('"', fp);
            }
            else
            {
                fputc(*p, fp);
            }
        }
    }
    fputc('"', fp);
}

static void flushLine(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
    }
}

static int fileExists(const char* path)
{
    FILE* f = fopen(path, "r");
    if (f)
    {
        fclose(f);
        return 1;
    }
    return 0;
}

static void toAbsPath(const char* in, char* out, size_t outSize)
{
    if (!realpath(in, out))
    {
        safeCopy(out, outSize, in);
        out[outSize - 1] = '\0';
    }
}

static void safeCopy(char* dst, size_t dstSize, const char* src)
{
    size_t i = 0;

    if (!dst || dstSize == 0)
    {
        return;
    }

    if (!src)
    {
        dst[0] = '\0';
        return;
    }

    for (; i + 1 < dstSize && src[i] != '\0'; ++i)
    {
        dst[i] = src[i];
    }

    dst[i] = '\0';
}

static int isIdUsed(const Task tasks[], int count, int id)
{
    int i;
    for (i = 0; i < count; ++i)
    {
        if (tasks[i].id == id)
            return 1;
    }
    return 0;
}

static int nextFreePositiveId(const Task tasks[], int count, int startFrom)
{
    int candidate = (startFrom <= 0) ? 1 : startFrom;
    while (isIdUsed(tasks, count, candidate))
    {
        ++candidate;
    }
    return candidate;
}

// 6) File I/O

void inputReadFile(const char* filePath, Task tasks[], int* taskCount)
{
    FILE* fp;
    enum { MAX_LINE = 4096 };
    char line[MAX_LINE];

    if (!filePath || !tasks || !taskCount)
    {
        return;
    }

    *taskCount = 0;
    fp         = fopen(filePath, "r");
    if (!fp)
    {
        perror("Cannot open task file");
        return;
    }

    while (fgets(line, sizeof line, fp))
    {
        size_t pos                 = 0;
        char   idBuf[32]           = {0};
        char   titleBuf[MAX_TITLE] = {0};
        char   skipBuf[128]        = {0};
        char   statusBuf[32]       = {0};
        int rawId;
        int uniqueId;

        stripRight(line);
        if (line[0] == '\0')
        {
            continue;
        }

        // Parse: ID, Title, (maybe Detail), Status
        csvReadField(line, &pos, idBuf, sizeof idBuf);
        csvReadField(line, &pos, titleBuf, sizeof titleBuf);

        // detail giữa title và status (nếu có) → bỏ qua
        csvReadField(line, &pos, skipBuf, sizeof skipBuf);
        if (strchr(skipBuf, '%') || isdigit((unsigned char)skipBuf[0]))
        {
            safeCopy(statusBuf, sizeof statusBuf, skipBuf);
        }
        else
        {
            csvReadField(line, &pos, statusBuf, sizeof statusBuf);
        }

        // Bỏ qua header
        if (!isNumericId(idBuf))
        {
            continue;
        }

        if (*taskCount >= MAX_TASK)
        {
            break;
        }

        rawId = (idBuf[0] != '\0') ? atoi(idBuf) : (*taskCount + 1);
        if (rawId <= 0) rawId = 1;
        uniqueId = isIdUsed(tasks, *taskCount, rawId)
                     ? nextFreePositiveId(tasks, *taskCount, rawId + 1)
                     : rawId;
        tasks[*taskCount].id = uniqueId;

        safeCopy(tasks[*taskCount].title,
                 sizeof tasks[*taskCount].title,
                 titleBuf);
        tasks[*taskCount].title[MAX_TITLE - 1] = '\0';

        tasks[*taskCount].status = parseProgressSafe(statusBuf);

        (*taskCount)++;
    }

    fclose(fp);
}

/* 
 * Giữ nguyên outputWriteFile(...) như bạn muốn.
 * (Đã viết theo định dạng 6 cột; nếu bản của bạn khác vẫn dùng bản của bạn.)
 */
void outputWriteFile(const char* filePath, const Task tasks[], int taskCount)
{
    FILE* fp;
    int   i;
    char  absFile[PATH_MAX];

    if (!filePath || !tasks)
    {
        return;
    }

    fp = fopen(filePath, "w");
    if (!fp)
    {
        perror("Cannot open task file for write");
        return;
    }

    fputs("ID,Title,Detail,Status,Priority,Deadline\n", fp);
    fputs("\"Unique task identifier\",\"Brief task name\","
          "\"Task description\",\"Completion percentage\","
          "\"Priority level\",\"Task due date\"\n",
          fp);

    for (i = 0; i < taskCount && i < MAX_TASK; ++i)
    {
        int  id = tasks[i].id > 0 ? tasks[i].id : (i + 1);
        int  p  = tasks[i].status;
        char statusBuf[16];

        if (p < 0) p = 0;
        if (p > 100) p = 100;

        fprintf(fp, "%d,", id);
        csvWriteQuoted(fp, tasks[i].title); fputc(',', fp);
        csvWriteQuoted(fp, "");             fputc(',', fp);
        snprintf(statusBuf, sizeof statusBuf, "%d%%", p);
        fputs(statusBuf, fp);               fputc(',', fp);
        csvWriteQuoted(fp, "");             fputc(',', fp);
        csvWriteQuoted(fp, "");             fputc('\n', fp);
    }

    fclose(fp);

    toAbsPath(filePath, absFile, sizeof absFile);
    printf("Saved %d task(s) to: %s\n", taskCount, absFile);
}

// 7) main
int main(void)
{
    Task        tasks[MAX_TASK];
    const char* file      = "./docs/task.csv";
    int         taskCount = 0;
    char        absFile[PATH_MAX];

    if (!fileExists(file))
    {
        fprintf(stderr, "Data file not found: %s\n", file);
        fprintf(stderr, "Program will not create it automatically.\n");
        return 1;
    }

    inputReadFile(file, tasks, &taskCount);
    toAbsPath(file, absFile, sizeof absFile);
    printf("Loaded %d tasks from: %s\n", taskCount, absFile);

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
        systemResponse(userOption, tasks, &taskCount);
        /* Persist only when data is mutated by user:
           1: Add, 2: Delete, 3: Edit
           Search (5) & Sort (4) do NOT write to file.
        */
        if (userOption == 1 || userOption == 2 || userOption == 3) {
            outputWriteFile(file, tasks, taskCount);
        }
    }
}
