# ECE 252 — Lecture Study Notes & Exercises

One folder per lecture. Each folder contains:

- `README.md` — the core content of the lecture explained in depth, plus a table of every new function / system call introduced.
- `ex1_*`, `ex2_*` — two short exercises per lecture. `.c` files have the task in the header comment and starter code with `TODO`s. `.md` files are pen-and-paper or command-line exercises.
- `old-exercises/` — the original, longer exercise sets, kept for extra practice later.

| # | Folder | Topic |
|---|--------|-------|
| 1 | [L01-Introduction-and-C-Toolkit](L01-Introduction-and-C-Toolkit/) | OS background, concurrency, C refresher |
| 2 | [L02-Interrupts-and-System-Calls](L02-Interrupts-and-System-Calls/) | Interrupts, traps, user/kernel mode, `read` |
| 3 | [L03-The-File-System](L03-The-File-System/) | Files, `fopen` modes, seeking, locking, permissions, links |
| 4 | [L04-Processes](L04-Processes/) | PCB, process lifecycle, 5- and 7-state models |
| 5 | [L05-Processes-in-UNIX](L05-Processes-in-UNIX/) | `fork`, `exec`, `wait`, signals |
| 6 | [L06-Inter-Process-Communication](L06-Inter-Process-Communication/) | IPC models, signal handlers, message queues |
| 7 | [L07-Sockets](L07-Sockets/) | Sockets, byte order, addresses, connect/bind/listen/accept |
| 8 | [L08-Network-Communication](L08-Network-Communication/) | `send`/`recv`, UDP, serialization, libcurl |
| 9 | [L09-Pipes-and-Shared-Memory](L09-Pipes-and-Shared-Memory/) | Pipes, FIFOs, System V shared memory, `mmap` |
| 10 | [L10-Threads](L10-Threads/) | Threads vs processes, pthreads |

## Setup (WSL)

This repo lives inside WSL at `~/projects/learn/ece-252`, so everything runs natively on Linux. Keep it in the WSL file system (not under `/mnt/c/...`), where builds are faster and file permissions behave properly.

Install the tools once:

```bash
sudo apt update && sudo apt install build-essential gdb valgrind strace libcurl4-openssl-dev netcat-openbsd
```

To open the folder in VS Code from WSL, run `code .` (needs the WSL extension). From Windows Explorer the files are at `\\wsl$\Ubuntu\home\<you>\projects\learn\ece-252`.

Course work is graded on **ecelinux** (`ssh <userid>@ecelinux.uwaterloo.ca`). Test there before submitting anything.

## Compiling

Default for every exercise:

```bash
gcc -Wall -Wextra -g ex1_something.c -o ex1
```

Extra flags where needed:

- Threads (L10): add `-pthread`
- libcurl (L08): add `-lcurl`

Run memory-related exercises under `valgrind --leak-check=full ./ex1` to catch leaks and invalid accesses.

For the socket exercises (L07, L08), run the server in one WSL terminal and connect from a second one with `nc localhost 2520`.
