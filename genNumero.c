#include<time.h>
#include<stdlib.h>
#include<stdio.h>

int main(){
    srand(time(NULL));
    int numero = rand() % 100;
    return numero;
}