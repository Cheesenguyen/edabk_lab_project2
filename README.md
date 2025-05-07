# TO-DO LIST SOFTWARE

## 1. Overview

- The program helps manage a To-Do List.
- Provides basic functions to add, edit, delete, sort, and search for tasks.
- Objective: Create a simple tool to track personal task progress.

## 2. Features

- Add new tasks.
- Delete tasks.
- Edit tasks (title or status).
- Sort task list by completion status.
- Search for tasks by title.

![image](https://github.com/user-attachments/assets/5e88c0db-b09c-47dc-bb82-7d20ce224e26)


## 3. Getting Started

### 3.1 Prerequisites

- OS: Ubuntu 22.04.5 LTS.
- gcc (tested sucessfully with 11.4.0).
- git (tested sucessfully with 2.34.1).
- For Ubuntu 22.04.5 LTS: run this command to install `git` and `gcc`:
    ```sh
       sudo apt install git build-essential -y
    ``` 
### 3.2 Quick Start

1. Clone the repository:
   ```sh
   git clone https://github.com/Cheesenguyen/edabk_lab_project2.git
   ```
2. Navigate to `utils` directory:
   ```sh
   cd edabk_lab_project2/utils
   ```
3. Compile the program:
   ```sh
   ./build.sh
   ```
4. Run the program:
   ```sh
   ./run.sh
   ```
5. Clean up generated files (if needed):
   ```sh
   ./clean.sh
   ```

## 4. Configuration (for developer)

To keep the codebase clean and consistent, this project provides a code style guideline in the file `CODE_CONVENTION.md`.

#### About `CODE_CONVENTION.md`
- Contains naming rules, commenting standards, file structure, and formatting practices for C source files.
- Helps contributors follow a unified coding style.

### Formatting Code with `clang-format`

Before committing code, make sure it's properly formatted.

- Format a single file:
  ```sh
  clang-format -i fileName.c
  ```

- Format all `.c` and `.h` files in the current directory:
  ```sh
  clang-format -i *.c *.h
  ```

> 💡 **Tip:** Run `clang-format` before every commit to ensure code style consistency.

