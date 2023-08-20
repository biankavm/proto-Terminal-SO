#include <stdio.h>

int main() {
    char input[100];
    //fgets(input, sizeof(input), stdin);
    scanf("%100s", input);
    printf("Olá %s\n", input);
    return 0;
}
