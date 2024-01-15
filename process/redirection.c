#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    printf("hello (pid:%d)\n", (int) getpid());
    int rc = fork();
    if (rc < 0) {
        // fork failed
        printf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process): redirect standard output to a file
        close(STDOUT_FILENO);
        open("./redirection.output", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
        // now exec "wc"...
        char *myargs[3];
        myargs[0] = strdup("wc");      // program: "wc"
        myargs[1] = strdup("redirection.c");  // arg: "input file"
        myargs[2] = NULL;
        execvp(myargs[0], myargs);     // runs word count
    } else {
        // parent goes down this path (main)
        int rc_wait = wait(NULL);
        printf("parent of %d (rc_wait:%d) (pid:%d)\n", rc, rc_wait, (int) getpid());
    }
    return 0;
}

/*
OUTPUT (in file redirection.output):
    46     205    1589 redirection.c

EXPLAIN:
    - It shows the motivation of separate `fork()` and `exec()`.
      Sometimes we want to do something after `fork()` but before `exec()`
    - Here in the example, we close the standard output, and opens a new file,
      Letting all the outputs of `wc` be sent to the file
    - Similar things happen in UNIX shells as well.
      The shell is a user program, after receiving command, it `fork()` and `exec()`.
      The separation make it easier to do things like redirect the ouput into files
*/