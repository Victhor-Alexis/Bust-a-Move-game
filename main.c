#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <termios.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <time.h>

int main(void) {
    int i,j;
    char map[9][16];

    // Loading map
    for (i = 8; i >= 0; --i) {
        for (j = 0; j < 16; ++j) {
            if (i == 8 || i == 0 || j == 0 || j == 15)
                map[i][j] = '#';
            else
                map[i][j] = ' ';
        }
    }

    // Displaying map
    for (i = 8; i >= 0; --i) {
        for (j = 0; j < 16; ++j) {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }

    return 0;
}
