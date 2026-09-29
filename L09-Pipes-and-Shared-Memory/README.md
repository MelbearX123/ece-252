# Lecture 9 — Pipes and Shared Memory

Two more same-machine IPC mechanisms. **Pipes** sit between message passing and shared memory (the OS moves bytes for you through a buffer in memory); **shared memory** lets processes read and write the *same* RAM directly.

## 1. UNIX Pipes

A **pipe** connects a producer to a consumer: the producer writes into one end, the consumer reads from the other. It is **unidirectional** — for two-way communication you need **two pipes**.

```c
int pipe( int fileDescriptors[2] );
```

- `fileDescriptors[0]` — the **read end**.
- `fileDescriptors[1]` — the **write end**.
- Returns 0 on success, -1 on error.
- They're ordinary file descriptors ("everything is a file"), even though the pipe lives **in memory**, so you use `read`, `write`, and `close` on them.

Mnemonic: 0 = stdin = read, 1 = stdout = write.

### How it works inside

The pipe is a block of main memory treated as a **circular queue** of small fixed-size entries (usually one character). The sender may write in chunks, but the reader receives a stream of bytes — **message boundaries are not preserved**. So the two sides must agree how to find the end of a message:

- a **terminator** character (e.g. newline or `'\0'`), or
- a **length prefix** stating how many characters follow.

Behaviour you'll observe (standard POSIX semantics): `read` blocks while the pipe is empty; `write` blocks while it's full; once **every write end is closed**, `read` returns **0** (EOF). That's why each process must **close the ends it doesn't use** — otherwise the reader never sees EOF.

### Anonymous vs. named pipes

| | **Regular (anonymous) pipe** | **Named pipe (FIFO)** |
|-|------------------------------|------------------------|
| Created with | `pipe(fd)` | `mkfifo(path, mode)` |
| Has a name on disk | No | Yes (but data still in memory) |
| Who can use it | Only processes that inherit the fds → requires a **parent–child** relationship (created before `fork`) | **Any** process that can open the path |
| Lifetime | Only while the processes are using it | **Persists** after the creator exits (until deleted) |
| Direction | Unidirectional | Can be used in either direction, but **one direction at a time** (need two for simultaneous two-way) |
| Accessed with | `read`/`write`/`close` | `open`/`read`/`write`/`close` (it's a file) |

You've used pipes at the shell: `cat fork.c | less` — the shell creates a pipe, connecting `cat`'s output to `less`'s input.

### Example: parent sends "Greetings" to child

```c
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main( int argc, char **argv ) {
    char write_msg[BUFFER_SIZE] = "Greetings";
    char read_msg[BUFFER_SIZE];
    int fd[2];
    pid_t pid;

    if ( pipe( fd ) == -1 ) {                 /* create BEFORE fork */
        fprintf( stderr, "Pipe failed" );
        return 1;
    }

    pid = fork();
    if ( pid < 0 ) {
        fprintf( stderr, "Fork Failed" );
        return 1;
    }

    if ( pid > 0 ) {                          /* parent: producer */
        close( fd[READ_END] );                /* close unused end */
        write( fd[WRITE_END], write_msg, strlen( write_msg ) );
        close( fd[WRITE_END] );
    } else {                                  /* child: consumer */
        close( fd[WRITE_END] );               /* close unused end */
        read( fd[READ_END], read_msg, BUFFER_SIZE );
        printf( "read %s", read_msg );
        close( fd[READ_END] );
    }
    return 0;
}
```

**The bug**: sometimes a random character appears after "Greetings". `strlen` doesn't count the `'\0'`, so the terminator isn't sent, and `read_msg` is uninitialized — `printf("%s")` runs off into garbage. **Fix**: `memset( read_msg, 0, BUFFER_SIZE )` before reading (and/or send `strlen + 1` bytes). General rule: initialize buffers to 0.

## 2. Shared Memory

Normally each region of memory belongs to exactly one process; if another process touches it, the OS intervenes (error). With **shared memory**, a region is designated as shared and **multiple processes can read and write it directly**.

- The OS must be told (setup), and must track that multiple processes reference the region — if the creator exits and is reaped, the region must stay alive while others use it.
- After setup, access is **just normal memory access** — no system calls, no copying. The kernel is involved only in **setup and cleanup**. This is why it's the fastest IPC.
- The shared block usually lives in the address space of the process that created it; it could be anywhere.
- ⚠️ Because both can write, one can **overwrite the other's changes**. Coordination (synchronization) is needed — a topic coming later in the course.

### System V shared memory

**Key**: same as message queues — `IPC_PRIVATE` (parent/child) or `ftok(path, proj)` (unrelated processes).

Functions (`<sys/shm.h>`):

```c
int   shmget( key_t key, size_t size, int shmflg );
void *shmat ( int shmid, const void *shmaddr, int shmflg );
int   shmdt ( const void *shmaddr );
int   shmctl( int shmid, int cmd, struct shmid_ds *buf );
```

Workflow:

1. **Create / look up** the segment — `shmget`.
2. **Attach** it to your address space — `shmat`.
3. **Use it** like any memory.
4. **Detach** — `shmdt` (happens automatically at process exit if forgotten, but do it).
5. **Delete** — `shmctl(..., IPC_RMID, NULL)`, by **one** process only.

Details:

- **`shmget(key, size, flags)`** — `size` in bytes. `flags` = permissions (e.g. `0600`, `0666`) OR'ed with `IPC_CREAT` (create if missing) and optionally `IPC_EXCL` (fail if it exists). Returns the segment **id**.
- **`shmat(id, NULL, flags)`** — always pass **`NULL`** for the address and let the kernel choose (a hard-coded address might be in use on another machine or at another time). `flags` may be `SHM_RDONLY` (writing then causes a segmentation fault). **Returns a normal pointer** to the memory (or `(void *) -1` on error).
- **Sharing with others**: unrelated processes call `ftok` with the same arguments to get the same key. Or: a parent that **attaches before `fork`** — the child inherits the attachment.
- **`shmdt(ptr)`** — takes the **pointer from `shmat`**, not the id.
- **`shmctl(id, IPC_RMID, NULL)`** — marks the segment for deletion. It is **not removed immediately if still attached** somewhere; it goes away when the last process detaches (just like a UNIX file deleted while open). Linux lets you attach to a marked-for-deletion segment, but that's non-portable — don't.
- **Security**: you can only attach to segments whose permission bits allow it — a process run by user `j999doe` can only attach to that user's segments (unless permissions grant others access).

Segments outlive processes if not removed — inspect with `ipcs -m`, remove with `ipcrm -m <id>`.

### Example: child → parent via shared memory

```c
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main( int argc, char **argv ) {
    int shmid = shmget( IPC_PRIVATE, 32, IPC_CREAT | 0666 );

    int pid = fork();
    if ( pid > 0 ) {                                 /* Parent */
        waitpid( pid, NULL, 0 );                     /* wait for THAT child */
        void *mem = shmat( shmid, NULL, 0 );
        printf( "The message the parent received from the child is %s.\n", (char *) mem );
        shmdt( mem );
        shmctl( shmid, IPC_RMID, NULL );
    } else if ( pid == 0 ) {                         /* Child */
        void *mem = shmat( shmid, NULL, 0 );
        memset( mem, 0, 32 );
        sprintf( mem, "Hello World" );
        shmdt( mem );
    }
    return 0;
}
```

Note: `waitpid(pid, NULL, 0)` waits for a **specific** child (NULL = don't care about status). Here waiting is the *only* coordination: the parent reads only after the child has finished writing.

## 3. `mmap` — Memory-Mapped Files

`mmap` maps (part of) a **file** into memory. Reading from the buffer reads the file; writing to it writes the file — file I/O as memory accesses. With the shared flag, it also works as IPC.

```c
void *mmap( void *address, size_t length, int protection, int flag, int fd, off_t offset );
int mprotect( void *address, size_t length, int prot );
int msync( void *address, size_t length, int flags );
int munmap( void *address, size_t length );
```

`mmap` arguments:

| Arg | Meaning |
|-----|---------|
| `address` | Where to place the mapping — pass **`NULL`** to let the system choose. |
| `length` | Number of bytes to map. |
| `protection` | `PROT_READ`, `PROT_WRITE`, `PROT_EXEC`, or `PROT_NONE`, combined with `|`. Must be **consistent with how the file was opened** (can't map `PROT_WRITE` on a file opened read-only). |
| `flag` | `MAP_SHARED` — changes visible to other processes mapping the file **and written to the file** (maybe not instantly). `MAP_PRIVATE` — changes are private copies; not visible to others, not written to the file. |
| `fd` | File descriptor of the (already `open`ed) file. |
| `offset` | Where in the file the mapping starts (a multiple of the page size). |

Returns a pointer to the mapped region (`MAP_FAILED` on error).

- **`PROT_NONE`** forbids all access (any touch → segfault). Why? **Guard regions** at the start/end of a buffer, so overruns crash immediately instead of silently corrupting memory.
- **`mprotect(addr, len, prot)`** — change protections on a mapped region later.
- **`msync(addr, len, MS_SYNC)`** — shared-mapping writes may not reach the file immediately (performance). `msync` forces them out; `MS_SYNC` makes it a blocking call. Does nothing for `MAP_PRIVATE`.
- **`munmap(addr, len)`** — unmap when done (automatic at exit, but be polite).
- `MAP_PRIVATE` is for working with a file yourself; `MAP_SHARED` is for IPC.

### Example: child overwrites a file, parent sees it

```c
int fd = open( "example.txt", O_RDWR );
struct stat st;
stat( "example.txt", &st );                     /* get the file size */
ssize_t size = st.st_size;
void *mapped = mmap( NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 );

int pid = fork();
if ( pid > 0 ) {                                /* Parent */
    waitpid( pid, NULL, 0 );
    printf( "The new content of the file is: %s.\n", (char *) mapped );
    munmap( mapped, size );
} else if ( pid == 0 ) {                        /* Child */
    memset( mapped, 0, size );                  /* erase what's there */
    sprintf( mapped, "It is now Overwritten" );
    msync( mapped, size, MS_SYNC );             /* make sure it's written */
    munmap( mapped, size );
}
close( fd );
```

**Limitations**: the mapped size **can't change**. Writing more than the file's size can't grow the file; writing fewer bytes leaves the rest (here zeros) — "things look strange." Linux has `mremap`, but it's not portable. A perfect fit is something like **sorting an array** in a file: the output is the same size as the input, and multiple processes could share the work.

---

## New Functions Introduced

| Function | Header | Signature | Purpose |
|----------|--------|-----------|---------|
| `pipe` | `<unistd.h>` | `int pipe( int fd[2] )` | Create an anonymous pipe: `fd[0]` read end, `fd[1]` write end. |
| `write` | `<unistd.h>` | `ssize_t write( int fd, const void *buf, size_t n )` | Write bytes to a file descriptor (pipe, file, socket). |
| `mkfifo` | `<sys/stat.h>` | `int mkfifo( const char *path, mode_t mode )` | Create a named pipe (FIFO) in the file system. |
| `shmget` | `<sys/shm.h>` | `int shmget( key_t key, size_t size, int flags )` | Create/find a shared memory segment; returns id. |
| `shmat` | `<sys/shm.h>` | `void *shmat( int shmid, const void *addr, int flags )` | Attach a segment; returns a pointer to it. |
| `shmdt` | `<sys/shm.h>` | `int shmdt( const void *addr )` | Detach a segment (takes the pointer). |
| `shmctl` | `<sys/shm.h>` | `int shmctl( int shmid, int cmd, struct shmid_ds *buf )` | Control a segment; `IPC_RMID` marks it for deletion. |
| `mmap` | `<sys/mman.h>` | `void *mmap( void *addr, size_t len, int prot, int flags, int fd, off_t off )` | Map a file into memory (`MAP_SHARED`/`MAP_PRIVATE`). |
| `mprotect` | `<sys/mman.h>` | `int mprotect( void *addr, size_t len, int prot )` | Change protection of a mapped region. |
| `msync` | `<sys/mman.h>` | `int msync( void *addr, size_t len, int flags )` | Flush a shared mapping to the file (`MS_SYNC` = blocking). |
| `munmap` | `<sys/mman.h>` | `int munmap( void *addr, size_t len )` | Unmap a region. |
| `stat` | `<sys/stat.h>` | `int stat( const char *path, struct stat *buf )` | Get file metadata (e.g. `st_size`). |
| `waitpid` | `<sys/wait.h>` | `pid_t waitpid( pid_t pid, int *status, int options )` | Wait for a specific child. |
| `sprintf` | `<stdio.h>` | `int sprintf( char *str, const char *fmt, ... )` | `printf` into a buffer (prefer `snprintf` to avoid overflow). |
| `open` (`O_RDWR`) | `<fcntl.h>` | `int open( const char *path, O_RDWR )` | Open for reading and writing. |

Constants: `SHM_RDONLY`, `IPC_CREAT`, `IPC_EXCL`, `IPC_RMID`, `PROT_READ`, `PROT_WRITE`, `PROT_EXEC`, `PROT_NONE`, `MAP_SHARED`, `MAP_PRIVATE`, `MS_SYNC`.

Commands: `ipcs -m`, `ipcrm -m <id>`, `mkfifo` (shell), `|`.

Useful related (used in exercises): `dup2(oldfd, newfd)` — make `newfd` refer to the same thing as `oldfd` (how the shell wires a pipe to stdin/stdout); `MAP_ANONYMOUS` (shared memory with no backing file); `ftruncate` (set a file's size); `sysconf(_SC_PAGESIZE)`.

## Exercises

1. [ex1_pipe.c](ex1_pipe.c) — send a message from parent to child through a pipe.
2. [ex2_shared_memory.c](ex2_shared_memory.c) — child returns a value to the parent through shared memory.

Older, longer exercises are in [old-exercises/](old-exercises/) for extra practice.
