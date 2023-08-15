#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
char entrada_usuario[200]; // entrada 

/*
Definição do processo:
1. Ler entrada digitada pelo usuário e quebrar a string
2. Fazer forks
3. Fazer condicionais para modificadores de acesso
*/

typedef enum {
    OPERATOR_NONE,
    OPERATOR_INPUT,
    OPERATOR_OUTPUT,
    OPERATOR_PIPE,
    OPERATOR_BACKGROUND
} OperatorType;

OperatorType identificaOperador(const char *command) {
    if (strchr(command, '<') != NULL) {
        return OPERATOR_INPUT;
    } else if (strchr(command, '>') != NULL) {
        return OPERATOR_OUTPUT;
    } else if (strchr(command, '|') != NULL) {
        return OPERATOR_PIPE;
    } else if (strchr(command, '&') != NULL) {
        return OPERATOR_BACKGROUND;
    } else {
        return OPERATOR_NONE;
    }
}

void executaComandoComPipe(char *comando1[], char *comando2[]) {
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("Erro ao criar o pipe");
        exit(1);
    }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
        execvp(comando1[0], comando1);
        perror("Erro ao executar o primeiro comando");
        exit(1);
    } else if (pid1 < 0) {
        perror("Erro no fork");
        exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        close(pipe_fd[1]);
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);
        execvp(comando2[0], comando2);
        perror("Erro ao executar o segundo comando");
        exit(1);
    } else if (pid2 < 0) {
        perror("Erro no fork");
        exit(1);
    }

    close(pipe_fd[0]);
    close(pipe_fd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}



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
    if (strcmp(vetorDeEntradas[0], "cd") == 0) {
        if (quantidade_de_termos > 1) {
            if (chdir(vetorDeEntradas[1]) == -1) {
                perror("cd");
            }
        } else {
            fprintf(stderr, "Usage: cd <directory>\n");
        }
    }

    else{
        char ponto_barra[500] = "./"; // colocando o ./ para verificar se é programa
        char aux[200];
        strcpy(aux, vetorDeEntradas[0]);
        strcat(ponto_barra, vetorDeEntradas[0]);
        //printf("%s\n", ponto_barra);

        strcpy(vetorDeEntradas[0], ponto_barra);

  
        int rc = fork();
        if (rc < 0){
            fprintf(stderr, "Erro no fork.\n");
        }
    else if (rc == 0){

        int i = 0;
        char *myargs[quantidade_de_termos+1];
        for (int i = 0; i < quantidade_de_termos; i++){
            myargs[i] = strdup(vetorDeEntradas[i]);
        }
        myargs[quantidade_de_termos] = NULL;
        execvp(myargs[0], myargs);  // roda comando

        strcpy(vetorDeEntradas[0], aux);
        i = 0;
        for (int i = 0; i < quantidade_de_termos; i++){
            myargs[i] = strdup(vetorDeEntradas[i]);
        }
        myargs[quantidade_de_termos] = NULL;
        execvp(myargs[0], myargs);  // roda comando


        printf("Erro.\n");
        exit(1);
    }
    else{
        int wc = wait(NULL);
    }    }
    
}

int main() {
    char cwd[200];
    char *args[10];  // Máximo de 10 argumentos
    int num_args;

    while (1) {
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("[%s] >>> ", cwd);
        } else {
            perror("getcwd");
            printf(">>> ");
        }
        fgets(entrada_usuario, sizeof(entrada_usuario), stdin);
        entrada_usuario[strcspn(entrada_usuario, "\n")] = '\0';

        if (!strcmp(entrada_usuario, "exit")) {
            break; // Saia do loop principal ao detectar "exit"
        }

        int tam = tamanhoEntrada(entrada_usuario);
        char vetorDeEntrada[tam][200];
        percorreStringEAdicionaNoVetor(entrada_usuario, vetorDeEntrada);
        
        // sequência de ifs para identificar se há algum identificador. Caso não haja, a função executaFork() é rodada.
        if (identificaOperador((char*) vetorDeEntrada) == OPERATOR_NONE){
            executaFork(vetorDeEntrada, tam);
        }
        else if (identificaOperador((char*) vetorDeEntrada) == OPERATOR_INPUT){
            // função para o '>'
        }
        else if (identificaOperador((char*) vetorDeEntrada) == OPERATOR_OUTPUT){
            // função para o '<'
        }
        else if (identificaOperador((char*) vetorDeEntrada) == OPERATOR_PIPE){
            // função para o '|'
        }
        else if (identificaOperador((char*) vetorDeEntrada) == OPERATOR_BACKGROUND){
            // função para o '&'
        }

    }
    return 0;
}
