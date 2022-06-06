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
#define ESC 0x1B
#define SPACE 0x20
#define SHIFT 0x10
#define SALTO_MAX 6

//Para compilar usar gcc game2.c -o game2 -lwinmm

typedef struct {
    int X;
    int Y;
    char forma;
} Pixel;

typedef struct {
    Pixel *info;
    int direccion;
} Bullet;

typedef struct {
    int altura_max;
    int tipo_salto;
    bool cima;
    bool existSalto;
} Salto;

typedef struct {
    Pixel *info;
    List *balas;
    int health;
    bool havePistol;
    int direccionH;
    int direccionV;
    bool gameOver;
    Salto *jump;
} Player;

typedef struct {
    Pixel *info;
    int limiteS;
    int limiteI;
    int direccion;
} Enemies;

List *enemies; 
List *obstaculos;

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

void *createBullet(int X, int Y, int direccion, List *balas) {
    Bullet *newBullet = (Bullet *) malloc(sizeof(Bullet));

    newBullet->direccion = direccion;
    newBullet->info = createPixel(X, Y, BALA);
    /*if (direccion == 68) newBullet->info = createPixel(X, Y, BALA_D);
    if (direccion == 65) newBullet->info = createPixel(X, Y, BALA_I);
    if (direccion == 87) newBullet->info = createPixel(X, Y, BALA_A);*/

    pushBack(balas, newBullet);
}

Salto *createSalto() {
    Salto *newSalto = (Salto *) malloc(sizeof(Salto));

    newSalto->altura_max = 0;
    newSalto->cima = false;
    newSalto->existSalto = false;
    newSalto->tipo_salto = 0;

    return newSalto;
}

/****************** | createPlayer() | *****************/ 
/* El proposito de esta función es crear una variable  *
 * tipo Player la cual contiene los datos del jugador. *
 * No recibe valores.                                  *
 * Retorna una dirección de momeria de la nueva        *
 * variable.                                           */
Player *createPlayer() {
    Player *newPlayer = (Player *) malloc(sizeof(Player));

    newPlayer->direccionH = RIGHT;
    newPlayer->direccionV = DOWN;
    newPlayer->havePistol = true;
    newPlayer->gameOver = false;
    newPlayer->health = 4;
    newPlayer->info = createPixel(2, 18, FACE);
    newPlayer->jump = createSalto();
    newPlayer->balas = createList();

    return newPlayer;
}

Player *jugador;

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

void leerArchivoObstaculos() {
    FILE *archivo = fopen("csv\\obstaculos.csv", "r");
    char linea[1024];
    int limiteX_S;
    int limiteX_I;
    int limiteY_S;
    int limiteY_I;
    int tipo;
    Limites *limits;
    Obstaculo *obstacle;

    fgets(linea, 1023, archivo);
    while (fgets(linea, 1023, archivo) != NULL) {
        limiteX_I = toNumber((char *)get_csv_field(linea, 0));
        limiteX_S = toNumber((char *)get_csv_field(linea, 1));
        limiteY_S = toNumber((char *)get_csv_field(linea, 2));
        limiteY_I = toNumber((char *)get_csv_field(linea, 3));
        tipo = toNumber((char *)get_csv_field(linea, 4));
        limits = createLimites(limiteX_I, limiteX_S, limiteY_I, limiteY_S);
        obstacle = createObstaculo(limits, tipo);
        pushBack(obstaculos, obstacle);
    }
}

void leerArchivoEnemies() {
    FILE *archivo = fopen("csv\\enemies.csv", "r");
    char linea[1024];
    int X;
    int Y;
    int forma;
    int limiteS;
    int limiteI;
    int direccion;
    Enemies *enemy;

    fgets(linea, 1023, archivo);
    while (fgets(linea, 1023, archivo) != NULL) {
        X = toNumber(get_csv_field(linea, 0));
        Y = toNumber(get_csv_field(linea, 1));
        direccion = toNumber(get_csv_field(linea, 2));
        limiteI = toNumber(get_csv_field(linea, 3));
        limiteS = toNumber(get_csv_field(linea, 4));
        enemy = createEnemies(X, Y, limiteS, limiteI);
        printf("%d\n",enemy->info->X);
        printf("%d\n",enemy->info->Y);
        printf("%c\n",enemy->info->forma);
        printf("%c\n", enemy->direccion);
        printf("%d\n",enemy->limiteI);
        printf("%d\n\n",enemy->limiteS);
        pushBack(enemies, enemy);
    }

}

bool hitBoxVPlayer() {
    Pixel *pos = jugador->info;
    Obstaculo *obstacle = firstList(obstaculos);
    Limites *limits;

    while(obstacle != NULL) {
        limits = obstacle->limits;
        if (jugador->direccionV == UP) {
            if (pos->Y == limits->limiteY_I && (pos->X >= limits->limiteX_I - 1 && pos->X <= limits->limiteX_S)) {
                if (obstacle->tipo == 2) jugador->gameOver = true;
                return true;
            }
        }
        if (jugador->direccionV == DOWN) {
            if (pos->Y + 1 == limits->limiteY_S && (pos->X >= limits->limiteX_I - 1 && pos->X <= limits->limiteX_S)) {
                if (obstacle->tipo == 2) jugador->gameOver = true;
                return true;
            }
        }

        obstacle = nextList(obstaculos);
    }
    if (jugador->direccionV == DOWN && pos->Y != 27) {
        jugador->jump->cima = true;
        jugador->jump->existSalto = true;
    }
    return false;
}

bool hitBoxHPlayer() {
    Pixel *pos = jugador->info;
    Obstaculo *obstacle = firstList(obstaculos);
    Limites *limits;

    //verificacion objetos
    while (obstacle != NULL) {
        limits = obstacle->limits;
        if (jugador->direccionH == LEFT) {
            if (pos->X - 1 == limits->limiteX_S && (pos->Y <= limits->limiteY_I && pos->Y >= limits->limiteY_S)) return true;
        }
        if (jugador->direccionH == RIGHT) {
            if (pos->X + 1 == limits->limiteX_I - 1 && (pos->Y <= limits->limiteY_I && pos->Y >= limits->limiteY_S)) return true;
        }
        obstacle = nextList(obstaculos);
    }

    return false;
}

void mostrarObstaculos() {
    Obstaculo *obstacle = firstList(obstaculos);
    Limites *limits;
    Pixel pos;

    while (obstacle != NULL) {
        limits = obstacle->limits;
        for (pos.X = limits->limiteX_I; pos.X < limits->limiteX_S; pos.X++) {
            gotoxy(pos.X, limits->limiteY_S);
            if(obstacle->tipo == 1) printf("%c", BASE);
            if(obstacle->tipo == 2) printf("%c", PINCHOS);
        }
        if(obstacle->tipo == 1) {
            pos.X = limits->limiteX_I - 1;
            gotoxy(pos.X, limits->limiteY_S);
            printf("%c", ESQUINA_SI);
            for (pos.Y = limits->limiteY_S + 1; pos.Y < limits->limiteY_I; pos.Y++) {
                gotoxy(pos.X, pos.Y);
                printf("%c", ALTURA);
            }
            gotoxy(pos.X, pos.Y);
            printf("%c", ESQUINA_II);
        
            for (pos.X = limits->limiteX_I; pos.X < limits->limiteX_S; pos.X++) {
                gotoxy(pos.X, pos.Y);
                if(obstacle->tipo == 1) printf("%c", BASE);
            }
            gotoxy(pos.X, limits->limiteY_S);
            printf("%c", ESQUINA_SD);    
            for (pos.Y = limits->limiteY_S + 1; pos.Y < limits->limiteY_I; pos.Y++) {
                gotoxy(pos.X, pos.Y);
                printf("%c", ALTURA);
            }
            gotoxy(pos.X, pos.Y);
            printf("%c", ESQUINA_ID);
        }
        obstacle = nextList(obstaculos);
    }
}

void movimientoEnemigos() {
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
void salto() {
    Salto *jump = jugador->jump;
    Pixel *pos = jugador->info;

    if ((jump->tipo_salto == 1 && pos->X != 117) && !hitBoxHPlayer()) pos->X++;
    if ((jump->tipo_salto == 2 && pos->X != 1) && !hitBoxHPlayer()) pos->X--; 

    if (jump->cima == false) {
        if (pos->Y != jump->altura_max) pos->Y--;
        if ((pos->Y == jump->altura_max || pos->Y == 1) || hitBoxVPlayer()) {
            jump->cima = true;
            jugador->direccionV = DOWN;
        }
    }
    if (jump->cima == true) {
        if (pos->Y != 27 && !hitBoxVPlayer()) pos->Y++;
        if (pos->Y == 27 || hitBoxVPlayer()) {
            jump->existSalto = false;
            jump->cima = false;     
        }
    }
}

/************* | movimientoLateral() | **************/
/* El proposito de esta función es llevar a cabo el *
 * movimiento de izquierda a derecha del jugador.   *
 * Recibe la información del jugador.               *
 * No retorna valores.                              */
void movimientoLateral() {
    Pixel *pos = jugador->info;

    if (kbhit()) {
        if (GetAsyncKeyState(RIGHT)) {
            jugador->direccionH = RIGHT;
            if (pos->X != 117 && !hitBoxHPlayer()) pos->X++;
        }
        if (GetAsyncKeyState(LEFT)) {
            jugador->direccionH = LEFT;
            if(pos->X != 1 && !hitBoxHPlayer()) pos->X--;
        }
    }
    hitBoxVPlayer();
}

/************* | movimientoVertical() | *************/
/* El proposito de esta función es llevar a cabo el *
 * tipo de movimiento vertical decidido por el      *
 * usuario.                                         *
 * Recibe la información del jugador.               *
 * No retorna valores.                              */
void movimientoVertical() {
    Pixel *pos = jugador->info;
    Salto *jump = jugador->jump;

    if (kbhit()) {
        if (jump->existSalto == false) {
            jump->altura_max = pos->Y - SALTO_MAX;
            if (GetAsyncKeyState(RIGHT) && GetAsyncKeyState(SPACE)) {
                jump->tipo_salto = 1;
                jump->existSalto = true;
                jugador->direccionV = UP;
                salto();
            }
            else if (GetAsyncKeyState(LEFT) && GetAsyncKeyState(SPACE)) {
                jump->tipo_salto = 2;
                jump->existSalto = true;
                jugador->direccionV = UP;
                salto();
            }
            else if (GetAsyncKeyState(SPACE)) {
                jump->tipo_salto = 0;
                jump->existSalto = true;
                jugador->direccionV = UP;
                salto();
            }
        }
    }
    if (jump->existSalto == true) {
        salto();
    }
}

void disparo(List *balas) {
    Bullet *bala;

    bala = (Bullet *)firstList(balas);
    while (bala != NULL) {

        gotoxy(bala->info->X, bala->info->Y);
        printf(" ");
        if (bala->direccion == 68) {
            if (bala->info->X != 117) bala->info->X++;
            gotoxy(bala->info->X, bala->info->Y);
            printf("%c", bala->info->forma);
            if (bala->info->X == 117) {
                gotoxy(bala->info->X, bala->info->Y);
                printf(" ");
                popCurrent(balas);
            }
        }
        if (bala->direccion == 65) {
            if (bala->info->X != 1) bala->info->X--;
            gotoxy(bala->info->X, bala->info->Y);
            printf("%c", bala->info->forma);
            if (bala->info->X == 1) {
                gotoxy(bala->info->X, bala->info->Y);
                printf(" ");
                popCurrent(balas);
            }
        }  
        if (bala->direccion == 87) {
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

void acciones() {
    Pixel *pos = jugador->info;

    gotoxy(jugador->info->X, jugador->info->Y);
    printf(" ");
    if (jugador->jump->existSalto != true) movimientoLateral();
    printf("%d ", jugador->gameOver);
    if (jugador->gameOver != true) {
        movimientoVertical();
        gotoxy(jugador->info->X, jugador->info->Y);
        printf("%c", jugador->info->forma);
        if (kbhit()) {
            if (GetAsyncKeyState(ESC)) exit(0);
            if (GetAsyncKeyState(SHIFT)) {
                createBullet(pos->X, pos->Y, jugador->direccionH, jugador->balas);
                //sndPlaySound("sound\\SPOILER_Sr_Pelo_Boom_Sound_Effect.wav", SND_ASYNC);
            }
        }  
        disparo(jugador->balas);
    }
}

void HUD() {
    int cont;

    gotoxy(0,30);
    printf("Vida = ");
    for (cont = 0; cont < jugador->health; cont++) printf("%c ", FACE);
    if (jugador->havePistol == false) printf(" Pistol = FALSE");
    if (jugador->havePistol == true) printf(" Pistol = TRUE");
}

void GameOver() {
    system("cls");
    gotoxy(50,10);
    printf("PERDISTEEEEE!!!!!");
    gotoxy(0,20);
    system("pause");
    exit(0);
}

int main () {
    jugador = createPlayer();
    Pixel *posI = createPixel(jugador->info->X, jugador->info->Y, FACE);
    enemies = createList();
    obstaculos = createList();
    //List *torretas = createList();

    leerArchivoEnemies(enemies);
    leerArchivoObstaculos(obstaculos);

    system("cls");
    ocultarCursor();
    mostrarEscenario();
    mostrarObstaculos(obstaculos);
    //PlaySound("sound\\Plants vs Zombies Soundtrack. [Main Menu].wav", NULL, SND_ALIAS | SND_APPLICATION | SND_ASYNC);
    gotoxy(jugador->info->X, jugador->info->Y);
    printf("%c", jugador->info->forma);
    while (true) {
        Sleep(220);
        movimientoEnemigos();
        acciones(jugador);
        if (jugador->gameOver == true) {
            printf("%d ", jugador->gameOver);
            jugador->gameOver = false;
            printf("%d", jugador->gameOver);
            jugador->info = posI;
            if (jugador->health == -1) GameOver();
        }
        HUD();
    }

    return EXIT_SUCCESS;
}



