#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 32

int readLine(FILE* file, char buffer[]);

int main()
{
    FILE* file;

    float in_value, sum;
    unsigned int line = 0;
    char buffer [BUFFER_SIZE];
    char command [4];
    sum = 0;

    printf("Welcome to basic COM4521 calculator\n");

    file = fopen("/Users/nyxfer/Documents/GitHub/gpu/lab/lab01/exercise05/commands.calc", "r");
    if (file == NULL) {
        fprintf(stderr, "File not found\n");
        exit(0);
    }

    while (readLine(file, buffer)){
        line++;
        if (!(isalpha(buffer[0]) &&  isalpha(buffer[1]) && isalpha(buffer[2]) && buffer[3]==' ')){
            fprintf(stderr, "Unknown command in the file! %u\n", line);
            exit(1);
        }

        sscanf(buffer, "%s %f", command, &in_value);
        if (strcmp(command, "add")==0){
            sum += in_value;
        }else if (strcmp(command, "sub")==0){
            sum -= in_value;
        }else if (strcmp(command, "div")==0){
            sum /= in_value;
        }else if (strcmp(command, "mul")==0){
            sum *= in_value;
        }else{
            fprintf(stderr, "Unknown command in the file!\n");
            exit(1);
        }
    }

    printf("\tSum is %f\n", sum);
    fclose(file);
    return 0;
}

int readLine(FILE* file, char buffer[]){
    int i=0;
    char c;

    while ((c = (char) getc(file)) != '\n'){
//        printf("%c\n", c);
        if (c == EOF){
            return 0;
        }
        buffer[i++] = c;
        if (i == BUFFER_SIZE) {
            fprintf(stderr, "Buffer size is too small for line input\n");
            exit(0);
        }
    }


    buffer[i] = '\0';

    if (strncmp(buffer, "exit", 4) == 0)
        return 0;
    else
        return 1;

}