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
//#define SHIFT 0x10
#define ENTER 0X0D
#define SALTO_MAX 6

//Para compilar usar gcc game2.c -o game2 -lwinmm

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
    Salto *jump;
} Player;

typedef struct {
    Pixel *info;
    int tipo;
} Object;

typedef struct {
    int intervalo;
    int cont;
} Coldown;

typedef struct {
    Pixel *info;
    List *balas;
    int direccion;
    Coldown *tiempo;

} Turrets;

typedef struct {
    Pixel *info;
    int limiteS;
    int limiteI;
    int direccion;
} Enemies;

Pixel *createPixel(int X, int Y, char forma);
Salto *createSalto();
Player *createPlayer();
Enemies *createEnemies(int X, int Y, int limiteS, int limiteI, int direccion);
Turrets *createTurrets(int X, int Y, int tiempo, int direccion);
Coldown *createColdown(int intervalo);
void *createBullet(int X, int Y, int direccion, List *balas);
void leerArchivoObstaculos();
void leerArchivoEnemies();
void leerArchivoTurrets();
void leerArchivoObjetos();
void mostrarObstaculos();
void mostrarTurrets();
void mostrarObjetos();
void movimientoEnemigos();
void accionTurrets();
void salto();
void movimientoLateral();
void movimientoVertical();
void disparo(List *balas);
void acciones();
void menu();
void HUD();
void level2();
void level1();
void GameOver();
void prepareVariables();
void cleanVariables();
void instrucciones();
void muertePlayer();
bool hitBoxVPlayer();
bool hitBoxHPlayer();
bool hitBoxHbullet(Bullet *bala);
bool hitBoxVbullet(Bullet *bala);
int main();

List *enemies; 
List *obstaculos;
List *torretas;
List *objetos;
Player *jugador;

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
    if (direccion == RIGHT) X++;
    if (direccion == LEFT) X--;
    if (direccion == UP) Y--;
    if (direccion == DOWN) Y++;
    newBullet->info = createPixel(X, Y, BALA);

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
Player *createPlayer(int X, int Y) {
    Player *newPlayer = (Player *) malloc(sizeof(Player));

    newPlayer->direccionH = RIGHT;
    newPlayer->direccionV = DOWN;
    newPlayer->havePistol = false;
    //newPlayer->gameOver = 0;
    newPlayer->health = 3;
    newPlayer->info = createPixel(X, Y, FACE);
    newPlayer->jump = createSalto();
    newPlayer->balas = createList();

    return newPlayer;
}

/****************** | createEnemies() | ******************/
/* El proposito de esta función es crear una variable    *
 * tipo Enemies la cual almacenara todos los datos       *
 * de un enemigo que se encuentre en el mapa.            *
 * Recibe la posición inicial del enemigo y los          *
 * limites de movimiento.                                *
 * Retorna la dirección de momeria de la nueva variable. */
Enemies *createEnemies(int X, int Y, int limiteS, int limiteI, int direccion) {
    Enemies *newEnemies = (Enemies *) malloc(sizeof(Enemies));

    if (direccion == RIGHT) newEnemies->direccion = RIGHT;
    if (direccion == LEFT) newEnemies->direccion = LEFT;
    newEnemies->info = createPixel(X, Y, ENEMIES);
    newEnemies->limiteI = limiteI;
    newEnemies->limiteS = limiteS;

    return newEnemies;
}

Object *createObject(int X, int Y, int tipo) {
    Object *newObject = (Object *) malloc(sizeof(Object));

    newObject->tipo = tipo;
    if (tipo == 1)newObject->info = createPixel(X, Y, PISTOL);
    if (tipo == 2)newObject->info = createPixel(X, Y, POCION);
    if (tipo == 3)newObject->info = createPixel(X, Y, BANDERA);

    return newObject;
}

Turrets *createTurrets(int X, int Y, int tiempo, int direccion)
{
    Turrets *newTurrets = (Turrets*) malloc(sizeof(Turrets));
    newTurrets->tiempo = createColdown(tiempo);
    newTurrets->balas = createList();
    newTurrets->direccion = direccion;
    if(direccion == LEFT)newTurrets->info = createPixel(X, Y, TORRETA_L);
    if(direccion == RIGHT)newTurrets->info = createPixel(X, Y, TORRETA_R);
    if(direccion == UP)newTurrets->info = createPixel(X, Y, TORRETA_U);
    if(direccion == DOWN)newTurrets->info = createPixel(X, Y, TORRETA_D);

    return newTurrets;
}

Coldown *createColdown(int intervalo)
{
    Coldown *newColdown = (Coldown*) malloc(sizeof(Coldown));

    newColdown->intervalo = intervalo;
    newColdown->cont = 0;

    return newColdown;
}

void leerArchivoObstaculos(char *ubicacion) {
    FILE *archivo = fopen(ubicacion, "r");
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
    fclose(archivo);
}

void leerArchivoEnemies(char *ubicacion) {
    FILE *archivo = fopen(ubicacion, "r");
    char linea[1024];
    int X;
    int Y;
    int limiteS;
    int limiteI;
    char direccion[5];
    int direccionAux;
    Enemies *enemy;

    fgets(linea, 1023, archivo);
    while (fgets(linea, 1023, archivo) != NULL) {
        X = toNumber((char *)get_csv_field(linea, 0));
        Y = toNumber((char *)get_csv_field(linea, 1));
        strcpy(direccion, get_csv_field(linea, 2));
        direccionAux = hexToDecimal(direccion);
        limiteI = toNumber((char *)get_csv_field(linea, 3));
        limiteS = toNumber((char *)get_csv_field(linea, 4));
        enemy = createEnemies(X, Y, limiteS, limiteI, direccionAux);
        pushBack(enemies, enemy);
    }
    fclose(archivo);
}

void leerArchivoObjetos(char *ubicacion) {
    FILE *archivo = fopen(ubicacion, "r");
    Object *objeto;
    char linea[1024];
    int X;
    int Y;
    int tipo;

    fgets(linea, 1023, archivo);
    while (fgets(linea, 1023, archivo) != NULL) {
        X = toNumber((char *)get_csv_field(linea, 0));
        Y = toNumber((char *)get_csv_field(linea, 1));
        tipo = toNumber((char *)get_csv_field(linea, 2));
        objeto = createObject(X, Y, tipo);
        pushBack(objetos, objeto);
    }
    fclose(archivo);

}

void leerArchivoTurrets(char *ubicacion) {
    FILE *archivo = fopen(ubicacion, "r");
    char linea[1024];
    int X;
    int Y;
    int direccion;
    int tiempo;

    Turrets *turret;
    fgets(linea, 1023, archivo);
    while(fgets(linea, 1023, archivo) != NULL)
    {
        X = toNumber((char *)get_csv_field(linea, 0));
        Y = toNumber((char *)get_csv_field(linea, 1));
        direccion = toNumber((char *)get_csv_field(linea, 2));
        tiempo = toNumber((char *)get_csv_field(linea, 3));
        turret = createTurrets(X, Y, tiempo, direccion);
        pushBack(torretas, turret);
    }
    fclose(archivo);
}

bool hitBoxVPlayer() {
    Pixel *pos = jugador->info;
    Obstaculo *obstacle = firstList(obstaculos);
    Enemies *enemy = firstList(enemies);
    Pixel *posE;
    Limites *limits;

    while(obstacle != NULL) {
        limits = obstacle->limits;
        if (jugador->direccionV == UP) {
            if (pos->Y == limits->limiteY_I && (pos->X >= limits->limiteX_I - 1 && pos->X <= limits->limiteX_S)) {
                if (obstacle->tipo == 2) {
                    muertePlayer();
                }
                return true;
            }
        }
        if (jugador->direccionV == DOWN) {
            if (pos->Y + 1 == limits->limiteY_S && (pos->X >= limits->limiteX_I - 1 && pos->X <= limits->limiteX_S)) {
                if (obstacle->tipo == 2) {
                    muertePlayer();
                }
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
    Object *objeto = firstList(objetos);
    Pixel *posO;
    Limites *limits;

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
    
    while (objeto != NULL) {
        posO = objeto->info;
        if (pos->X == posO->X && pos->Y == posO->Y) {
            if (objeto->tipo == 1) jugador->havePistol = true;
            if (objeto->tipo == 2) jugador->health++;
            if (objeto->tipo == 3) {
                cleanVariables();
                level2();
            }
            popCurrent(objetos);
        }
        objeto = nextList(objetos);
    }

    return false;
}

bool hitBoxVbullet(Bullet *bala) {
    Obstaculo *obstacle = firstList(obstaculos);
    Limites *limits;
    Pixel *posB = bala->info;
    Enemies *enemy = firstList(enemies);
    Pixel *posE;
    Pixel *posP = jugador->info;

    while (obstacle != NULL) {
        limits = obstacle->limits;
        if (bala->direccion == UP) {
            if (posB->Y - 1  == limits->limiteY_I && (posB->X >= limits->limiteX_I - 1 && posB->X <= limits->limiteX_S + 1)) return true;
        }
        if (bala->direccion == DOWN) {
            if (posB->Y + 1 == limits->limiteY_S && (posB->X >= limits->limiteX_I - 1 && posB->X <= limits->limiteX_S + 1)) return true;
        }
        obstacle = nextList(obstaculos);
    }

    /*while (enemy != NULL) {
        posE = enemy->info;
        if (posE->X == posB->X && posE->Y && posB->Y) popCurrent(enemies);
        enemy = nextList(enemies);
    }

    if (posB->X == posP->X && posB->Y == posP->Y) {
        muertePlayer();
    }*/
    return false;
}

bool hitBoxHbullet(Bullet *bala) {
    Obstaculo *obstacle = firstList(obstaculos);
    Limites *limits;
    Pixel *posB = bala->info;
    Enemies *enemy = firstList(enemies);
    Pixel *posE;
    Pixel *posP = jugador->info;

    while (obstacle != NULL) {
        limits = obstacle->limits;
        if (bala->direccion == LEFT) {
            if (posB->X - 1 == limits->limiteX_S && (posB->Y <= limits->limiteY_I && posB->Y >= limits->limiteY_S)) return true;
        }
        if (bala->direccion == RIGHT) {
            if (posB->X + 1 == limits->limiteX_I - 1 && (posB->Y <= limits->limiteY_I && posB->Y >= limits->limiteY_S)) return true;
        }
        obstacle = nextList(obstaculos);
    }

    while (enemy != NULL) {
        posE = enemy->info;
        if (posE->X == posB->X && posE->Y && posB->Y) popCurrent(enemies);
        enemy = nextList(enemies);
    }

    if (posB->X == posP->X && posB->Y == posP->Y) {
        muertePlayer();
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

void mostrarTurrets() {
    Turrets *turret = firstList(torretas);
    Pixel *pos;
    
    while (turret != NULL) {
        pos = turret->info;
        gotoxy(pos->X, pos->Y);
        printf("%c", turret->info->forma);
        turret = nextList(torretas);
    }
}

void mostrarObjetos() {
    Object *objeto = firstList(objetos);
    Pixel *pos;

    while (objeto != NULL) {
        pos = objeto->info;
        gotoxy(pos->X, pos->Y);
        printf("%c", pos->forma);
        objeto = nextList(objetos);
    }
}

void movimientoEnemigos() {
    Enemies *enemy = firstList(enemies);
    Pixel *posP = jugador->info;
    Pixel *posE;

    while(enemy != NULL) {
        posE = enemy->info;

        gotoxy(posE->X, posE->Y);
        printf(" ");
        if (enemy->direccion == RIGHT) {
            if (posE->X != enemy->limiteS) posE->X++;
            gotoxy(posE->X, posE->Y);
            printf("%c", posE->forma);
            if (posE->X == enemy->limiteS) {
                gotoxy(posE->X, posE->Y);
                printf(" ");
                enemy->direccion = LEFT;
            }
        }
        if (enemy->direccion == LEFT) {
            if (posE->X != enemy->limiteI) posE->X--;
            gotoxy(posE->X, posE->Y);
            printf("%c", posE->forma);
            if (posE->X == enemy->limiteI) {
                enemy->direccion = RIGHT;
                gotoxy(posE->X, posE->Y);
                printf(" ");
            }
        }

        if (posE->X == posP->X && posE->Y == posP->Y) {
            muertePlayer();
        }
        enemy = nextList(enemies);
    }
}

void accionTurrets() {
    Turrets *turret = firstList(torretas);
    Coldown *time ;
    Pixel *pos;

    while(turret != NULL)
    {
        time = turret->tiempo;
        pos = turret->info;

        if(time->cont == 1)
        {
            createBullet(pos->X, pos->Y, turret->direccion, turret->balas);
        }

        time->cont++; 
        if(time->cont == time->intervalo)
            time->cont = 0;

        disparo(turret->balas);
        turret = nextList(torretas);
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
        if (bala->direccion == RIGHT) {
            if (bala->info->X != 117 && !hitBoxHbullet(bala)) bala->info->X++;
            gotoxy(bala->info->X, bala->info->Y);
            printf("%c", bala->info->forma);
            if (bala->info->X == 117 || hitBoxHbullet(bala)) {
                gotoxy(bala->info->X, bala->info->Y);
                printf(" ");
                popCurrent(balas);
            }
        }
        if (bala->direccion == LEFT) {
            if (bala->info->X != 1 && !hitBoxHbullet(bala)) bala->info->X--;
            gotoxy(bala->info->X, bala->info->Y);
            printf("%c", bala->info->forma);
            if (bala->info->X == 1 || hitBoxHbullet(bala)) {
                gotoxy(bala->info->X, bala->info->Y);
                printf(" ");
                popCurrent(balas);
            }
        }  
        if (bala->direccion == UP) {
            if (bala->info->Y != 1 && !hitBoxVbullet(bala)) {
                bala->info->Y--;
            }
            gotoxy(bala->info->X, bala->info->Y);
            printf("%c", bala->info->forma);
            if (bala->info->Y == 1 || hitBoxVbullet(bala)) {
                gotoxy(bala->info->X, bala->info->Y);
                printf(" ");
                popCurrent(balas);
            }
        }
        if (bala->direccion == DOWN) {
            if (bala->info->Y != 27 && !hitBoxVbullet(bala)) bala->info->Y++;
            gotoxy(bala->info->X, bala->info->Y);
            printf("%c", bala->info->forma);
            
            if (bala->info->Y == 27 || hitBoxVbullet(bala)) {
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
    hitBoxHPlayer();
    //if (jugador->gameOver != 1) {
        if (jugador->jump->existSalto != true) movimientoLateral();
    
        movimientoVertical();
        gotoxy(jugador->info->X, jugador->info->Y);
        printf("%c", jugador->info->forma);
        if (kbhit()) {
            if (GetAsyncKeyState(ESC)) GameOver(1);
            if (GetAsyncKeyState(ENTER) && jugador->havePistol == true) {
                createBullet(pos->X, pos->Y, jugador->direccionH, jugador->balas);
                jugador->havePistol = false;
                //sndPlaySound("sound\\SPOILER_Sr_Pelo_Boom_Sound_Effect.wav", SND_ASYNC);
            }
        }  
        disparo(jugador->balas);
    //}
}

void HUD() {
    int cont;

    gotoxy(0,29);
    printf("                                                        ");
    gotoxy(0,29);
    printf("Vida = ");
    for (cont = 0; cont < jugador->health; cont++) {
        printf("%c ", (char*) 3);
    }
    if (jugador->havePistol == false) printf(" Pistol = FALSE");
    if (jugador->havePistol == true) printf(" Pistol = TRUE");
    gotoxy(85,29);
    printf("Pulse Escape para volver al menu.");
}

void GameOver(int flag) {
    cleanList(torretas);
    cleanList(enemies);
    cleanList(obstaculos);
    cleanList(objetos);
    if (flag == 1) {
        system("cls");
        gotoxy(50,10);
        //sndPlaySound("sound\\PERDISTE.wav", SND_ASYNC);
        printf("PERDISTEEEEE!!!!!");
        gotoxy(0,30);
        printf("Pulse Escape para volver al menu.");
        while (true) {
            if (GetAsyncKeyState(ESC)) main();
        }
    }
}

void muertePlayer() {
    //sndPlaySound("sound\\SOUND OFF ROBLOX SONIDO OFF MUERTE DE ROBLOX.wav", SND_SYNC);
    //jugador->gameOver = 0;
    jugador->info->X = 2;
    jugador->info->Y = 18;
    jugador->health--;
    if (jugador->health == -1) GameOver(1);
}

void cleanVariables() {
    cleanList(torretas);
    cleanList(enemies);
    cleanList(obstaculos);
    cleanList(objetos);
}

void prepareVariables() {
    enemies = createList();
    obstaculos = createList();
    torretas = createList();
    objetos = createList();
}

void level2() {
    prepareVariables();
    jugador = createPlayer(1, 27);

    leerArchivoEnemies("csv\\Level2\\enemies.csv");
    printf("xd");
    leerArchivoObstaculos("csv\\Level2\\obstaculos.csv");
    leerArchivoTurrets("csv\\Level2\\turrets.csv");
    leerArchivoObjetos("csv\\Level2\\objetos.csv");

    system("cls");
    mostrarCursor(false);
    mostrarEscenario(117,28);
    mostrarObstaculos();
    mostrarTurrets();
    mostrarObjetos();
    //PlaySound("sound\\Plants vs Zombies Soundtrack. [Main Menu].wav", NULL, SND_ASYNC | SND_NOSTOP);
    gotoxy(jugador->info->X, jugador->info->Y);
    printf("%c", jugador->info->forma);

    while (true) {
        Sleep(FPS);
        movimientoEnemigos();
        acciones();
        accionTurrets(torretas);
        HUD();
    }
}

void level1() {
    prepareVariables();
    jugador = createPlayer(2, 18);

    leerArchivoEnemies("csv\\Level1\\enemies.csv");
    leerArchivoObstaculos("csv\\Level1\\obstaculos.csv");
    leerArchivoTurrets("csv\\Level1\\turrets.csv");
    leerArchivoObjetos("csv\\Level1\\objetos.csv");

    system("cls");
    mostrarCursor(false);
    mostrarEscenario(117,28);
    mostrarObstaculos();
    mostrarTurrets();
    mostrarObjetos();
    //PlaySound("sound\\Plants vs Zombies Soundtrack. [Main Menu].wav", NULL, SND_ASYNC | SND_NOSTOP);
    gotoxy(jugador->info->X, jugador->info->Y);
    printf("%c", jugador->info->forma);

    while (true) {
        Sleep(FPS);
        movimientoEnemigos();
        acciones();
        accionTurrets(torretas);
        HUD();
    }
}

void instrucciones() {
    system("cls");
    mostrarEscenario(51,27);
    gotoxy(19,4);
    printf("INSTRUCCIONES");
    gotoxy(2,6);
    printf("Teclas:                Enemigos:");
    gotoxy(4,7);
    printf("A = Izquierda         %c Funciones enemigas! ", ENEMIES);
    gotoxy(4,8);
    printf("D = Derecha           %c y Torretas que disparan", TORRETA_L);
    gotoxy(4,9);
    printf("ENTER = Disparar        %c Copyright!!", BALA);
    gotoxy(4,10);
    printf("ESPACIO = Saltar        Aunque tu tambien");
    gotoxy(28,11);
    printf("podras ;D");
    gotoxy(2,13);
    printf("Obstaculos:            Para pasar de nivel:");
    gotoxy(4,14);
    printf("%c%c%c = Superficie       Solo debes tomar esta", BASE,BASE,BASE);
    gotoxy(4,15);
    printf("%c = Pinchos!!          bandera = %c.", PINCHOS, BANDERA);
    gotoxy(2,17);
    printf("Habilidades: ");
    gotoxy(4,18);
    printf("%c = Pistola", PISTOL);
    gotoxy(4,19);
    printf("Solo podras disparar una bala cada");
    gotoxy(4,20);
    printf("vez que la encuentres.");
    gotoxy(4,22);
    printf("%c = Pocion de vida", POCION);
    gotoxy(4,23);
    printf("Aumenta en 1 tu vida asi que");
    gotoxy(4,24);
    printf("aprovechala :3.");
    gotoxy(1,26);
    printf("Pulse Escape para volver al menu.");
    while (true) {
        if (GetAsyncKeyState(ESC)) menu();
    }
}

void menu() {
    Pixel pos;
    int time = 0;

    pos.X = 8;
    pos.Y = 11;
    system("cls");
    fflush(stdin);
    mostrarCursor(true);
    mostrarEscenario(50,27);
    gotoxy(19,6);
    printf("Nachito Bross");
    gotoxy(8,11);
    printf("Nueva Partida");
    gotoxy(30,11);
    printf("Continuar Partida");
    gotoxy(8,16);
    printf("Opciones");
    gotoxy(30,16);
    printf("Instrucciones");
    gotoxy(22, 21);
    printf("Salir");
    GetAsyncKeyState(ENTER);

    gotoxy(pos.X, pos.Y);
    while (true) {
        Sleep(FPS);
        
        if (time == 10) {
            if (GetAsyncKeyState(RIGHT)) {
                if (pos.X == 8 && pos.Y == 11) pos.X = 30;
                if (pos.X == 8 && pos.Y == 16) pos.X = 30;
            }
            if (GetAsyncKeyState(LEFT)) {
                if (pos.X == 30 && pos.Y == 11) pos.X = 8;
                if (pos.X == 30 && pos.Y == 16) pos.X = 8;
            }
            if (GetAsyncKeyState(DOWN)) {
                if ((pos.X == 8 && pos.Y == 16) || (pos.X == 30 && pos.Y == 16)) {
                    pos.X = 22;
                    pos.Y = 21;
                }
                if (pos.X == 8 && pos.Y == 11) pos.Y = 16;
                if (pos.X == 30 && pos.Y == 11) pos.Y = 16;
            }
            if (GetAsyncKeyState(UP)) {
                if (pos.X == 8 && pos.Y == 16) pos.Y = 11;
                if (pos.X == 30 && pos.Y == 16) pos.Y = 11;
                if (pos.X == 22 && pos.Y == 21) {
                    pos.X = 8;
                    pos.Y = 16;
                }
            }
            if (GetAsyncKeyState(ENTER)) {
                if (pos.X == 8 && pos.Y == 11) level1();
                if (pos.X == 30 && pos.Y == 16) instrucciones();
                if (pos.X == 22 && pos.Y == 21) exit(1);
            }
            time = 0;
            gotoxy(pos.X, pos.Y);
        }
        time++;
    }
}

int main() {

    menu();

    return EXIT_SUCCESS;
}



