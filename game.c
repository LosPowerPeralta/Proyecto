#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <stdbool.h>
#include "lib\list.c"
#include "lib\graphics.h"
#include "lib\operadores.h"
#define UP 0x57
#define DOWN 0x53
#define RIGHT 0x44
#define LEFT 0x41
#define ALTURA 186
#define SPACE 0x20
#define SHIFT 0x10
#define SALTO_MAX 6

//Para compilar usar gcc game.c -o game -lwinmm

typedef struct {
    int X;
    int Y;
    char forma;
} Pixel;

typedef struct {
    int time;
    bool flag;
} CoolDown;

typedef struct {
    Pixel *info;
    int direccion;
} Bullet;

typedef struct {
    Pixel *info;
    List *balas;
    int health;
    bool havePistol;
    int direccion;
} Player;

typedef struct {
    CoolDown *CDshoot;
    Pixel *info;
    int direccion;
    List *balas;
} Torretas;

typedef struct {
    Pixel *info;
    int limiteS;
    int limiteI;
    int direccion;
} Enemies;

/***************** | createPixel() | ****************/
/* El proposito de esta función es crear una        *
 * variable tipo Pixel para cada objeto utilizado  *
 * dentro de la pantalla.                           *
 * Recibe una coordenada y la forma del pixel.      *
 * Retorna una dirección de memoria del nuevo pixel */
Pixel *createPixel(int X, int Y, char forma) {
    Pixel *newPixel = (Pixel *) malloc(sizeof(Pixel));

    newPixel->forma = forma;
    newPixel->X = X;
    newPixel->Y = Y;

    return newPixel;
}

/***************** | createCoolDown() | *******************/
/* El proposito de esta funcion es crear una variable     *
 * tipo CoolDown la cual servirá para un tiempo de espera *
 * en cada disparo de las torretas que se encontraran     *
 * al rededor del mapa.                                   *
 * Recibe el tiempo entre cada disparo.                   *
 * Retorna una dirección de memoria de la nueva variable. */
CoolDown *createCoolDown(int time) {
    CoolDown *newCoolDown = (CoolDown *) malloc(sizeof(CoolDown));

    newCoolDown->flag = false;
    newCoolDown->time = time;

    return newCoolDown;
}

/**************** | createBullet() | ***************/
/* El proposito de esta función es crear una bala  *
 * y añadirla en una lista.                        *
 * Recibe la posición inicial de la bala y una     *
 * lista en donde estará guardada.                 *
 * No retorna valores.                             */
void *createBullet(int X, int Y, int direccion, List *balas) {
    Bullet *newBullet = (Bullet *) malloc(sizeof(Bullet));

    newBullet->direccion = direccion;

    if (direccion == RIGHT) newBullet->info = createPixel(X, Y, BALA_D);
    if (direccion == LEFT) newBullet->info = createPixel(X, Y, BALA_I);
    if (direccion == UP) newBullet->info = createPixel(X, Y, BALA_A);

    pushBack(balas, newBullet);
}

/****************** | createPlayer() | *****************/ 
/* El proposito de esta función es crear una variable  *
 * tipo Player la cual contiene los datos del jugador. *
 * No recibe valores.                                  *
 * Retorna una dirección de momeria de la nueva        *
 * variable.                                           */
Player *createPlayer() {
    Player *newPlayer = (Player *) malloc(sizeof(Player));

    newPlayer->direccion = RIGHT;
    newPlayer->havePistol = true;
    newPlayer->health = 3;
    newPlayer->info = createPixel(1, 22, FACE);
    newPlayer->balas = createList();
    
    return newPlayer;
}

/****************** | createTorretas() | *****************/
/* El proposito de esta función es crear una variable    *
 * tipo Torreta la cual contiene todos los datos de      *
 * alguna torreta que se encuentre en el mapa.           *
 * Recibe las coordenadas de la torreta, su dirección    *
 * y el tiempo entre cada disparo.                       *
 * Retorna la dirección de memoria de la nueva variable. */
Torretas *createTorretas(int X, int Y, int direccion, int time) {
    Torretas *newTorretas = (Torretas *) malloc(sizeof(Torretas));
    char forma;

    newTorretas->balas = createList();
    newTorretas->CDshoot = createCoolDown(time);

    if (direccion == RIGHT) forma = TORRETA_R;
    if (direccion == LEFT) forma = TORRETA_L;
    if (direccion == DOWN) forma = TORRETA_D;
    if (direccion == UP) forma = TORRETA_U;

    newTorretas->direccion = direccion;
    newTorretas->info = createPixel(X, Y, forma);

    return newTorretas;
}

/****************** | createEnemies() | ******************/
/* El proposito de esta función es crear una variable    *
 * tipo Enemies la cual almacenara todos los datos       *
 * de un enemigo que se encuentre en el mapa.            *
 * Recibe la posición inicial del enemigo y los          *
 * limites de movimiento.                                *
 * Retorna la dirección de momeria de la nueva variable. */
Enemies *createEnemies(int X, int Y, int limiteS, int limiteI) {
    Enemies *newEnemies = (Enemies *) malloc(sizeof(Enemies));

    newEnemies->direccion = LEFT;
    newEnemies->info = createPixel(X, Y, ENEMIES);
    newEnemies->limiteI = limiteI;
    newEnemies->limiteS = limiteS;

    return newEnemies;
}

void leerArchivo(List *enemies) {
    FILE *archivo = fopen("csv\\enemies.csv", "r");
    char linea[1024];
    char X[3];
    char Y[2];
    char forma[3];
    char limiteS[3];
    char limiteI[3];
    char direccion[5];
    Enemies *enemy;

    while (fgets(linea, 1023, archivo) != NULL) {
        strcpy(X, get_csv_field(linea, 0));
        strcpy(Y, get_csv_field(linea, 1));
        strcpy(direccion, get_csv_field(linea, 2));
        strcpy(limiteI, get_csv_field(linea, 3));
        strcpy(limiteS, get_csv_field(linea, 4));
        enemy = createEnemies(toNumber(X), toNumber(Y), toNumber(limiteS), toNumber(limiteI));
        pushBack(enemies, enemy);
    }

}

void movimientoEnemigos(List *enemies) {
    Enemies *enemy = firstList(enemies);
    Pixel *pos;

    while(enemy != NULL) {
        pos = enemy->info;

        gotoxy(pos->X, pos->Y);
        printf(" ");
        if (enemy->direccion == RIGHT) {
            if (pos->X != enemy->limiteS) pos->X++;
            gotoxy(pos->X, pos->Y);
            printf("%c", pos->forma);
            if (pos->X == enemy->limiteS) {
                gotoxy(pos->X, pos->Y);
                printf(" ");
                enemy->direccion = LEFT;
            }
        }
        if (enemy->direccion == LEFT) {
            if (pos->X != enemy->limiteI) pos->X--;
            gotoxy(pos->X, pos->Y);
            printf("%c", pos->forma);
            if (pos->X == enemy->limiteI) {
                enemy->direccion = RIGHT;
                gotoxy(pos->X, pos->Y);
                printf(" ");
            }
        }
        enemy = nextList(enemies);
    }
}

void disparo(List *balas) {
    Bullet *bala;

    bala = (Bullet *)firstList(balas);
    while (bala != NULL) {

        gotoxy(bala->info->X, bala->info->Y);
        printf(" ", bala->info->forma);
        if (bala->direccion == RIGHT) {
            if (bala->info->X != 117) bala->info->X++;
            gotoxy(bala->info->X, bala->info->Y);
            printf("%c", bala->info->forma);
            if (bala->info->X == 117) {
                gotoxy(bala->info->X, bala->info->Y);
                printf(" ");
                popCurrent(balas);
            }
        }
        if (bala->direccion == LEFT) {
            if (bala->info->X != 1) bala->info->X--;
            gotoxy(bala->info->X, bala->info->Y);
            printf("%c", bala->info->forma);
            if (bala->info->X == 1) {
                gotoxy(bala->info->X, bala->info->Y);
                printf(" ");
                popCurrent(balas);
            }
        }  
        if (bala->direccion == UP) {
            if (bala->info->Y != 1) bala->info->Y--;
            gotoxy(bala->info->X, bala->info->Y);
            printf("%c", bala->info->forma);
            if (bala->info->Y == 1) {
                gotoxy(bala->info->X, bala->info->Y);
                printf(" ");
                popCurrent(balas);
            }
        }
        bala = (Bullet *)nextList(balas);
    }

}

/************** | salto() | **************/
/* El proposito de esta función es       *
 * povocar el salto del jugador.         *
 * Recibe una variable pixel con toda la *
 * info. del usuario y un variable tipo  *
 * que indica el tipo de salto.          *
 * tipo = 1 (Salto hacia la derecha)     *
 * tipo = 2 (Salto hacia la izquierda)   *
 * tipo = c/n (salto vertical)           *
 * No retorna valores.                   */ 
void salto(Pixel *pos, int tipo, List *balas, List *enemies) {
    int alturaMax = pos->Y - SALTO_MAX;

    //Subida
    while (pos->Y > alturaMax) {
        Sleep(FPS);
        disparo(balas);
        movimientoEnemigos(enemies);
        gotoxy(pos->X, pos->Y);
        printf(" ");
        if (tipo == 1 && pos->X != 117) pos->X++;
        if (tipo == 2 && pos->X != 1) pos->X--;
        if (pos->Y == 1) break;
        pos->Y--;
        gotoxy(pos->X, pos->Y);
        printf("%c", pos->forma);
    }

    gotoxy(pos->X, pos->Y);
    printf(" ");
    if (tipo == 1 && pos->X != 117) pos->X++;
    if (tipo == 2 && pos->X != 1) pos->X--;
    gotoxy(pos->X, pos->Y);
    printf("%c", pos->forma);
    
    //Bajada
    while (pos->Y < 27) {
        Sleep(FPS);
        disparo(balas);
        movimientoEnemigos(enemies);
        gotoxy(pos->X, pos->Y);
        printf(" ");
        if (tipo == 1 && pos->X != 117) pos->X++;
        if (tipo == 2 && pos->X != 1) pos->X--;
        if (pos->Y == 27) break;
        pos->Y++;
        gotoxy(pos->X, pos->Y);
        printf("%c", pos->forma);
    }
}

/************* | movimientoLateral() | **************/
/* El proposito de esta función es llevar a cabo el *
 * movimiento de izquierda a derecha del jugador.   *
 * Recibe la información del jugador.               *
 * No retorna valores.                              */
void movimientoLateral(Player *jugador) {
    Pixel *pos = jugador->info;

    if (kbhit()) {
        if (GetAsyncKeyState(RIGHT)) {
            jugador->direccion = RIGHT;
            if (pos->X != 117) pos->X++;
        }
        if (GetAsyncKeyState(LEFT)) {
            jugador->direccion = LEFT;
            if(pos->X != 1) pos->X--;
        }
    }
}

/************* | movimientoVertical() | *************/
/* El proposito de esta función es llevar a cabo el *
 * tipo de movimiento vertical decidido por el      *
 * usuario.                                         *
 * Recibe la información del jugador.               *
 * No retorna valores.                              */
void movimientoVertical(Player *jugador, List *enemies) {
    Pixel *pos = jugador->info;

    if (kbhit()) {
        if (GetAsyncKeyState(RIGHT) && GetAsyncKeyState(SPACE)) salto(pos, 1, jugador->balas, enemies);
        else if (GetAsyncKeyState(LEFT) && GetAsyncKeyState(SPACE)) salto(pos, 2, jugador->balas, enemies);
        else if (GetAsyncKeyState(SPACE)) salto(pos, 0, jugador->balas, enemies);
    }
}

void acciones(Player *jugador, List *enemies) {
    Pixel *pos = jugador->info;

    movimientoLateral(jugador);
    movimientoVertical(jugador, enemies);
    if (kbhit()) {
        if (GetAsyncKeyState(UP)) jugador->direccion = UP;
        if (GetAsyncKeyState(SHIFT)) {
            createBullet(pos->X, pos->Y, jugador->direccion, jugador->balas);
            sndPlaySound("sound\\SPOILER_Sr_Pelo_Boom_Sound_Effect.wav", SND_ASYNC);
        }
    }   
    disparo(jugador->balas);
}

int main () {
    Player *jugador = createPlayer();
    List *enemies = createList();
    leerArchivo(enemies);
    //List *obstaculos = createList();
    //List *torretas = createList();

    system("cls");
    ocultarCursor();
    mostrarEscenario();
    gotoxy(jugador->info->X, jugador->info->Y);
    printf("%c", jugador->info->forma);

    while (true) {
        Sleep(FPS);
        gotoxy(jugador->info->X, jugador->info->Y);
        printf(" ");
        acciones(jugador, enemies);
        movimientoEnemigos(enemies);
        gotoxy(jugador->info->X, jugador->info->Y);
        printf("%c", jugador->info->forma);
    }

    return EXIT_SUCCESS;
}
