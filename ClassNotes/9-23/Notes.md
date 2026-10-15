# Abstraction and Modularity

## Abstraction

Abstraction is the principle of representing only the essential details of data or actions while hiding the inessential ones.

- Separates **what** something does from **how** it does it.

## Data Abstraction

Data abstraction hides the details of how data is stored and exposes the information needed to use it.

```c
typedef struct {
    int day;
    int month;
    int year;
} Date;
```

## Operation Abstraction

Operation abstraction focuses on what an operation does without requiring the user to know how it works internally.

```c
int x = getx();
setx(10);
```

Here, `getx()` returns the current integer value, while `setx()` changes the stored value.

## Real-World Programs

- Most real-world programs are complex enough that it is impossible to keep track of every detail in the code at any given moment.
- Abstraction helps manage this complexity by allowing us to use modules, which are groups of reusable code and functions.

## Modules

- A module is a potentially separately compiled unit of code that can be reused in other projects.
- Each module has an **interface** that describes the services it provides.
- The C standard library contains many modules. For example, the math library is accessed through `math.h`.
- Programmers can also define and use their own modules.

## Advantages of Modularity

- Dividing a program into modules cleanly separates the **what** from the **how**.
- Useful modules can be reused across multiple programs.
- Code is easier to maintain.
  - If there is a problem with a function, we only need to update and recompile the module containing that function.

> **Key takeaway:** Abstraction hides unnecessary implementation details, while modularity organizes related functionality into reusable and maintainable units.
    
## Using Custom Modules

A custom module is usually split into two files:

- A header file (`.h`) that contains the module's public interface, such as function declarations.
- A source file (`.c`) that contains the implementation of those functions.

Any program that uses the module includes the header file. The module's source file is compiled and linked with the program.

### Example: A Basic Fake Module

#### 1. Create the interface: `fake_module.h`

```c
#ifndef FAKE_MODULE_H
#define FAKE_MODULE_H

int getx(void);
void setx(int value);

#endif
```

The header tells other files which functions the module provides without exposing how they work.

#### 2. Create the implementation: `fake_module.c`

```c
#include "fake_module.h"

static int x = 0;

int getx(void) {
  return x;
}

void setx(int value) {
  x = value;
}
```

The source file defines how `getx()` reads the value and how `setx()` changes it. The `#include "fake_module.h"` line helps ensure the definitions match the declarations.

#### 3. Use the module in `main.c`

```c
#include <stdio.h>
#include "fake_module.h"

int main(void) {
  setx(42);
  printf("x = %d\n", getx());
  return 0;
}
```

The main program only needs to know the module's interface. It does not need to know where the value is stored.

#### 4. Compile the program

Compile both source files together:

```bash
clang main.c fake_module.c -o main
./main
```

This produces the output:

```text
x = 42
```

## Specification Files and Implementation Files

When creating a custom C module, the module is commonly divided into a **specification file** and an **implementation file**.

| File | Common extension | Purpose |
| --- | --- | --- |
| Specification file | `.h` | Describes what the module provides through function declarations, constants, and types. |
| Implementation file | `.c` | Defines how the module provides those functions and performs its work. |

### Specification File

The specification file is also called a **header file**. It acts as the module's public interface:

- It contains function prototypes, such as `int getx(void);` and `void setx(int value);`.
- It tells other source files which functions they are allowed to call.
- It should expose only the information needed to use the module.
- Include guards prevent the header from being included more than once in the same compilation unit.

For the fake module, `fake_module.h` is the specification file:

```c
#ifndef FAKE_MODULE_H
#define FAKE_MODULE_H

int getx(void);
void setx(int value);

#endif
```

### Implementation File

The implementation file contains the actual function definitions:

- It includes its corresponding header file.
- It contains the code that performs each operation.
- Its internal details can change without requiring changes to programs that only use the interface.

For the fake module, `fake_module.c` is the implementation file:

```c
#include "fake_module.h"

static int x = 0;

int getx(void) {
  return x;
}

void setx(int value) {
  x = value;
}
```

### How the Files Work Together

The application includes the specification file, while the compiler receives both the application and implementation files:

```c
// main.c
#include <stdio.h>
#include "fake_module.h"

int main(void) {
  setx(42);
  printf("x = %d\n", getx());
  return 0;
}
```

```bash
clang main.c fake_module.c -o main
```

This arrangement allows `main.c` to use `getx()` and `setx()` without knowing how the value is stored. The header provides the **what**, and the source file provides the **how**.

## `#ifndef`, `#define`, and `#endif`

These are C preprocessor directives commonly used to create an **include guard** in a header file. An include guard prevents the contents of a header from being included more than once during compilation.

```c
#ifndef FAKE_MODULE_H
#define FAKE_MODULE_H

int getx(void);
void setx(int value);

#endif
```

### `#ifndef`

`#ifndef` means **if not defined**. It checks whether `FAKE_MODULE_H` has not already been defined.

### `#define`

`#define` creates the `FAKE_MODULE_H` preprocessor symbol. It is needed because `#ifndef` only checks whether the symbol exists; it does not create the symbol by itself.

The name is arbitrary, but it should be unique to the header. It is usually written in uppercase and based on the file name.

### `#endif`

`#endif` marks the end of the conditional section that began with `#ifndef`.

### Why `#define` Is Needed

Consider what happens when `fake_module.h` is included:

1. **First inclusion:** `FAKE_MODULE_H` is not defined, so `#ifndef` is true. The preprocessor includes the declarations and then reaches `#define FAKE_MODULE_H`.
2. **Later inclusion:** `FAKE_MODULE_H` is now defined, so `#ifndef` is false. The preprocessor skips everything through `#endif`.

Without `#define FAKE_MODULE_H`, the condition would be true every time the header was included, so the declarations would not be protected from being processed repeatedly.

Together, these directives mean:

1. If `FAKE_MODULE_H` has not been defined, include the declarations.
2. Define `FAKE_MODULE_H` so the declarations are not included again.
3. End the conditional section with `#endif`.

Include guards are especially useful when multiple source files include the same header, because they prevent duplicate declarations and related compilation errors.

## Compiling and Linking Module Files

When a program is divided into multiple source files, each `.c` file can be compiled separately. The resulting object files can then be linked together to create one executable.

### Compiling with `gcc -c`

The `-c` option tells `gcc` to compile a source file into an **object file** without linking it into an executable.

```bash
gcc -c fake_module.c -o fake_module.o
gcc -c main.c -o main.o
```

These commands create:

- `fake_module.o`, which contains the compiled implementation of `getx()` and `setx()`.
- `main.o`, which contains the compiled `main()` function and references to `getx()` and `setx()`.

The header file does not need to be compiled separately. It is included in each source file during preprocessing.

### Linking Object Files Together

The linker combines the object files and resolves references between them. In this example, it connects the calls in `main.o` to the function definitions in `fake_module.o`:

```bash
gcc main.o fake_module.o -o main
```

This creates the executable named `main`. It can then be run with:

```bash
./main
```

The output is:

```text
x = 42
```

### Compile and Link in One Command

`gcc` can also compile and link both source files in one command:

```bash
gcc main.c fake_module.c -o main
```

This performs the separate compilation steps and linking automatically. The result is the same `main` executable, but the intermediate object files may not be kept for later use.

## Information Hiding

**Information hiding** is the practice of keeping implementation details private and exposing only the information other parts of a program need to use a module.

Information hiding works closely with abstraction:

- **Abstraction** focuses on what a module does.
- **Information hiding** keeps the details of how it does that private.

### Information Hiding in the Fake Module

The header file exposes only the public functions:

```c
int getx(void);
void setx(int value);
```

The implementation file hides the variable that stores the value:

```c
static int x = 0;
```

Because `x` is declared `static` at file scope, it can only be accessed inside `fake_module.c`. Code in `main.c` cannot directly change `x`; it must use `getx()` or `setx()`.

```c
// main.c
#include "fake_module.h"

int main(void) {
  setx(42);
  return getx();
}
```

### What `main.c` Cannot Do

The variable `x` is not declared in `fake_module.h`, and its `static` declaration limits its scope to `fake_module.c`. Therefore, this code in `main.c` does not compile:

```c
// This is an invalid example.
#include "fake_module.h"

int main(void) {
  x = 42;       // Error: x is not visible in main.c.
  return x;     // Error: x is not visible in main.c.
}
```

The compiler reports that `x` is undeclared because `main.c` cannot see the private variable. The correct approach is to use the public functions:

```c
int main(void) {
  setx(42);
  return getx();
}
```

This prevents `main.c` from changing `x` in an uncontrolled way and ensures that all access to the value goes through the module's interface.

This protects the module's internal state from accidental changes. The implementation can also change later, such as storing the value in a different way, without requiring changes to programs that use the header.

### Benefits of Information Hiding

- Prevents other parts of the program from directly changing internal data.
- Reduces the number of details a programmer must understand at once.
- Makes modules easier to test and maintain.
- Allows the implementation to change while the public interface stays the same.

## 