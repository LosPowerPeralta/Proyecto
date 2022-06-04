#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#define ESQUINA_SI 201
#define ESQUINA_SD 187
#define ESQUINA_II 200
#define ESQUINA_ID 188
#define BASE 205
#define ALTURA 186
#define FPS 16

typedef struct{
    int X;
    int Y;
} Coordenadas;

typedef struct{
    Coordenadas pos;
    char forma[1];
    int limiteS;
    int limiteI;
} Obstaculo;

Obstaculo *createObstaculo(int X, int Y, int limiteS, int limiteI, char *forma) {
    Obstaculo *newObstaculo = (Obstaculo *) malloc(sizeof(Obstaculo));

    strcpy(newObstaculo->forma, forma);
    newObstaculo->limiteI = limiteI;
    newObstaculo->limiteS = limiteS;
    newObstaculo->pos.X = X;
    newObstaculo->pos.Y = Y;

    return newObstaculo;
}

void mostrarEscenario() {
    int cont = 0;

    printf("%c", ESQUINA_SI);
    for (cont = 0; cont < 117; cont++) {
        printf("%c", BASE);
    }
    printf("%c\n", ESQUINA_SD);

    for (cont = 0; cont < 27; cont++) {
        printf("%c%118c\n", ALTURA, ALTURA);
    }

    printf("%c", ESQUINA_II);
    for (cont = 0; cont < 117; cont++) {
        printf("%c", BASE);
    }
    printf("%c", ESQUINA_ID);
}

void gotoxy(int x, int y) {
    HANDLE consola = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;

    pos.X = x;
    pos.Y = y;

    SetConsoleCursorPosition(consola, pos);
}

void ocultarCursor() {
    HANDLE consola = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor;

    cursor.bVisible = FALSE;
    cursor.dwSize = 50;
    SetConsoleCursorInfo(consola, &cursor);
}

