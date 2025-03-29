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

![image]![alt text](image.png)

## 3. Getting Started

### 3.1 Prerequisites

- The program can run on Windows 11 24H2 (OS Build 26100.3476) and Ubuntu 22.04.5 LTS.
- A C compiler is required to run the program.

### 3.2 Quick Start

1. Prepare
   
   **Make sure to have enough tools include Git and at least 01 shell on at least 01 OS (Window or Ubuntu). If not, you cannot run those command in part 2 and later.**

   1.1 Download and open command line interface  
   
   The following commands have been successfully tested on:

   - **PowerShell version 5.1.26100.2161 for Windows 11 24H2 (OS Build 26100.3476)**: Runs normally.  
     > Note: PowerShell is pre-installed on machines from Windows 7 and later.  

   - **WSL version 2.3.26.0 for Ubuntu 22.04.5 LTS**:  
     WSL and Ubuntu are not pre-installed on Windows. We have to install Ubuntu, follow these steps:  

     1. Open **PowerShell** or **Command Prompt** as administrator.  
     2. Run the following command:  
        ```sh
        wsl --install
        ```
        This command will install WSL with **Ubuntu** as the default distribution on **Windows 11**. After install WSL you can use those command below.
   -  **Git version 2.48.1.windows.1 on Windows or Git version git version 2.34.1 on Ubuntu.** 

      detail instruction for downloading Git in part 1.2 because you also need it to use 
      ```sh
      git clone
      ``` 
   
   1.2 Download git 
   - Use 
    ```sh
   git --version
   ``` 
   on PowerShell, Command Prompt or WSL to check is you have git on your computer. If you see the version number, the installation was successful!

   - If you do not have Git on your computer, please download Git.
     + For **WSL version 2.3.26.0 with Ubuntu 22.04.5 LTS**, use these commands:
       
       ```sh
       apt --version
       ``` 
       Check if `apt` is installed. If not, you need to install it. However, on most Ubuntu versions, `apt` is the default package manager.
       
       ```sh
       sudo apt update -y
       ``` 
       Update `apt` to the latest version.
       
       ```sh
       sudo apt install git -y
       ```
       Install Git.
       
       ```sh
       git --version
       ``` 
       Check if Git was installed successfully. If you see the version number, the installation was successful!
     
     + For **Windows version 24H2 (OS Build 26100.3476)**: Installing Git via the command line is more complicated, so we recommend using the **GUI installer**.


- **Note:** Command Prompt (`cmd.exe`) will not work with these command lines.


2. Clone the repository at the diretory where you want to save this project:
   ```sh
   git clone https://github.com/Cheesenguyen/edabk_lab_project2.git
   ```
3. Navigate to the project directory at the diretory you have choosen:
   ```sh
   cd edabk_lab_project2/utils
   ```
4. Compile the program at utils:
   ```sh
   ./build.sh
   ```
5. Run the program at utils:
   ```sh
   ./run.sh
   ```
6. Clean up generated files (if needed) at utils:
   ```sh
   ./clear.sh
   ```

##
