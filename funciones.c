#include "biblioteca.h"

void gotoxy(int x, int y) {
    HANDLE consola = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD posicion;
    posicion.X = x; // Columna
    posicion.Y = y; // Fila
    SetConsoleCursorPosition(consola, posicion);
}

void mostrar_matriz(int mat[CANT_FIL][CANT_COL]){
    int i,j;
    int offset_x = 10;
    int offset_y = 2;
    for(i=0; i<CANT_FIL; i++){
        gotoxy(offset_x, offset_y+i);
        for(j=0; j<CANT_COL; j++)
            if(mat[i][j]==0)
                printf(". ");
            else
                printf("[]");
    }
}

void mostrar_interfaz(){
    gotoxy(35, 5);
    printf("T E T R I S");

    gotoxy(35, 8);
    printf("PUNTAJE:");

    gotoxy(35, 12);
    printf("PROXIMA PIEZA:");

    //Bordes fijos alrededor de la matriz???
}

void actualizar_puntaje(int puntos){
    gotoxy(44, 8);
    printf("%d", puntos);
}
