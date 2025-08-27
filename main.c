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

/* ---- FILE I/O ---- */
void inputReadFile(const char *filePath, Task tasks[], int *taskCount)
{
    if (!filePath || !tasks || !taskCount) return;

    *taskCount = 0;
    FILE *fp = fopen(filePath, "r");
    if (!fp)
    {
        perror("Cannot open task file");
        return;
    }

    char line[4096];
    int lineNum = 0;

    while (fgets(line, sizeof(line), fp))
    {
        lineNum++;
        if (lineNum <= 2) continue;               /* bỏ header 2 dòng */

        line[strcspn(line, "\r\n")] = '\0';

        /* Parse: ID,Title,Detail,Status,Priority,Deadline */
        char *p = line;
        char *comma = strchr(p, ',');
        if (!comma) continue;
        *comma = '\0';
        int id = atoi(p);
        p = comma + 1;

        char title[MAX_TITLE] = "";
        if (*p == '"') {
            p++;
            char *endQuote = strchr(p, '"');
            if (!endQuote) continue;
            *endQuote = '\0';
            strncpy(title, p, MAX_TITLE - 1);
            title[MAX_TITLE - 1] = '\0';
            p = endQuote + 2; /* bỏ dấu ", */
        } else {
            char *nextComma = strchr(p, ',');
            if (!nextComma) continue;
            *nextComma = '\0';
            strncpy(title, p, MAX_TITLE - 1);
            title[MAX_TITLE - 1] = '\0';
            p = nextComma + 1;
        }

        /* Bỏ Detail và Priority (2 cột) rồi lấy Status */
        for (int i = 0; i < 2; ++i) {
            comma = strchr(p, ',');
            if (!comma) break;
            p = comma + 1;
        }

        char status[16];
        strncpy(status, p, sizeof(status) - 1);
        status[sizeof(status) - 1] = '\0';

        size_t n = strlen(status);
        if (n > 0 && status[n - 1] == '%') status[n - 1] = '\0';

        int progress = atoi(status);
        if (progress < 0 || progress > 100) progress = 0;

        if (*taskCount < MAX_TASK)
        {
            tasks[*taskCount].id = id;
            strncpy(tasks[*taskCount].list, title, MAX_TITLE - 1);
            tasks[*taskCount].list[MAX_TITLE - 1] = '\0';
            tasks[*taskCount].progress = progress;
            (*taskCount)++;
        }
    }

    fclose(fp);
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
