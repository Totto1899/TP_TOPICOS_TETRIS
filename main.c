#include "biblioteca.h"

int main(int argc, char* argv[]){
    char opcion, tecla;
    int cond_victoria;
    tPieza* pieza;
    int mat[CANT_FIL][CANT_COL]={0};
    srand(time(NULL));
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
    
    mostrar_interfaz();
    if(pieza=generar_pieza() == NULL){
            system("cls");
            printf("Ocurrió un error! lo sentimos...");
            exit(1);
        }
    
    if(verificar_derrota(mat, pieza))
        cond_victoria = 0;

    while(cond_victoria==1){
        mostrar_matriz(mat);
        mostrar_pieza(pieza);
        if(kbhit()){
            tecla = getch();
            tecla = tolower(tecla);
            switch(tecla){
                case 'a':
                    pieza->posX--;
                    break;
                case'd':
                    pieza->posX++;
                    break;
                case's':
                    pieza->posY++;
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
