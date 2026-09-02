#ifndef BIBLIOTECA_H_INCLUDED
#define BIBLIOTECA_H_INCLUDED

///BIBLIOTECAS USADAS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>

///ESTRUCTURAS
typedef struct{
    int matriz_forma[4][4];
    int posX;
    int posY;
    int tipo;
}tPieza;

typedef struct{
    char nombre[10];
    size_t puntos;
}tUsuario;

///MACROS
#define CANT_FIL 20
#define CANT_COL 10
#define ARCH_PUNTOS "tablaPuntaje.bin"
#define ERR_AP -1

///FUNCIONES
void gotoxy(int x, int y);
void mostrar_matriz(int mat[CANT_FIL][CANT_COL]);
void mostrar_interfaz();
void actualizar_puntaje(size_t puntos);
void generar_pieza();

int mostrar_tabla_puntos();
int actualizar_tabla_puntos(tUsuario* usuario);
tUsuario* sesion_juego();

#endif // BIBLIOTECA_H_INCLUDED
