#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 25
#define COLS 80

int main() {
    srand(time(NULL));
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%d", rand() % 2);
        }
        printf("\n");
    }
    return 0;
}