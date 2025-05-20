#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_TITLE 50
#define MAX_TASK 10
#define LAST_OPTION 5

int  inputGetOption(void);
int  inputGetID(int listLength);
int  inputGetProgress(void);
void inputNewTask(int id[], char list[][MAX_TITLE], int progress[], int *listLength);
    int  systemDeleteTask(char list[][MAX_TITLE], int *listLength, int id);
void systemEditTask(char list[][MAX_TITLE], int progress[], int listLength);
void outputViewTasks(char list[][MAX_TITLE], int listLength, int progress[]);
void systemSortListOfTask(
    int id[], char list[][MAX_TITLE], int progress[], int listLength);
void systemSearchTask(char list[][MAX_TITLE], int listLength);
void systemResponse(
    int choice, int id[], char list[][MAX_TITLE], int progress[],
    int *listLength);
int is_next_integer(const char* ptr);
int is_detail_empty(const char* ptr);

struct taskInfo{
  int  id[MAX_TASK];
  char list[MAX_TASK][MAX_TITLE];
  int  progress[MAX_TASK];
  int  listLength;
};
typedef struct taskInfo taskInfo;

int main(void)
{ 
  taskInfo taskData;
  taskData.listLength = 0;

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
    systemResponse(userOption, taskData.id, taskData.list, taskData.progress, &taskData.listLength);
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
// sửa task
void systemEditTask(const char *filePath, taskInfo *taskData) {
    // Bước 1: Load dữ liệu từ file
    INPUT_read_file(filePath, taskData, &taskData->listLength);

    if (taskData->listLength == 0) {
        printf("No tasks available to edit.\n");
        return;
    }

    // Bước 2: Nhập ID cần sửa
    int idEditTask = inputGetID(taskData->listLength) - 1; // Hiển thị từ 1

    char newTitle[MAX_TITLE];
    char inputBuffer[10];

    printf("Editing Task [%d]: %s - Progress: %d%%\n",
           idEditTask + 1,
           taskData->list[idEditTask],
           taskData->progress[idEditTask]);

    // Nhập tiêu đề mới (cho phép bỏ qua)
    printf("Enter new task title (press Enter to skip): ");
    while (getchar() != '\n'); // Xóa bộ đệm
    fgets(newTitle, MAX_TITLE, stdin);
    if (newTitle[0] != '\n') {
        newTitle[strcspn(newTitle, "\n")] = '\0'; // Xoá newline
        strcpy(taskData->list[idEditTask], newTitle);
    }

    // Nhập tiến độ mới
    printf("Enter new progress (press Enter to skip): ");
    fgets(inputBuffer, sizeof(inputBuffer), stdin);
    if (inputBuffer[0] != '\n') {
        int newProgress = atoi(inputBuffer);
        if (newProgress >= 1 && newProgress <= 100) {
            taskData->progress[idEditTask] = newProgress;
        } else {
            printf("Invalid progress, keeping previous value.\n");
        }
    }

    printf("Task updated successfully!\n");

    // Bước 3: Ghi lại toàn bộ file
    SYSTEM_overwrite_file(filePath, taskData);
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
    int *listLength)
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
      systemEditTask(list, progress, *listLength);
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

int is_next_integer(const char* ptr) {
    int temp;
    return sscanf(ptr, "%d", &temp) == 1;
}

int is_detail_empty(const char* ptr) {
    char temp[2];
    return (sscanf(ptr, "\"%1[^\"]\"", temp) != 1);
}

void INPUT_read_file(const char* filePath, taskInfo *taskData, int *taskCount) {
    FILE* fileStream = fopen(filePath, "r");
    if (fileStream == NULL) {
      fprintf(stderr, "Error: Cannot open file '%s'\n", filePath);
      perror("System error");
      return;
    }
    int currentIndex = 0;
    char rawLine[512];

    if (!fileStream) {
        perror("Cannot open file");
        return;
    }

    // Bỏ qua dòng tiêu đề và mô tả
    fgets(rawLine, sizeof(rawLine), fileStream);
    fgets(rawLine, sizeof(rawLine), fileStream);

    while ((fgets(rawLine, sizeof(rawLine), fileStream)) && currentIndex < MAX_TASK) {
        char extractedTitle[MAX_TITLE] = "";
        int charOffset = 0;
        char* linePtr = rawLine;

        // Đọc ID (có thể bị thiếu)
        if (is_next_integer(linePtr)) {
            sscanf(linePtr, "%d,%*[^,],%n", &taskData->id[currentIndex], &charOffset);
            linePtr += charOffset;
        } else {
            taskData->id[currentIndex] = -1;
            linePtr = strchr(linePtr, '\"');
        }

        // Đọc nội dung task
        if (sscanf(linePtr, "\"%[^\"]\"%n", extractedTitle, &charOffset) == 1) {
            strncpy(taskData->list[currentIndex], extractedTitle, MAX_TITLE - 1);
            taskData->list[currentIndex][MAX_TITLE - 1] = '\0';
            linePtr += charOffset;
        } else {
            strcpy(taskData->list[currentIndex], "\0");
            linePtr = strchr(linePtr, ',');
        }

        // Đọc tiến độ %
        if (sscanf(linePtr, ",%d%%,", &taskData->progress[currentIndex]) != 1) {
            taskData->progress[currentIndex] = -1;
        }

        currentIndex++;
    }

    *taskCount = currentIndex;
    fclose(fileStream);
}

void SYSTEM_write_task_to_file(const char* filePath, taskInfo *taskData) {
    FILE* fileStream = fopen(filePath, "a");

    if (fileStream == NULL) {
        printf("File pointer is NULL.\n");
        return;
    }

    int lastIndex = taskData->listLength - 1;

    fprintf(fileStream, "%d, ,\"%s\",%d%%, , \n",
            taskData->id[lastIndex],
            taskData->list[lastIndex],
            taskData->progress[lastIndex]);

    fclose(fileStream);
}

