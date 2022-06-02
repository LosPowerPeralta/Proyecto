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

void movimientosPixel(int x, int y)
{
  while(1)
  {
    Sleep(100);

    if(GetAsyncKeyState(0x41))
    {
      if(x != 1)
      {
        gotoxy(x,y);
        printf(" ");
        x--;
        gotoxy(x, y);
        printf("*");
      }
    }

    if(GetAsyncKeyState(0x44))
    {
      if(x != 63)
      {
        gotoxy(x,y);
        printf(" ");
        x++;
        gotoxy(x, y);
        printf("*");
      }
    }

    if(GetAsyncKeyState(0x57))
    {
      if(y != 2)
      {
        gotoxy(x,y);
        printf(" ");
        y--;
        gotoxy(x, y);
        printf("*");
      }
    }

    if(GetAsyncKeyState(0x53))
    {
      if(y != 19)
      {
        gotoxy(x,y);
        printf(" ");
        y++;
        gotoxy(x, y);
        printf("*");
      } 
    }
  }
}

int main ()
{
  Pixel pixel;
  pixel.x = 10;
  pixel.y = 10;

  mostrarEscenario();
  movimientosPixel(pixel.x, pixel.y);
  gotoxy(pixel.x, pixel.y);
  printf("*");
  

  return 0;
}
