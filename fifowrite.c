#include<stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define SIZE_BUFF 4096
int main(){
    int fifo, file;

    fifo = open("myfifo",O_RDONLY);
    if(fifo == -1){
        perror("Errore apertura FIFO");
        return 1;
    }

    file = open("output.txt",O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(file == 0){
        perror("Errore apertura file");
        exit(EXIT_FAILURE);
    }

    char out[SIZE_BUFF];
    ssize_t a = read(fifo, out, sizeof(char)* SIZE_BUFF);
    if(a == 0){
        perror("Error lettura fifo");
    }

    if(write(file,out,sizeof(char)*a) == 0){
        perror("errore scrittura fifo");
    }

    close(file);

    return 0;
}