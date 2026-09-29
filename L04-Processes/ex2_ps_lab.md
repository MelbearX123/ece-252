# Lecture 4 — Exercise 2: Look at Real Processes

Run these in your WSL terminal and answer the questions.

1. `ps` — which processes are listed? Which one is your shell?
2. `ps -ef | head` — what is PID 1 called?
3. `echo $$` prints your shell's PID. Run `cat /proc/$$/status | head` and find the lines for the process's **name**, **state**, **PID** and **parent PID (PPid)**. These are PCB fields.
4. Run `sleep 100 &` then `ps -o pid,stat,cmd`. `S` means sleeping. Which five-state model state is that? Stop it with `kill %1`.
