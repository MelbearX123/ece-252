# Lecture 3 — Exercise 7: Links, Deletion, and Permissions (shell lab)

Do this in a scratch directory on Linux (ecelinux or WSL — **not** under `/mnt/c/...` in WSL, since Windows file systems don't support UNIX permissions/links properly). Before each step, **write down your prediction**, then run it.

```bash
mkdir ~/fslab && cd ~/fslab
echo "original content" > file.txt
```

## Part A — Hard links and reference counting

1. `ls -li file.txt` — note the **first number** (inode = file identifier) and the number after the permissions (the **link count**).
2. `ln file.txt hard.txt` then `ls -li`. What is the same between the two entries? What changed in the link count?
3. `echo "added via hard" >> hard.txt` then `cat file.txt`. Explain.
4. `rm file.txt` then `cat hard.txt` and `ls -li`. Does the data still exist? What is the link count now?
5. Explain in one sentence, using the words *reference count*, when the data blocks are actually freed.

## Part B — Symbolic links

1. `echo "v1" > target.txt && ln -s target.txt sym.txt && ls -li`
   - Is the inode of `sym.txt` the same as `target.txt`? What does `ls -l` show for `sym.txt`?
2. `cat sym.txt`, then `rm target.txt`, then `cat sym.txt`. What error do you get? What's this called?
3. `echo "v2" > target.txt` then `cat sym.txt`. Why does it work again? (Would a hard link behave the same way?)
4. `ln -s /tmp tmplink && cd tmplink && pwd && pwd -P` — symlinks work for directories too.

## Part C — Deleting an open file (UNIX vs. Windows)

1. Terminal 1: `python3 -c "import time; f=open('open.txt','w'); [ (f.write(f'{i}\n'), f.flush(), time.sleep(1)) for i in range(60)]"`
   (or use your `ex5_flock` program with a long duration).
2. Terminal 2: `rm open.txt && ls`. The file is gone from the directory…
3. …but find the process's open file: `ls -l /proc/$(pgrep -f open.txt)/fd` (or `pgrep python3`). What does the entry say? You can even `cat /proc/<pid>/fd/<n>` to read the "deleted" file, which is still growing.
4. Relate this to the lecture: when is the storage finally freed?

## Part D — Permissions

1. `echo secret > p.txt && ls -l p.txt`. Write the octal value of the default permissions. (Why is it 644 or 664? Look up `umask`.)
2. `chmod 000 p.txt && cat p.txt` → ? Now `chmod 400 p.txt && echo x >> p.txt` → ?
3. `chmod 070 p.txt && cat p.txt` — you are the owner **and** in the file's group. Does it work? Why not? (Precedence rule.)
4. Create a shell script `hi.sh` containing `echo hi`. Run `./hi.sh`. What's the error? Fix it with the smallest possible `chmod` (use octal).
5. Convert by hand, then check with `chmod` + `ls -l`:
   - `rwxr-x--x` → ?
   - `rw-------` → ?
   - `751` → ?
   - `604` → ?

## Part E — Absolute vs. relative paths

1. `pwd`, then `cat ./hard.txt`, `cat ../fslab/hard.txt`, and `cat ~/fslab/hard.txt`. Write the absolute path the system builds for each.
2. Make a symlink using a **relative** target (`ln -s hard.txt rel`) and one with an **absolute** target (`ln -s ~/fslab/hard.txt abs`). Move both links into a subdirectory (`mkdir sub && mv rel abs sub/`). Which one breaks, and why?

## Cleanup

```bash
cd ~ && rm -rf ~/fslab
```
