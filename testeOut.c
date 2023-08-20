#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <sys/wait.h>

int main(int argc, char *argv[]){
    char *nome = argv[1];    
    printf("P. [%s] diz olá\n", nome);
    printf("Opiniões:\n");
    printf("%s diz que esse trabalho causa dor\n%s diz que sente fome\n%s diz que nunca mais quer ver forks\n%s se arrepende de seus pecados\n%s etc etc\n", nome, nome, nome, nome, nome);
}