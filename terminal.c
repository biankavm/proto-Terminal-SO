#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/*
Definição do processo:
1. Ler entrada digitada pelo usuário e quebrar a string
2. Fazer forks
3. Fazer condicionais para modificadores de acesso
*/

int tamanhoEntrada(char entrada[]){
    int i = 0, cont = 1;
    while(entrada[i] != '\0'){
        if (entrada[i] == ' ' || entrada[i] == '>' || entrada[i] == '<' || entrada[i] == '&' || entrada[i] == '|'){
            cont++;
        }
        i++;
    }
    return cont;
}



void mostrarVetor(int tamanho, char vetorDeEntrada[][200]){
    for(int i = 0; i < tamanho; i++){
        printf("%s\n", vetorDeEntrada[i]);
    }
}

void percorreStringEAdicionaNoVetor(char entrada_usuario[200], char vetor_entradas[][200]){
    int i = 0, j = 0, k = 0;
    char resultado[200];
    while(entrada_usuario[i] != '\0'){
        if (entrada_usuario[i] != ' '){
            resultado[j] = entrada_usuario[i];
            j++;
            i++;
        }
        else{
           resultado[j] = '\0'; // Terminar a string temporária com o caractere nulo
            strcpy(vetor_entradas[k], resultado); // Copiar a string temporária para o vetor de strings
            k++;
            j = 0;
            i++;
        }
    }

    // Adicionar a última palavra no vetor de strings, se houver
    if (j > 0) {
        resultado[j] = '\0'; // Terminar a última string temporária com o caractere nulo
        strcpy(vetor_entradas[k], resultado); // Copiar a última string temporária para o vetor de strings
    }
}

// Função para executar os comandos por fork
void executaFork(char vetorDeEntradas[][200], int quantidade_de_termos){
    int rc = fork();
    if (rc < 0){
        fprintf(stderr, "Erro no fork.\n");
    }
    else if (rc == 0){
        int i = 0;
        char *myargs[quantidade_de_termos];
        for (int i = 0; i < quantidade_de_termos; i++){
            myargs[i] = strdup(vetorDeEntradas[i]);
        }
        myargs[quantidade_de_termos] = NULL;
        execvp(myargs[0], myargs);  // roda comando
        printf("Isso nao deve ser imprimido");
    }
    else{
        int wc = wait(NULL);
    }    
}

int main(){

    char entrada_usuario[200];
    while(1){
        printf(">>> ");
        fgets(entrada_usuario, sizeof(entrada_usuario), stdin);
        entrada_usuario[strcspn(entrada_usuario, "\n")] = '\0';
        // Parte de processamento da string
        // Suponhamos que os termos da string sejam colocados em um vetor.
        if (!strcmp(entrada_usuario, "exit")){
            break;
        }
        else{
            int tam = tamanhoEntrada(entrada_usuario);
            char vetorDeEntrada[tam][200]; // duzentos é o tamanho máximo de uma palavra. A variável tam representa a quantidade de termos da entrada.
            percorreStringEAdicionaNoVetor(entrada_usuario, vetorDeEntrada);
            //mostrarVetor(tam, vetorDeEntrada);
            executaFork(vetorDeEntrada, tam);
        }
       
    }
    return 0;
}