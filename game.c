#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>
#include "list.c"
#define RIGTH 0x44
#define LEFT 0x41
#define SPACE 0x20
#define SHIFT 0x10
#define FPS 16

typedef struct {
    int x;
    int y;
} Coordenadas;

typedef struct {
    char forma;
    Coordenadas pos;
} Bullet;

typedef struct {
    char forma;
    Coordenadas *posicion;
} Jugador;

void mostrarEscenario() { 
  printf("=================================================================\n");
  printf("|                                                               |\n");
  printf("|                                                               |\n");
  printf("|                                                               |\n");
  printf("|                                                               |\n");
  printf("|                                                               |\n");
  printf("|                                                               |\n");
  printf("|                                                               |\n");
  printf("|                                                               |\n");
  printf("|                                                               |\n");
  printf("|                                                               |\n");
  printf("|                                                               |\n");
  printf("|                                                               |\n");
  printf("|                                                               |\n");
  printf("|                                                               |\n");
  printf("|                                                               |\n");
  printf("|                                                               |\n");
  printf("|                                                               |\n");
  printf("|                                                               |\n");
  printf("=================================================================\n");
}

void gotoxy(int x, int y) {
  HANDLE consola = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD pos;

  pos.X = x;
  pos.Y = y;

  SetConsoleCursorPosition(consola, pos);
}

Jugador *createJugador() {
    Jugador *newPlayer = (Jugador *) malloc(sizeof(Jugador));

    newPlayer->forma = 'x';
    newPlayer->posicion = (Coordenadas *) malloc(sizeof(Coordenadas));
    newPlayer->posicion->x = 2;
    newPlayer->posicion->y = 2;

    return newPlayer;
}

void imprimirDisparo(List *proyectiles) {
    Bullet *proyectil;

    firstList(proyectiles);

    while ((proyectil = (Bullet *)nextList(proyectiles)) != NULL) {
        if (proyectil->pos.x == 63) {
            popCurrent(proyectiles);
            gotoxy(proyectil->pos.x, proyectil->pos.y);
            printf(" ");
        }
        else {
            gotoxy(proyectil->pos.x, proyectil->pos.y);
            printf(" ");
            proyectil->pos.x++;
            gotoxy(proyectil->pos.x, proyectil->pos.y);
            printf("%c", proyectil->forma);
        }
    }
}

void saltoDerecha(Coordenadas *pos, List *proyectiles) {
    int alturaMax = pos->y - 4; 

    while (pos->y > alturaMax) {
        Sleep(FPS);
        imprimirDisparo(proyectiles);
        gotoxy(pos->x, pos->y);
        printf(" ");
        if (pos->x != 63) pos->x++;
        if (pos->y == 1) break;
        pos->y--;
        gotoxy(pos->x, pos->y);
        printf("x");
    }
    
    gotoxy(pos->x, pos->y);
    printf(" ");
    gotoxy(pos->x, pos->y);
    printf("x");

    while (pos->y < 18) {
        Sleep(FPS);
        imprimirDisparo(proyectiles);
        gotoxy(pos->x, pos->y);
        printf(" ");
        if (pos->x != 63) pos->x++;
        if (pos->y != 18) pos->y++;
        gotoxy(pos->x, pos->y);
        printf("x");
    }
}

void saltoIzquierda(Coordenadas *pos, List *proyectiles) {
    int alturaMax = pos->y - 4; 

    while (pos->y > alturaMax) {
        Sleep(FPS);
        imprimirDisparo(proyectiles);
        gotoxy(pos->x, pos->y);
        printf(" ");
        if (pos->x != 1) pos->x--;
        if (pos->y == 1) break;
        pos->y--;
        gotoxy(pos->x, pos->y);
        printf("x");
    }
    
    gotoxy(pos->x, pos->y);
    printf(" ");
    gotoxy(pos->x, pos->y);
    printf("x");

    while (pos->y < 18) {
        Sleep(FPS);
        imprimirDisparo(proyectiles);
        gotoxy(pos->x, pos->y);
        printf(" ");
        if (pos->x != 1) pos->x--;
        if (pos->y != 18) pos->y++;
        gotoxy(pos->x, pos->y);
        printf("x");
    }
}

void saltoVertical(Coordenadas *pos, List *proyectiles) {
    int alturaMax = pos->y - 4;

    while (pos->y > alturaMax) {
        Sleep(FPS);
        imprimirDisparo(proyectiles);
        gotoxy(pos->x, pos->y);
        printf(" ");
        if (pos->y == 1) break;
        pos->y--;
        gotoxy(pos->x, pos->y);
        printf("x");
    }
    
    while (pos->y < alturaMax + 4) {
        Sleep(FPS);
        imprimirDisparo(proyectiles);
        gotoxy(pos->x, pos->y);
        printf(" ");
        pos->y++;
        gotoxy(pos->x, pos->y);
        printf("x");
    }
}

void createBala(Coordenadas *pos, List *proyectiles) {
    Bullet *bala = (Bullet *) malloc(sizeof(Bullet));

    bala->forma = '-';
    bala->pos.x = pos->x;
    bala->pos.y = pos->y;

    pushBack(proyectiles, bala);
}

void movimiento(Jugador *player, List *proyectiles) {
    Coordenadas *pos = player->posicion;

    if (kbhit()) {
        if (GetAsyncKeyState(LEFT)) {
            if (GetAsyncKeyState(SPACE)) saltoIzquierda(pos, proyectiles);
            else if (pos->x != 1) pos->x--;
        }
        if (GetAsyncKeyState(RIGTH)) {
            if (GetAsyncKeyState(SPACE)) saltoDerecha(pos, proyectiles);
            else if (pos->x != 63) pos->x++;
        }
        if (GetAsyncKeyState(SPACE)) saltoVertical(pos, proyectiles);
    }
    if (kbhit()) {
        if (GetAsyncKeyState(SHIFT)) createBala(pos, proyectiles);
    }
    imprimirDisparo(proyectiles);
}

int main() {
    Jugador *player = createJugador();
    Jugador *auxPlayer = createJugador();
    List *proyectiles = createList();

    system("cls");
    mostrarEscenario();
    gotoxy(player->posicion->x, player->posicion->y);
    printf("%c", player->forma);

    while (true) {
        Sleep(FPS);
        auxPlayer = player;
        gotoxy(auxPlayer->posicion->x, auxPlayer->posicion->y);
        printf(" ");
        movimiento(player, proyectiles);
        gotoxy(player->posicion->x, player->posicion->y);
        printf("%c", player->forma);
    }
}