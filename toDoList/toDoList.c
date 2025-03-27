#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILENAME "task.txt"
#define ANSI_RESET "\033[0m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_RED "\033[31m"
#define ANSI_RED_BOLD "\033[1;31m"
#define ANSI_ORANGE "\033[38;5;214m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_GREEN "\033[32m"
#define ANSI_BLUE "\033[34m"
#define ANSI_CYAN "\033[36m"
#define ANSI_WHITE "\033[37m"
#define DESC_WIDTH 50

// struct cua deadline
typedef struct Deadline{
    //format: hh/dd/mm/yy
    int time;           // hour
    int date;
    int month;
    int year;
} Deadline;
//Deadline Dl;


// struct cua task con
typedef struct subTask {
    int ID;
    char title[100];
    char *des;
    int completed;
    int priority;
    Deadline Dl;
    struct subTask *next;  // Sửa lại đúng tên struct
} subTask;

// struct cua task lon
typedef struct Task{
    int ID;
    char title [100];
    char *des;
    int completed;          // tinh theo ti le %
    int priority;
    Deadline Dl;
    subTask *subtasks; 
    struct Task *next;
} Task;

// ghi vào file
void addTask(Task **head, int ID, char* title, char *des, int completed, int priority, Deadline Dl) {
    Task *newTask = (Task *)malloc(sizeof(Task));
    if (!newTask) {
        printf("Memory allocation failed!\n");
        return;
    }
    newTask->ID = ID;
    strcpy(newTask->title, title);
    newTask->des = strdup(des);
    newTask->completed = completed;
    newTask->priority = priority;
    newTask->Dl = Dl;
    newTask->subtasks = NULL;
    newTask->next = *head;
    *head = newTask;
}

// them task con trong task moi
void addSubTask(Task *task, int ID, char* title, char* des, int completed, int priority, Deadline Dl) {
    if (task == NULL) return;
    
    subTask *newSubTask = (subTask *)malloc(sizeof(subTask));
    newSubTask->ID = ID;
    strcpy(newSubTask->title, title);
    newSubTask->des = strdup(des);
    newSubTask->completed = completed;
    newSubTask->priority = priority;
    newSubTask->Dl = Dl;
    newSubTask->next = task->subtasks;
    task->subtasks = newSubTask;
}

// sửa tiêu đề
void editTitle(Task **head, int ID, char *newTitle) {
    Task *currentTask = *head;
    while (currentTask != NULL) {
        if (currentTask->ID == ID) {
            strcpy(currentTask->title, newTitle);
            return;
        }
        subTask *currentSubTask = currentTask->subtasks;
        while (currentSubTask != NULL) {
            if (currentSubTask->ID == ID) {
                strcpy(currentSubTask->title, newTitle);
                return;
            }
            currentSubTask = currentSubTask->next;
        }
        currentTask = currentTask->next;
    }
}

// sua Description cho task lon/con
void editDescription(Task **head, int ID, char *newDes) {
    Task *currentTask = *head;
    while (currentTask != NULL) {
        if (currentTask->ID == ID) {
            if (currentTask->des) free(currentTask->des);
            currentTask->des = strdup(newDes);
            if (!currentTask->des) {
                printf("Memory allocation failed!\n");
                return;
            }
            return;
        }
        subTask *currentSubTask = currentTask->subtasks;
        while (currentSubTask != NULL) {
            if (currentSubTask->ID == ID) {
                if (currentSubTask->des) free(currentSubTask->des);
                currentSubTask->des = strdup(newDes);
                if (!currentSubTask->des) {
                    printf("Memory allocation failed!\n");
                    return;
                }
                return;
            }
            currentSubTask = currentSubTask->next;
        }
        currentTask = currentTask->next;
    }
    printf("Task/SubTask with ID %d not found!\n", ID);
}

// sua trang thai hoan thanh
void editCompleted(Task **head, int ID, int newCom) {
    Task *currentTask = *head;
    while (currentTask != NULL) {
        if (currentTask->ID == ID) {
            currentTask->completed = newCom;
            return;
        }
        subTask *currentSubTask = currentTask->subtasks;
        while (currentSubTask != NULL) {
            if (currentSubTask->ID == ID) {
                currentSubTask->completed = newCom;
                return;
            }
            currentSubTask = currentSubTask->next;
        }
        currentTask = currentTask->next;
    }
}

// sua muc do uu tien
void editPriority(Task **head, int ID, int newPri) {
    Task *currentTask = *head;
    while (currentTask != NULL) {
        if (currentTask->ID == ID) {
            currentTask->priority = newPri;
            return;
        }
        subTask *currentSubTask = currentTask->subtasks;
        while (currentSubTask != NULL) {
            if (currentSubTask->ID == ID) {
                currentSubTask->priority = newPri;
                return;
            }
            currentSubTask = currentSubTask->next;
        }
        currentTask = currentTask->next;
    }
}

// sua deadline
void editDeadline(Task **head, int ID, Deadline newDl) {
    Task *currentTask = *head;
    while (currentTask != NULL) {
        if (currentTask->ID == ID) {
            currentTask->Dl = newDl;
            return;
        }
        subTask *currentSubTask = currentTask->subtasks;
        while (currentSubTask != NULL) {
            if (currentSubTask->ID == ID) {
                currentSubTask->Dl = newDl;
                return;
            }
            currentSubTask = currentSubTask->next;
        }
        currentTask = currentTask->next;
    }
}

// xóa task
void deleteTaskOrSubTask(Task **head, int ID) {
    Task *currentTask = *head, *prevTask = NULL;

    while (currentTask != NULL) {
        if (currentTask->ID == ID) {  // Xóa Task lớn
            if (prevTask == NULL) {
                *head = currentTask->next;
            } else {
                prevTask->next = currentTask->next;
            }
            // Giải phóng bộ nhớ của tất cả các SubTask
            subTask *currentSubTask = currentTask->subtasks;
            while (currentSubTask != NULL) {
                subTask *tempSub = currentSubTask;
                currentSubTask = currentSubTask->next;
                free(tempSub->des);
                free(tempSub);
            }
            free(currentTask->des);
            free(currentTask);
            printf("Task ID %d has been deleted successfully.\n", ID);
            return;
        }

        // Kiểm tra trong danh sách SubTask
        subTask *currentSubTask = currentTask->subtasks, *prevSubTask = NULL;
        while (currentSubTask != NULL) {
            if (currentSubTask->ID == ID) {  // Xóa SubTask
                if (prevSubTask == NULL) {
                    currentTask->subtasks = currentSubTask->next;
                } else {
                    prevSubTask->next = currentSubTask->next;
                }
                free(currentSubTask->des);
                free(currentSubTask);
                printf("SubTask ID %d has been deleted successfully.\n", ID);
                return;
            }
            prevSubTask = currentSubTask;
            currentSubTask = currentSubTask->next;
        }

        prevTask = currentTask;
        currentTask = currentTask->next;
    }
    printf("Task or SubTask ID %d not found!\n", ID);
}

// hàm tìm kiếm task theo id
void searchTask(Task *head, int ID) {
    Task *current = head;
    while (current != NULL) {
        if (current->ID == ID) {
            printf("Task found:\n");
            printf("ID: %d\nTitle: %s\nDescription: %s\nCompleted: %d%%\nPriority: %d\nDeadline: %02d:%02d/%02d/%d\n",
                   current->ID, current->title, current->des, current->completed,
                   current->priority, current->Dl.time, current->Dl.date,
                   current->Dl.month, current->Dl.year);
            
            subTask *sub = current->subtasks;
            while (sub) {
                printf("\n   SubTask ID: %d\n   Title: %s\n   Description: %s\n   Completed: %d%%\n   Priority: %d\n   Deadline: %02d:%02d/%02d/%d\n",
                       sub->ID, sub->title, sub->des, sub->completed,
                       sub->priority, sub->Dl.time, sub->Dl.date,
                       sub->Dl.month, sub->Dl.year);
                sub = sub->next;
            }
            return;
        }
        subTask *sub = current->subtasks;
        while (sub) {
            if (sub->ID == ID) {
                printf("SubTask found:\n");
                printf("ID: %d\nTitle: %s\nDescription: %s\nCompleted: %d%%\nPriority: %d\nDeadline: %02d:%02d/%02d/%d\n",
                       sub->ID, sub->title, sub->des, sub->completed,
                       sub->priority, sub->Dl.time, sub->Dl.date,
                       sub->Dl.month, sub->Dl.year);
                return;
            }
            sub = sub->next;
        }
        current = current->next;
    }
    printf("Task ID %d not found!\n", ID);
}
void display(Task *head); // Khai báo hàm display

// so sánh 
void sortTasks(Task **head, int (*compare)(const void *, const void *)) {
    int count = 0;
    Task *current = *head;
    while (current) {
        count++;
        current = current->next;
    }
    if (count < 2) return;

    Task **taskArray = malloc(count * sizeof(Task *));
    current = *head;
    for (int i = 0; i < count; i++) {
        taskArray[i] = current;
        current = current->next;
    }

    qsort(taskArray, count, sizeof(Task *), compare);
    
    for (int i = 0; i < count - 1; i++) {
        taskArray[i]->next = taskArray[i + 1];
    }
    taskArray[count - 1]->next = NULL;
    *head = taskArray[0];
    free(taskArray);
}
// so sánh phần trăm hoàn thành
int compareCompleted(const void *a, const void *b) {
    return ((Task *)a)->completed - ((Task *)b)->completed;
}
// so sánh mứu độ ưu tiên
int comparePriority(const void *a, const void *b) {
    return ((Task *)a)->priority - ((Task *)b)->priority;
}

// Sắp xếp theo trạng thái hoàn thành
void sortByCompletion(Task **head) {
    sortTasks(head, compareCompleted);
    printf("Sorted by completion status:\n");
    display(*head);
}

// Sắp xếp theo độ ưu tiên
void sortByPriority(Task **head) {
    sortTasks(head, comparePriority);
    printf("Sorted by priority:\n");
    display(*head);
}


// Hàm in mô tả với xuống dòng tự động
void printDescription(const char *desc) {
    int len = strlen(desc);
    int i = 0;
    while (i < len) {
        printf("%-50.*s", DESC_WIDTH, desc + i);
        i += DESC_WIDTH;
        if (i < len) printf("|\n|      |                        | ");
    }
}

// Chỉnh sửa hàm display để hiển thị bảng đẹp hơn, có căn chỉnh cột
void display(Task *head) {
    printf("%s+------+----------------------+----------------------------------------------------+------------+------+-----------------+%s\n", ANSI_YELLOW, ANSI_RESET);
    printf("%s| ID   | Title                | Description                                        | Completion | Pri. | Deadline        |%s\n", ANSI_YELLOW, ANSI_RESET);
    printf("%s+------+----------------------+----------------------------------------------------+------------+------+-----------------+%s\n", ANSI_YELLOW, ANSI_RESET);
    
    Task *currentTask = head;
    while (currentTask != NULL) {
        char *completionColor;
        if (currentTask->completed <= 20) {
            completionColor = ANSI_RED_BOLD;
        } else if (currentTask->completed <= 50) {
            completionColor = ANSI_ORANGE;
        } else if (currentTask->completed <= 75) {
            completionColor = ANSI_YELLOW;
        } else {
            completionColor = ANSI_GREEN;
        }
        
        char *priorityColor;
        if (currentTask->priority == 5) {
            priorityColor = ANSI_RED_BOLD;
        } else if (currentTask->priority == 4) {
            priorityColor = ANSI_ORANGE;
        } else if (currentTask->priority == 3) {
            priorityColor = ANSI_YELLOW;
        } else {
            priorityColor = ANSI_GREEN;
        }
        
        printf("| %-4d | %-20.20s | ", currentTask->ID, currentTask->title);
        printDescription(currentTask->des);
        printf(" | %s%-10d%%%s | %s%-4d%s | %02d/%02d/%04d |\n",
               completionColor, currentTask->completed, ANSI_RESET,
               priorityColor, currentTask->priority, ANSI_RESET,
               currentTask->Dl.date, currentTask->Dl.month, currentTask->Dl.year);
        
        subTask *currentSub = currentTask->subtasks;
        while (currentSub != NULL) {
            if (currentSub->completed <= 20) {
                completionColor = ANSI_RED_BOLD;
            } else if (currentSub->completed <= 50) {
                completionColor = ANSI_ORANGE;
            } else if (currentSub->completed <= 75) {
                completionColor = ANSI_YELLOW;
            } else {
                completionColor = ANSI_GREEN;
            }
            
            if (currentSub->priority == 5) {
                priorityColor = ANSI_RED_BOLD;
            } else if (currentSub->priority == 4) {
                priorityColor = ANSI_ORANGE;
            } else if (currentSub->priority == 3) {
                priorityColor = ANSI_YELLOW;
            } else {
                priorityColor = ANSI_GREEN;
            }
            
            printf("| %-4d | %-20.20s | ", currentSub->ID, currentSub->title);
            printDescription(currentSub->des);
            printf(" | %s%-10d%%%s | %s%-4d%s | %02d/%02d/%04d |\n",
                   completionColor, currentSub->completed, ANSI_RESET,
                   priorityColor, currentSub->priority, ANSI_RESET,
                   currentSub->Dl.date, currentSub->Dl.month, currentSub->Dl.year);
            currentSub = currentSub->next;
        }
        
        currentTask = currentTask->next;
    }
    printf("%s+------+----------------------+---------------------------+------------+------+-------------------------------------------+%s\n", ANSI_YELLOW, ANSI_RESET);
}


// Hàm kiểm tra ID đã tồn tại hay chưa
int isIDExist(Task *head, int ID) {
    Task *current = head;
    while (current) {
        if (current->ID == ID) return 1;
        subTask *sub = current->subtasks;
        while (sub) {
            if (sub->ID == ID) return 1;
            sub = sub->next;
        }
        current = current->next;
    }
    return 0;
}

// lưu danh sách vào file
void saveToFile(Task *head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Cannot open file to write.\n");
        return;
    }

    Task *current = head;
    int taskIndex = 1;  

    while (current != NULL) {
        // ghi Task chính
        fprintf(file, "%d. ID (%d): %s\n", 
                taskIndex, 
                current->ID, 
                current->title);
        fprintf(file, "   - Description: %s\n", current->des ? current->des : "n/a");
        fprintf(file, "   - Completed status: %d\n", current->completed);
        fprintf(file, "   - Priority: %d\n", current->priority);
        fprintf(file, "   - Deadline: %02d:%02d/%02d/%d\n",
                current->Dl.time,
                current->Dl.date,
                current->Dl.month,
                current->Dl.year);

        // ghi SubTask nếu có
        subTask *sub = current->subtasks;
        if (sub != NULL) {
            int subIndex = 1;
            while (sub != NULL) {
                fprintf(file, "   %d.%d. ID (%d): %s\n", 
                        taskIndex, 
                        subIndex, 
                        sub->ID, 
                        sub->title);
                fprintf(file, "      - Description: %s\n", sub->des ? sub->des : "n/a");
                fprintf(file, "      - Completed status: %d\n", sub->completed);
                fprintf(file, "      - Priority: %d\n", sub->priority);
                fprintf(file, "      - Deadline: %02d:%02d/%02d/%d\n",
                        sub->Dl.time,
                        sub->Dl.date,
                        sub->Dl.month,
                        sub->Dl.year);
                sub = sub->next;
                subIndex++;
            }
        }

        fprintf(file, "\n"); 
        current = current->next;
        taskIndex++;
    }

    fclose(file);
}

// hàm đọc dữ liệu từ file
Task* loadFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open file %s.\n", filename);
        return NULL;
    }

    Task *head = NULL, *last = NULL;
    char line[1024];

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;  

        if (isdigit(line[0])) {  
            int id, completed, priority, t, d, m, y;
            char title[100], description[500];

            sscanf(line, "%*d. ID (%d): %[^\n]", &id, title);

            fgets(line, sizeof(line), file);
            sscanf(line, "   - Description: %[^\n]", description);

            fgets(line, sizeof(line), file);
            sscanf(line, "   - Completed status: %d", &completed);

            fgets(line, sizeof(line), file);
            sscanf(line, "   - Priority: %d", &priority);

            fgets(line, sizeof(line), file);
            sscanf(line, "   - Deadline: %d:%d/%d/%d", &t, &d, &m, &y);

            Task *newTask = (Task*)malloc(sizeof(Task));
            newTask->ID = id;
            strcpy(newTask->title, title);
            newTask->des = strdup(description);
            newTask->completed = completed;
            newTask->priority = priority;
            newTask->Dl.time = t;
            newTask->Dl.date = d;
            newTask->Dl.month = m;
            newTask->Dl.year = y;
            newTask->subtasks = NULL;
            newTask->next = NULL;

            if (!head) {
                head = newTask;
            } else {
                last->next = newTask;
            }
            last = newTask;
        } 

        else if (strchr(line, '.')) {  
            int parentIndex, subIndex, subID, completed, priority, t, d, m, y;
            char title[100], description[500];

            sscanf(line, "   %d.%d. ID (%d): %[^\n]", &parentIndex, &subIndex, &subID, title);

            fgets(line, sizeof(line), file);
            sscanf(line, "      - Description: %[^\n]", description);

            fgets(line, sizeof(line), file);
            sscanf(line, "      - Completed status: %d", &completed);

            fgets(line, sizeof(line), file);
            sscanf(line, "      - Priority: %d", &priority);

            fgets(line, sizeof(line), file);
            sscanf(line, "      - Deadline: %d:%d/%d/%d", &t, &d, &m, &y);

            Task *parent = head;
            while (parent && parentIndex-- > 1) { 
                parent = parent->next;
            }
            if (parent) {
                subTask *newSub = (subTask*)malloc(sizeof(subTask));
                newSub->ID = subID;
                strcpy(newSub->title, title);
                newSub->des = strdup(description);
                newSub->completed = completed;
                newSub->priority = priority;
                newSub->Dl.time = t;
                newSub->Dl.date = d;
                newSub->Dl.month = m;
                newSub->Dl.year = y;
                newSub->next = parent->subtasks;
                parent->subtasks = newSub;
            }
        }
    }

    fclose(file);
    return head;
}

//giải phóng bộ nhớ
void freeTasks(Task *head) {
    while (head != NULL) {
        Task *temp = head;
        head = head->next;

        subTask *sub = temp->subtasks;
        while (sub != NULL) {
            subTask *subTemp = sub;
            sub = sub->next;
            free(subTemp->des);
            free(subTemp);
        }

        free(temp->des);
        free(temp);
    }
}


int main() {
    Task *taskList = loadFromFile(FILENAME);  // đọc dữ liệu từ file

    int choice, ID, completed, priority;
    int editMode;
    char title[100], des[500], cont;
    Deadline deadline;

    printf ("Welcome!");
    printf ("\n");
    printf("Some notes before starting the program:\n");
    printf("1. Components of a task:\n");
    printf("  - ID: We recommend setting the ID as a meaningful number or in sequential order. Note that each task has a unique ID, and it should not be duplicated.\n");
    printf("  - Title: It should be a brief line describing the task.\n");
    printf("  - Description: A detailed explanation of the task. If there is not one, write \"n/a.\"\n");
    printf("  - Completed status: Calculated as a percentage. You can update this in the edit feature.\n");
    printf("  - Priority: A priority level with 5 being the highest and 1 being the lowest.\n");
    printf("  - Deadline: The first number is the hour. Followed by the day, month, and year.\n");
    printf("2. In the Edit Task feature, you can modify all parts except for the ID.\n");
    printf("3. In the Delete feature, ensure that you delete the correct task as it will be permanently removed. If you're not sure, you can use the Display feature to check before deleting.\n");



    do {
        printf("\n=== TASK MANAGEMENT ===\n");
        printf("1. Add Task\n");
        printf("2. Add SubTask\n");
        printf("3. Edit Task\n");;
        printf("4. Display All Tasks\n");
        printf("5. Delete Task or SubTask\n");
        printf("6. Search task by ID\n");
        printf("7. Sort by Completion Status\n");
        printf("8. Sort by Priority\n");
        printf("9. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar(); // loại bỏ ký tự xuống dòng dư

        switch (choice) {
            case 1: { // thêm Task
                do{
                    printf("Enter Task ID: ");
                    scanf("%d", &ID);
                }
                while(isIDExist(taskList, ID));
                getchar();  
                
                printf("Enter Task Title: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0;

                printf("Enter Task Description: ");
                fgets(des, sizeof(des), stdin);
                des[strcspn(des, "\n")] = 0;

                printf("Enter Completion Status (0-100): ");
                scanf("%d", &completed);
                printf("Enter Priority Level (1-5): ");
                scanf("%d", &priority);
                printf("Enter Deadline (hh dd mm yyyy): ");
                scanf("%d %d %d %d", &deadline.time, &deadline.date, &deadline.month, &deadline.year);

                addTask(&taskList, ID, title, des, completed, priority, deadline);
                saveToFile(taskList, FILENAME);
                break;
            }

            case 2: { // thêm SubTask
                printf("Enter Parent Task ID: ");
                scanf("%d", &ID);
                getchar();  

                Task *parentTask = taskList;
                while (parentTask && parentTask->ID != ID) {
                    parentTask = parentTask->next;
                }
                if (!parentTask) {
                    printf("Task not found!\n");
                    break;
                }

                do{
                    printf("Enter Task ID: ");
                    scanf("%d", &ID);
                }
                while(isIDExist(taskList, ID));
                getchar(); 

                printf("Enter SubTask Title: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0;

                printf("Enter SubTask Description: ");
                fgets(des, sizeof(des), stdin);
                des[strcspn(des, "\n")] = 0;

                printf("Enter Completion Status (0-100): ");
                scanf("%d", &completed);
                printf("Enter Priority Level (1-5): ");
                scanf("%d", &priority);
                printf("Enter Deadline (hh dd mm yyyy): ");
                scanf("%d %d %d %d", &deadline.time, &deadline.date, &deadline.month, &deadline.year);

                addSubTask(parentTask, ID, title, des, completed, priority, deadline);
                saveToFile(taskList, FILENAME);
                break;
            }

            case 3:
                do{
                printf ("which part do you want to edit?\n");
                printf ("1. Edit title\n2. Edit description\n3. Edit completed status \n4. Edit priority\n5. Edit deadline\n");
                scanf("%d", &editMode);
                }
                while (editMode <1 || editMode > 5 );
                switch(editMode){
                    case 1:
                        printf("Enter Task/SubTask ID to edit title: ");
                        scanf("%d", &ID);
                        getchar();
                        printf("Enter New Title: ");
                        fgets(title, sizeof(title), stdin);
                        //title[strcspn(title, "\n")] = 0;
                        editTitle(&taskList, ID, title);
                        //saveToFile(taskList, "tasks.txt");
                        saveToFile(taskList, FILENAME);
                        break;
                    case 2:
                        printf("Enter Task/SubTask ID to edit: ");
                        scanf("%d", &ID);
                        getchar();
                                
                        printf("Enter New Description: ");
                        fgets(des, sizeof(des), stdin);
                        des[strcspn(des, "\n")] = 0;
                        editDescription(&taskList, ID, des);
                        saveToFile(taskList, FILENAME);
                        break;
                    case 3:
                        printf("Enter Task/SubTask ID to edit completion status: ");
                        scanf("%d", &ID);
                        printf("Enter New Completion Status (0-100%%): ");
                        scanf("%d", &completed);
                        editCompleted(&taskList, ID, completed);
                        //saveToFile(taskList, "tasks.txt");
                        saveToFile(taskList, FILENAME);
                        break;
                    case 4:
                        printf("Enter Task/SubTask ID to edit priority: ");
                        scanf("%d", &ID);
                        printf("Enter New Priority Level (1-5): ");
                        scanf("%d", &priority);
                        editPriority(&taskList, ID, priority);
                        //saveToFile(taskList, "tasks.txt");
                        saveToFile(taskList, FILENAME);
                        break;
                    case 5:
                        printf("Enter Task/SubTask ID to edit: ");
                        scanf("%d", &ID);
                        if (choice == 5) {
                            printf("Enter New Completion Status (0-100): ");
                            scanf("%d", &completed);
                            editCompleted(&taskList, ID, completed);
                        } else if (choice == 6) {
                            printf("Enter New Priority Level (1-5): ");
                            scanf("%d", &priority);
                            editPriority(&taskList, ID, priority);
                        } else {
                            printf("Enter New Deadline (hh dd mm yyyy): ");
                            scanf("%d %d %d %d", &deadline.time, &deadline.date, &deadline.month, &deadline.year);
                            editDeadline(&taskList, ID, deadline);
                        }
            
                        saveToFile(taskList, FILENAME);
                        break;
                }
    
            case 4: 
                if (taskList == NULL) {
                    printf("No tasks to display!\n");
                } else {
                    display(taskList);
                }
                break;
            
            case 5:
                printf("Enter Task or SubTask ID to delete: ");
                scanf("%d", &ID);
                deleteTaskOrSubTask(&taskList, ID);
                saveToFile(taskList, FILENAME);  // Cập nhật file sau khi xóa
                break;
            
            case 6:
                printf("Enter Task or SubTask ID to search: ");
                scanf("%d", &ID);
                searchTask(taskList, ID);
                break;
            
            case 7:
                sortByCompletion(&taskList);
                break;
            case 8:
                sortByPriority(&taskList);
                break;

            case 9: 
                printf("Exiting program...\n");
                freeTasks(taskList);
                return 0;    
            default:
                printf("Invalid option! Try again.\n");
            }
    
        printf("\nDo you want to continue? (Press any key to continue, X to exit): ");
        scanf(" %c", &cont);
        getchar(); 
    
    } while (cont != 'X' && cont != 'x'); 
    
    printf("Program terminated.\n");
    freeTasks(taskList);
    return 0;
}
