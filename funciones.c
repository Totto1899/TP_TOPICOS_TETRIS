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
    int y;
    gotoxy(35, 5);
    printf("T E T R I S");
    gotoxy(35, 8);
    printf("PUNTAJE:");
    gotoxy(35, 12);
    printf("PROXIMA PIEZA:");
    for(y=2; y<CANT_FIL + 2; y++){
        gotoxy(8, y);
        printf("|");
        gotoxy(31, y);
        printf("|");
    }
    gotoxy(8, CANT_FIL + 2);
    printf("------------------------");
}

void actualizar_puntaje(size_t puntos){
    gotoxy(44, 8);
    printf("%zu", puntos);
}

int mostrar_tabla_puntos(){
    tUsuario usuario;
    int i = 0;
    FILE* pf = fopen(ARCH_PUNTOS, "rb");
    if(!pf)
        return ERR_AP;
    gotoxy(5, 5);
    printf("TABLA HISTORICA DE PUNTOS");
    gotoxy(5, 7);
    printf("USER");
    gotoxy(20, 7);
    printf("PUNTOS");
    while(fread(&usuario, sizeof(tUsuario), 1, pf)==1){
        gotoxy(5, 9+i);
        printf("%s", usuario.nombre);
        gotoxy(20, 9+i);
        printf("%zu", usuario.puntos);
        i++;
    }
    fclose(pf);
    return 0;
}

int actualizar_tabla_puntos(tUsuario* usuario){
    FILE* pf = fopen(ARCH_PUNTOS, "a+b");
    if(!pf)
        return ERR_AP;
    fwrite(usuario, sizeof(tUsuario), 1, pf);
    fclose(pf);
    return 0;
}

tUsuario* sesion_juego(){
    tUsuario* usuario = malloc(sizeof(tUsuario));
    if(!usuario)
        return NULL;
    printf("Ingrese su nombre para esta partida: ");
    scanf("%9s", usuario->nombre);
    usuario->puntos = 0;
    system("cls");
    return usuario;
}

tPieza* generar_pieza(){
    int tipo_pieza;
    tPieza* pieza = malloc(sizeof(tPieza));
    if(!pieza)
        return NULL;
    tipo_pieza = rand()%7+1;
    pieza->posX = 3;
    pieza->posY = 0;
    pieza->tipo = tipo_pieza;

    switch(tipo_pieza){
        case 1: //2x2
            memcpy(pieza->matriz_forma, MOLDE_O, sizeof(int)*16);
            break;
        case 2: //3x1 centro
            memcpy(pieza->matriz_forma, MOLDE_T, sizeof(int)*16);
            break;
        case 3: //3x1 derecha
            memcpy(pieza->matriz_forma, MOLDE_L, sizeof(int)*16);
            break;
        case 4: //3x1 izquierda
            memcpy(pieza->matriz_forma, MOLDE_J, sizeof(int)*16);
            break;
        case 5: //2x2 cruzado
            memcpy(pieza->matriz_forma, MOLDE_S, sizeof(int)*16);
            break;
        case 6: //2x2 cruzado al revés
            memcpy(pieza->matriz_forma, MOLDE_Z, sizeof(int)*16);
            break;
        case 7: //4x1
            memcpy(pieza->matriz_forma, MOLDE_I, sizeof(int)*16);
            break;
    }
    return pieza;
}

void mostrar_pieza(tPieza* pieza){
    int i, j, x_consola, y_consola;
    int offset_x = 10;
    int offset_y = 2;

    for(i=0; i<4; i++)
        for(j=0; j<4; j++)
            if(pieza->matriz_forma[i][j] != 0){
                x_consola = offset_x + (pieza->posX+j)*2;
                y_consola = offset_y + pieza->posY+i;
                gotoxy(x_consola, y_consola);
                printf("[]");
            }
}

bool verificar_derrota(int mat[CANT_FIL][CANT_COL], tPieza* pieza){
    int i, j;
    for(i=0; i<4; i++)
        for(j=0; j<4; j++)
            if(pieza->matriz_forma[i][j] != 0)
                if(mat[pieza->posY + i][pieza->posX +j] != 0)
                    return true; //game over
    return false;
}
