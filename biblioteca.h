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

///MOLDES DE LAS PIEZAS
#define MOLDE_O (int[4][4]){ {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} }
#define MOLDE_T (int[4][4]){ {0, 2, 0, 0}, {2, 2, 2, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} }
#define MOLDE_S (int[4][4]){ {0, 3, 3, 0}, {3, 3, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} }
#define MOLDE_Z (int[4][4]){ {4, 4, 0, 0}, {0, 4, 4, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} }
#define MOLDE_L (int[4][4]){ {0, 0, 5, 0}, {5, 5, 5, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} }
#define MOLDE_J (int[4][4]){ {6, 0, 0, 0}, {6, 6, 6, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} }
#define MOLDE_I (int[4][4]){ {0, 0, 0, 0}, {7, 7, 7, 7}, {0, 0, 0, 0}, {0, 0, 0, 0} }

///FUNCIONES
void gotoxy(int x, int y);
void mostrar_matriz(int mat[CANT_FIL][CANT_COL]);
void mostrar_interfaz();
void actualizar_puntaje(size_t puntos);
tPieza* generar_pieza();
void mostrar_pieza(tPieza* pieza);
bool verificar_derrota(int mat[CANT_FIL][CANT_COL], tPieza* pieza);

int mostrar_tabla_puntos();
int actualizar_tabla_puntos(tUsuario* usuario);
tUsuario* sesion_juego();

#endif // BIBLIOTECA_H_INCLUDED
