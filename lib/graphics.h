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
#define BALA_D 175      // »
#define BALA_I 174      // « 
#define BALA_A 94       // ^
#define FACE 154        // Ü
#define FACE_SHOOT 153  // Ö
#define ENEMIES 159     // ƒ
#define TORRETA_R 195   // ├
#define TORRETA_L 180   // ┤
#define TORRETA_U 193   // ┴
#define TORRETA_D 194   // ┬
#define PINCHOS 65      // A

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
