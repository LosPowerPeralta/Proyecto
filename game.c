#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
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
    int time;
    bool flag;
} CountDown;

typedef struct {
    char forma[2];
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

    strcpy(newPlayer->forma, ":D");  
    newPlayer->posicion = (Coordenadas *) malloc(sizeof(Coordenadas));
    newPlayer->posicion->x = 2;
    newPlayer->posicion->y = 2;

    return newPlayer;
}

CountDown *createTimer() {
    CountDown *newCountDown = (CountDown *) malloc(sizeof(CountDown));

    newCountDown->time = 100;
    newCountDown->flag = false;

    return newCountDown;
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
        printf("  ");
        if (pos->x != 63) pos->x++;
        if (pos->y == 1) break;
        pos->y--;
        gotoxy(pos->x, pos->y);
        printf(":D");
    }
    
    gotoxy(pos->x, pos->y);
    printf("  ");
    gotoxy(pos->x, pos->y);
    printf(":D");

    while (pos->y < 18) {
        Sleep(FPS);
        imprimirDisparo(proyectiles);
        gotoxy(pos->x, pos->y);
        printf("  ");
        if (pos->x != 63) pos->x++;
        if (pos->y != 18) pos->y++;
        gotoxy(pos->x, pos->y);
        printf(":D");
    }
}

void saltoIzquierda(Coordenadas *pos, List *proyectiles) {
    int alturaMax = pos->y - 4; 

    while (pos->y > alturaMax) {
        Sleep(FPS);
        imprimirDisparo(proyectiles);
        gotoxy(pos->x, pos->y);
        printf("  ");
        if (pos->x != 1) pos->x--;
        if (pos->y == 1) break;
        pos->y--;
        gotoxy(pos->x, pos->y);
        printf(":D");
    }
    
    gotoxy(pos->x, pos->y);
    printf("  ");
    gotoxy(pos->x, pos->y);
    printf(":D");

    while (pos->y < 18) {
        Sleep(FPS);
        imprimirDisparo(proyectiles);
        gotoxy(pos->x, pos->y);
        printf("  ");
        if (pos->x != 1) pos->x--;
        if (pos->y != 18) pos->y++;
        gotoxy(pos->x, pos->y);
        printf(":D");
    }
}

void saltoVertical(Coordenadas *pos, List *proyectiles) {
    int alturaMax = pos->y - 4;

    while (pos->y > alturaMax) {
        Sleep(FPS);
        imprimirDisparo(proyectiles);
        gotoxy(pos->x, pos->y);
        printf("  ");
        if (pos->y == 1) break;
        pos->y--;
        gotoxy(pos->x, pos->y);
        printf(":D");
    }
    
    while (pos->y < alturaMax + 4) {
        Sleep(FPS);
        imprimirDisparo(proyectiles);
        gotoxy(pos->x, pos->y);
        printf("  ");
        pos->y++;
        gotoxy(pos->x, pos->y);
        printf(":D");
    }
}

void createBala(Coordenadas *pos, List *proyectiles) {
    Bullet *bala = (Bullet *) malloc(sizeof(Bullet));

    bala->forma = '-';
    bala->pos.x = pos->x;
    bala->pos.y = pos->y;
    sndPlaySound("SPOILER_Sr_Pelo_Boom_Sound_Effect.wav", SND_ASYNC);
    pushBack(proyectiles, bala);
}

void movimiento(Jugador *player, List *proyectiles, CountDown *CDshoot) {
    Coordenadas *pos = player->posicion;

    if (kbhit()) {
        if (GetAsyncKeyState(LEFT)) {
            if (GetAsyncKeyState(SPACE)) saltoIzquierda(pos, proyectiles);
            else if (pos->x != 1) pos->x--;
        }
        if (GetAsyncKeyState(RIGTH)) {
            if (GetAsyncKeyState(SPACE)) saltoDerecha(pos, proyectiles);
            else if (pos->x != 62) pos->x++;
        }
        if (GetAsyncKeyState(SPACE)) saltoVertical(pos, proyectiles);
    }
    if (kbhit()) {
        if (CDshoot->flag == false) 
            if (GetAsyncKeyState(SHIFT)) {
                createBala(pos, proyectiles);
                CDshoot->flag = true;
            }
    }

    if (CDshoot->time == 100) {
        gotoxy(78,1);
        printf("   ");
        CDshoot->time = 0;
        CDshoot->flag = false;
    }
    if (CDshoot->flag == true) CDshoot->time++;
    gotoxy(65,1);
    printf("Count Down = %d", CDshoot->time);
    imprimirDisparo(proyectiles);
}

int main() {
    Jugador *player = createJugador(); 
    Jugador *auxPlayer = createJugador();
    List *proyectiles = createList();
    CountDown *CDshoot = createTimer();

    system("cls");
    mostrarEscenario();
    gotoxy(player->posicion->x, player->posicion->y);
    printf("%c", player->forma);

    while (true) {
        Sleep(FPS);
        auxPlayer = player;
        gotoxy(auxPlayer->posicion->x, auxPlayer->posicion->y);
        printf("  ");
        movimiento(player, proyectiles, CDshoot);
        gotoxy(player->posicion->x, player->posicion->y);
        printf("%s", player->forma);
    }
}
