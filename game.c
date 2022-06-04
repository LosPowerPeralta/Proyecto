#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <stdbool.h>
#include "lib\list.c"
#include "lib\graphics.h"
#define SALTO_MAX 6
#define RIGTH 0x44
#define LEFT 0x41
#define ALTURA 186
#define SPACE 0x20
#define SHIFT 0x10
#define BALA 175        // »
#define FACE 154        // Ü
#define FACE_SHOOT 153  // Ö
#define ENEMIES 159     // ƒ
#define TORRETA_R 195   // ├
#define TORRETA_L 180   // ┤
#define TORRETA_U 193   // ┴
#define TORRETA_D 194   // ┬

//Para compilar usar gcc game.c -lwinmm

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


/************* | hexToDecimal() | **************/
/* El proposito de esta función es convertir   *
 * un codigo hexadecimal en decimal.           *
 * Recibe una cadena con el codigo hexadecimal.*
 * Retorna el codigo en su forma decimal.      */
int hexToDecimal(char *hex) {
    size_t decimal = 0;
    size_t base = 1;
    int i = 0;
    int size;

    size = strlen(hex);
    for(i = size--; i >= 0; i--) {
        if(hex[i] >= '0' && hex[i] <= '9') {
            decimal += (hex[i] - 48) * base;
            base *= 16;
        }
        else if(hex[i] >= 'A' && hex[i] <= 'F') {
            decimal += (hex[i] - 55) * base;
            base *= 16;
        }
        else if(hex[i] >= 'a' && hex[i] <= 'f') {
            decimal += (hex[i] - 87) * base;
            base *= 16;
        }
    }
    return decimal;
}

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

/****************** | createPlayer() | *****************/ 
/* El proposito de esta función es crear una variable  *
 * tipo Player la cual contiene los datos del jugador. *
 * No recibe valores.                                  *
 * Retorna una dirección de momeria de la nueva        *
 * variable.                                           */
Player *createPlayer() {
    Player *newPlayer = (Player *) malloc(sizeof(Player));

    newPlayer->direccion = hexToDecimal("0x44");
    newPlayer->havePistol = false;
    newPlayer->health = 3;
    newPlayer->info = createPixel(1, 22, FACE);
    
    return newPlayer;
}

/****************** | createTorretas() | *****************/
/* El proposito de esta función es crear una variable    *
 * tipo Torreta la cual contiene todos los datos de      *
 * alguna torreta que se encuentre en el mapa.           *
 * Recibe las coordenadas de la torreta, su dirección    *
 * y el tiempo entre cada disparo.                       *
 * Retorna la dirección de memoria de la nueva variable. */
Torretas *createTorretas(int X, int Y, char *direccion, int time) {
    Torretas *newTorretas = (Torretas *) malloc(sizeof(Torretas));
    char forma;
    int direccionD; //dirección en decimal

    direccionD = hexToDecimal(direccion);
    newTorretas->balas = createList();
    newTorretas->CDshoot = createCoolDown(time);

    if (direccion == 68) forma = TORRETA_R;
    if (direccion == 65) forma = TORRETA_L;
    if (direccion == 83) forma = TORRETA_D;
    if (direccion == 87) forma = TORRETA_U;

    newTorretas->direccion = direccionD;
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

    newEnemies->direccion = hexToDecimal(LEFT);
    newEnemies->info = createPixel(X, Y, ENEMIES);
    newEnemies->limiteI = limiteI;
    newEnemies->limiteS = limiteS;

    return newEnemies;
}

/**************** | createBullet() | ***************/
/* El proposito de esta función es crear una bala  *
 * y añadirla en una lista.                        *
 * Recibe la posición inicial de la bala y una     *
 * lista en donde estará guardada.                 *
 * No retorna valores.                             */
void createBullet(int X, int Y, List *balas) {
    Pixel *newBala = createPixel(X, Y, BALA);

    pushBack(balas, newBala);
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
void salto(Pixel *pos, int tipo) {
    int alturaMax = pos->Y - SALTO_MAX;

    //Subida
    while (pos->Y > alturaMax) {
        Sleep(FPS);
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
        if (GetAsyncKeyState(RIGTH)) 
            if (pos->X != 117) pos->X++;
        if (GetAsyncKeyState(LEFT)) 
            if(pos->X != 1) pos->X--;
    }
}

/************* | movimientoVertical() | *************/
/* El proposito de esta función es llevar a cabo el *
 * tipo de movimiento vertical decidido por el      *
 * usuario.                                         *
 * Recibe la información del jugador.               *
 * No retorna valores.                              */
void movimientoVertical(Player *jugador) {
    Pixel *pos = jugador->info;

    if (kbhit()) {
        if (GetAsyncKeyState(RIGTH) && GetAsyncKeyState(SPACE)) salto(pos, 1);
        else if (GetAsyncKeyState(LEFT) && GetAsyncKeyState(SPACE)) salto(pos, 2);
        else if (GetAsyncKeyState(SPACE)) salto(pos, 0);
    }
}

int main () {
    Player *jugador = createPlayer();
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
        movimientoLateral(jugador);
        movimientoVertical(jugador);
        gotoxy(jugador->info->X, jugador->info->Y);
        printf("%c", jugador->info->forma);
    }

    return EXIT_SUCCESS;
}

