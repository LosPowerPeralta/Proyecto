#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#define RIGTH 0x44
#define LEFT 0x41
#define UP 0x57
#define DOWN 0x53
#define SPACE 0x20
/*
Derecha(D) = 0x44
izquierda(A) = 0x41
arriba(W) =   0x57
abajo(S) = 0x53
espacio( ) =  VK_BACK
.\proyecto.exe
*/
typedef struct
{
  int x, y;
}Pixel;

Pixel *createCabeza(){
    Pixel *newCabeza = (Pixel *) malloc(sizeof(Pixel));

    newCabeza->x = 1;
    newCabeza->y = 1;

    return newCabeza;
}

void gotoxy(int x, int y)
{
  HANDLE consola = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD pos;

  pos.X = x;
  pos.Y = y;

  SetConsoleCursorPosition(consola, pos);
}

void mostrarEscenario() 
{ 
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

void saltoDerecha(Pixel *cabeza) {
    int alturaMax = cabeza->y - 4; 
    int distanciaMax = cabeza->x + 9;

    while (cabeza->y > alturaMax) {
        Sleep(40);
        gotoxy(cabeza->x, cabeza->y);
        printf(" ");
        if (cabeza->x != 63) cabeza->x++;
        if (cabeza->y == 1) break;
        cabeza->y--;
        gotoxy(cabeza->x, cabeza->y);
        printf("x");
    }
    
    gotoxy(cabeza->x, cabeza->y);
    printf(" ");
    gotoxy(cabeza->x, cabeza->y);
    printf("x");

    while (cabeza->y < 18) {
        Sleep(40);
        gotoxy(cabeza->x, cabeza->y);
        printf(" ");
        if (cabeza->x != 63) cabeza->x++;
        if (cabeza->y != 18) cabeza->y++;
        gotoxy(cabeza->x, cabeza->y);
        printf("x");
    }
}

void saltoIzquierda(Pixel *cabeza) {
    int alturaMax = cabeza->y - 4; 

    while (cabeza->y > alturaMax) {
        Sleep(40);
        gotoxy(cabeza->x, cabeza->y);
        printf(" ");
        if (cabeza->x != 1) cabeza->x--;
        if (cabeza->y == 1) break;
        cabeza->y--;
        gotoxy(cabeza->x, cabeza->y);
        printf("x");
    }
    
    gotoxy(cabeza->x, cabeza->y);
    printf(" ");
    gotoxy(cabeza->x, cabeza->y);
    printf("x");

    while (cabeza->y < 18) {
        Sleep(40);
        gotoxy(cabeza->x, cabeza->y);
        printf(" ");
        if (cabeza->x != 1) cabeza->x--;
        if (cabeza->y != 18) cabeza->y++;
        gotoxy(cabeza->x, cabeza->y);
        printf("x");
    }
}

void saltoVertical(Pixel *cabeza)
{
  int alturaMax = cabeza->y - 4;

  while(cabeza->y > alturaMax)
  {
    Sleep(40);
    gotoxy(cabeza->x, cabeza->y);
    printf(" ");
    if (cabeza->y == 1) break;
    cabeza->y--;
    gotoxy(cabeza->x, cabeza->y);
    printf("x");
  }

  while(cabeza->y < alturaMax + 4)
  {
    Sleep(40);
    gotoxy(cabeza->x, cabeza->y);
    printf(" ");
    if (cabeza->y == 1) break;
    cabeza->y++;
    gotoxy(cabeza->x, cabeza->y);
    printf("x");
  }
}

void movimiento(Pixel *user) {
    if (GetAsyncKeyState(LEFT))
        if (GetAsyncKeyState(SPACE)) saltoIzquierda(user);
        else if (user->x != 1) user->x--;
    if (GetAsyncKeyState(RIGTH)) {
        if (GetAsyncKeyState(SPACE)) saltoDerecha(user);
        else if (user->x != 63) user->x++;
    }
    if(GetAsyncKeyState(SPACE))
      saltoVertical(user);
    /*
    if (GetAsyncKeyState(UP))
        if (user->y != 1) user->y--;
    if (GetAsyncKeyState(DOWN)) 
        if (user->y != 18) user->y++;*/
}

void nuevaPartida(Pixel* cabeza)
{
  Pixel *auxCabeza = createCabeza();
  system("cls");
  mostrarEscenario();
  gotoxy(cabeza->x, cabeza->y);
  printf("x");

  while (true) 
  {
    Sleep(100);
    auxCabeza = cabeza;
    gotoxy(auxCabeza->x, auxCabeza->y);
    printf(" ");
    movimiento(cabeza);
    gotoxy(cabeza->x, cabeza->y);
    printf("x");
  }

}

int main ()
{
  Pixel *cabeza = createCabeza();
  int opcion;

  opcion = 0;

  while(opcion != 4) 
  {
    //system("cls");

    printf("\n========== NACHITO BROS ==========\n");
    printf("(1) NUEVA PARTIDA\n");
    printf("(2) CARGAR PARTIDA\n");
    printf("(3) OPCIONES\n");
    printf("(4) SALIR DEL JUEGO\n");

    printf("\nINGRESE SU OPCION: ");
    fflush(stdin);
    scanf("%i", &opcion);

    switch(opcion)
    {
      
      case 1:
        nuevaPartida(cabeza);
        break;
      /*
      case 2:
        cargarPartida();
        break;
      case 3:
        opcionesJuego();
        break;*/
      case 4:
        return 0;
    }
  }

  return EXIT_SUCCESS;
}
