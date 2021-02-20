#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <termios.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <math.h>

char randChar();
int getch();
int mExist(int x, int y);
float calculatingM(int x, int y);
float calculatingN(int x, int y, float m);
float lineEquationAproxValue(int x, int y, float m, float n);

int main(void) {
    int i,j;
    char map[9][16];
    char piece = randChar();
    char action = ' ';
    int xf = 8, yf = 7, x, y, auxX, auxY, limXRight = 1, limXLeft = -2;
    int mNaN = 0;
    float m, n;
    float mostAprox;

    do {
        // map initialization
        for (i = 8; i >= 0; --i)
            for (j = 0; j < 16; ++j)
                map[i][j] = ' ';

        x = xf;
        y = yf;

        //printf("m:%f, n:%f\n", m,n);

        // Loading map
        for (i = 8; i >= 0; --i) {
            for (j = 0; j < 16; ++j) {
                if (i == 8 || i == 0 || j == 0 || j == 15) {
                    map[i][j] = '#';
                }
                else if (i == 1 && j == 8) {
                    map[i][j] = piece;
                }
                else if (i == yf && j == xf) {// Drawing aim trajectory
                    if (mNaN == 0) {
                        while (y > 1) {
                            map[y][x] = '.';
                            --y;
                        }
                    }
                    else {
                        map[y][x] = '.';
                        //printf("Inc(%d,%d)\n", x,y);

                        while(y > 2) {
                            mostAprox = 1000000;
                            //printf("Inc(%d,%d)\n", x,y);

                            if (yf < 7 && xf > 8) { // infinity points bug
                                limXRight = 0;
                            }
                            else if (yf < 7 && xf < 8){
                                limXLeft = -1;
                            }
                            else {
                                limXLeft = -2;
                                limXRight = 1;
                            }

                            // Storing previous line coordinates;

                            // Finding the most approximate point for the non-continuous terminal space:
                            for (int k = limXRight; k > limXLeft; --k) {
                                for (int l = -1; l < 1; ++l) {
                                    if (!(k == 0 && l == 0)) {
                                        if (mostAprox > fabs(lineEquationAproxValue(x+k,y+l,m,n))) {
                                            mostAprox = fabs(lineEquationAproxValue(x+k,y+l,m,n));
                                            auxX = x+k;
                                            auxY = y+l;
                                            //printf("menor: %f\n", mostAprox);
                                        }
                                        //printf("%f ", lineEquationAproxValue(x+k,y+l,m,n));
                                        //printf("(x+%d,y+%d)\n", k,l);
                                        //printf("(%d,%d)\n", x+k,y+l);
                                        //printf("(%d,%d)\n", x,y);
                                    }// "adjacent points"
                                }
                            }// k loop brace

                            //printf("OutG(%d,%d)\n", auxX, auxY);
                            //printf("\n");
                            x = auxX;
                            y = auxY;
                            map[auxY][auxX] = '.';
                        }
                    }
                }
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

        switch (action) {// movement
            case 'w':
                piece = randChar();
            break;

            case 'a':
                if (yf < 7 && xf > 8) { // right map limit
                    ++yf;
                }
                else {
                    if (map[yf][xf-1] == ' ') {// Respecting limits of the map
                        --xf;
                    }
                    else if ((map[yf][xf-1] == '#' || map[yf][xf-1] != '#') && yf > 2) {
                        --yf;

                        if (map[yf][xf-1] == ' ') { // pieces in the way
                            --xf;
                        }
                    }
                }

                m = calculatingM(xf,yf);
                n = calculatingN(xf,yf,m);

                mNaN = mExist(xf,yf);
            break;

            case 'd':
                if (yf < 7 && xf < 8) { // left map limit
                    ++yf;
                }
                else {
                    if (map[yf][xf+1] == ' ') {// Respecting limits of the map
                        ++xf;
                    }
                    else if ((map[yf][xf+1] == '#' || map[yf][xf+1] != '#') && yf > 2) { // for pieces in the way and when the aim reach the map limit (#)
                        --yf;

                        if (map[yf][xf+1] == ' ') { // pieces
                            ++xf;
                        }
                    }
                }

                m = calculatingM(xf,yf);
                n = calculatingN(xf,yf,m);

                mNaN = mExist(xf,yf);
            break;
        }

        system("clear");

    } while (action != 'c');

    return 0;
}

char randChar() {
    int randNum = rand() % 5 + 1;
    char letra;

    return ('A' + rand() % 5);
}

int mExist(int x, int y) { // Checking if the angular coefficient is equal to tan(90 deg);
    return !(x == 8 && y == 7);
}

float calculatingM(int x, int y) {
    return ((float)(y - 2)/(float)(x - 8));
}

float calculatingN(int x, int y, float m) {
    return (float)(y - m*x);
}

float lineEquationAproxValue(int x, int y, float m, float n) {
    return (float)(m*x + n - y);
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
