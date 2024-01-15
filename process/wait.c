#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
    } else {
        // parent goes down this path (main)
        int rc_wait = wait(NULL);
        printf("parent of %d (rc_wait:%d) (pid:%d)\n", rc, rc_wait, (int) getpid());
    }
    return 0;
}

/*
OUTPUT:
    hello (pid:29301)
    child (pid29302)
    parent of 29302 (rc_wait:29302) (pid:29301)

EXPLAIN:
    - The `wait()` call delay the excuation of parent until the any one child finishes executing
    - If the parent needs to wait for another child process to finish, it need to call `wait(NULL)` again
    - To wait for a specific child, use `waitpid()`
*/