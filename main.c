#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#define RIGTH 0x44
#define LEFT 0x41
#define UP 0x57
#define DOWN 0x53
#define SPACE VK_BACK

typedef struct
{
  int x;
  int y;
}Pixel;

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

Pixel *createCabeza(){
    Pixel *newCabeza = (Pixel *) malloc(sizeof(Pixel));

    newCabeza->x = 1;
    newCabeza->y = 1;

    return newCabeza;
}

void movimiento(Pixel *user) {
    if (GetAsyncKeyState(LEFT))
        if (user->x != 1) user->x--;
    if (GetAsyncKeyState(RIGTH))
        if (user->x != 63) user->x++;
    if (GetAsyncKeyState(UP))
        if (user->y != 1) user->y--;
    if (GetAsyncKeyState(DOWN)) 
        if (user->y != 19) user->y++;
}

int main() {
    Pixel *cabeza = createCabeza();
    Pixel *auxCabeza = createCabeza();
    
    system("cls");
    mostrarEscenario();
    gotoxy(cabeza->x, cabeza->y);
    printf("x");

    while (true) {
        Sleep(40);
        auxCabeza = cabeza;
        gotoxy(auxCabeza->x, auxCabeza->y);
        printf(" ");
        movimiento(cabeza);
        gotoxy(cabeza->x, cabeza->y);
        printf("x");
    }
    

    return EXIT_SUCCESS;
}
