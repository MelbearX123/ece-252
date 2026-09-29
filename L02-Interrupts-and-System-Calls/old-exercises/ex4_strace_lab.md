# Lecture 2 — Exercise 4: Watching System Calls with `strace`

`strace` runs a program and prints **every system call it makes** — every trap into the kernel. This makes the user/kernel boundary visible.

Install if needed: `sudo apt install strace` (already on ecelinux).

## Part A — Hello World

1. Write the smallest possible `hello.c` using `printf("Hello World\n");` and compile it.
2. Run `strace ./hello`.
3. Answer:
   - Which system call actually puts "Hello World" on the screen? (It is **not** called `printf`.) What are its three arguments?
   - What is the first argument's value and what does it mean?
   - Roughly how many system calls happen *before* `main` even runs? What do they seem to be doing (look for `openat`, `mmap`, `read` of `.so` files)?
   - What is the last system call? Where did it come from if you just did `return 0`?

## Part B — `printf` buffering

1. Change `hello.c` to print 5 lines in a loop with `printf("line %d\n", i);`.
2. `strace ./hello` — how many `write` calls?
3. Now run `strace ./hello > out.txt`. How many `write` calls now? Why is it different when output goes to a file instead of a terminal?
4. Remove the `\n` from each printf and run on the terminal again. What changes?

> Takeaway: `printf` is a **library function** (runs in user mode) that buffers text and eventually calls the **system call** `write`. Not every library call is a system call.

## Part C — your `mycat`

1. `strace -e trace=openat,read,write,close ./mycat README.md`
2. Match each line of `strace` output to a line of your code from Exercise 1.
3. Run it on a missing file. What does `strace` show as the return value, and what errno name does it print?

## Part D — counting

1. `strace -c ls /usr/bin > /dev/null` prints a summary table. Which system call is made most often?
2. `strace -c ./ex2 big.bin 1` vs `strace -c ./ex2 big.bin 65536` (from Exercise 2). Compare the `calls` column for `read`.

## Part E — map it to the lecture

For one `read` line in your `strace` output, write out (in your own words) the 8 steps from the lecture that happened to produce that single line — from pushing arguments to returning the byte count.
