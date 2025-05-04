
# C Programming Standards

This document defines coding conventions for the C programming language, applied to personal projects. The content is compiled and refined from reputable sources: MaJerle C Style, GNOME Coding Guidelines, and GNU Coding Standards. This document focuses on three main areas: **Naming variables and functions**, **Writing comments**, **Organizing file structure in a C project** and **code formating**

---

## 1. Naming Conventions for Variables and Functions

### 1.1 General Rules

- All variable and function names in the program must follow the `camelCase` naming convention.
- Macro and constant names should be written in all uppercase letters and separated by underscores (`_`). Example: `MAX_TASK`, `DEFAULT_BUFFER_SIZE`.
- Source file names should be lowercase and separated by underscores. Example: `main.c`, `input_handler.c`.

### 1.2 Naming Variables

- Variable names must be nouns that clearly describe the function or data the variable stores.
- Avoid using obscure abbreviations or names with no specific meaning.
- Main nouns should come first, followed by additional descriptive nouns.

#### Correct Examples:
- `taskId`: Identifier of a task.
- `taskListLength`: Length of the task list.
- `taskProgress`: Progress of a task.

#### Incorrect Examples:
- `task_id`: Uses `snake_case`, which is not suitable for `camelCase`.
- `prg`: Abbreviation, unclear meaning.
- `data1`: No clear meaning.

### 1.3 Naming Functions

- Function names should follow the structure: **verb + noun** to clearly reflect the action the function performs.
- Function names also use `camelCase`.
- It's recommended to use prefixes indicating the function's scope, such as: `input`, `system`, `output`, to group functions by module.

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

Comments are essential for helping readers understand the purpose and behavior of the code. However, comments should be concise, clear, and meaningful — avoid repeating what is already evident from the code.

### 2.1 Function Header Comments

- Each function should have a comment describing its main purpose.
- Use multi-line comment style `/** ... */` for longer descriptions. Do not use this style for short comments.

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

Each `.c` file in the project should follow a consistent structural order to facilitate readability and maintenance. Below is a suggested order for components in a `.c` file:

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

### A. Basic Configuration

- Based on LLVM style — customizable and clear.
- Even for C code, use `Cpp` language in clang-format.
- Minimum C++ standard is Cpp11 — also suitable for modern C code.
- Do not disable clang-format.
- Do not use a separate format for C++11 initializer lists.
- Do not use macros to define statements.
- Do not infer pointer alignment — follow explicit rules.

### B. Indentation & Layout

- Indent with 4 spaces, no tabs.
- Do not use tab characters, only spaces.
- Indent 4 spaces for continued content.
- Indent `case` inside `switch`.
- Indent inside `struct`, `class` with 2 additional spaces.
- Do not indent function name when breaking to a new line.
- Do not indent namespace contents.
- Indent ObjC blocks with 4 spaces.
- Indent preprocessor directives after `#`.

### C. Alignment

- Align `=` signs in consecutive assignments.
- Align consecutive variable declarations.
- Align long expressions in operations.
- Align end-of-line comments.
- Do not align consecutive macros.
- Left-align escape lines in strings.
- Multi-line parameters/arguments align after `(` if present.

### D. Braces & Line Breaks

- `{` always on its own line (Allman style) — per GNU/GNOME standard.
- Use `{}` for all `if`, `for`, `while`, even single-line bodies.
- Do not put functions, loops, conditions, or `if` blocks on the same line if too short.
- Place `else`, `catch` on their own lines.
- Break line before binary operators like `+`, `-`, `*`, ...
- Break line before `?:` operator for readability.
- Do not put return type on a separate line.
- Do not break template lists into new lines.
- Do not apply line break rules for inheritance lists or constructor initializers.
- Long string literals should be split for readability.

### E. Spaces

- Space before `=` in assignments.
- Space after C-style casts.
- No space after `!`.
- Space after `template` keyword.
- Space before `:` in constructor initializer lists (C++).
- Space before `:` in inheritance (C++).
- Space before `:` in for-range loops.
- Space before `(` in `if`, `for`, `while`, etc.
- No space inside parentheses `()`.
- No space inside square brackets `[]`.
- No space inside C-style casts.
- No space in empty parentheses.
- No space inside angle brackets `< >`.
- No space in container initialization like `{1, 2, 3}`.
- 1 space before end-of-line comments.

### F. Argument & Declaration Breaking

- One argument per line if needed.
- One parameter per line if declaration is long.
- Do not allow multi-line constructor initializers (not applicable to C).
- Do not apply C++ inheritance list rules.
- Do not force all arguments onto next lines.
- Do not force initializers or constructor parameters onto new lines.

### G. Brace Wrapping

- `{` on line after `case`.
- `{` on line after `class`, `struct`, `union`, `enum`, `namespace`, `extern`, `function`, `control` (`if`, `while`, ...).
- Indent for `{ }` blocks.
- Insert empty line if block is empty (empty function/struct/namespace).

### H. Comment Handling

- Allow reformatting of comments for brevity.
- Keep at most 1 consecutive empty line.
- Do not alter namespace closing comments.
- Do not keep blank line at start of block.
- Do not change special pragma comments.

### I. Include Sorting

- Do not sort `#include` lines.
- Do not change order of `using namespace`.
- Preserve order and grouping of `#include` blocks.
- Do not group `#include` by category.
- Do not reorder includes.
- No need for special macros for `foreach`, `typename`, or namespaces.

### J. Other Specific Rules

- Line length limit: 80 characters.
- Do not use raw string literals (`R""()`).
- Do not use macros to define blocks.
- Do not reorder imports in JavaScript.
- Preserve quote style in JavaScript.
- Do not group JavaScript imports.
- Add space after property and before protocol list in ObjC.

### K. Line Break Penalties

- Breaking on assignment: low penalty.
- Breaking before first argument in function call: medium.
- Breaking comment lines: high penalty.
- Breaking after `<<` operator: medium.
- Breaking string literals: medium.
- Breaking in template declarations: low.
- Line exceeding character limit: very high penalty.
- Breaking return type: medium penalty.

