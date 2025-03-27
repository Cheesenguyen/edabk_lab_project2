#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 50
#define MAX_TASK 10
#define LAST_OPTION 5

int INPUT_get_option(void) {
    int option;
    while (1) {
        printf("Your option: ");
        if (scanf("%d", &option) == 1 && option >= 0 && option <= LAST_OPTION) // Cho phép nhập 0
            return option;
        printf("Invalid option. Please try again!\n");
        while (getchar() != '\n');  // Xóa bộ đệm nhập
    }
}

int INPUT_get_ID(int list_length) {
    int id;
    while (1) {
        printf("Task ID: ");
        if (scanf("%d", &id) == 1 && id >= 1 && id <= list_length) 
            return id;
        printf("Invalid ID. Please try again!\n");
        while (getchar() != '\n');
    }
}

int INPUT_get_progress(void) {
    int progress;
    while (1) {
        printf("Progress (1-100): ");
        if (scanf("%d", &progress) == 1 && progress >= 1 && progress <= 100) 
            return progress;
        printf("Invalid progress. Please try again!\n");
        while (getchar() != '\n');
    }
}

void INPUT_new_task(int id[], char list[][MAX_TITLE], int progress[], int *list_length) {
    if (*list_length == MAX_TASK) {
        printf("Task list is full!\n");
        return;
    }

    printf("Enter task title: ");
    while (getchar() != '\n');  // Xóa bộ đệm
    fgets(list[*list_length], MAX_TITLE, stdin);
    list[*list_length][strcspn(list[*list_length], "\n")] = '\0';  // Xóa ký tự '\n'

    id[*list_length] = *list_length + 1;
    progress[*list_length] = INPUT_get_progress();
    (*list_length)++;

    printf("Task added successfully!\n");
}

int SYSTEM_delete_task(char list[][MAX_TITLE], int *list_length, int id) {
    if (id < 1 || id > *list_length) {
        printf("Invalid ID!\n");
        return 0;
    }

    for (int i = id - 1; i < *list_length - 1; i++) {
        strcpy(list[i], list[i + 1]);
    }
    (*list_length)--;
    return 1;
}

void SYSTEM_edit_task(int id[], char list[][MAX_TITLE], int progress[], int list_length) {
    if (list_length == 0) {
        printf("No tasks available to edit.\n");
        return;
    }

    int edit_id = INPUT_get_ID(list_length) - 1;  // ID hiển thị từ 1, nhưng mảng từ 0

    printf("Editing Task [%d]: %s - Progress: %d%%\n", edit_id + 1, list[edit_id], progress[edit_id]);

    // Nhập tiêu đề mới (cho phép bỏ qua)
    printf("Enter new task title (press Enter to skip): ");
    while (getchar() != '\n');  // Xóa bộ đệm trước khi nhập chuỗi
    char new_title[MAX_TITLE];
    fgets(new_title, MAX_TITLE, stdin);
    if (new_title[0] != '\n') {  // Nếu không phải chỉ nhấn Enter
        new_title[strcspn(new_title, "\n")] = '\0';  // Xóa ký tự xuống dòng
        strcpy(list[edit_id], new_title);
    }

    // Nhập tiến độ mới (cho phép bỏ qua)
    printf("Enter new progress (press Enter to skip): ");
    char progress_input[10];
    fgets(progress_input, sizeof(progress_input), stdin);
    if (progress_input[0] != '\n') {  // Nếu không phải chỉ nhấn Enter
        int new_progress = atoi(progress_input);  // Chuyển đổi sang số
        if (new_progress >= 1 && new_progress <= 100) {
            progress[edit_id] = new_progress;
        } else {
            printf("Invalid progress, keeping previous value.\n");
        }
    }

    printf("Task updated successfully!\n");
}

void OUTPUT_view_tasks(char list[][MAX_TITLE], int list_length, int progress[]) {
    if (list_length == 0) {
        printf("No tasks available.\n");
        return;
    }
    for (int i = 0; i < list_length; i++) {
        printf("[%d] %s - Progress: %d%%\n", i + 1, list[i], progress[i]);
    }
}

void SYSTEM_sort_list_of_task(int id[], char list[][MAX_TITLE], int progress[], int list_length) {
    if (list_length == 0) {
        printf("No tasks available to sort.\n");
        return;
    }

    for (int gap = list_length / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < list_length; i++) {
            int tmp_id = id[i];
            int tmp_progress = progress[i];
            char tmp_title[MAX_TITLE];
            strcpy(tmp_title, list[i]);

            int j;
            for (j = i; j >= gap && progress[j - gap] > tmp_progress; j -= gap) {
                progress[j] = progress[j - gap];
                id[j] = id[j - gap];
                strcpy(list[j], list[j - gap]);
            }

            progress[j] = tmp_progress;
            id[j] = tmp_id;
            strcpy(list[j], tmp_title);
        }
    }

    printf("Tasks sorted by progress.\n");
    OUTPUT_view_tasks(list, list_length, progress);
}


void SYSTEM_search_task(char list[][MAX_TITLE], int list_length) {
    if (list_length == 0) {
        printf("No tasks available to search.\n");
        return;
    }

    printf("Enter task title to search: ");
    while (getchar() != '\n'); // Xóa bộ đệm trước khi nhập
    char keyword[MAX_TITLE];
    fgets(keyword, MAX_TITLE, stdin);
    keyword[strcspn(keyword, "\n")] = '\0'; // Xóa ký tự xuống dòng

    int found = 0;
    printf("Search results:\n");
    for (int i = 0; i < list_length; i++) {
        if (strstr(list[i], keyword) != NULL) { // Kiểm tra xem keyword có trong tiêu đề không
            printf("[%d] %s\n", i + 1, list[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("No matching tasks found.\n");
    }
}

void SYSTEM_response(int choice, int id[], char list[][MAX_TITLE], int progress[], int *list_length) {
    switch (choice) {
        case 1:
            INPUT_new_task(id, list, progress, list_length);
            break;
        case 2: {
            int delete_id = INPUT_get_ID(*list_length);
            if (SYSTEM_delete_task(list, list_length, delete_id))
                printf("Task deleted successfully!\n");
            break;
        }
        case 3:
            SYSTEM_edit_task(id, list, progress, *list_length);
            break;
        case 4:
            SYSTEM_sort_list_of_task(id, list, progress, *list_length);
            break;
        case 5:
            SYSTEM_search_task(list, *list_length);
            break;
        case 0:
            printf("You exit successfully!\n");
            fflush(stdout); // Đảm bảo in ra màn hình trước khi thoát
            exit(0);
    }
}

int main() {
    int id[MAX_TASK];
    char list[MAX_TASK][MAX_TITLE];
    int progress[MAX_TASK];
    int list_length = 0;
    
    while (1) {
        printf("\nTask Manager\n");
        printf("1. Add Task\n");
        printf("2. Delete Task\n");
        printf("3. Edit task\n");
        printf("4. Sort list of Task\n");
        printf("5. Search Task\n");  // Mới thêm
        printf("0. Exit\n");
        int choice = INPUT_get_option();
        SYSTEM_response(choice, id, list, progress, &list_length);
    }
}
