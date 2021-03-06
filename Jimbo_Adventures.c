#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <stdbool.h>
#include "lib\list.c"
#include "lib\hashmap.c"
#include "lib\graphics.h"
#include "lib\operadores.h"
#define UP 0x57
#define DOWN 0x53
#define RIGHT 0x44
#define LEFT 0x41
#define ESC 0x1B
#define SPACE 0x20
#define ENTER 0X0D
#define SALTO_MAX 6

//Para compilar usar gcc Jimbo_Adventures.c -o Jimbo_Adventures -lwinmm

typedef struct {
    char name[15];
    char password[11];
    char level[7];
    bool havePistol;
    int X;
    int Y;
    int health;
    int music;
} User;

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
    char level[7];
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
void level3(int firstTime);
void level2(int firstTime);
void level1(int firstTime);
void GameOver();
void prepareVariables();
void cleanVariables();
void instrucciones();
void iniciarSesion();
void muertePlayer();
void registro();
bool hitBoxVPlayer();
bool hitBoxHPlayer();
bool hitBoxHbullet(Bullet *bala);
bool hitBoxVbullet(Bullet *bala);
int main();

List *enemies; 
List *obstaculos;
List *torretas;
List *objetos;
User *usuario;
Player *jugador;
HashMap *usuarios;

/***************** | createPixel() | ****************/
/* El proposito de esta funci??n es crear una        *
 * variable tipo Pixel para cada objeto utilizado  *
 * dentro de la pantalla.                           *
 * Recibe una coordenada y la forma del pixel.      *
 * Retorna una direcci??n de memoria del nuevo pixel */
Pixel *createPixel(int X, int Y, char forma) {
    Pixel *newPixel = (Pixel *) malloc(sizeof(Pixel));

    newPixel->forma = forma;
    newPixel->X = X;
    newPixel->Y = Y;

    return newPixel;
}


/***************** | createBullet() | *******************/
/* El proposito de esta funci??n es crear una variable   *
 * tipo Bullet para cada bala que sea disparada por las *
 * torretas o por el usuario.                           *
 * Recibe la posicion inicial de la bala, la direccion  *
 * y la lista en donde debe estar guardada.             *
 * No retorna valores.                                  */
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

/************** | createSalto() | *****************/
/* El proposito de esta funci??n es crear una      * 
 * variable tipo Salto la cual almacenar??         *
 * todo tipo de variables que afectan             *
 * dentro de la accion.                           *
 * No recibe valores.                             *
 * Retorna la direccion de memoria de la          *
 * nueva variable.                                */
Salto *createSalto() {
    Salto *newSalto = (Salto *) malloc(sizeof(Salto));

    newSalto->altura_max = 0;
    newSalto->cima = false;
    newSalto->existSalto = false;
    newSalto->tipo_salto = 0;

    return newSalto;
}

/****************** | createPlayer() | *****************/ 
/* El proposito de esta funci??n es crear una variable  *
 * tipo Player la cual contiene los datos del jugador. *
 * No recibe valores.                                  *
 * Retorna una direcci??n de momeria de la nueva        *
 * variable.                                           */
Player *createPlayer(int X, int Y, int health, char *level, int havePistol) {
    Player *newPlayer = (Player *) malloc(sizeof(Player));

    newPlayer->direccionH = RIGHT;
    newPlayer->direccionV = DOWN;
    newPlayer->havePistol = havePistol;
    strcpy(newPlayer->level, level);
    newPlayer->health = health;
    newPlayer->info = createPixel(X, Y, FACE);
    newPlayer->jump = createSalto();
    newPlayer->balas = createList();

    return newPlayer;
}

/****************** | createEnemies() | ******************/
/* El proposito de esta funci??n es crear una variable    *
 * tipo Enemies la cual almacenara todos los datos       *
 * de un enemigo que se encuentre en el mapa.            *
 * Recibe la posici??n inicial del enemigo y los          *
 * limites de movimiento.                                *
 * Retorna la direcci??n de momeria de la nueva variable. */
Enemies *createEnemies(int X, int Y, int limiteS, int limiteI, int direccion) {
    Enemies *newEnemies = (Enemies *) malloc(sizeof(Enemies));

    if (direccion == RIGHT) newEnemies->direccion = RIGHT;
    if (direccion == LEFT) newEnemies->direccion = LEFT;
    newEnemies->info = createPixel(X, Y, ENEMIES);
    newEnemies->limiteI = limiteI;
    newEnemies->limiteS = limiteS;

    return newEnemies;
}

/**************** | createObject() | ******************/
/* El proposito de esta funcion es crear una variable *
 * tipo Object el cual almacenar?? la posicion del     *
 * objeto y el tipo del mismo.                        *
 * tipo 1 = pistola                                   *
 * tipo 2 = pocion de vida                            *
 * tipo 3 = bandera                                   *
 * Recibe la posicion del objeto y el tipo.           *
 * Retorna el objeto.                                 */
Object *createObject(int X, int Y, int tipo) {
    Object *newObject = (Object *) malloc(sizeof(Object));

    newObject->tipo = tipo;
    if (tipo == 1)newObject->info = createPixel(X, Y, PISTOL);
    if (tipo == 2)newObject->info = createPixel(X, Y, POCION);
    if (tipo == 3)newObject->info = createPixel(X, Y, BANDERA);

    return newObject;
}

/************************** | createUser() | *****************************/
/* El proposito de esta funcion es crear una variable de tipo User el    *
 * cual almacenar?? todos los datos del usuario, como por ejemplo         *
 * la posicion de la ??ltima partida y la cantidad de vidas.              *
 * Recibe la posicion inicial del usuario, la contrase??a, su nombre.     *
 * la vida, el nivel y una bandera que indica si tiene una pistola o no. *
 * Retorna la variable.                                                  */
User *createUser(int X, int Y, char *password, char *name, char *level, int health, bool flag) {
    User *newUser = (User *) malloc(sizeof(User));

    newUser->havePistol = flag;
    strcpy(newUser->level, level);
    strcpy(newUser->name, name);
    strcpy(newUser->password, password); 
    newUser->health = health;
    newUser->X = X;
    newUser->Y = Y;
    newUser->music = 1;

    return newUser;
}

/******************** | createTurrets() | ***********************/
/* El proposito de esta funci??n es crear una variable tipo      *
 * Turrets la cual almacenar?? la posicion inicial, el tiempo    *
 * entre cada disparo y su direccion.                           *
 * Recibe los parametros anteriormente indicados.               *
 * Retorna la nueva variable.                                   */
Turrets *createTurrets(int X, int Y, int tiempo, int direccion) {
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

/************ | createColdown() | *******************/
/* El proposito de esta funcion es crear una        *
 * variable tipo Coldown la cual almacenar?? el      *
 * el tiempo entre cada accion.                     *
 * Recibe el intervalo de tiempo.                   *
 * No retorna valores.                              */
Coldown *createColdown(int intervalo)
{
    Coldown *newColdown = (Coldown*) malloc(sizeof(Coldown));

    newColdown->intervalo = intervalo;
    newColdown->cont = 0;

    return newColdown;
}

/****************** | leerArchivoUsuarios() | *****************/
/* El proposito de esta funci??n es leer un archivo el cual    *
 * almacena la informaci??n de todos los usuarios registrados. *
 * Recibe un hashMap en el cual se almacenaran los usuarios   *
 * para una mejor busqueda.                                   *
 * No retorna valores.                                        */
void leerArchivoUsuarios(HashMap *usuarios) {
    FILE *archivo = fopen("csv\\Usuarios.csv", "r");
    User *usuario;
    char linea[1024];
    char name[15];
    char password[11];
    char level[7];
    char flag[6];
    int X;
    int Y;
    int health;

    fgets(linea, 1023, archivo);
    while (fgets(linea, 1023, archivo) != NULL) {
        strcpy(name, get_csv_field(linea, 0));
        strcpy(password, get_csv_field(linea, 1));
        strcpy(level, get_csv_field(linea, 2));
        X = toNumber((char *)get_csv_field(linea, 3));
        Y = toNumber((char *)get_csv_field(linea, 4));
        health = toNumber((char *)get_csv_field(linea, 5));
        strcpy(flag, get_csv_field(linea, 6));
        if (strcmp(flag, "false") == 0) usuario = createUser(X, Y, password, name, level, health, false);
        if (strcmp(flag, "true") == 0) usuario = createUser(X, Y, password, name, level, health, true);
        insertMap(usuarios, usuario->name, usuario);
    }
}

/******************** | leerArchivoObstaculos() | ********************/ 
/* El proposito de esta funci??n es leer un archivo en donde est??n    *
 * almacenados todos los obstaculos que se encuentran dentro del     *
 * juego.                                                            *
 * Recibe la ubicaci??n del archivo.                                  *
 * No retorna valores.                                               */ 
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

/*************** | leerArchivoEnemies() | ***************/
/* El proposito de esta funci??n es leer un archivo que  *
 * contiene a todos los enemigos de cada nivel.         *
 * Recibe la ubicacion del archivo.                     *
 * No retorna valores.                                  */
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

/**************** | leerArchivoObjetos() | ********************/
/* El proposito de esta funci??n es leer un archivo en donde   *
 * est??n contenidos todos los objetos de cada nivel.          *
 * Recibe la ubicacion del archivo.                           *
 * No retorna valores.                                        */
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

/********************** | leerArchivoTurrets() | *********************/
/* El proposito de esta funci??n es leer todos las torretas que se    *
 * encuentran en cada nivel.                                         *
 * Recibe la ubicacion del archivo.                                  *
 * No retorna valores.                                               */
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

/*************** | winGame() | **************/
/* El proposito de esta funci??n es mostrar  *
 * La pantalla de victoria al jugador.      *
 * No recibe valores y tampoco retorna.     */
void winGame() {
    system("cls");  
    mostrarEscenario(50,10);
    sndPlaySound("sound\\Kirby Victory Dance.wav", SND_ASYNC);
    gotoxy(20,5);
    printf("Ganaste!!!!");
    gotoxy(1,9);
    printf("Pulse Escape para volver al menu.");
    while (true) {
        if (GetAsyncKeyState(ESC)) menu();
    }

}

/**************** | hitBoxVPlayer() | **************/
/* El proposito de esta funcion es verificar si el *
 * jugador est?? colisionando con un obstaculo,     *
 * con alg??n pincho o enemigo.                     *
 * No recibe valores.                              *
 * Retorna false al no colisionar o rue en caso    *
 * contrario.                                      */
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

/**************** | hitBoxHPlayer() | ******************/
/* Tiene el mismo proposito que hitBoxVPlayer().      */
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
                if (strcmp(jugador->level, "level1") == 0) level2(1);
                if (strcmp(jugador->level, "level2") == 0) level3(1);
                if (strcmp(jugador->level, "level3") == 0) winGame();
            }
            popCurrent(objetos);
        }
        objeto = nextList(objetos);
    }

    return false;
}

/**************** | hitBoxVbullet() | ******************/
/* El proposito de esta funcion es verificar si alguna * 
 * bala est?? colisionando con alg??n objeto, jugador o  *
 * enemigo.                                            *
 * Recibe la bala que est?? en movimiento.              *
 * Retorna false en caso de no colisionar o true       *
 * en caso contrario.                                  */
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

    while (enemy != NULL) {
        posE = enemy->info;
        if (posE->X == posB->X && posE->Y == posB->Y) popCurrent(enemies);
        enemy = nextList(enemies);
    }

    if (posB->X == posP->X && posB->Y == posP->Y) {
        muertePlayer();
    }
    return false;
}

/****************** | hitBoxHbullet() | ************************/
/* El proposito de esta funcion es el mismo que hitBoxVbullet. */
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
        if (posE->X == posB->X && posE->Y == posB->Y) popCurrent(enemies);
        enemy = nextList(enemies);
    }

    if (posB->X == posP->X && posB->Y == posP->Y) {
        muertePlayer();
    }

    return false;
}

/*************** | mostrarObstaculos() | ******************/
/* El proposito de esta funcion es mostrar los obstaculos *
 * por pantalla.                                          *
 * No recibe valores y tampoco retorna.                   */
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

/*************** | movimientoEnemigos() | **************/
/* El proposito de esta funcion es realizar el         *
 * movimeinto del enemigo y corroborar si el jugador   *
 * est?? chocando con el.                               *  
 * No recibe valores y tampoco retorna.                */
void movimientoEnemigos() {
    Enemies *enemy = firstList(enemies);
    Pixel *posP = jugador->info;
    Pixel *posE;

    while(enemy != NULL) {
        posE = enemy->info;

        gotoxy(posE->X, posE->Y);
        printf(" ");
        if (enemy->direccion == RIGHT) {
            if (posE->X <= enemy->limiteS) posE->X++;
            gotoxy(posE->X, posE->Y);
            printf("%c", posE->forma);
            if (posE->X == enemy->limiteS) {
                gotoxy(posE->X, posE->Y);
                printf(" ");
                enemy->direccion = LEFT;
            }
        }
        if (enemy->direccion == LEFT) {
            if (posE->X >= enemy->limiteI) posE->X--;
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

/************** | accionTurrets() | *****************/
/* El proposito de esta funcion que cada torreta    *
 * que se encuentre dentro del mapa realice un      *
 * disparo dependiendo de su coldown.               *
 * No recibe valores y tampoco retorna.             */
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
/* El proposito de esta funci??n es       *
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
/* El proposito de esta funci??n es llevar a cabo el *
 * movimiento de izquierda a derecha del jugador.   *
 * Recibe la informaci??n del jugador.               *
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
/* El proposito de esta funci??n es llevar a cabo el *
 * tipo de movimiento vertical decidido por el      *
 * usuario.                                         *
 * Recibe la informaci??n del jugador.               *
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

/***************** | disparo() | ****************/
/* El proposito de esta funcion es realizar el  *
 * movimiento de la bala dentro del mapa        *
 * verificando si choca contra alg??n objeto     *
 * o jugador.                                   *
 * Recibe la lista de balas de cada entidad.    *
 * No retorna valores.                          */
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
            if (jugador->info->X == bala->info->X && jugador->info->Y == bala->info->Y) muertePlayer();
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
            if (jugador->info->X == bala->info->X && jugador->info->Y == bala->info->Y) muertePlayer();
            if (bala->info->Y == 27 || hitBoxVbullet(bala)) {
                gotoxy(bala->info->X, bala->info->Y);
                printf(" ");
                popCurrent(balas);
            }
        }
        bala = (Bullet *)nextList(balas);
    }

}

/************ | saveData() | ***********/
/* El proposito de esta funci??n es     *
 * guardar toda la informaci??n del     *
 * luego de jugar cada partida.        *
 * No recibe valores y tampoco retorna */
void saveData() {
    FILE *archivo = fopen("csv\\Usuarios.csv", "w");
    Pair *aux;
    User *auxUser;

    if (jugador != NULL) {
        usuario->havePistol = jugador->havePistol;
        usuario->health = jugador->health;
        strcpy(usuario->level, jugador->level);
        usuario->X = jugador->info->X;
        usuario->Y = jugador->info->Y;
    }

    fprintf(archivo, "Name,Password,Level,posX,posY,Health,Pistol\n");
    aux = firstMap(usuarios);
    while (aux != NULL) {
        auxUser = aux->value;

        fprintf(archivo, "%s,", auxUser->name);
        fprintf(archivo, "%s,", auxUser->password);
        fprintf(archivo, "%s,", auxUser->level);
        fprintf(archivo, "%d,", auxUser->X);
        fprintf(archivo, "%d,", auxUser->Y);
        fprintf(archivo, "%d,", auxUser->health);
        if (auxUser->havePistol == false) fprintf(archivo, "false\n");
        else if (auxUser->havePistol == true) fprintf(archivo, "true\n");

        aux = nextMap(usuarios);
    }

    fclose(archivo);
}

/*************** | acciones() | ******************/
/* El proposito de esta funci??n es permitir las  *
 * acciones del jugador como el movimiento o el  *
 * disparo.                                      *
 * No recibe valores y tampoco retorna.          */
void acciones() {
    Pixel *pos = jugador->info;

    gotoxy(jugador->info->X, jugador->info->Y);
    printf(" ");
    if (jugador->jump->existSalto != true) movimientoLateral();
    
    movimientoVertical();
    gotoxy(jugador->info->X, jugador->info->Y);
    printf("%c", jugador->info->forma);
    if (kbhit()) {
        if (GetAsyncKeyState(ESC)) {
            saveData();
            menu();
        }
        if (GetAsyncKeyState(ENTER) && jugador->havePistol == true) {
            createBullet(pos->X, pos->Y, jugador->direccionH, jugador->balas);
            jugador->havePistol = false;
        }
    }  
    disparo(jugador->balas);
}

/*************** | HUD() | **************/
/* El proposito de esta funci??n es      *
 * mostrar informacion al usuario a     *
 * medida que juego.                    *
 * No recibe ni retorna valores.        */
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

/************* | GameOver() | ****************/
/* El proposito de esta funci??n es mostrar   *
 * la pantalla de muerte al usuario y        *
 * llevarlo al menu.                         *
 * Recibe una bandera que indica si el       *
 * jugador murio o salio del juego por       *
 * desicion propia.                          *
 * No retorna valores.                       */
void GameOver(int flag) {
    cleanList(torretas);
    cleanList(enemies);
    cleanList(obstaculos);
    cleanList(objetos);
    if (flag == 1) {
        system("cls");
        gotoxy(50,10);
        if (usuario->music == 1) sndPlaySound("sound\\Sadness and Sorrow-8 Bits.wav", SND_ASYNC);
        printf("PERDISTEEEEE!!!!!");
        gotoxy(0,30);
        printf("Pulse Escape para volver al menu.");
        while (true) {
            if (GetAsyncKeyState(ESC)) menu();
        }
    }
}

/************ | muertePlayer() | **************/
/* El proposito de esta funci??n es provocar   *
 * la muerte del usuario y llevarlo a la      *
 * posicion inicial de cada nivel.            *
 * No recibe ni retorna valores.              */
void muertePlayer() {
    if (strcmp(jugador->level, "level1") == 0) {
        jugador->info->X = 2;
        jugador->info->Y = 18;
    }
    if (strcmp(jugador->level, "level2") == 0) {
        jugador->info->X = 1;
        jugador->info->Y = 23;
    }
    if (strcmp(jugador->level, "level3") == 0) {
        jugador->info->X = 117;
        jugador->info->Y = 4;
    }
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

void level3(int firstTime) {
    prepareVariables();

    if (firstTime == 1) {
        jugador = createPlayer(117, 4, 3, "level3", false);
    }

    leerArchivoEnemies("csv\\Level3\\enemies.csv");
    leerArchivoObstaculos("csv\\Level3\\obstaculos.csv");
    leerArchivoTurrets("csv\\Level3\\turrets.csv");
    leerArchivoObjetos("csv\\Level3\\objetos.csv");

    system("cls");
    mostrarCursor(false);
    mostrarEscenario(117,28);
    mostrarObstaculos();
    mostrarTurrets();
    gotoxy(jugador->info->X, jugador->info->Y);
    printf("%c", jugador->info->forma);

    while (true) {
        Sleep(FPS);
        mostrarObjetos();
        movimientoEnemigos();
        acciones();
        accionTurrets(torretas);
        HUD();
    }
}

void level2(int firstTime) {
    prepareVariables();

    if (firstTime == 1) {
        jugador = createPlayer(1, 23, 3, "level2", false);
    }

    leerArchivoEnemies("csv\\Level2\\enemies.csv");
    leerArchivoObstaculos("csv\\Level2\\obstaculos.csv");
    leerArchivoTurrets("csv\\Level2\\turrets.csv");
    leerArchivoObjetos("csv\\Level2\\objetos.csv");

    system("cls");
    mostrarCursor(false);
    mostrarEscenario(117,28);
    mostrarObstaculos();
    mostrarTurrets();
    gotoxy(jugador->info->X, jugador->info->Y);
    printf("%c", jugador->info->forma);

    while (true) {
        Sleep(FPS);
        mostrarObjetos();
        movimientoEnemigos();
        acciones();
        accionTurrets(torretas);
        HUD();
    }
}

void level1(int firstTime) {
    if (firstTime == 1) {
        jugador = createPlayer(2, 18, 3, "level1", false);
    }

    if (usuario->music == 1) sndPlaySound("sound\\Boss 2 Aaron's Remix (Terraria).wav", SND_ASYNC | SND_LOOP);
    prepareVariables();

    leerArchivoEnemies("csv\\Level1\\enemies.csv");
    leerArchivoObstaculos("csv\\Level1\\obstaculos.csv");
    leerArchivoTurrets("csv\\Level1\\turrets.csv");
    leerArchivoObjetos("csv\\Level1\\objetos.csv");

    system("cls");
    mostrarCursor(false);
    mostrarEscenario(117,28);
    mostrarObstaculos();
    mostrarTurrets();
    gotoxy(jugador->info->X, jugador->info->Y);
    printf("%c", jugador->info->forma);

    while (true) {
        Sleep(FPS);
        mostrarObjetos();
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

void registro() {
    char name[15];
    char password[11];
    char password2[11];
    char accion;

    system("cls");
    mostrarEscenario(60,16);
    gotoxy(25,3);
    printf("REGISTRO");
    gotoxy(2,6);
    printf("Nuevo nombre de usuario: ");
    fflush(stdin);
    gets(name);
    gotoxy(2,8);

    if (searchMap(usuarios, name) != NULL) {
        printf("El usuario ingresado ya existe");
        gotoxy(2,9);
        printf("deseas iniciar sesi??n? (s/n): ");
        while (true) {
            fflush(stdin);
            accion = getch();
            accion = tolower(accion);
            if (accion == 's') iniciarSesion();
            if (accion == 'n') registro();
            printf("Opcion escogida incorrecta, intentelo de nuevo:           ");
            gotoxy(2,11);
        }
    }
    else {
        while(true) {
            gotoxy(2, 8);
            printf("Password:                                                 ");
            gotoxy(12, 8);
            fflush(stdin);
            gets(password);
            gotoxy(2, 10);
            printf("Confirm Password:                                         ");
            gotoxy(20, 10);
            fflush(stdin);
            gets(password2);
            if (strcmp(password, password2) != 0) {
                gotoxy(2, 12);
                printf("Las contrase??as no coinciden");
            }
            else break;
        }
    }
    usuario = createUser(2, 18, password, name, "level1", 3, false);
    insertMap(usuarios, name, usuario);
    menu();
}

void eliminarPersonaje() {
    int cont;

    gotoxy(2,12);
    printf("Eliminando Personaje");
    for (cont = 0; cont < 3; cont++) {
        Sleep(300);
        printf(".");
    }
    eraseMap(usuarios, usuario->name);
    gotoxy(2,13);
    printf("Personaje eliminado con exito!");

}

void opciones() {
    COORD pos;
    char accion;
    char opcion;

    pos.X = 2;
    pos.Y = 8;

    system("cls");
    mostrarEscenario(60,20);
    gotoxy(25,3);
    printf("OPCIONES");
    gotoxy(2,8);
    printf("Musica: Si");
    gotoxy(2,10);
    printf("Eliminar Personaje");
    gotoxy(2, 16);
    printf("Pulse Escape para volver al menu.");
    gotoxy(2, 18);
    printf("A = Izquierda  D = Derecha  W = Arriba  S = Abajo");
    gotoxy(21,19);
    printf("E = Enter");
    gotoxy(pos.X, pos.Y);

    while (true) {
        accion = getch();
        accion = tolower(accion);

        if (accion == 's') 
            if (pos.X == 2 && pos.Y == 8) pos.Y = 10;
        if (accion == 'w')
            if (pos.X == 2 && pos.Y == 10) pos.Y = 8;
        if (accion == 'e') {
            if (pos.X == 2 && pos.Y == 8) {
                gotoxy(2,8);
                if (usuario->music == 1) {
                    printf("Musica: No");
                    usuario->music = 0;
                }
                else {
                    printf("Musica: Si");
                    usuario->music = 1;
                }
                gotoxy(2,8);
            }
            if (pos.X == 2 && pos.Y == 10) {
                gotoxy(2,11);
                printf("Estas seguro de esto :c? (s/n): ");
                while (true) {
                    gotoxy(33,11);
                    opcion = getch();
                    opcion = tolower(opcion);
                    
                    if (opcion == 's') {
                        eliminarPersonaje();
                        while(true) {
                            if (GetAsyncKeyState(ESC)) {
                                saveData();
                                main();
                            }
                        }
                    }
                    if (opcion == 'n') opciones();
                }
            }
        }

        if (GetAsyncKeyState(ESC)) menu();
        gotoxy(pos.X, pos.Y);
    }
}

void iniciarSesion() {
    char name[15];
    char password[11];
    char accion;
    usuario = NULL;

    system("cls");
    mostrarEscenario(60,16);
    gotoxy(23,3);
    printf("INICIO DE SESION");

    if (usuarios->size == 0) {
        gotoxy(2,6);
        printf("No hay usuarios registrados.");
        gotoxy(2,7);
        printf("Desea hacerlo? (s/n): ");
        gotoxy(23,7);
        while (true) {
            fflush(stdin);
            accion = getch();
            accion = tolower(accion);
            if (accion == 's') registro();
            if (accion == 'n') main();
            gotoxy(2,7);
            printf("Opcion incorrecta, intentelo de nuevo: ");
            gotoxy(40, 7);
        }
    }
    gotoxy(2,6);
    printf("Name: ");
    fflush(stdin);
    gets(name);
    gotoxy(2,8);

    if (searchMap(usuarios, name) != NULL) {
        usuario = (User *)searchMap(usuarios, name)->value;
        printf("Password: ");
        while (true) {
            fflush(stdin);
            gets(password);

            if (strcmp(usuario->password, password) == 0) {
                jugador = createPlayer(usuario->X, usuario->Y, usuario->health, usuario->level, usuario->havePistol);
                menu();
            }
            else {
                gotoxy(2,10);
                printf("Contrasena incorrecta, intentelo de nuevo:               ");
                gotoxy(45, 10);
            }
        }
    }
    else {
        printf("Usuario no encontrado.\n");
        gotoxy(2,9);
        printf("Desea registrarse? (s/n): ");
        fflush(stdin);
        accion = getch();
        accion = tolower(accion);
        if (accion == 's') registro(); 
    }

    
    gotoxy(1,15);
    printf("Pulse Escape para volver al menu.");
    while (true) {
        if (GetAsyncKeyState(ESC)) menu();
    }
}

void menu() {
    COORD pos;
    char accion;
    char basura[100];

    if (usuario->music == 1) sndPlaySound("sound\\Menu music.wav", SND_ASYNC | SND_LOOP);
    pos.X = 9;
    pos.Y = 11;

    system("cls");
    mostrarCursor(true);
    mostrarEscenario(51,27);
    gotoxy(18,6);
    printf("Jimbo Adventures");
    gotoxy(9,11);
    printf("Nueva Partida");
    gotoxy(31,11);
    printf("Continuar Partida");
    gotoxy(9,16);
    printf("Opciones");
    gotoxy(31,16);
    printf("Instrucciones");
    gotoxy(23, 21);
    printf("Salir");
    gotoxy(2, 25);
    printf("A = Izquierda  D = Derecha  W = Arriba  S = Abajo");
    gotoxy(21,26);
    printf("E = Enter");

    gotoxy(pos.X, pos.Y);
    while (true) {
        
        accion = getch();
        accion = tolower(accion);

        if (accion == 'd') {
            if (pos.X == 9 && pos.Y == 11) pos.X = 31;
            if (pos.X == 9 && pos.Y == 16) pos.X = 31;
        }
        if (accion == 'a') {
            if (pos.X == 31 && pos.Y == 11) pos.X = 9;
            if (pos.X == 31 && pos.Y == 16) pos.X = 9;
        }
        if (accion == 's') {
            if ((pos.X == 9 && pos.Y == 16) || (pos.X == 31 && pos.Y == 16)) {
                pos.X = 23;
                pos.Y = 21;
            }
            if (pos.X == 9 && pos.Y == 11) pos.Y = 16;
            if (pos.X == 31 && pos.Y == 11) pos.Y = 16;
        }
        if (accion == 'w') {
            if (pos.X == 9 && pos.Y == 16) pos.Y = 11;
            if (pos.X == 31 && pos.Y == 16) pos.Y = 11;
            if (pos.X == 23 && pos.Y == 21) {
                pos.X = 9;
                pos.Y = 16;
            }
        }

        if (accion == 'e') {
            if (pos.X == 9 && pos.Y == 11) level1(1);
            if (pos.X == 9 && pos.Y == 16) opciones();
            if (pos.X == 31 && pos.Y == 11) {
                if (usuario->music == 1) sndPlaySound("sound\\Boss 2 Aaron's Remix (Terraria).wav", SND_ASYNC | SND_LOOP);
                if (strcmp(jugador->level, "level1") == 0) level1(0);
                if (strcmp(jugador->level, "level2") == 0) level2(0);
                if (strcmp(jugador->level, "level3") == 0) level3(0);
            }
            if (pos.X == 31 && pos.Y == 16) instrucciones();
            if (pos.X == 23 && pos.Y == 21) {
                saveData();
                exit(1);
            }
        }
        gotoxy(pos.X, pos.Y);
    }
}

int main() {
    COORD pos;
    char accion;
    char basura[100];
    usuarios = createMap(25);

    leerArchivoUsuarios(usuarios);
    sndPlaySound("sound\\Menu music.wav", SND_ASYNC | SND_LOOP);
    pos.X = 19;
    pos.Y = 11;

    system("cls");
    mostrarCursor(true);
    mostrarEscenario(51,27);
    gotoxy(18,6);
    printf("Jimbo Adventures");
    gotoxy(19,11);
    printf("Iniciar Sesion");
    gotoxy(20,16);
    printf("Registrarse");
    gotoxy(23, 21);
    printf("Salir");
    gotoxy(2, 25);
    printf("A = Izquierda  D = Derecha  W = Arriba  S = Abajo");
    gotoxy(21,26);
    printf("E = Enter");
    gotoxy(pos.X, pos.Y);
    while (true) {
        accion = getch();
        accion = tolower(accion);

        if (accion == 's') {
            if (pos.Y == 11) {
                pos.Y = 16;
                pos.X = 20;
            }
            else if (pos.Y == 16) {
                pos.X = 23;
                pos.Y = 21;
            }
        }
        if (accion == 'w') {
            if (pos.Y == 21) {
                pos.X = 20;
                pos.Y = 16;
            }
            else if (pos.Y == 16) {
                pos.Y = 11;
                pos.X = 19;
            }
        }

        if (accion == 'e') {
            if (pos.Y == 11) iniciarSesion();
            if (pos.Y == 16) registro();
            if (pos.Y == 21) exit(1);
        }
        gotoxy(pos.X, pos.Y);
    }

    return EXIT_SUCCESS;
}