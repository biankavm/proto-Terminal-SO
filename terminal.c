#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
Definição do processo:
1. Ler entrada digitada pelo usuário e quebrar a string
2. Fazer forks
3. Fazer condicionais para modificadores de acesso
*/

int main(){

    char entrada_usuario[200];
    printf(">>> ");
    while(1){
        fgets(entrada_usuario, sizeof(entrada_usuario), stdin);
        entrada_usuario[strcspn(entrada_usuario, "\n")] = '\0';
        // Parte de processamento da string
        if (!strcmp(entrada_usuario, "exit")){
            break;
        }
    }
}