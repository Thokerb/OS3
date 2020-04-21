#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#include <unistd.h>

#define SERVER_QUEUE_NAME   "/task3"
#define QUEUE_PERMISSIONS 0777
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256

int main()
{
    mqd_t qd_server;   // queue descriptors
    //long token_number = 1; // next token to be given to client

    printf("[Server] Server Start\n");
    
    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;


    if ((qd_server = mq_open (SERVER_QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        perror ("[Server] mq_open");
        exit (1);
    }



    char in_buffer [1024];
    char out_buffer [1024];

    int fds[2];
    pipe(fds);
    dup2(fds[1], STDOUT_FILENO);
    close(fds[1]);

    while (1)
    {
        if (mq_receive (qd_server, in_buffer, 1024, NULL) == -1) {  //blocks until a message comes
            perror ("Server: mq_receive");
            exit (1);
        }

        if(!strcmp(in_buffer,"bye")){
            break;
        }
        
        system(in_buffer);        
        read(fds[0], &out_buffer, sizeof(out_buffer));


        if (mq_send (qd_server, out_buffer, strlen (out_buffer) + 1, 0) == -1) {
            perror ("Server: Not able to send message to client");
            continue;
        }

    }
    
    //closing server
    mq_close(qd_server);

    return EXIT_SUCCESS;

}