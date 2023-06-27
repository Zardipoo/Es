#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define NUM_ELEMENTI 10

int main() {
    key_t key1 = ftok(".", 'A');
    key_t key2 = ftok(".", 'B');

    // Creazione della memoria condivisa per il primo array
    int shmid1 = shmget(key1, sizeof(int) * NUM_ELEMENTI, IPC_CREAT | 0666);
    if (shmid1 == -1) {
        perror("Errore creazione memoria condivisa per il primo array");
        exit(EXIT_FAILURE);
    }

    // Creazione della memoria condivisa per il secondo array
    int shmid2 = shmget(key2, sizeof(int) * NUM_ELEMENTI, IPC_CREAT | 0666);
    if (shmid2 == -1) {
        perror("Errore creazione memoria condivisa per il secondo array");
        exit(EXIT_FAILURE);
    }

    // Attach della memoria condivisa per il primo array
    int* array1 = (int*)shmat(shmid1, NULL, 0);
    if (array1 == (int*)-1) {
        perror("Errore attach memoria condivisa per il primo array");
        exit(EXIT_FAILURE);
    }

    // Attach della memoria condivisa per il secondo array
    int* array2 = (int*)shmat(shmid2, NULL, 0);
    if (array2 == (int*)-1) {
        perror("Errore attach memoria condivisa per il secondo array");
        exit(EXIT_FAILURE);
    }

    // Inizializzazione dei vettori con numeri casuali
    srand(time(NULL));
    for (int i = 0; i < NUM_ELEMENTI; i++) {
        array1[i] = rand() % 100;
        array2[i] = rand() % 100;
    }

    int risultati[NUM_ELEMENTI];

    for (int i = 0; i < NUM_ELEMENTI; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("Errore creazione processo figlio");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Figlio
            int somma = array1[i] + array2[i];
            exit(somma);
        }
    }

    for (int i = 0; i < NUM_ELEMENTI; i++) {
        int status;
        pid_t pid = wait(&status);

        if (WIFEXITED(status)) {
            int somma = WEXITSTATUS(status);
            risultati[i] = somma;
        }
    }

    printf("Risultati: ");
    for (int i = 0; i < NUM_ELEMENTI; i++) {
        printf("%d ", risultati[i]);
    }
    printf("\n");

    // Detach della memoria condivisa per il primo array
    shmdt(array1);

    // Detach della memoria condivisa per il secondo array
    shmdt(array2);

    // Deallocazione della memoria condivisa per il primo array
    shmctl(shmid1, IPC_RMID, NULL);

    // Deallocazione della memoria condivisa per il secondo array
    shmctl(shmid2, IPC_RMID, NULL);

    return 0;
}
