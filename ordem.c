#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <sys/wait.h>

int main(int argc, char *argv[]){
    char *nome = argv[1];

    char *p;
    long t = strtol(argv[2], &p, 10);
    
    printf("P. [%s] começou a executar por [%lds]\n", nome, t);
    
    
    for (int i = 0; i < t; i++){
        //printf("[%s]\t\t[%ds]. . . \n", nome, i);

        sleep(1);
    }
    
   //sleep(t);
    printf("\n");
    printf("P. [%s] [%lds] terminou de executar\n", nome, t);

    /*
    1- o último programa da ordem faz o terminal voltar. 
    O que isso quer dizer em relação processos pais e filhos? 
    Quem é o pai??? Quem é o filho?

    R- Não importa, todos eles são filhos do mesmo terminal.
    */
}