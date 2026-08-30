#include "biblioteca.h"

int main(){
    system("mode con cols=80 lines=30");
    int cond_victoria = 1;
    int puntos = 0;
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
            Sleep(50);
        }
        actualizar_puntaje(puntos);
        cond_victoria = 0;
    }
    gotoxy(0, 25);
    return 0;
}
