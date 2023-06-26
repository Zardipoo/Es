#include<stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    int fifo;
    if (mkfifo("myfifo", 0666) == -1) {
        perror("Errore creazione FIFO");
        return 1;
    }
    
    fifo = open("myfifo",O_WRONLY);
    if(fifo == -1){
        perror("Errore creaziione fifo");
        return 1;
    }

    close(1);
    dup(fifo);
    close(fifo);

    execl("/bin/ls", "ls", NULL);
}