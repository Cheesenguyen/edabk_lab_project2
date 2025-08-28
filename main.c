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
typedef struct
{
    int  id;
    char list[MAX_TITLE];
    int  progress; // Completion percentage
} Task;

// 4) Function prototypes

int inputGetOption(void);
int inputGetId(int totalTasks);
int inputGetProgress(void);

void inputNewTask(Task tasks[], int* taskCount);
int  systemDeleteTask(Task tasks[], int* taskCount, int index1Based);
void systemEditTask(const char* filePath, Task tasks[], int* taskCount);
void outputViewTasks(const Task tasks[], int taskCount);
void systemSortListOfTask(Task tasks[], int taskCount);
void systemSearchTask(const Task tasks[], int taskCount);
void systemResponse(int         choice,
                    Task        tasks[],
                    int*        taskCount,
                    const char* filePath);

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
static void ensureParentDir(const char* filePath);
static void toAbsPath(const char* in, char* out, size_t outSize);
static void safeCopy(char* dst, size_t dstSize, const char* src);

// 5) Function implementations

/*
 + inputGetOption - Read a menu option from the user.
 + Returns: integer in range [0, LAST_OPTION].
 */
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

/*
 + inputGetId - Read a task ID (1-based) from the user.
 + totalTasks: number of tasks currently available.
 * Returns: integer in range [1, totalTasks].
 */
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

/*
 + inputGetProgress - Read a progress percentage from the user.
 + Returns: integer in range [0, 100].
 */
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

/**
 * inputNewTask - Append a new task to the array.
 * @tasks:      array of Task.
 * @taskCount:  in/out count of tasks.
 */
void inputNewTask(Task tasks[], int* taskCount)
{
    if (*taskCount == MAX_TASK)
    {
        printf("Task list is full!\n");
        return;
    }

    printf("Enter task title: ");
    flushLine();

    if (!fgets(tasks[*taskCount].list, MAX_TITLE, stdin))
    {
        fprintf(stderr, "Input error (title).\n");
        return;
    }

    tasks[*taskCount].list[strcspn(tasks[*taskCount].list, "\n")] = '\0';

    tasks[*taskCount].id       = *taskCount + 1; // Keep legacy numbering
    tasks[*taskCount].progress = inputGetProgress();
    (*taskCount)++;

    printf("Task added successfully!\n");
}

/**
 * systemDeleteTask - Delete a task by 1-based index.
 * Returns: 1 on success, 0 on failure.
 */
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

/**
 * systemEditTask - Edit a task (title and/or progress).
 * Reads the file first to keep previous behavior.
 */
void systemEditTask(const char* filePath, Task tasks[], int* taskCount)
{
    int  index0;
    char newTitle[MAX_TITLE];
    char buf[16];
    int  p;

    inputReadFile(filePath, tasks, taskCount);
    if (*taskCount == 0)
    {
        printf("No tasks available to edit.\n");
        return;
    }

    index0 = inputGetId(*taskCount) - 1;

    printf("Editing [%d] %s - %d%%\n",
           index0 + 1,
           tasks[index0].list,
           tasks[index0].progress);

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
        safeCopy(tasks[index0].list, sizeof tasks[index0].list, newTitle);
        tasks[index0].list[MAX_TITLE - 1] = '\0';
    }

    // New progress (Enter to skip): accept N or N%
    printf("New progress (Enter to skip): ");
    if (!fgets(buf, sizeof buf, stdin))
    {
        fprintf(stderr, "Input error (progress).\n");
        return;
    }

    if (buf[0] != '\n')
    {
        p = atoi(buf);
        if (p >= 0 && p <= 100)
        {
            tasks[index0].progress = p;
        }
        else
        {
            printf("Invalid progress, keeping previous value.\n");
        }
    }

    outputWriteFile(filePath, tasks, *taskCount);
    printf("Task updated successfully!\n");
}

/**
 * outputViewTasks - Print all tasks.
 */
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
        printf("[%d] %s - Progress: %d%%\n",
               i + 1,
               tasks[i].list,
               tasks[i].progress);
    }
}

/**
 * systemSortListOfTask - Sort tasks by progress (ascending) using Shell sort.
 */
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

/**
 * systemSearchTask - Search tasks by title substring.
 */
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
        if (strstr(tasks[i].list, keyword) != NULL)
        {
            printf("[%d] %s\n", i + 1, tasks[i].list);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No matching tasks found.\n");
    }
}

/**
 * systemResponse - Dispatch a menu choice.
 */
void systemResponse(int         choice,
                    Task        tasks[],
                    int*        taskCount,
                    const char* filePath)
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
            systemEditTask(filePath, tasks, taskCount);
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

/* ----- Helpers ----- */

/**
 * stripRight - Trim trailing newline/CR/space characters (in place).
 */
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

/**
 * csvReadField - Read one CSV field respecting quotes and commas.
 * No dynamic allocation, safe for fixed-size outputs.
 */
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
                    /* "" -> a single " in data */
                    if (o + 1 < outSize)
                    {
                        out[o++] = '"';
                    }
                    i += 2;
                }
                else
                {
                    /* end field */
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

/**
 * parseProgressSafe - Parse "N" or "N%" into [0..100].
 */
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

    if (val < 0)
    {
        val = 0;
    }
    if (val > 100)
    {
        val = 100;
    }

    return (int) val;
}

/**
 * isNumericId - Check if a string is a pure unsigned integer (ignoring BOM
 * and surrounding whitespace). Allows optional leading '+'.
 */
static int isNumericId(const char* s)
{
    int seen = 0;

    if (!s)
    {
        return 0;
    }

    /* Skip UTF-8 BOM if present */
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

/**
 * csvWriteQuoted - Write a CSV field enclosed in quotes.
 * Internal quotes are doubled per RFC 4180.
 */
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

/**
 * flushLine - Discard all characters up to newline/EOF.
 */
static void flushLine(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        /* no-op */
    }
}

/**
 * fileExists - Return 1 if file at path can be opened for reading.
 */
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

/**
 * ensureParentDir - Create parent directory (idempotent).
 */
static void ensureParentDir(const char* filePath)
{
    char  buf[PATH_MAX];
    char* slash;

    safeCopy(buf, sizeof buf, filePath);
    buf[sizeof buf - 1] = '\0';

    slash = strrchr(buf, '/');
    if (slash)
    {
        *slash = '\0';
        /* If already exists, mkdir will fail harmlessly. */
        mkdir(buf, 0755);
    }
}

/**
 * toAbsPath - Resolve absolute path if possible; otherwise copy input.
 */
static void toAbsPath(const char* in, char* out, size_t outSize)
{
    if (!realpath(in, out))
    {
        /* realpath fails when file doesn't yet exist */
        safeCopy(out, outSize, in);
        out[outSize - 1] = '\0';
    }
}

/**
 * safeCopy - Copy at most dstSize-1 chars and NUL-terminate.
 */
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

/* 7) File I/O */

/**
 * inputReadFile - Load tasks from a CSV file into a fixed-size array.
 * Skips headers (non-numeric first field) and tolerates empty fields.
 */
void inputReadFile(const char* filePath, Task tasks[], int* taskCount)
{
    FILE* fp;
    enum
    {
        MAX_LINE = 4096
    };
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
        char   tmpBuf[128]         = {0}; /* Skip "Detail" */
        char   statusBuf[32]       = {0};

        stripRight(line);
        if (line[0] == '\0')
        {
            continue;
        }

        /* Parse first 4 fields: ID, Title, Detail, Status */
        csvReadField(line, &pos, idBuf, sizeof idBuf);
        csvReadField(line, &pos, titleBuf, sizeof titleBuf);
        csvReadField(line, &pos, tmpBuf, sizeof tmpBuf);       /* Detail */
        csvReadField(line, &pos, statusBuf, sizeof statusBuf); /* Status */

        /* Skip header lines where first field is not numeric */
        if (!isNumericId(idBuf))
        {
            continue;
        }

        if (*taskCount >= MAX_TASK)
        {
            break;
        }

        /* ID from file or auto if invalid */
        tasks[*taskCount].id =
           (idBuf[0] != '\0') ? atoi(idBuf) : (*taskCount + 1);

        /* Title */
        safeCopy(tasks[*taskCount].list,
                 sizeof tasks[*taskCount].list,
                 titleBuf);
        tasks[*taskCount].list[MAX_TITLE - 1] = '\0';

        /* Progress accepts "x%", " x  %", etc. */
        tasks[*taskCount].progress = parseProgressSafe(statusBuf);

        (*taskCount)++;
    }

    fclose(fp);
    /* Keep IDs from file; UI uses 1..N indices separately. */
}

/**
 * outputWriteFile - Write tasks to CSV with 6 columns:
 * ID,Title,Detail,Status,Priority,Deadline
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

    /* Header 2 lines (parser skips them because first field isn't numeric) */
    fputs("ID,Title,Detail,Status,Priority,Deadline\n", fp);
    fputs("\"Unique task identifier\",\"Brief task name\","
          "\"Task description\",\"Completion percentage\","
          "\"Priority level\",\"Task due date\"\n",
          fp);

    for (i = 0; i < taskCount && i < MAX_TASK; ++i)
    {
        int  id = tasks[i].id > 0 ? tasks[i].id : (i + 1);
        int  p  = tasks[i].progress;
        char statusBuf[16];

        if (p < 0)
            p = 0;
        if (p > 100)
            p = 100;

        /* ID (unquoted) */
        fprintf(fp, "%d,", id);

        /* Title */
        csvWriteQuoted(fp, tasks[i].list);
        fputc(',', fp);

        /* Detail: currently unmanaged -> empty but keep column */
        csvWriteQuoted(fp, "");
        fputc(',', fp);

        /* Status: as "N%" (unquoted; parser accepts it) */
        snprintf(statusBuf, sizeof statusBuf, "%d%%", p);
        fputs(statusBuf, fp);
        fputc(',', fp);

        /* Priority (empty) */
        csvWriteQuoted(fp, "");
        fputc(',', fp);

        /* Deadline (empty) */
        csvWriteQuoted(fp, "");
        fputc('\n', fp);
    }

    fclose(fp);

    toAbsPath(filePath, absFile, sizeof absFile);
    printf("Saved %d task(s) to: %s\n", taskCount, absFile);
}

/* 8) main - placed last per file structure rules. */
int main(void)
{
    Task        tasks[MAX_TASK];
    const char* file      = "./docs/task.csv";
    int         taskCount = 0;
    char        absFile[PATH_MAX];
    int         created = 0;

    /* If file doesn't exist -> create with 2-line header */
    if (!fileExists(file))
    {
        ensureParentDir(file);
        outputWriteFile(file, tasks, 0);
        created = 1;
    }

    /* Load data */
    inputReadFile(file, tasks, &taskCount);
    toAbsPath(file, absFile, sizeof absFile);

    if (created)
    {
        printf("Created new data file: %s\n", absFile);
    }
    else
    {
        printf("Loaded %d tasks from: %s\n", taskCount, absFile);
    }

    /* Menu loop */
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
