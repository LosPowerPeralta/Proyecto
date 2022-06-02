#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#define RIGTH 0x44
#define LEFT 0x41
#define UP 0x57
#define DOWN 0x53
#define SPACE VK_BACK
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

void movimiento(Pixel *user) 
{
    if (GetAsyncKeyState(LEFT))
        if (user->x != 1) user->x--;
    if (GetAsyncKeyState(RIGTH))
        if (user->x != 63) user->x++;
    if (GetAsyncKeyState(UP))
        if (user->y != 1) user->y--;
    if (GetAsyncKeyState(DOWN)) 
        if (user->y != 19) user->y++;
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
/*
void mapaMovimiento()
{
  Pixel pixel;
  pixel.x = 10;
  pixel.y = 10;

  gotoxy(pixel.x, pixel.y);
  printf("*");

  while(1)
  {
    //Limite para el ciclo while
    Sleep(100);
    //Movimiento izquierda
    if(GetAsyncKeyState(0x41))
    {
      if(pixel.x != 1)
      {
        gotoxy(pixel.x,pixel.y);
        printf(" ");
        pixel.x--;
        gotoxy(pixel.x, pixel.y);
        printf("*");
      }
    }

    //Movimiento derecha
    if(GetAsyncKeyState(0x44))
    {
      if(pixel.x != 63)
      {
        gotoxy(pixel.x,pixel.y);
        printf(" ");
        pixel.x++;
        gotoxy(pixel.x, pixel.y);
        printf("*");
      }
    }

    //Movimiento arriba
    if(GetAsyncKeyState(0x57))
    {
      if(pixel.y != 2)
      {
        gotoxy(pixel.x,pixel.y);
        printf(" ");
        pixel.y--;
        gotoxy(pixel.x, pixel.y);
        printf("*");
      }
    }

    //Movimiento abajo
    if(GetAsyncKeyState(0x53))
    {
      if(pixel.y != 19)
      {
        gotoxy(pixel.x,pixel.y);
        printf(" ");
        pixel.y++;
        gotoxy(pixel.x, pixel.y);
        printf("*");
      } 
    }
  }
}
*/

int main ()
{
  Pixel *cabeza = createCabeza();
  int opcion;

  opcion = 0;

  while(opcion != 12) 
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
