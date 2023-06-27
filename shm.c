#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_ELEMENTI 10
#define KEY 1234

int main() {
    // Creazione della memoria condivisa
    int shmid = shmget(KEY, sizeof(int) * NUM_ELEMENTI, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("Errore creazione memoria condivisa");
        exit(EXIT_FAILURE);
    }

    // Attach della memoria condivisa
    int* memoria_condivisa = (int*)shmat(shmid, NULL, 0);
    if (memoria_condivisa == (int*)-1) {
        perror("Errore attach memoria condivisa");
        exit(EXIT_FAILURE);
    }

    // Inizializzazione dei vettori con numeri casuali
    for (int i = 0; i < NUM_ELEMENTI; i++) {
        memoria_condivisa[i] = rand() % 100;
    }

    // Creazione dei processi figli
    pid_t pid;
    int risultati[NUM_ELEMENTI];

    for (int i = 0; i < NUM_ELEMENTI; i++) {
        pid = fork();

        if (pid == -1) {
            perror("Errore creazione processo figlio");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Processo figlio
            int somma = memoria_condivisa[i] + i;
            shmdt(memoria_condivisa); // Detach della memoria condivisa
            exit(somma);
        }
    }

    // Raccolta dei valori di ritorno dei figli
    for (int i = 0; i < NUM_ELEMENTI; i++) {
        int status;
        wait(&status);

        if (WIFEXITED(status)) {
            risultati[i] = WEXITSTATUS(status);
        }
    }

    // Stampa dei risultati
    printf("Risultati: ");
    for (int i = 0; i < NUM_ELEMENTI; i++) {
        printf("%d ", risultati[i]);
    }
    printf("\n");

    // Deallocazione della memoria condivisa
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}