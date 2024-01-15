#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    printf("hello (pid:%d)\n", (int) getpid());
    int rc = fork();
    if (rc < 0) {
        // fork failed
        printf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        printf("child (pid%d)\n", (int) getpid());
        char *myargs[3];
        myargs[0] = strdup("wc");      // program: "wc"
        myargs[1] = strdup("exec.c");  // arg: "input file"
        myargs[2] = NULL;
        execvp(myargs[0], myargs);     // runs word count
        printf("this shouldn't print out");
    } else {
        // parent goes down this path (main)
        int rc_wait = wait(NULL);
        printf("parent of %d (rc_wait:%d) (pid:%d)\n", rc, rc_wait, (int) getpid());
    }
    return 0;
}

/*
OUTPUT:
    hello (pid:34152)
    child (pid34153)
          40     163    1251 exec.c
    parent of 34153 (rc_wait:34153) (pid:34152)

EXPLAIN:
    - `exec()` system call allow us to run a different program, `execvp()` is a variant of `exec`
    - In this example, `wc` runs in child
    - NOTE that `exec()` loads code from the executable `wc`
      and *overwrites* its current code segement (and current static data) with it.
      The heap, stack and other parts of the memory space of the program are re-initialized
    - So a successful call to `exec()` never returns
*/