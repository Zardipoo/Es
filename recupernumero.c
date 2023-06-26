#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

int main(){
    pid_t pid;
    int status;

    pid = fork();

    if(pid == -1){
        perror("Errore creazione processo figlio");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0){
        execl("./genNumero", "genNumero", NULL);
        perror("Errore esecuzione execl");
        exit(EXIT_FAILURE);
    }
    else{
        waitpid(pid,&status,0);
        if(WIFEXITED(status)){
            int numero = WEXITSTATUS(status);
            printf("il numero generato è %d \n",numero);
        }
    }

    return 0;
}