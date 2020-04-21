#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if(argc != 2){
        printf("wrong number of parameters");
        return EXIT_FAILURE;
    }
    char* arg = argv[1];
    int pipefd[2];
    //pid_t cpid;
    pipe(pipefd);   //pipe is a way of communication between a child process and parent; pipefd[0] reading; pipefd[1] writing


    int status = fork();
    
    switch (status){
    
    case 0:{

        int status2 = fork();

        if(status2 == 0){
            dup2(pipefd[1],STDOUT_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);
            execl("/bin/ls", "ls", "-l", NULL);
            perror("excel");
            exit(EXIT_FAILURE);        
        }
        if(status2 == -1){
            perror("fork");
            return EXIT_FAILURE;
        }
        wait(NULL);
        dup2(pipefd[0],STDIN_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execl("/bin/grep", "grep",arg, NULL);
        
        perror("excel");
        exit(EXIT_FAILURE);
        break;
    }
    case -1:{
        perror("fork");
        exit(EXIT_FAILURE);
        break;
    }
    default:{
        return EXIT_SUCCESS;
    }
    }
}