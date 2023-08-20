# proto-Terminal-SO

Trabalho de Sistemas Operacionais referente à elaboração de um protótipo de terminal do Linux.

O trabalho possui um arquivo principal (`proto_terminal.c`) e um arquivo de cabeçalhos (`proto_terminal.h`) com a definição de todas as funções e estruturas utilizadas.

## Funcionalidades

- **Comandos Básicos:** O terminal é capaz de executar comandos básicos, como programas e comandos do sistema.

- **Prompt Personalizado:** Um prompt customizado é exibido, mostrando o diretório atual do usuário.

- **Redirecionamento de Entrada:** É possível redirecionar a entrada de um programa a partir de um arquivo utilizando o operador `<`.

- **Redirecionamento de Saída:** É possível redirecionar a saída de um programa para um arquivo utilizando o operador `>`.

- **Tubulação (Pipe):** Os resultados da saída de um programa podem ser redirecionados como entrada para outro programa usando o operador `|`.

- **Execução em Segundo Plano:** Um programa pode ser executado em segundo plano usando o operador `&`.

## Arquivos de teste
- **delay.c:** Conta de 1 até 5, para testar o modificador & no terminal.
- **delay2.c:** Dá sleep e volta a aparecer no terminal após 20 segundos.
- **ordem.c:** Recebe um nome e uma quantidade de segundos na entrada, finalizando a execução após os segundos determinados.
- **prog1.c:** Dá print de algo na tela.
- **prog2.c:** Recebe string no scanf e diz olá.

## Compilação e Execução

Para compilar e executar o código, utilizam-se os seguintes comandos no terminal:

```bash
gcc proto_terminal.c -o proto
./proto




