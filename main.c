#include <stdio.h>
#include <windows.h>
/*
Derecha(D) = 0x44
izquierda(A) = 0x41
arriba(W) =   0x57
abajo(S) = 0x53
espacio( ) =  VK_BACK
.\prototipo1.exe
*/
typedef struct
{
  int x, y;
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


int main ()
{
  int opcion;
  opcion = 0;

  while(opcion != 12) 
  {
    //system("cls");

    printf("\n========== NACHITO BROS ==========\n");
    printf("(1) NUEVA PARTIDA");
    printf("(2) CARGAR PARTIDA");
    printf("(3) OPCIONES");
    printf("(4) SALIR DEL JUEGO");

    printf("\nINGRESE SU OPCION: ");
    fflush(stdin);
    scanf("%i", &opcion);

    switch(opcion)
    {
      case 1:
        nuevaPartida();
        break;
      /*case 2:
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
