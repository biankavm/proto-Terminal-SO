#include <stdio.h>
#include <unistd.h>

int main() {
    for (int i = 1; i <= 5; i++) {
        printf("Contagem: %d\n", i);
        sleep(1);
    }
    return 0;
}
