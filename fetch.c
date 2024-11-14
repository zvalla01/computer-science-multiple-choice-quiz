#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

char* fetch(char *url) {
    int pipefds[2];
    pid_t pid;
    char *response = NULL;
    size_t response_size = 0;

    //creating a pipe to read the curl output
    if (pipe(pipefds) == -1) {
        perror("pipe");
        return NULL;
    }

    //createing a child process
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return NULL;
    }

    if (pid == 0) {  //child process
        //lecture: close the unused pipe write end
        close(pipefds[0]);

        //prepare arguments for curl
        char *args[] = {"curl", "-s", url, NULL};

        //redirect stdout to the pipe
        dup2(pipefds[1], STDOUT_FILENO);
        close(pipefds[1]);

        //execute curl
        execvp(args[0], args);
        perror("execvp");
        exit(1);  //only reached if execvp fails
    } else {  //the parent process
        // lecture: close unused pipe write end
        close(pipefds[1]);

        //read the output from the pipe
        ssize_t nread;
        char buffer[1024];
        while ((nread = read(pipefds[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[nread] = '\0';
            size_t new_size = response_size + nread + 1;
            response = realloc(response, new_size);
            if (!response) {
                perror("realloc");
                return NULL;
            }
            strcat(response, buffer);
            response_size = new_size - 1;
        }

        close(pipefds[0]);

        //wait for the child process to finish
        wait(NULL);

        return response;
    }
}
