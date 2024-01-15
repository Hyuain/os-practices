#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
    } else {
        // parent goes down this path (main)
        printf("parent of %d (pid:%d)\n", rc, (int) getpid());
    }
    return 0;
}

/*
OUTPUT:
    hello (pid:28987)
    parent of 28988 (pid:28987)  // 1
    child (pid28988)             // 2

    // the order of 1 and 2 is not deterministic

EXPLAIN:
    - `fork()` is a system call to create new process
    - After `fork()`, the new process is an (almost) exact copy of the calling process
    - Both parent and child are about to return from `fork()` and continue
        - In parent, the `rc` returned from `fork()` is the pid of its child
        - Child doesn't start from printing "hello" message
          **it starts as if it had called `fork()` it self**
          The `rc` in child is `0`
*/