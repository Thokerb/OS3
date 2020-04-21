#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>

#define MAX 1024 

#define SERVER_QUEUE_NAME   "/task3"
#define QUEUE_PERMISSIONS 0777
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256


int main()
{
    mqd_t qd_server;   // queue descriptors

    char buf[MAX]; 
    char in_buffer[MAX];



    if ((qd_server = mq_open (SERVER_QUEUE_NAME, O_RDWR)) == -1) {
        perror ("[Server] mq_open");
        exit (1);
    }


    do
    {
        fgets(buf, MAX, stdin); 
        printf("Sending Message: %s",buf);

        if(!strcmp(buf,"\n")){
            printf("%s is empty",buf);
            break;
        }

        if (mq_send (qd_server, buf, strlen (buf) + 1, 0) == -1) {
            perror ("Client: Not able to send message to server");
            continue;
        }

        if (mq_receive (qd_server, in_buffer, 1024, NULL) == -1) {  //blocks until a message comes
            perror ("Server: mq_receive");
            exit (1);
        }
        printf("[Client] Response: %s",in_buffer);

    } while (1);

        if (mq_send (qd_server,"bye", strlen ("bye") + 1, 0) == -1) {
            perror ("Client: Not able to send message to server");
            
        }


return EXIT_SUCCESS;

}