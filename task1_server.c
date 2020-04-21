#include <stdlib.h>
#include <stdio.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


const char* BASE_PATH = "/home/thomas/Temp/";


void makeFifoPipe(const char* fifoName){
    char* str = malloc(sizeof(BASE_PATH)+sizeof(fifoName));
    strcat(str,BASE_PATH);
    strcat(str,fifoName);
    printf("making pipe %s\t",str);
    if ( mkfifo(str, 0777) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
    printf("created pipe %s\n",str);
}

int openFifoPipe(const char* fifoName){
    int fp_fifo = 0;

    char* str = malloc(sizeof(BASE_PATH)+sizeof(fifoName));
    strcat(str,BASE_PATH);
    strcat(str,fifoName);
    printf("opening connection for %s\t",str);

    if ( (fp_fifo = open(str, O_RDONLY)) == -1){
        perror("open");
        exit(EXIT_FAILURE);
    }
    printf("%s connected\n",fifoName);
    return fp_fifo;
}



int main(int argc, char const *argv[])
{
    fd_set connections;
    FD_ZERO(&connections);
    int connectionValues[argc-1];
    int conn = 0;
    for(int i = 1; i < argc; i++){
        makeFifoPipe(argv[i]);
    }
        for(int i = 1; i < argc; i++){
        conn = openFifoPipe(argv[i]);
        connectionValues[i-1]= conn;
        FD_SET(conn,&connections);
    }

    int openConn = 0;
    char buf[1024];
    int res = 0;
    int disconnected = 0;
 
    while(1){
        openConn = select(FD_SETSIZE,&connections,NULL,NULL,NULL);
        if(disconnected == argc-1){
            break;
        }
        
        if(openConn == -1){
            perror("select");
            exit(EXIT_FAILURE);
        }
        for(int k = 0; k < argc;k++){
            if(connectionValues[k] == -1){
                continue;
            }
            if(FD_ISSET(connectionValues[k],&connections)){
                res = read(connectionValues[k],buf,sizeof(buf));
                if (res > 0)
                {
                    if(strcmp(buf,"") != 0){
                        connectionValues[k] = -1;
                        printf("%s disconnected \n",argv[k+1]);
                        disconnected++;
                        break;
                    }
                    printf("%s: %s\n",argv[k+1],buf);
                }
                break;
            }
        }
    }

    printf("all clients disconnected, server is shutting down\n");
    return EXIT_SUCCESS;
}
