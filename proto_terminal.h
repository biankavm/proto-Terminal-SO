#ifndef PROTO_TERMINAL_H
#define PROTO_TERMINAL_H

typedef enum {
    OPERATOR_NONE,
    OPERATOR_INPUT,
    OPERATOR_OUTPUT,
    OPERATOR_PIPE,
    OPERATOR_BACKGROUND
} OperatorType;

OperatorType identificaOperador(const char *command);

void executaComandoComPipe(char *comando1[], char *comando2[]);

int tamanhoEntrada(char entrada[]);

void printPrompt();

void mostrarVetor(int tamanho, char vetorDeEntrada[][200]);

void printPID();

void mostrarVetor(int tamanho, char vetorDeEntrada[][200]);

void percorreStringEAdicionaNoVetor(char entrada_usuario[200], char vetor_entradas[][200]);

int comandoVazio();

void adicionaPontoBarra(char vetorDeEntradas[][200]);

void executaFork(char vetorDeEntradas[][200], int quantidade_de_termos);

void executaInput(char vetorDeEntradas[][200], int quantidade_de_termos);

void executaOutput(char vetorDeEntradas[][200], int quantidade_de_termos);

void executaPipe(char vetorDeEntradas[][200], int quantidade_de_termos);

void executaBackground(char vetorDeEntradas[][200], int quantidade_de_termos);

void mateCrianca(); // função apenas para teste do código

void pareEstaCriancaPLMDS(int sinal); // função de teste

void processosBackground(char vetorDeEntradas[][200], int quantidade_de_termos);

#endif
