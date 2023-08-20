/*
Protótipo de Terminal do Linux - Sistemas Operacionais
Alunos:
Amanda Spellen
Arthur Matias
Bianka Vasconcelos
Lucas Darcio

Para rodar o terminal:
gcc proto_terminal.c -o proto
./proto

Desenvolvemos um arquivo com a extensão .h para armazenar a assinatura das funções e a estrutura "enum", utilizada para o tratamento dos modificadores.
Segue abaixo o código completo.

Para mais informações, acesse o arquivo README.md.
*/

#include "proto_terminal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

char entrada_usuario[200]; // entrada 
int rc; // id de processo

/*
Definição do processo de elaboração do terminal:
1. Ler entrada digitada pelo usuário e subdividir a string
2. Fazer os forks
3. Fazer condicionais para modificadores de acesso
*/

// Estrutura para identificar os modificadores
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

// Quantidade de argumentos passados na entrada
int tamanhoEntrada(char entrada[]){
    int i = 0, cont = 1;
    while(entrada[i] != '\0' && entrada[i] != '\n'){
        if (entrada[i] == ' '){
            cont++;
        }
        i++;
    }
    return cont;
}

// Função para que o diretório atual do usuário apareça no terminal
void printPrompt() {
    char cwd[200];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("[%s] >>> ", cwd);
    } else {
        perror("getcwd");
        printf(">>> ");
    }
    fflush(stdout); // limpa o buffer
}

// Função para mostrar o vetor de entrada (para teste do código)
void mostrarVetor(int tamanho, char vetorDeEntrada[][200]){
    for(int i = 0; i < tamanho; i++){
        printf("%s ", vetorDeEntrada[i]);
    }
}

void printPID() {
    printf("[PID %d]\n", rc);
}

// Separar a string digitada pelo usuário e adicioná-la em um vetor de caracteres
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

// Função para adicionar o ./ ao comando, se necessário
void adicionaPontoBarra(char vetorDeEntradas[][200]) {
    char ponto_barra[201] = "./"; // colocando o ./ para verificar se é programa
    char aux[200];
    strcpy(aux, vetorDeEntradas[0]);
    strcat(ponto_barra, vetorDeEntradas[0]);
    //printf("%s\n", ponto_barra);

    strcpy(vetorDeEntradas[0], ponto_barra);
}

int comandoVazio() {
    int vazio = 0, i = 0;
    int tam_entrada = strcspn(entrada_usuario, "\0");

    // Se a string só for composta de espaços ou vazio
    while (entrada_usuario[i] == ' ') {
        vazio++;
        i++;
    }
    
    if ((vazio == tam_entrada) || !tam_entrada) {
        return 1;
    }
    else {
        return 0;
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
        char ponto_barra[201] = "./"; // colocando o ./ juntamente a variável aux, pois caso não seja um programa, o ./ deve ser removido.
        char aux[200];
        strcpy(aux, vetorDeEntradas[0]);
        strcat(ponto_barra, vetorDeEntradas[0]);
        //printf("%s\n", ponto_barra);

        strcpy(vetorDeEntradas[0], ponto_barra);
  
        int rc = fork();
        if (rc < 0){
            fprintf(stderr, "Erro no fork.\n");
        }

        // Execução do comando no processo pai
        else if (rc == 0){
            // printf("t: '%s' forkelif[PID %d]\n", entrada_usuario, rc); 
            int i = 0;
            char *myargs[quantidade_de_termos+1];
            for (int i = 0; i < quantidade_de_termos; i++){
                myargs[i] = strdup(vetorDeEntradas[i]);
            }
            myargs[quantidade_de_termos] = NULL;
            execvp(myargs[0], myargs);  // roda comando

                   
            strcpy(myargs[0], aux);
            /* //del
            strcpy(vetorDeEntradas[0], aux);
            myargs[0] = strdup(vetorDeEntradas[0]);
            
            i = 0;
            for (int i = 0; i < quantidade_de_termos; i++){
                myargs[i] = strdup(vetorDeEntradas[i]);
            }
            myargs[quantidade_de_termos] = NULL;
            */

            execvp(myargs[0], myargs);  // roda comando
            if (!comandoVazio()) {
                printf("Comando inválido.\n");
            }
            exit(1);
        }

        // O processo filho espera enquanro o pai executa
        else{
            // printf("t: '%s' forkelse[PID %d]\n", entrada_usuario, rc); //del
            waitpid(rc, NULL, 0);

            //int wc = wait(NULL);
        }    
    }
    
}

// Funções para o tratamento dos modificadores

// 1. Input < (roda prog1 e recebe como input o arquivo entrada)
void executaInput(char vetorDeEntradas[][200], int quantidade_de_termos) {
    //adicionaPontoBarra(vetorDeEntradas);
    int idx_input = -1;
    for (int i = 0; i < quantidade_de_termos; i++) {
        if (!strcmp(vetorDeEntradas[i], "<")) {
            idx_input = i;
            break;
        }
    }

    int rc = fork();
    if (rc < 0){
        fprintf(stderr, "Erro no fork.\n");
    }    
 
    else if (rc == 0) {
        // Abre o arquivo de entrada
        //printf("%s  %s\n", vetorDeEntradas[idx_input + 1], vetorDeEntradas[quantidade_de_termos - 1]);
        int input_fd = open(vetorDeEntradas[quantidade_de_termos - 1], O_RDONLY);
        if (input_fd == -1) {
            perror("Erro ao abrir o arquivo de entrada");
            exit(1);
        }
        
        // Redireciona a entrada padrão para o arquivo
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);

        // Executa o comando
        char *myargs[quantidade_de_termos];
        for (int i = 0; i < quantidade_de_termos - 1; i++) {
            myargs[i] = strdup(vetorDeEntradas[i]);
        }
        myargs[quantidade_de_termos - 1] = NULL;
        
        char ponto_barra[201] = "./"; // colocando o ./ para verificar se é programa
        char aux[200];
        strcpy(aux, myargs[0]);
        strcat(ponto_barra, myargs[0]);
        strcpy(myargs[0], ponto_barra);
        //printf("%s\n", myargs[0]);
        execvp(myargs[0], myargs);

        strcpy(myargs[0], aux);
        //printf("%s\n", myargs[0]);
        
        // Tirando o "<" dos argumentos
        //printf("%d %d\n", idx_input+1, quantidade_de_termos);
        char args[quantidade_de_termos+1][200];
        int j = 0;
        for (int i = 0; i < quantidade_de_termos; i++) {
            if (vetorDeEntradas[i][0] != '<'){
                strcpy(myargs[j], vetorDeEntradas[i]);
                j++;
            }
        }
        myargs[j] = NULL;

        execvp(myargs[0], myargs);

        // Se chegar aqui, houve um erro na execução
        perror("Erro ao executar o comando");
        exit(1);
    }
    else {
        int wc = wait(NULL);
    }
}

// 2. Output > (roda prog1 e joga o output dele para o arquivo saída)
void executaOutput(char vetorDeEntradas[][200], int quantidade_de_termos) {
    adicionaPontoBarra(vetorDeEntradas);
    int rc = fork();
    if (rc < 0){
        fprintf(stderr, "Erro no fork.\n");
    }    
    else if (rc == 0) {
        // Abre o arquivo de saída (ou cria se não existir)
        int output_fd = open(vetorDeEntradas[quantidade_de_termos - 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (output_fd == -1) {
            perror("Erro ao abrir o arquivo de saída");
            exit(1);
        }
        // Redireciona a saída padrão para o arquivo
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);

        // Executa o comando
        char *myargs[quantidade_de_termos];
        for (int i = 0; i < quantidade_de_termos -1; i++) {
            myargs[i] = strdup(vetorDeEntradas[i]);
        }
        myargs[quantidade_de_termos - 1] = NULL;
        execvp(myargs[0], myargs);

        // Se chegar aqui, houve um erro na execução
        perror("Erro ao executar o comando");
        exit(1);
    }
    else {
        int wc = wait(NULL);
    }
}

// 3. Pipe | (faz a saída de prog1 ser a entrada de prog2)
void executaPipe(char vetorDeEntradas[][200], int quantidade_de_termos) {
    // Encontrar a posição do pipe "|"
    int pipeIndex = -1;
    for (int i = 0; i < quantidade_de_termos; i++) {
        if (strcmp(vetorDeEntradas[i], "|") == 0) {
            pipeIndex = i;
            break;
        }
    }

    if (pipeIndex == -1) {
        fprintf(stderr, "Nenhum pipe encontrado no comando.\n");
        return;
    }

    // Montar os vetores de argumentos para os comandos
    char *comando1Args[pipeIndex + 1];
    for (int i = 0; i < pipeIndex; i++) {
        comando1Args[i] = vetorDeEntradas[i];
    }
    comando1Args[pipeIndex] = NULL;

    char comando1[200];
    char aux[200];
    strcpy(comando1, "./");
    strcpy(aux, comando1Args[0]);
    strcat(comando1, comando1Args[0]);
    comando1Args[0] = strdup(comando1);  // Adiciona "./" ao comando
    //printf("1: %s vs %s\n", aux, comando1Args[0]);

    // No trecho onde você está construindo o vetor de argumentos do segundo comando:
    char *comando2Args[quantidade_de_termos - pipeIndex];
    int j = 0;
    for (int i = pipeIndex + 1; i < quantidade_de_termos; i++) {
        comando2Args[j] = vetorDeEntradas[i];
        j++;
    }
    comando2Args[j] = NULL;

    char comando2[200];
    char aux2[200];
    strcpy(comando2, "./");
    strcpy(aux2, comando2Args[0]);
    strcat(comando2, comando2Args[0]);
    comando2Args[0] = strdup(comando2);  // Adiciona "./" ao comando

    //printf("2: %s vs %s\n", aux2, comando2Args[0]);

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
        execvp(comando1Args[0], comando1Args);
        comando1Args[0] = strdup(aux);
        execvp(comando1Args[0], comando1Args);

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
        execvp(comando2Args[0], comando2Args);
        comando2Args[0] = strdup(aux2);
        execvp(comando2Args[0], comando2Args);
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

// Função 1 para parar uma criança (testando o código)
void mateCrianca() {
    if (rc != 0) {
        if (kill(rc, SIGTERM) == 0) {
            printf("Sinalização de parada é enviada para o processo filho.\n");
        } else {
            perror("Erro ao enviar o sinal de parada.");
        }
    }
}

// Função 2 para parar uma criança (testando o código)
void pareEstaCriancaPLMDS(int sinal) {
    printf("Crianca, PARE.\n");
    exit(0);
}

void processosBackground(char vetorDeEntradas[][200], int quantidade_de_termos) {
    
    //printf("n = %d\n", quantidade_de_termos);
    //printf("roda <[%s %d] -> [%s %d]>\n", vetorDeEntradas[0], 0, vetorDeEntradas[quantidade_de_termos-1], quantidade_de_termos-1);

    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "Erro no fork.\n");
    } 
    
    else if (rc == 0) {
        // printf("t: '%s' &elif[PID %d]\n", entrada_usuario, rc); //del

        // Criar nova sessão independente do terminal para o programa
        setsid();

        // Ignora o sinal SIGHUP para continuar executando após o terminal ser fechado
        signal(SIGHUP, SIG_IGN);
        
        // Executa o comando em segundo plano
        char *myargs[quantidade_de_termos+1];
        for (int i = 0; i < quantidade_de_termos; i++) {
            myargs[i] = strdup(vetorDeEntradas[i]);
        }

        myargs[quantidade_de_termos] = NULL;
        
        char ponto_barra[201] = "./"; // colocando o ./ para verificar se é programa
        char aux[200];
        strcpy(aux, myargs[0]);
        strcat(ponto_barra, myargs[0]);
        strcpy(myargs[0], ponto_barra);

        execvp(myargs[0], myargs);

        strcpy(myargs[0], aux);

        execvp(myargs[0], myargs);


        // Se chegar aqui, houve um erro na execução
        perror("Erro ao executar o comando em background");
        exit(1);

    } else {
        struct timespec delay = { 0, 10000 };
        nanosleep(&delay, NULL);
        // printf("t: '%s' &else[PID %d]\n", entrada_usuario, rc); //del

        printf("Comando em background iniciado. PID: %d\n", rc);
        //mateCrianca();

        
    }
}

// 4. Background & (roda prog1 e deixa o terminal livre para outros comandos)
void executaBackground(char vetorDeEntradas[][200], int quantidade_de_termos) {
    

    // for (int i = 0; i < quantidade_de_termos; i++) {
    //     printf("e[%s %d] ", vetorDeEntradas[i], i);
    // }
    int idx_ampersand = -1;
    for (int i = 0; i < quantidade_de_termos; i++) {
        if (!strcmp(vetorDeEntradas[i], "&")) {
            idx_ampersand = i;
            break;
        }
    }

    // Caso base: Fim dos "&"
    if (idx_ampersand == -1) {
        if (quantidade_de_termos == 0) {
            //printf("vazioooo\n");
            return;
        }
        else {
            processosBackground(vetorDeEntradas, quantidade_de_termos);
            return;

        }
        //printf("aaa %d\n", quantidade_de_termos);
    }
    else {
        char args1[idx_ampersand][200];
        char args2[quantidade_de_termos][200];


        //printf("idx %d\n", idx_ampersand);
        // Separando primeiro comando do restante do comando completo
        for (int i = 0; i < idx_ampersand; i++) {
            strcpy(args1[i], vetorDeEntradas[i]);
        }
        //printf("a1 <%s -> %s>\n", args1[0], args1[idx_ampersand-1]);

        int j = 0;
        for (int i = idx_ampersand+1; i < quantidade_de_termos; i++) {
            strcpy(args2[j], vetorDeEntradas[i]);
            //printf("%s ", args2[j]);
            j++;
        }
        //printf("a2 <%s -> %s>\n", args2[0], args2[j-1]);
        //printf("\n");
        //printf("executar: [%s %d] -> [%s %d] n=%d\n", args1[0], 0, args1[idx_ampersand-1], idx_ampersand-1, idx_ampersand);
        processosBackground(args1, idx_ampersand);
        executaBackground(args2, j);
    }

}

// Função principal para rodar o terminal
int main() {
    char cwd[200];
    char *args[10];  // Máximo de 10 argumentos
    int num_args;

    while (1) {
        printPrompt();
        fgets(entrada_usuario, sizeof(entrada_usuario), stdin);
        //scanf("%200s", entrada_usuario);
        entrada_usuario[strcspn(entrada_usuario, "\n")] = '\0';

        OperatorType operador = identificaOperador(entrada_usuario);

        if (!strcmp(entrada_usuario, "exit")) {
            break; // Saia do loop principal ao detectar "exit"
        }

        int tam = tamanhoEntrada(entrada_usuario);
        char vetorDeEntrada[tam][200];
        percorreStringEAdicionaNoVetor(entrada_usuario, vetorDeEntrada);

        // sequência de ifs para identificar se há algum identificador. Caso não haja, a função executaFork() é rodada.
        if (operador== OPERATOR_NONE){
            executaFork(vetorDeEntrada, tam);
        }
        else if (operador == OPERATOR_INPUT){
            // função para o '>'
            executaInput(vetorDeEntrada, tam);
        }
        else if (operador == OPERATOR_OUTPUT){
            // função para o '<'
            executaOutput(vetorDeEntrada, tam);

        }
        else if (operador == OPERATOR_PIPE){
            // função para o '|'
            executaPipe(vetorDeEntrada, tam);
        }
        else if (operador == OPERATOR_BACKGROUND){
            // função para o '&'
            executaBackground(vetorDeEntrada, tam);

        }
        //printf("Tamanho da entrada: %d\n", tam);
        //printf("%d\n", comandoVazio());
        //printf("END\n");

    }
    return 0;
}
