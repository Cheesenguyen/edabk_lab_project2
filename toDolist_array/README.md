# To-Do List Software Introduction

## I. Software Introduction

### Features include:
1. Add task
2. Delete task
3. Edit task by title or status
4. Sort list of tasks (by status)
5. Search task (by title)

### Detailed Usage Guide for Each Function:
- When the program starts, a menu appears, and the user must select numbers from 0 to 5 according to the menu to choose a function.
1. **Add task**: The user enters the task title and completion percentage. The ID will be added automatically.
2. **Delete task**: The user must enter the ID to delete. You can find the task ID using the search function.
3. **Edit task**: The user can edit all fields in the task and press Enter if they do not want to modify a field.
4. **Sort list of tasks**: Tasks are sorted in descending order of completion and automatically displayed. This function helps users identify tasks that are the least completed.
5. **Search task**: The user enters a few characters from the title. The program will display all task IDs containing the corresponding characters.

---

## II. Script Directory Introduction

### 1. The directory includes the following files:
- **compile**: Compiles the program.
- **run**: Runs the program.
- **clear**: Deletes generated files during execution (in this case, `main`).
- **clone**: Clones a repository from GitHub.

### 2. Usage:
- For the `compile`, `run`, and `clear` files, the user only needs to run `./*.sh`, where `*` is the file name.
- For the `clone` file, run the file using the command `source ./clone.sh <repository_link>`.