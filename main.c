#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 50
#define MAX_TASK 10
#define LAST_OPTION 5

struct taskInfo{
  int  id[MAX_TASK];
  char list[MAX_TASK][MAX_TITLE];
  int  progress[MAX_TASK];
};
typedef struct taskInfo taskInfo;

int  inputGetOption(void);
int  inputGetID(int listLength);
int  inputGetProgress(void);
void inputNewTask(int id[], char list[][MAX_TITLE], int progress[], int *listLength);
int  systemDeleteTask(char list[][MAX_TITLE], int *listLength, int id);
void systemEditTask(const char *filePath, taskInfo *taskData, int *taskCount);
void outputViewTasks(char list[][MAX_TITLE], int listLength, int progress[]);
void systemSortListOfTask(int id[], char list[][MAX_TITLE], int progress[], int listLength);
void systemSearchTask(char list[][MAX_TITLE], int listLength);
void systemResponse( int choice, int id[], char list[][MAX_TITLE], int progress[], int *listLength, const char* filePath, taskInfo *taskData);

void inputReadFile(const char* file_name, taskInfo *task_data, int *task_count);
void outputWriteFile(const char *filePath, const taskInfo *taskData, int taskCount);

int main(void)
{ 
  taskInfo taskData;
  const char* file = "./data/task.csv";
  int listLength = 0;

  inputReadFile(file, &taskData, &listLength);
  taskData.id[listLength] = listLength + 1;

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
    systemResponse(userOption, taskData.id, taskData.list, taskData.progress, &listLength, file, &taskData);
    outputWriteFile(file, &taskData, listLength);
  }
}


//lấy lựa chọn của user
int inputGetOption(void)
{
  int userOption;
  while (1)
  {
    printf("Your option: ");
    if (scanf("%d", &userOption) == 1 && userOption >= 0 && userOption <= LAST_OPTION)
    {
      return userOption;
    }
    printf("Invalid option. Please try again!\n");
    while (getchar() != '\n')
      ;
  }
}

// lấy ID
int inputGetID(int totalTasks)
{
  int taskID;
  while (1)
  {
    printf("Task ID: ");
    if (scanf("%d", &taskID) == 1 && taskID >= 1 && taskID <= totalTasks)
    {
      return taskID;
    }
    printf("Invalid ID. Please try again!\n");
    while (getchar() != '\n')
      ;
  }
}


// Lấy % hoàn thành
int inputGetProgress(void)
{
  int taskProgress;
  while (1)
  {
    printf("Progress (1-100): ");
    if (scanf("%d", &taskProgress) == 1 && taskProgress >= 1 && taskProgress <= 100)
    {
      return taskProgress;
    }
    printf("Invalid progress. Please try again!\n");
    while (getchar() != '\n')
      ;
  }
}

// lấy nội dung của task mới
void inputNewTask(
    int id[], char list[][MAX_TITLE], int progress[], int *listLength)
{
  if (*listLength == MAX_TASK)
  {
    printf("Task list is full!\n");
    return;
  }

  printf("Enter task title: ");
  while (getchar() != '\n')
    ; // Xóa bộ đệm
  fgets(list[*listLength], MAX_TITLE, stdin);
  list[*listLength][strcspn(list[*listLength], "\n")] = '\0'; // Xóa ký tự '\n'

  id[*listLength]       = *listLength + 1;
  progress[*listLength] = inputGetProgress();
  (*listLength)++;

  printf("Task added successfully!\n");
}

// xóa task
int systemDeleteTask(char list[][MAX_TITLE], int *listLength, int id)
{
  if (id < 1 || (id > *listLength && listLength != 0))
  {
    printf("Invalid ID!\n");
    return 0;
  }
  if (listLength == 0)
  {
    printf("No task available to delete.\n");
    return 0;
  }

  for (int i = id - 1; i < *listLength - 1; i++)
  {
    strcpy(list[i], list[i + 1]);
  }
  (*listLength)--;
  return 1;
}
void systemEditTask(const char *filePath, taskInfo *taskData, int *taskCount)
{
    /* nạp lại dữ liệu từ file */
    inputReadFile(filePath, taskData, taskCount);
    if (*taskCount == 0)
    {
        printf("No tasks available to edit.\n");
        return;
    }

    /* chọn ID cần sửa (hiển thị từ 1) */
    int idx = inputGetID(*taskCount) - 1;

    char newTitle[MAX_TITLE];
    char buf[16];

    printf("Editing [%d] %s - %d%%\n",
           idx + 1,
           taskData->list[idx],
           taskData->progress[idx]);

    /* nhập tiêu đề mới (Enter = giữ nguyên) */
    printf("New title (Enter to skip): ");
    while (getchar() != '\n');                    /* flush */
    fgets(newTitle, MAX_TITLE, stdin);
    if (newTitle[0] != '\n')
    {
        newTitle[strcspn(newTitle, "\n")] = '\0';
        strncpy(taskData->list[idx], newTitle, MAX_TITLE - 1);
        taskData->list[idx][MAX_TITLE - 1] = '\0';
    }

    /* nhập tiến độ mới (Enter = giữ nguyên) */
    printf("New progress (Enter to skip): ");
    fgets(buf, sizeof(buf), stdin);
    if (buf[0] != '\n')
    {
        int p = atoi(buf);
        if (p >= 1 && p <= 100)
            taskData->progress[idx] = p;
        else
            printf("Invalid progress, keeping previous value.\n");
    }

    /* ghi lại toàn bộ file */
    outputWriteFile(filePath, taskData, *taskCount);
    printf("Task updated successfully!\n");
}


// xem task
void outputViewTasks(char list[][MAX_TITLE], int listLength, int progress[])
{
  if (listLength == 0)
  {
    printf("No tasks available.\n");
    return;
  }
  for (int i = 0; i < listLength; i++)
  {
    printf("[%d] %s - Progress: %d%%\n", i + 1, list[i], progress[i]);
  }
}

// sắp xếp task theo thứ tự ưu tiên theo % hoàn thành từ thấp đến cao
void systemSortListOfTask(
    int id[], char list[][MAX_TITLE], int progress[], int listLength)
{
  if (listLength == 0)
  {
    printf("No tasks available to sort.\n");
    return;
  }

  for (int gap = listLength / 2; gap > 0; gap /= 2)
  {
    for (int i = gap; i < listLength; i++)
    {
      int  tempId = id[i];
      int  j;
      int  tempProgress = progress[i];
      char tmp_title[MAX_TITLE];
      strcpy(tmp_title, list[i]);

      for (j = i; j >= gap && progress[j - gap] > tempProgress; j -= gap)
      {
        progress[j] = progress[j - gap];
        id[j]       = id[j - gap];
        strcpy(list[j], list[j - gap]);
      }

      progress[j] = tempProgress;
      id[j]       = tempId;
      strcpy(list[j], tmp_title);
    }
  }

  printf("Tasks sorted by progress.\n");
  outputViewTasks(list, listLength, progress);
}

// tìm kiếm task theo title (tìm theo từng chữ cái)
void systemSearchTask(char list[][MAX_TITLE], int listLength)
{
  char keyword[MAX_TITLE];
  int  found = 0;
  if (listLength == 0)
  {
    printf("No tasks available to search.\n");
    return;
  }

  printf("Enter task title to search: ");
  while (getchar() != '\n')
    ; // Xóa bộ đệm trước khi nhập
  fgets(keyword, MAX_TITLE, stdin);
  keyword[strcspn(keyword, "\n")] = '\0'; // Xóa ký tự xuống dòng

  printf("Search results:\n");
  for (int i = 0; i < listLength; i++)
  {
    if (strstr(list[i], keyword) != NULL)
    { // Kiểm tra xem keyword có trong tiêu đề không
      printf("[%d] %s\n", i + 1, list[i]);
      found = 1;
    }
  }

  if (!found)
  {
    printf("No matching tasks found.\n");
  }
}
// phản hồi với user
void systemResponse(
    int choice, int id[], char list[][MAX_TITLE], int progress[],
    int *listLength, const char* filePath, taskInfo *taskData)
{
  switch (choice)
  {
    case 1:
    {
      inputNewTask(id, list, progress, listLength);
      break;
    }
    case 2:
    {
      int delete_id = inputGetID(*listLength);
      if (systemDeleteTask(list, listLength, delete_id))
      {
        printf("Task deleted successfully!\n");
      }
      break;
    }
    case 3:
    {
      systemEditTask(filePath, taskData, listLength);
      break;
    }
    case 4:
    {
      systemSortListOfTask(id, list, progress, *listLength);
      break;
    }
    case 5:
    {
      systemSearchTask(list, *listLength);
      break;
    }
    case 0:
    {
      printf("You exit successfully!\n");
      fflush(stdout); // Đảm bảo in ra màn hình trước khi thoát
      exit(0);
    }
  }
}

void inputReadFile(const char *filePath, taskInfo   *taskData, int *taskCount)
{
    if (taskData == NULL || taskCount == NULL)
    {
        return;                         /* tham số không hợp lệ */
    }

    *taskCount = 0;                     /* mặc định 0 task     */
    FILE *fp   = fopen(filePath, "r");
    if (fp == NULL)
    {                                   /* không mở được file  */
        perror("Cannot open task file");
        return;
    }

    char lineBuf[256];
    while (fgets(lineBuf, sizeof(lineBuf), fp) != NULL)
    {
        /* Bỏ qua dòng trống hoặc chỉ có xuống dòng */
        if (lineBuf[0] == '\n' || lineBuf[0] == '\r')
        {
            continue;
        }

        /* Xoá ký tự xuống dòng cuối (nếu có) */
        lineBuf[strcspn(lineBuf, "\r\n")] = '\0';

        /* Tách các trường – định dạng: title,progress */
        char *titleToken    = strtok(lineBuf, ",");
        char *progressToken = strtok(NULL, ",");

        if (titleToken == NULL)         /* Thiếu tiêu đề → bỏ qua */
        {
            continue;
        }
        if (*taskCount >= MAX_TASK)     /* Không ghi quá kích thước mảng */
        {
            break;
        }

        // Lưu title
        strncpy(taskData->list[*taskCount], titleToken, MAX_TITLE - 1);
        taskData->list[*taskCount][MAX_TITLE - 1] = '\0';

        // Lưu progress
        int prog = 0;                   /* mặc định 0 % nếu lỗi */
        if (progressToken != NULL)
        {
            prog = atoi(progressToken);
            if (prog < 0 || prog > 100) /* giá trị không hợp lệ */
            {
                prog = 0;
            }
        }
        taskData->progress[*taskCount] = prog;

        taskData->id[*taskCount] = *taskCount + 1;

        (*taskCount)++;                 
    }

    fclose(fp);
}

// ghi toàn bộ task vào file
void outputWriteFile(const char *filePath,
                     const taskInfo *taskData,
                     int taskCount)
{
    if (!filePath || !taskData) return;          /* tham số lỗi */

    FILE *fp = fopen(filePath, "w");
    if (!fp)
    {
        perror("Cannot open task file");
        return;
    }

    /* ghi title,progress cho từng task */
    for (int i = 0; i < taskCount && i < MAX_TASK; ++i)
    {
        fprintf(fp, "%s,%d\n",
                taskData->list[i][0] ? taskData->list[i] : "",
                taskData->progress[i]);
    }

    fclose(fp);
}