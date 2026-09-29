# Lecture 1 — Introduction and Our C Toolkit

## 1. What an Operating System Is

An **operating system (OS)** is the software layer between the hardware and application programs. Think of it as the computer's **resource manager**:

- **Resource allocation** — CPU time, memory, disk, and devices are finite. The OS decides who gets what, keeps track of who owns what, and resolves conflicting requests.
- **Abstraction** — programs shouldn't need to know the specifics of every CPU, disk, keyboard, or monitor. The OS hides those details so "Hello World" works the same on any hardware combination.
- **Referee and facilitator** — many programs run at once. The OS enforces rules so they share fairly, and it also helps programs that want to *cooperate*.

A key point: unlike a human secretary who works *in parallel* with you, the OS typically runs *while programs are paused*, and has to put itself to sleep to let programs run. (This idea comes back in Lecture 2 with interrupts and traps.)

## 2. Systems Programming

Systems programming is the layer **just above the OS kernel**: programs that ship with the OS and are useful tools, but are not part of the kernel itself. Examples:

- **File manipulation** — create, delete, copy, rename, print files and directories.
- **Communication** — remote login, file transfer, downloads, messaging.
- **Process and thread management** — creating, coordinating, and cleaning up processes/threads.

Why it's harder than "regular" programming:

- You may need hardware knowledge, and debugging tools may be limited.
- You **must handle concurrency** — many programs may use your system program at the same time.
- The defining feature: there are things the OS **won't let a program do directly**. Instead, the program must **ask the OS to do it** (a *system call*). Learning to use OS-provided functionality is central to writing non-trivial programs.

## 3. Concurrency vs. Parallelism

- **Concurrent**: two or more actions are *in progress* at the same time (they may take turns on one CPU).
- **Parallel**: two or more actions are *executing simultaneously* (actually at the same instant, e.g. on different cores).

Every parallel program is concurrent, but not every concurrent program is parallel.

Concurrency buys you **speed** (more work in the same time), but costs you **difficulty** — both in writing the code and in *being sure it is correct*. A concurrency bug can make a program:

1. consistently wrong,
2. give different answers on different runs with the same input, or
3. right *most* of the time.

None of these is acceptable. (A payroll system that's right 98% of the time with 26 pay periods/year will very likely mess up your pay within two years.) **Correctness matters.**

## 4. The C Toolkit

C is **procedural**: it has functions, but no classes, inheritance, interfaces, or access modifiers.

### 4.1 Functions, headers, and prototypes

```c
int function_name( double arg1, int arg2, char arg3 ) { ... }
```

Headers are pulled in with the preprocessor:

```c
#include <stdio.h>   /* printf, fprintf, FILE ... */
#include <stdlib.h>  /* malloc, free, atoi, exit ... */
#include <string.h>  /* memset, strlen, memcpy ... */
```

If the compiler says it doesn't know a function: check spelling, check the number of parameters, **then check you included the right header**. An *"implicit declaration of function"* warning means a header is missing — fix it even though it compiles.

**The single-pass rule.** The C compiler reads top-to-bottom and won't look ahead. If `foo()` calls `bar()` and `bar()` is defined *below* `foo()`, you get an error. Two fixes:

1. Move `bar()` above `foo()`, or
2. Declare a **function prototype** near the top of the file:

```c
int bar( int v1, int v2 );          /* prototype: "this exists, trust me" */

int foo() { return bar( 7, 11 ); }
int bar( int v1, int v2 ) { return v1 * v2; }
```

### 4.2 Small language details

- **No built-in `bool`.** Use `int` (0 = false, non-zero = true), or `#include <stdbool.h>` for `bool`, `true`, `false`.
- **C99 and later** let you declare variables anywhere (e.g. `for (int i = 0; ...)`). Pre-C99 required declarations at the top of a block. Assume C99+ in this course.
- **Comments**: `/* ... */` always works; `// ...` is C99+.

### 4.3 Structures and `typedef`

C's only programmer-defined compound type is the **struct** — a bundle of variables, nothing more (no methods).

```c
struct point {
    double x;
    double y;
    double z;
};

struct point p1;
p1.x = 1.9;       /* members accessed with the dot operator */
```

Writing `struct point` everywhere is tedious, so `typedef` gives the type a shorter name:

```c
typedef struct point {   /* the tag "point" is optional here */
    double x, y, z;
} point_t;

point_t add( point_t p1, point_t p2 );
```

The `_t` suffix is a convention (you'll see `pid_t`, `size_t`, `pthread_t`). It is discouraged for your *own* types, but it's convention, not law.

### 4.4 Memory: global, stack, heap

| Kind | Where declared | Lifetime | Notes |
|------|----------------|----------|-------|
| **Global** | Outside any function | Entire program | May be zero-initialized, but don't rely on it. Overuse = poor design. |
| **Stack** | Local variables inside a function | Until the function returns | Limited size. **Uninitialized locals contain garbage.** |
| **Heap** | Explicitly requested with `malloc` | Until you `free` it | Very large but not infinite. You own it. |

**Uninitialized variables** are one of the most common bugs, even among experienced programmers. Always initialize.

#### `malloc`, `sizeof`, `free`

`malloc(n)` asks for `n` bytes on the heap and returns a pointer to them (or `NULL` on failure). The memory is **not initialized**.

How many bytes is an `int`? The C standard only gives *minimum* sizes for many types — the real size is implementation-specific. So always use **`sizeof`**, which the compiler replaces with the correct size:

```c
int *x = malloc( sizeof( int ) );   /* no cast needed in C (unlike C++) */
*x = 0;                             /* initialize it! */
...
free( x );                          /* give it back */
```

`sizeof( point_t )` automatically stays correct even if you later add fields to the struct.

Rules for `free`:

- Every `malloc` needs a matching `free`, possibly in a different function. When you allocate, **decide where it will be freed**. Forgetting = **memory leak** (slow death for long-running programs).
- If a library function returns allocated memory, its documentation will say whether *you* must free it.
- `free` only marks memory as available — it doesn't erase it. **Use-after-free** may "work" in testing and crash in production. Convention: set the pointer to `NULL` after freeing so misuse crashes immediately.
- **Double free** → very likely immediate crash.
- **Freeing stack memory** → crash.

### 4.5 Pointers: `*`, `&`, `->`

- `*` in a declaration makes a pointer: `int *p;`
- `*` in an expression **dereferences**: `*p = 5;` writes to what `p` points at.
- `&` is **address-of**: gives a pointer to an existing variable.

```c
int work( int *x, int *y ) { return *x + *y; }

void baz() {
    int a = 2, b = 4;           /* stack variables */
    int z = work( &a, &b );     /* pass their addresses */
}
```

⚠️ Stack memory does not outlive its function — and sometimes not even one loop iteration. Passing `&local` somewhere that keeps it around (e.g. another thread, later in the course) is a classic bug source.

**Arrow operator**: for a pointer to a struct, `p1->x = 99.9;` is exactly the same as `(*p1).x = 99.9;`.

### 4.6 Arrays

```c
int array[10];                          /* stack array: garbage contents */
int *array = malloc( 10 * sizeof(int) ); /* heap array */
array[0] = 5;                            /* same syntax either way */
free( array );                           /* same free for arrays */
```

A pointer can point to one element or to the first of many — C doesn't distinguish.

Initialize arrays with a loop, or faster with **`memset`** (from `<string.h>`):

```c
memset( array, 0, 10 * sizeof( int ) );  /* pointer, byte value, NUMBER OF BYTES */
```

`memset` sets each **byte** to the value (converted to `unsigned char`), so it is really only useful for setting to 0. `memset(arr, 1, ...)` on an int array does *not* give you 1s — each int becomes `0x01010101`.

**No bounds checking.** Reading element 12 of a 10-element array gives garbage or a crash. Because arrays don't know their own length, functions take an **extra length parameter**.

### 4.7 Strings

A C string is a `char` array terminated by a **null byte** (`'\0'`, value 0 — not the character `'0'`).

- String literals like `"Hello World"` get the terminator automatically.
- When you *build* a string yourself you must add `'\0'` and **allocate room for it** (`strlen(s) + 1` bytes).

### 4.8 Function conventions

- With no member functions, the **first parameter is usually the thing being modified/initialized**:
  `int initialize_header( struct header *h, int v1, double v2 );`
- Functions often return an `int` **result code**: `0` = success, non-zero = which error occurred.
- **`errno`** (from `<errno.h>`) is a global that many library/system calls set when they fail. Named constants like `EIO` (I/O error) or `ECONNREFUSED` (111, connection refused) tell you *what* went wrong — far more useful than a bare `-1`. (Modern code just `#include <errno.h>`; the lecture also mentions declaring `extern int errno;`.)

### 4.9 `printf`

```c
int x = -25;
printf( "The value of x is %d\n", x );
printf( "(%.1f, %.1f, %.1f)\n", p->x, p->y, p->z );
```

- Format specifiers are matched **in order** with the arguments after the string.
- Common specifiers: `%d`/`%i` (int), `%u` (unsigned), `%ld` (long), `%f` (double), `%.1f` (1 decimal), `%e` (scientific), `%c` (char), `%s` (string), `%p` (pointer), `%zu` (size_t), `%x` (hex).
- **Wrong specifier = garbage output** (e.g. printing a `double` with `%d`).
- **Fewer arguments than specifiers** → `printf` reads whatever is next on the stack: garbage.
- Don't forget `\n`.

### 4.10 `#define`

```c
#define BUFFER_SIZE 1000
char *buffer = malloc( BUFFER_SIZE );
```

Avoids "magic numbers" and centralizes constants. But substitution is **purely textual**:

```c
#define VALUE 2 * 25 + 4
int size = VALUE * 2;       /* becomes 2 * 25 + 4 * 2 = 58, not 108! */
#define VALUE (2 * 25 + 4)  /* fix: always parenthesize */
```

### 4.11 `main`, `argc`, `argv`

```c
int main( int argc, char **argv ) { ... return 0; }
```

For `./a.out Hello 17`:

- `argc == 3`
- `argv[0] == "./a.out"`, `argv[1] == "Hello"`, `argv[2] == "17"`

`argv[2]` is the **string** `"17"`, not a number. Convert with `atoi( argv[2] )`.

Return `0` from `main` on success, non-zero (e.g. `-1`) on error such as wrong argument count.

### 4.12 `void *`

`void *` is a pointer **with no type** (not a pointer to nothing). Functions like `memset` take `void *` so they work on any type of memory. You'll see it constantly: `malloc` returns one, thread functions take and return them, shared memory is attached as one.

---

## New Functions / Constructs Introduced

| Name | Header | Signature / Form | Purpose |
|------|--------|------------------|---------|
| `malloc` | `<stdlib.h>` | `void *malloc( size_t size )` | Allocate `size` bytes on the heap; returns pointer or `NULL`. Uninitialized. |
| `free` | `<stdlib.h>` | `void free( void *ptr )` | Release heap memory previously from `malloc`. |
| `sizeof` | (operator) | `sizeof( type )` / `sizeof expr` | Compile-time size in bytes of a type or object. |
| `memset` | `<string.h>` | `void *memset( void *s, int c, size_t n )` | Set `n` bytes starting at `s` to byte value `c` (practically: zero memory). |
| `printf` | `<stdio.h>` | `int printf( const char *fmt, ... )` | Formatted print to the console. |
| `atoi` | `<stdlib.h>` | `int atoi( const char *s )` | Parse a string into an `int`. |
| `errno` | `<errno.h>` | `int errno` (global) | Error code set by failing library/system calls. |
| `typedef` | (keyword) | `typedef struct {...} name_t;` | Create an alias for a type. |
| `#define` | (preprocessor) | `#define NAME value` | Textual constant/macro substitution. |
| `#include` | (preprocessor) | `#include <header.h>` | Import declarations from a header. |

## Exercises

1. [ex1_points.c](ex1_points.c) — structs, `typedef`, `malloc`/`free`, `->`.
2. [ex2_find_the_bugs.c](ex2_find_the_bugs.c) — fix three classic C pitfalls.

Older, longer exercises are in [old-exercises/](old-exercises/) for extra practice.
