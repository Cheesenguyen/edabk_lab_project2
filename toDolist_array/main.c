#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 50
#define MAX_TASK 10
#define LAST_OPTION 4

int INPUT_get_option(void) {
    int option;
    while (1) {
        printf("Your option: ");
        if (scanf("%d", &option) == 1 && option >= 1 && option <= LAST_OPTION) 
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

void OUTPUT_view_tasks(char list[][MAX_TITLE], int list_length, int progress[]) {
    if (list_length == 0) {
        printf("No tasks available.\n");
        return;
    }
    for (int i = 0; i < list_length; i++) {
        printf("[%d] %s - Progress: %d%%\n", i + 1, list[i], progress[i]);
    }
}

void SYSTEM_sort_task(int id[], char list[][MAX_TITLE], int progress[], int list_length) {
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
            OUTPUT_view_tasks(list, *list_length, progress);
            break;
        case 4:
            SYSTEM_sort_task(id, list, progress, *list_length);
            break;
        case 0:
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
        printf("3. View Tasks\n");
        printf("4. Sort Tasks by Progress\n");
        printf("0. Exit\n");
        int choice = INPUT_get_option();
        SYSTEM_response(choice, id, list, progress, &list_length);
    }
}
