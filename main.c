#include "biblioteca.h"

int main(int argc, char* argv[]){
    char opcion;
    system("mode con cols=80 lines=30");
    gotoxy(10, 10);
    printf("Toque 'h' para ver la tabla de puntos u otra tecla para jugar: ");
    scanf(" %c", &opcion);
    if(opcion == 'h'){
        mostrar_tabla_puntos();
        exit(1);
    }

    tUsuario* usuario = sesion_juego();
    if(!usuario){
        printf("Sin memoria disponible.\n");
        exit(1);
    }

    int cond_victoria = 1;
    char tecla;
    int mat[CANT_FIL][CANT_COL]={0};
    mostrar_interfaz();
    while(cond_victoria==1){
        mostrar_matriz(mat);
        if(kbhit()){
            tecla = getch();
            tecla = tolower(tecla);
            switch(tecla){
                case 'a':
                //pieza.posX--
                    break;
                case'd':
                    break;
                //pieza.posX++
                case's':
                //pieza.posY++ acelerar caida
                    break;
                case'w':
                //rotar pieza
                    break;
            }
        }
        Sleep(50);
        actualizar_puntaje(usuario->puntos);
        cond_victoria = 0;
    }
    actualizar_tabla_puntos(usuario);
    free(usuario);
    gotoxy(0, 25);
    return 0;
}
