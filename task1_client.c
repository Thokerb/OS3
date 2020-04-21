#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
#include <unistd.h>

const char* BASE_PATH = "/home/thomas/Temp/";


int main(int argc, char const *argv[])
{
    if(argc != 2){
        printf("wrong number of parameters");
        return EXIT_FAILURE;
    }
    char* str = malloc(sizeof(BASE_PATH)+sizeof(argv[1]));
    strcat(str,BASE_PATH);
    strcat(str,argv[1]);

    FILE *fp_fifo;
    if ( (fp_fifo = fopen(str, "w")) == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
        fprintf(fp_fifo, "%s", "hello from child\n");
        fprintf(fp_fifo, "%s", "2hello from child\n");


    if ( fclose(fp_fifo) == EOF ) {
        perror("fclose");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
