# Lecture 5 — Exercise 5: Fork Output Puzzles

For each snippet: **predict** the output (how many lines, which values, which orderings are possible), **then** paste it into a `main` and run it several times. Assume all needed headers are included and `fork` succeeds.

## Puzzle 1

```c
printf( "A\n" );
fork();
printf( "B\n" );
```

## Puzzle 2

```c
int x = 1;
if ( fork() == 0 ) {
    x = x + 10;
    printf( "child x = %d\n", x );
} else {
    wait( NULL );
    x = x * 10;
    printf( "parent x = %d\n", x );
}
```

Is there more than one possible output order? Why or why not?

## Puzzle 3

```c
fork();
fork();
printf( "X\n" );
```

How many X's? Draw the process tree.

## Puzzle 4

```c
if ( fork() && fork() ) {
    printf( "P\n" );
} else {
    printf( "C\n" );
}
```

How many P's and C's? (Remember `&&` short-circuits: what does the first child see for the first `fork()`?)

## Puzzle 5 — the buffering trap

```c
printf( "hello " );     /* note: no newline */
fork();
printf( "world\n" );
```

Run it on the terminal, then run `./a.out > out.txt; cat out.txt`. Explain why "hello" appears **twice**. (Hint: where does un-flushed `printf` output live, and what does `fork` copy?) How would `fflush(stdout)` before `fork` change things?

Also compare with Puzzle 1 redirected to a file: `./p1 > out.txt`. Is "A" duplicated? Why is that different from running in a terminal?

## Puzzle 6

```c
for ( int i = 0; i < 3; i++ ) {
    if ( fork() == 0 ) {
        printf( "child %d\n", i );
        exit( i );
    }
}
int status;
while ( wait( &status ) > 0 ) {
    printf( "reaped exit %d\n", WEXITSTATUS( status ) );
}
```

How many processes in total? Is the `reaped` order guaranteed? What happens if you delete the `exit(i)` line? (Count the processes carefully — draw it.)

## Puzzle 7

```c
pid_t pid = fork();
if ( pid == 0 ) {
    execlp( "echo", "echo", "from exec", NULL );
    printf( "after exec\n" );
}
wait( NULL );
printf( "done\n" );
```

How many times does "done" print? Does "after exec" print? What changes if you misspell `"echo"` as `"ecoh"`?

## Puzzle 8

```c
int status;
if ( fork() == 0 ) {
    return 256 + 3;
}
wait( &status );
printf( "%d %d\n", status == 0, WEXITSTATUS( status ) );
```

What prints? What does this tell you about using exit codes to return data from a child?
