#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <termios.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <time.h>

char randChar();
int getch();

int main(void) {
    int i,j;
    char map[9][16];
    char piece = randChar();
    char action = ' ';

    do {
        // Loading map
        for (i = 8; i >= 0; --i) {
            for (j = 0; j < 16; ++j) {
                if (i == 8 || i == 0 || j == 0 || j == 15)
                    map[i][j] = '#';
                else if (i == 1 && j == 8)
                    map[i][j] = piece;
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

        action = getch();

        system("clear");

    } while (action != 'c');

    return 0;
}

char randChar() {
    int randNum = rand() % 5 + 1;
    char letra;

    return ('A' + rand() % 5);
}

int getch() {
    int c = 0;
    struct termios org_opts, new_opts;
    int res = 0;
    /* Salva configurac¸˜oes atuais com uso do buffer */
    res=tcgetattr(STDIN_FILENO, &org_opts);
    assert(res == 0);
    /* Configura o terminal para uso especial */
    memcpy(&new_opts, &org_opts, sizeof(new_opts));
    /* new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
    */
    new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
    c = getchar();
    /* Restaura configurac¸˜oes */
    res=tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
    assert(res == 0);

    return(c);
}
