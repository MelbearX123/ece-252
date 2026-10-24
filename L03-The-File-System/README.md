# Lecture 3 — The File System

## 1. What Is a File?

To the computer, all data is just bytes. A **file** is a *logical unit* for organizing those bytes — an area of storage designated as belonging to that file. Files may hold programs (`word.exe`) or data (`report.docx`); the content's meaning is defined by whoever created it.

**UNIX philosophy: "everything is a file."** Devices, pipes, sockets, and regular files are all accessed through the same file interface. That means the functions you learn for files (`open`, `read`, `write`, `close`) will keep reappearing for pipes (L09) and sockets (L07/08).

### File attributes (metadata)

1. **Name** — human-readable.
2. **Identifier** — unique number inside the file system (in UNIX, the *inode number*).
3. **Type** — what kind of file it is.
4. **Location** — which device and where on it.
5. **Size** — current (and possibly maximum) size.
6. **Protection** — owner and who may read/write/execute.
7. **Time, date, user ID** — creation, last access, last modification; useful for protection, security, and usage monitoring.

Files live in a **directory structure** (folders). Directories themselves are just special files stored on disk that record which files are where.

## 2. The Six Basic File Operations

A file is a bit like an object: it has data (contents + metadata) and operations the OS provides.

| Operation | What happens |
|-----------|--------------|
| **Create** | Find space, allocate it and mark it used, add an entry to the directory. (Very like allocating memory.) |
| **Write** | Locate the file by name/identifier, put data at the **current write position**; either overwrite or append. The position advances automatically. |
| **Read** | Locate the file, copy the next chunk into memory at a location you give. Uses a **current read position** that advances automatically. |
| **Reposition (seek)** | Move the current position pointer without reading/writing. |
| **Delete** | Find the file, mark its space free, remove it from the directory. |
| **Truncate** | Erase the contents (length → 0, data area freed) but **keep the attributes**. |

Everything else (copy, rename…) can be built from these. E.g. copy = create new + read old + write new.

**Open/close**: except for create and delete, operations need the file to be **open**. Opening gives the program a reference, and the OS tracks which files each process has open. Close files when done (they're closed automatically at process exit — hopefully). Don't repeatedly open/close the same file if you'll use it again soon; keeping it open is fine and cheaper.

### `fopen` and its modes

```c
FILE *fopen( const char *filename, const char *mode );
```

Create, read, write, and truncate all go through `fopen` — the **mode** decides which:

| Mode | Read? | Write? | If file exists | If file missing | Initial position |
|------|-------|--------|----------------|-----------------|------------------|
| `"r"` | ✔ | | opened as-is | **fails** (NULL) | beginning |
| `"w"` | | ✔ | **truncated to 0** | created | beginning |
| `"a"` | | ✔ (end only) | opened as-is | created | end; all writes append |
| `"r+"` | ✔ | ✔ | opened as-is | **fails** | beginning |
| `"w+"` | ✔ | ✔ | **truncated** | created | beginning |
| `"a+"` | ✔ | ✔ (end only) | opened as-is | created | writes always at end; initial read position is platform-dependent (beginning on glibc/Linux, end on BSD/macOS/Android) |

- Add **`b`** (e.g. `"rb"`) for binary mode.
- C11 adds **`x`** (e.g. `"wx"`): fail if the file already exists instead of truncating it.
- `fopen` returns `NULL` on failure — always check.

```c
FILE *f = fopen( argv[1], "r" );
if ( f == NULL ) {
    printf( "Unable to open file! %s is invalid name?\n", argv[1] );
    return -1;
}
/* ... use f ... */
fclose( f );
```

`FILE *` (C standard library, buffered, "streams") vs. `int` file descriptors (`open`/`read`/`write`, OS-level, unbuffered) — `FILE *` is a library wrapper around a file descriptor.

### Repositioning: `fseek`

```c
int fseek( FILE *stream, long offset, int whence );  /* whence: SEEK_SET, SEEK_CUR, SEEK_END */
```

- Often the read and write positions are one shared **current position pointer**; seeking just moves it.
- Use it only to skip ahead or go back. Normal reads/writes advance the pointer themselves: read 48 bytes then seek +48 → you're 96 bytes from the start.
- Caution: you can land in the middle of a multi-byte character.
- You can't seek to *write* somewhere else in a file opened for append — writes always go to the end.

### Deleting: `remove`

```c
remove( argv[1] );   /* returns 0 on success, -1 on error */
```

A "simple" deletion just forgets the file (marks space free, removes the directory entry). The bytes remain on disk until overwritten — similar to freed memory still containing old data. Secure deletion overwrites the space with zeros.

## 3. File Locking

Some OSes lock files. Locks can be **exclusive** (only one holder) or **shared** (many readers).

- **Windows** locks by default: a file open in one program **can't be deleted**.
- **UNIX** doesn't lock by default. If a file is open in program A and user B deletes it, it disappears from the directory, **but A can keep using it** through its open reference. The space is freed only when nobody has it open anymore.
- UNIX programs *can* opt in to locking with **`flock`** ("file-lock"). These locks are **advisory**: they only work between programs that also call `flock`.

```c
FILE *f = fopen( "myfile.txt", "r" );
int file_desc = fileno( f );             /* FILE* -> file descriptor */
int result = flock( file_desc, LOCK_EX ); /* exclusive; blocks until available */
/* ... */
flock( file_desc, LOCK_UN );              /* unlock */
```

`LOCK_SH` = shared, `LOCK_EX` = exclusive, `LOCK_UN` = unlock. (Add `| LOCK_NB` to fail immediately instead of blocking.)

## 4. Reading and Writing Text: `fprintf` / `fscanf`

**`fprintf`** is `printf` with a `FILE *` as the first argument:

```c
void write_points_to_file( point_t *p, FILE *f ) {
    while ( p != NULL ) {
        fprintf( f, "(%.1f, %.1f, %.1f)\n", p->x, p->y, p->z );
        p = p->next;
    }
}
```

**`fscanf`** is the mirror image — same format specifiers, but you pass **addresses** where results should be stored:

```c
int i, isquared;
FILE *fp = fopen( "results.dat", "r" );
if ( fp == NULL ) return -1;
while ( fscanf( fp, "%d,%d\n", &i, &isquared ) == 2 ) {
    printf( "i: %d, isquared: %d\n", i, isquared );
}
fclose( fp );
```

- Return value = **number of items successfully matched** (here 2). Loop while it equals the number you expect; it returns `EOF` at end of file.
- `%d` **skips leading whitespace**, which is convenient but occasionally causes hard-to-find bugs.
- `scanf` is the same thing for keyboard input (stdin).
- `getline` reads a whole line into a (possibly auto-allocated) buffer:

```c
char *line = NULL; size_t cap = 0; ssize_t len;
while ( ( len = getline( &line, &cap, fp ) ) != -1 ) { /* line includes '\n' */ }
free( line );
```

## 5. File Types

Extensions (`.txt`, `.docx`) are **hints** to the OS/user about content. Any program can open any file; the OS merely lets you set a default program per extension.

## 6. Directories

A directory is a **symbol table** mapping names to directory entries. It should support: **search**, **add** a file, **remove** a file, **list**, **rename**, and **navigate** (enter subdirectories, go to parent).

**Tree-structured directories** (what we're used to): one root; every file has a unique full path from the root.

- UNIX root is `/`. `ls` lives at `/bin/ls`.
- **Absolute path** — from the root: `/home/jz/ece252/code/example.c`.
- **Relative path** — from the *current directory*: `code/example.c`, which the system turns into an absolute path by prepending the current directory.

**Deleting non-empty directories** is a design choice: refuse until empty, or delete everything inside. Many modern systems "delete" by moving to a trash/recycle bin first, where it can be restored.

### Links (sharing a file under more than one name)

| | **Symbolic link (symlink)** | **Hard link** |
|-|-----------------------------|----------------|
| What it is | A small file containing a *path name* — a "shortcut" | A second directory entry pointing to the **same underlying file** (same inode) |
| If the original is deleted | Link **dangles** — using it gives an error | File **still exists**; data freed only when the last hard link is removed |
| Mechanism | Name lookup at use time | **Reference counting** (link count) |
| Create (shell) | `ln -s target linkname` | `ln target linkname` |

## 7. File Permissions

Possible controlled operations: read, write, execute, append, delete, list attributes.

### UNIX-style permissions

Each file has an **owner**, a **group**, and permission bits for three classes: **owner (user)**, **group**, **others (everyone)**. Displayed as 10 characters:

```
- rwx r-- ---
│  │   │   └─ others: read, write, execute
│  │   └───── group
│  └───────── owner
└──────────── d = directory, - = regular file
```

`-rwxr-----` = regular file; owner can read/write/execute; group can read; others nothing.

**Precedence**: owner > group > others, determined by *who you are*. If you're the owner, **only** the owner bits apply — even if the group bits are more generous.

**Octal notation**: r = 4, w = 2, x = 1, summed per class.

| Octal | Bits | Meaning |
|-------|------|---------|
| 7 | rwx | 4+2+1 |
| 6 | rw- | 4+2 |
| 5 | r-x | 4+1 |
| 4 | r-- | 4 |
| 0 | --- | none |

So `750` = `rwxr-x---`; `644` = `rw-r--r--`; `600` = `rw-------`. You'll use these octal values again for message queues (`0666`) and shared memory (`0600`). Set them with `chmod 750 file`.

(Not covered: directory permission semantics, setuid, setgid, sticky bit.) The main weakness is coarseness — only three classes of users.

---

## New Functions Introduced

| Function | Header | Signature | Purpose |
|----------|--------|-----------|---------|
| `fopen` | `<stdio.h>` | `FILE *fopen( const char *filename, const char *mode )` | Open/create/truncate a file as a buffered stream. `NULL` on failure. |
| `fclose` | `<stdio.h>` | `int fclose( FILE *stream )` | Flush and close a stream. |
| `fseek` | `<stdio.h>` | `int fseek( FILE *stream, long offset, int whence )` | Move the current file position (`SEEK_SET`/`SEEK_CUR`/`SEEK_END`). |
| `remove` | `<stdio.h>` | `int remove( const char *path )` | Delete a file (0 success, -1 error). |
| `flock` | `<sys/file.h>` | `int flock( int fd, int operation )` | Advisory lock: `LOCK_SH`, `LOCK_EX`, `LOCK_UN` (optionally `| LOCK_NB`). |
| `fileno` | `<stdio.h>` | `int fileno( FILE *stream )` | Get the file descriptor behind a `FILE *`. |
| `fprintf` | `<stdio.h>` | `int fprintf( FILE *stream, const char *fmt, ... )` | Formatted write to a file. |
| `fscanf` | `<stdio.h>` | `int fscanf( FILE *stream, const char *fmt, ... )` | Formatted read from a file; returns number of items matched, or `EOF`. |
| `scanf` | `<stdio.h>` | `int scanf( const char *fmt, ... )` | Formatted read from standard input. |
| `getline` | `<stdio.h>` | `ssize_t getline( char **lineptr, size_t *n, FILE *stream )` | Read a whole line, growing the buffer as needed; `-1` at EOF. |

From the labs (binary files, open with `"rb"` / `"wb"`):

| Function | Header | Signature | Purpose |
|----------|--------|-----------|---------|
| `fwrite` | `<stdio.h>` | `size_t fwrite( const void *ptr, size_t size, size_t count, FILE *stream )` | Write `count` items of `size` bytes each from `ptr`. Returns how many items were written. |
| `fread` | `<stdio.h>` | `size_t fread( void *ptr, size_t size, size_t count, FILE *stream )` | Read up to `count` items of `size` bytes each into `ptr`. Returns how many items were read (fewer than `count` at end of file). |

Useful related (not in the lecture): `ftell` (current position), `rewind`, `fgets`, `stat`, `chmod`, `unlink`.

## Exercises

1. [ex1_squares_file.c](ex1_squares_file.c) — write with `fprintf`, read back with `fscanf`.
2. [ex2_binary_records.c](ex2_binary_records.c) — write structs to a binary file with `fwrite`, read them back with `fread`, jump to one with `fseek`.

Older, longer exercises are in [old-exercises/](old-exercises/) for extra practice.
