#include <stdio.h>
#include <windows.h>
#include <ctype.h>
#include <string.h>

const char *get_csv_field (char * tmp, int k) {
    int open_mark = 0;
    char* ret=(char*) malloc (100*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i+1]!='\0'){

        if(tmp[i]== '\"'){
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }

        if(open_mark || tmp[i]!= ','){
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }

        if(tmp[i]== ','){
            if(k==j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }

        i++;
    }

    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }


    return NULL;
}

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

size_t toNumber(char *string) {
    size_t cont;
    size_t num = 0;

    for (cont = 0; cont < strlen(string); cont++) {
        num = (num * 10) + (string[cont] - '0');
    }

    return num;
}