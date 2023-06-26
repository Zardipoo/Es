#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>

#define PIPE_BUF 4096

int main(){
    pid_t pid;
    int pipefd[2];
    int file;

    if(pipe(pipefd) == -1){
        perror("Errore creazione pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if(pid == -1){
        perror("Errore creazione processo figlio");
        exit(EXIT_FAILURE);
    }

    else if(pid == 0){
        close(STDOUT_FILENO);
        dup(pipefd[1]);
        close(pipefd[0]);
        
        execl("/bin/ls", "ls", NULL);
        perror("errore esecuzione execl");

        exit(EXIT_FAILURE);
    }
    else{

        close(pipefd[1]);

        char buffer[BUFSIZ];
        size_t a = read(pipefd[0], buffer,sizeof(char)*PIPE_BUF);
        if(a == 0){
            perror("errore lettura pipe");
        }
        
        file = open("output.txt",O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if(file == 0){
            perror("Errore apertura file");
            exit(EXIT_FAILURE);
        }
        
        if(write(file, buffer,sizeof(char)*a) == 0){
            perror("errore scrittura su file");
        }

        close(file);

        //printf("%s",buffer);

        wait(NULL);
    }
}