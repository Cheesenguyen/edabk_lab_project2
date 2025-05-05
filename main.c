#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 50
#define MAX_TASK 10
#define LAST_OPTION 5

int inputGetOption(void);
int inputGetID(int listLength);
int inputGetProgress(void);
void inputNewTask(int id[], char list[][MAX_TITLE], int progress[], int *listLength);
int systemDeleteTask(char list[][MAX_TITLE], int *listLength, int id);
void systemEditTask(char list[][MAX_TITLE], int progress[], int listLength);
void outputViewTasks(char list[][MAX_TITLE], int listLength, int progress[]);
void systemSortListOfTask(int id[], char list[][MAX_TITLE], int progress[], int listLength);
void systemSearchTask(char list[][MAX_TITLE], int listLength);
void systemResponse(int choice, int id[], char list[][MAX_TITLE], int progress[], int *listLength);


int main(void) {
    int id[MAX_TASK];
    char list[MAX_TASK][MAX_TITLE];
    int progress[MAX_TASK];
    int listLength  = 0;
    
    while (1) {
        int choice;
        printf("\nTask Manager\n");
        printf("1. Add Task\n");
        printf("2. Delete Task\n");
        printf("3. Edit task\n");
        printf("4. Sort list of Task\n");
        printf("5. Search Task\n");  // Mới thêm
        printf("0. Exit\n");
        choice = inputGetOption();
        systemResponse(choice, id, list, progress, &listLength);
    }
}

//lấy lựa chọn của user
int inputGetOption(void) {
    int option;
    while (1) {
        printf("Your option: ");
        if (scanf("%d", &option) == 1 && option >= 0 && option <= LAST_OPTION) // Cho phép nhập 0
            return option;
        printf("Invalid option. Please try again!\n");
        while (getchar() != '\n');  // Xóa bộ đệm nhập
    }
}
// lấy ID 
int inputGetID(int listLength) {
    int id;
    while (1) {
        printf("Task ID: ");
        if (scanf("%d", &id) == 1 && id >= 1 && id <= listLength) 
            return id;
        printf("Invalid ID. Please try again!\n");
        while (getchar() != '\n');
    }
}
// Lấy % hoàn thành
int inputGetProgress(void) {
    int progress;
    while (1) {
        printf("Progress (1-100): ");
        if (scanf("%d", &progress) == 1 && progress >= 1 && progress <= 100) 
            return progress;
        printf("Invalid progress. Please try again!\n");
        while (getchar() != '\n');
    }
}
// lấy nội dung của task mới 
void inputNewTask(int id[], char list[][MAX_TITLE], int progress[], int *listLength) {
    if (*listLength == MAX_TASK) {
        printf("Task list is full!\n");
        return;
    }

    printf("Enter task title: ");
    while (getchar() != '\n');  // Xóa bộ đệm
    fgets(list[*listLength], MAX_TITLE, stdin);
    list[*listLength][strcspn(list[*listLength], "\n")] = '\0';  // Xóa ký tự '\n'

    id[*listLength] = *listLength + 1;
    progress[*listLength] = inputGetProgress();
    (*listLength)++;

    printf("Task added successfully!\n");
}
// xóa task 
int systemDeleteTask(char list[][MAX_TITLE], int *listLength, int id) {
    if (id < 1 || (id > *listLength && listLength != 0)) {
        printf("Invalid ID!\n");
        return 0;
    }
    if (listLength == 0){
	printf("No task available to delete.\n");
	return 0;
	}

    for (int i = id - 1; i < *listLength - 1; i++) {
        strcpy(list[i], list[i + 1]);
    }
    (*listLength)--;
    return 1;
}
// sửa task
void systemEditTask(char list[][MAX_TITLE], int progress[], int listLength) {
    int idEditTask = inputGetID(listLength) - 1;  // ID hiển thị từ 1, nhưng mảng từ 0
    char newTitle[MAX_TITLE];
    char progress[10];
    if (listLength == 0) {
        printf("No tasks available to edit.\n");
        return;
    }

    printf("Editing Task [%d]: %s - Progress: %d%%\n", idEditTask + 1, list[idEditTask], progress[idEditTask]);

    // Nhập tiêu đề mới (cho phép bỏ qua)
    printf("Enter new task title (press Enter to skip): ");
    while (getchar() != '\n');  // Xóa bộ đệm trước khi nhập chuỗi
    fgets(newTitle, MAX_TITLE, stdin);
    if (newTitle[0] != '\n') {  // Nếu không phải chỉ nhấn Enter
        newTitle[strcspn(newTitle, "\n")] = '\0';  // Xóa ký tự xuống dòng
        strcpy(list[idEditTask], newTitle);
    }

    // Nhập tiến độ mới (cho phép bỏ qua)
    printf("Enter new progress (press Enter to skip): ");
    fgets(progress, sizeof(progress), stdin);
    if (progress[0] != '\n') {  // Nếu không phải chỉ nhấn Enter
        int newProgress = atoi(progress);  // Chuyển đổi sang số
        if (newProgress >= 1 && newProgress <= 100) {
            progress[idEditTask] = newProgress;
        } else {
            printf("Invalid progress, keeping previous value.\n");
        }
    }

    printf("Task updated successfully!\n");
}
// xem task
void outputViewTasks(char list[][MAX_TITLE], int listLength, int progress[]) {
    if (listLength == 0) {
        printf("No tasks available.\n");
        return;
    }
    for (int i = 0; i < listLength; i++) {
        printf("[%d] %s - Progress: %d%%\n", i + 1, list[i], progress[i]);
    }
}
// sắp xếp task theo thứ tự ưu tiên theo % hoàn thành 
void systemSortListOfTask(int id[], char list[][MAX_TITLE], int progress[], int listLength) {
    if (listLength == 0) {
        printf("No tasks available to sort.\n");
        return;
    }

    for (int gap = listLength / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < listLength; i++) {
            int tempId = id[i];
            int j;
            int tempProgress = progress[i];
            char tmp_title[MAX_TITLE];
            strcpy(tmp_title, list[i]);

            for (j = i; j >= gap && progress[j - gap] > tempProgress; j -= gap) {
                progress[j] = progress[j - gap];
                id[j] = id[j - gap];
                strcpy(list[j], list[j - gap]);
            }

            progress[j] = tempProgress;
            id[j] = tempId;
            strcpy(list[j], tmp_title);
        }
    }

    printf("Tasks sorted by progress.\n");
    outputViewTasks(list, listLength, progress);
}


void systemSearchTask(char list[][MAX_TITLE], int listLength) {
    char keyword[MAX_TITLE];
    int found = 0;
    if (listLength == 0) {
        printf("No tasks available to search.\n");
        return;
    }

    printf("Enter task title to search: ");
    while (getchar() != '\n'); // Xóa bộ đệm trước khi nhập
    fgets(keyword, MAX_TITLE, stdin);
    keyword[strcspn(keyword, "\n")] = '\0'; // Xóa ký tự xuống dòng

    printf("Search results:\n");
    for (int i = 0; i < listLength; i++) {
        if (strstr(list[i], keyword) != NULL) { // Kiểm tra xem keyword có trong tiêu đề không
            printf("[%d] %s\n", i + 1, list[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("No matching tasks found.\n");
    }
}

void systemResponse(int choice, int id[], char list[][MAX_TITLE], int progress[], int *listLength) {
    switch (choice) {
        case 1:
            inputNewTask(id, list, progress, listLength);
            break;
        case 2: {
            int delete_id = inputGetID(*listLength);
            if (systemDeleteTask(list, listLength, delete_id))
                printf("Task deleted successfully!\n");
            break;
        }
        case 3:
            systemEditTask(list, progress, *listLength);
            break;
        case 4:
            systemSortListOfTask(id, list, progress, *listLength);
            break;
        case 5:
            systemSearchTask(list, *listLength);
            break;
        case 0:
            printf("You exit successfully!\n");
            fflush(stdout); // Đảm bảo in ra màn hình trước khi thoát
            exit(0);
    }
}
