
# C Programming Standards

This document defines coding conventions for the C programming language, applied to personal projects. The content is compiled and refined from reputable sources: MaJerle C Style, GNOME Coding Guidelines, and GNU Coding Standards. This document focuses on three main areas: **Naming variables and functions**, **Writing comments**, **Organizing file structure in a C project** and **code formating**

---

## 1. Naming Conventions for Variables and Functions

### 1.1 General Rules

- All variable and function names in the program must follow the `camelCase` naming convention.
- Macro and constant names must be written in all uppercase letters and separated by underscores (`_`). Example: `MAX_TASK`, `DEFAULT_BUFFER_SIZE`.
- Source file names must be lowercase and separated by underscores. Example: `main.c`, `input_handler.c`.

### 1.2 Naming Variables

- Variable names must be nouns that clearly describe the function or data the variable stores.
- Avoid using obscure abbreviations or names with no specific meaning.
- Main nouns must come first, followed by additional descriptive nouns. Adjective goes first and follow by noun.

#### Correct Examples:
- `taskId`: Identifier of a task.
- `taskListLength`: Length of the task list.
- `taskProgress`: Progress of a task.

#### Incorrect Examples:
- `task_id`: Uses `snake_case`, which is not suitable for `camelCase`.
- `prg`: Abbreviation, unclear meaning.
- `data1`: No clear meaning.

### 1.3 Naming Functions

- Function names must follow the structure: **verb + noun** to clearly reflect the action the function performs.
- Function names also use `camelCase`.
- Must use prefixes indicating the function's scope, such as: `input`, `system`, `output`, to group functions by module.

#### Correct Examples:
- `inputGetOption()`: Get option from the user.
- `systemDeleteTask()`: Delete a task from the system.
- `outputViewTasks()`: Display the task list on screen.

#### Incorrect Examples:
- `INPUT_get_option()`: Incorrect naming style (`snake_case` and uppercase).
- `handle()`: Too generic, unclear what it handles.
- `ProcTask()`: Unclear name, does not indicate a specific action.

---

## 2. Commenting Guidelines

Comments are essential for helping readers understand the purpose and behavior of the code. However, comments must be concise, clear, and meaningful — avoid repeating what is already evident from the code.


### 2.1 Function Header Comments

- Each function should have a comment describing its main purpose.
- Use multi-line comment style `/** ... */` for longer descriptions and `//` for short one. Do not use `/** ... */` for short comments.

#### Example:

```c
/**
 * getProgress - Get progress from the user
 * Returns: An integer from 1 to 100
 */
int getProgress(void);
```

### 2.2 In-function Comments

- Use single-line comments `//` to explain complex or unclear lines of code inside the function body.  
- There is no need to comment on lines that are already self-explanatory.

#### Example:

```c
// Remove the '\n' character in the input string
list[i][strcspn(list[i], "\n")] = '\0';
```

### 2.3 Common Mistakes When Writing Comments

- Do not write vague or redundant comments.

#### Incorrect Example:

```c
/* get progress */  // Incomplete comment
// process data     // Unclear what is being processed
```

---

## 3. Rules for Organizing File Structure in a C Project

Each `.c` file in the project must follow a consistent structural order to facilitate readability and maintenance. Below is a order for components you must follow in a `.c` file:

1. Include standard system libraries (`#include <...>`)
2. Include project header files (`#include "..."`)
3. Define macros (`#define`)
4. Declare global variables (if needed)
5. Declare prototypes for internal functions (static function prototype)
6. Implement functions (excluding main)
7. Define functions

### Correct Example:

```c
// 1. System libraries
#include <stdio.h>
#include <string.h>

// 2. Project header files
#include "input.h"
#include "system.h"

// 3. Macro definitions
#define MAX_TASKS 100

// 4. Global variables
static int taskCount = 0;

// 5. Prototype for internal function
static void normalizeInput(char *str);

// 6. Function definitions
void inputReadTask(char *buffer) {
    fgets(buffer, 100, stdin);
    normalizeInput(buffer);
}

static void normalizeInput(char *str) {
    str[strcspn(str, "\n")] = '\0';
}
```

### Incorrect Example:

```c
// Defining function before include → incorrect
void inputReadTask(char *buffer) {
    fgets(buffer, 100, stdin);
}

// Including project file after macro → incorrect order
#define MAX_TASKS 100
#include "input.h"
#include <stdio.h>

// No prototype for internal function → lack of clarity
static void normalizeInput(char *str) {
    str[strcspn(str, "\n")] = '\0';
}
```


## 4. Code Formatting
### 4.1 Code Formatting without example

### A. Basic Configuration
- Based on LLVM style — customizable and clear.
- Even for C code, use Cpp language in clang-format.
- Do not disable clang-format.
- Do not use macros to define statements.
- Do not infer pointer alignment — follow explicit rules.

### B. Indentation & Layout
- Indent with 4 spaces, no tabs.
- Do not use tab characters, only spaces.
- Indent 4 spaces for continued content.
- Indent (4 spaces) case inside switch.
- Do not indent function name when breaking to a new line.

### C. Alignment
- Align = signs in consecutive assignments.
- Align consecutive variable declarations.
- Align long expressions in operations.
- Align end-of-line comments.
- Do not align consecutive macros.
- Left-align escape lines in strings.
- Multi-line parameters/arguments align after ( if present.

### D. Braces & Line Breaks
- { always on its own line (Allman style) — per GNU/GNOME standard.
- Use {} for all if, for, while, even single-line bodies.
- Do not put functions, loops, conditions, or if blocks on the same line if too short.
- Break line before binary operators like +, -, *, ...
- Break line before ?: operator for readability.
- Do not put return type on a separate line.
- Long string literals should be split for readability.

### E. Spaces
- Space before = in assignments.
- Space after C-style casts.
- No space after !.
- Space before ( in if, for, while, etc.
- No space inside parentheses ().
- No space inside square brackets [].
- No space inside C-style casts.
- No space in empty parentheses.
- No space inside angle brackets < >.
- No space in container initialization like {1, 2, 3}.
- 1 space before end-of-line comments.

### F. Argument & Declaration Breaking
- Left align parameter each line if declaration is long and write on next line after function name.
- Do not force all arguments onto next lines.

### G. Brace Wrapping
- { on next line after case.
- { on next line after struct, union, enum, extern, function, control (if, while, ...).
- Indent for { } blocks.
- Insert empty line if block is empty (empty function/struct/namespace).

### H. Comment Handling
- Allow reformatting of comments for brevity.
- Keep at most 1 consecutive empty line if needed
- Do not keep blank line at start of block.
- Do not change special pragma comments.

### I. Include Sorting
- Sort `#include` lines if needed. Make sure it follow the file structure

### J. Other Specific Rules
- Line length limit: 80 characters.
- Do not use macros to define blocks.

### 4.2 Code Formatting example
### A. Basic Configuration

- Based on LLVM style — customizable and clear.

*Correct example:*  
  ```c
  int main(void) {
    printf("Hello, world!\n");
    return 0;
  }
  ```
- Even for C code, use `Cpp` language in clang-format.

*Correct example .clang-format file* 
```c
Language:        Cpp
BasedOnStyle:    LLVM
IndentWidth:     2
```
- Do not disable clang-format.
*Bad example*
```c
// clang-format off
int a=1;int b=2;int c=3;
``` 
*Correct example*
```c
int a = 1;
int b = 2;
int c = 3;
```
- Do not use macros to define statements.

*Bad example*
```c
#define BEGIN int main() {
#define END return 0; }

BEGIN
  printf("Hello\n");
END
``` 
*Correct example*
```c
int main(void) {
  printf("Hello\n");
  return 0;
}
```
- Do not infer pointer alignment — follow explicit rules.

*Bad example*
```c
int* a, *b;
int * c;
``` 
*Correct example*
```c
int *a;
int *b;
```

### B. Indentation & Layout

- Indent with 4 spaces, no tabs.

*Correct example*
```c
int main(void) {
    printf("Hello\n");
    return 0;
}
```
- Do not use tab characters, only spaces.

*Bad example*
```c
int value1 = 10;
int   value2 = 10;
``` 
*Correct example*
```c
int value1 = 10;
int value2 = 10;
```
- Indent 4 spaces for continued content.
 
*Correct example*
```c
if (very_long_condition_name && another_condition &&
    yet_another_condition) {
    do_something();
}
```
- Indent (4 spaces) `case` inside `switch`.

*Correct example*
```c
switch (value) {
    case 1:
        printf("One\n");
        break;
    case 2:
        printf("Two\n");
        break;
    default:
        printf("Default\n");
}
```
- Do not indent function name when breaking to a new line.

*Correct example*
```c
void my_very_long_function_name(
    int a, int b, int c) {
    // function body
}
```
- No indent preprocessor directives after `#`.

*Correct example*
```c
#ifdef DEBUG
#define LOG(x) printf(x)
#else
#define LOG(x)
#endif
```
### C. Alignment

- Align `=` signs in consecutive assignments.

*Correct example*
```c
int count    = 10;
int maxCount = 100;
int minCount = 0;
```
- Align consecutive variable declarations.

*Correct example*
```c
int     a;
float   b;
double  c;
```
- Align long expressions in operations.

*Correct example*
```c
int result = value1 + value2 +
             value3 + value4;

```
- Align end-of-line comments.

*Correct example*
```c
int maxUsers   = 100;  // maximum users allowed
int timeout    = 30;   // timeout in seconds
int retryCount = 5;    // retry attempts
```
- Do not align consecutive macros.
*Bad example*
```c
#define MAX_SIZE 100
#define MIN_SIZE 10
#define NAME     "Config"

*Correct example*
```c
#define MAX_SIZE 100
#define MIN_SIZE 10
#define NAME "Config"
```
- Left-align escape lines in strings.

*Correct example*
```c
const char *query =
    "SELECT id, name, age "
    "FROM users "
    "WHERE age > 18";
```
- Multi-line parameters/arguments align after `(` if present.

*Correct example*
```c
void do_something_with_params(int param1,
                              int param2,
                              int param3);
```
### D. Braces & Line Breaks

- `{` always on its own line (Allman style) — per GNU/GNOME standard.

*Correct example*
```c
if (x > 0)
{
    printf("Positive\n");
}
```
- Use `{}` for all `if`, `for`, `while`, even single-line bodies.

*Correct example*
```c
if (x > 0)
{
    do_something();
}
```
- Do not put functions, loops, conditions, or `if` blocks on the same line if too short.

*Correct example*
```c
if (ready)
{
    start();
}
```
- Place `else` on their own lines.

*Correct example*
```c
if (error)
{
    handle_error();
}
else
{
    proceed();
}
```
- Do not break line before operators like `+`, `-`, `*`, ...

*Bad example*
```c
int result = value1
           + value2
           + value3;
```
*Correct example*
```c
int result = value1 + value2 + value3;
```
- Do not break line before `?:` operator for readability.

*Bad example*
```c
int value = condition
          ? 10
          : 20;
```
*Correct example*
```c
int value = condition? 10: 20;
```
- Do not put return type on a separate line.

*Bad example*
```c
int
compute_sum(int a, int b)
{
    return a + b;
}
```

*Correct example*
```c 
int compute_sum(int a, int b)
{
    return a + b;
}
```
- Long string literals should be split for readability.

*Correct example*
```c
const char *msg =
    "This is a long message "
    "that is split across lines.";
```
### E. Spaces

- Space before `=` in assignments.

*Correct example*
```c
int a = 10;
```
- Space after C-style casts.

*Correct example*
```c
int a = (int) 10;
```
- No space after `!`.

*Correct example*
```c
if (c != 10)
```
- Space before `(` in `if`, `for`, `while`, etc.

*Correct example*
```c
if (x > 0)
{
    do_something();
}
```
- No space inside parentheses `()`.

*Correct example*
```c
printf("Hello\n");
```
- No space inside square brackets `[]`.

*Correct example*
```c
int value = array[5];
```
- No space inside C-style casts.

*Correct example*
```c
int result = (int) value;
```
- No space in empty parentheses.

*Correct example*
```c
void log_message()
{
    // ...
}
```
- No space inside angle brackets `< >`.

*Correct example*
```c
#include <stdio.h>
```
- No space in container initialization like `{1, 2, 3}`.

*Bad example*
```c
int arr[] = {1, 2, 3 };
```
*Correct example*
```c
int arr[] = {1, 2, 3};
```
- 1 space before end-of-line comments.

*Correct example*
```c
int retries = 3; // number of attempts
```

### F. Argument & Declaration Breaking

- Left align parameter each line if declaration is long and write on next line after function name.

*Correct example*
```c
void long_declaration(
    int param1, int param4, int param5, int soMuchParamBehindHere
    double param2, char* param3
)
{
    // function body
}
```
- Do not force all arguments onto next lines.

*Correct example*
```c
void function(int x, int y)
{
    // function body
}
```
### G. Brace Wrapping

- `{` on line after `case`.

*Correct example*
```c
switch (x) {
    case 1:
    {
        printf("One\n");
        break;
    }
    case 2:
    {
        printf("Two\n");
        break;
    }
    default:
    {
        printf("Default\n");
        break;
    }
}

```
- Indent for `{ }` blocks.

*Correct example*
```c
if (x > 0)
{
    do_something();
    printf("Done\n");
}
```
- Insert empty line if block is empty (empty function/struct/namespace).


*Correct example*
```c
void empty_function()
{
    // Empty function
}

struct EmptyStruct
{
    // Empty struct
};
```
### H. Comment Handling

- Allow reformatting of comments for brevity.

*Bad example*
```c
// This function calculates the area of a circle using the formula pi * radius^2.
float area(float radius) {
    return 3.14 * radius * radius;
}
```
*Correct example*
```c
// This function calculates the area of a circle.
float area(float radius) {
    return 3.14 * radius * radius;
}
```

- Do not keep blank line at start of block.

  *Bad example*
```c

if (x > 0)
{

    printf("Positive\n");
}
```
*Correct example*
```c
if (x > 0)
{
    printf("Positive\n");
}
```
- Keep at most 1 consecutive empty line if needed.

*Correct example*
```c
int x = 10;

int y = 20;
```
- Do not change special pragma comments.

*Bad example*
```c
//#pragma once

// some code
```
*Correct example*
```c
#pragma once
// some code

```

### I. Include Sorting

- Sort `#include` lines if needed. Make sure it follow the file structure

*Bad example*
```c
#include "myheader.h"
#include <stdlib.h>
#include <stdio.h>
```
*Correct example*
```c
#include <stdio.h>
#include <stdlib.h>

#include "myheader.h"

```

### J. Other Specific Rules

- Line length limit: 80 characters.

*Bad example*
```c
const char *msg = "This is a long message that definitely goes beyond the eighty character limit.";
```
*Correct example*
```c
const char *msg = "This is a short message within eighty characters.";
```
- Do not use macros to define blocks.
*Bad example*
```c
#define BLOCK \
    printf("Start\n"); \
    printf("End\n")

void process()
{
    BLOCK;
}
```
*Correct example*
```c
void process()
{
    printf("Start\n");
    printf("End\n");
}
```

## 5. How to Use clang-format

To format a single file, use the command:

```
clang-format -i fileName.c
```

To format multiple files, use the command:

```
clang-format -i *.c *.h
```

with extensions matching all the files you want to format.

It is recommended to run `clang-format` before each commit to ensure consistent formatting.
