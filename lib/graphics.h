#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#define ESQUINA_SI 201  // ╔
#define ESQUINA_SD 187  // ╗
#define ESQUINA_II 200  // ╚
#define ESQUINA_ID 188  // ╝
#define BASE 205        // ═
#define ALTURA 186      // ║
#define BALA 184        // ©
#define FACE 154        // Ü
#define FACE_SHOOT 153  // Ö
#define ENEMIES 159     // ƒ
#define TORRETA_R 195   // ├
#define TORRETA_L 180   // ┤
#define TORRETA_U 193   // ┴
#define TORRETA_D 194   // ┬
#define PINCHOS 94/*65*/      // A
#define PISTOL 170      // ¬
#define FPS 16

typedef struct {
    int X;
    int Y;
    char forma;
} Pixel;

typedef struct{
    int limiteX_I;
    int limiteX_S;
    int limiteY_I;
    int limiteY_S;
} Limites;

typedef struct{
    Limites *limits;
    int tipo;
} Obstaculo;

void gotoxy(int x, int y);

Limites *createLimites(int limiteX_I, int limiteX_S, int limiteY_I, int limiteY_S) {
    Limites *newLimites = (Limites *) malloc(sizeof(Limites));

    newLimites->limiteX_I = limiteX_I;
    newLimites->limiteX_S = limiteX_S;
    newLimites->limiteY_I = limiteY_I;
    newLimites->limiteY_S = limiteY_S;

    return newLimites;
}

Obstaculo *createObstaculo(Limites *limits, int tipo) {
    Obstaculo *newObstaculo = (Obstaculo *) malloc(sizeof(Obstaculo));
    newObstaculo->tipo = tipo;
    newObstaculo->limits = limits;

    return newObstaculo;
}

void mostrarEscenario(int limiteX_S, int limiteY_I) {
    int cont = 0;
    
    printf("%c", ESQUINA_SI);
    for (cont = 0; cont < limiteX_S; cont++) {
        printf("%c", BASE);
    }

    for (cont = 0; cont < limiteY_I; cont++) {
        printf("%c\n", ALTURA);
    }
    gotoxy(limiteX_S + 1, 0);
    printf("%c\n", ESQUINA_SD);

    for (cont = 1; cont < limiteY_I; cont++) {
        gotoxy(limiteX_S + 1, cont);
        printf("%c\n", ALTURA);
    }
    gotoxy(0, limiteY_I);
    printf("%c", ESQUINA_II);
    for (cont = 0; cont < limiteX_S; cont++) {
        printf("%c", BASE);
    }
    gotoxy(limiteX_S + 1, limiteY_I);
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

