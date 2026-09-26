#include "biblioteca.h"

void gotoxy(int x, int y) {
    HANDLE consola = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD posicion;
    posicion.X = x; // Columna
    posicion.Y = y; // Fila
    SetConsoleCursorPosition(consola, posicion);
}

void mostrar_matriz(int mat[CANT_FIL][CANT_COL]){
    int offset_x = 10;
    int offset_y = 2;
    for(int i = 0; i < CANT_FIL; i++){
        gotoxy(offset_x, offset_y + i);
        for(int j = 0; j < CANT_COL; j++)
            if( mat[i][j] == 0 )
                printf(". ");
            else
                printf("[]");
    }
}

void mostrar_partida(){
    int y;
    gotoxy(35, 5);
    printf("T E T R I S");
    gotoxy(35, 8);
    printf("PUNTAJE:");
    gotoxy(35, 12);
    printf("PROXIMA PIEZA:");
    for( y = 2; y < CANT_FIL + 2; y++){
        gotoxy(8, y);
        printf("|");
        gotoxy(31, y);
        printf("|");
    }
    gotoxy(8, CANT_FIL + 2);
    printf("------------------------");
    gotoxy(35, 19);
    printf("MOVER PIEZA: A y D");
    gotoxy(35, 20);
    printf("ROTAR PIEZA: W");
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
    while(fread(&usuario, sizeof(tUsuario), 1, pf) == 1){
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
    tipo_pieza = rand() % 7 + 1;
    pieza->posX = 3;
    pieza->posY = 0;
    pieza->tipo = tipo_pieza;

    switch(tipo_pieza){
        case 1: //2x2
            memcpy(pieza->matriz_forma, MOLDE_O, sizeof(int) * 16);
            break;
        case 2: //3x1 centro
            memcpy(pieza->matriz_forma, MOLDE_T, sizeof(int) * 16);
            break;
        case 3: //3x1 derecha
            memcpy(pieza->matriz_forma, MOLDE_L, sizeof(int) * 16);
            break;
        case 4: //3x1 izquierda
            memcpy(pieza->matriz_forma, MOLDE_J, sizeof(int) * 16);
            break;
        case 5: //2x2 cruzado
            memcpy(pieza->matriz_forma, MOLDE_S, sizeof(int) * 16);
            break;
        case 6: //2x2 cruzado al revés
            memcpy(pieza->matriz_forma, MOLDE_Z, sizeof(int) * 16);
            break;
        case 7: //4x1
            memcpy(pieza->matriz_forma, MOLDE_I, sizeof(int) * 16);
            break;
    }
    return pieza;
}

void mostrar_pieza(tPieza* pieza){
    int x_consola, y_consola;
    int offset_x = 10;
    int offset_y = 2;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            if(pieza->matriz_forma[i][j] != 0){
                x_consola = offset_x + (pieza->posX+j) * 2;
                y_consola = offset_y + pieza->posY+i;
                gotoxy(x_consola, y_consola);
                printf("[]");
            }
}

bool verificar_derrota(int mat[CANT_FIL][CANT_COL], tPieza* pieza){
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            if(pieza->matriz_forma[i][j] != 0)
                if(mat[pieza->posY + i][pieza->posX +j] != 0)
                    return true; //game over
    return false;
}

bool es_movimiento_valido(int mat[CANT_FIL][CANT_COL], tPieza* pieza, int movX, int movY, int matriz_prueba[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (matriz_prueba[i][j] != 0) { // Si hay un bloque de la pieza
                int nuevoX = pieza->posX + j + movX;
                int nuevoY = pieza->posY + i + movY;

                // 1. Choca contra las paredes (izquierda o derecha)?
                if (nuevoX < 0 || nuevoX >= CANT_COL) return false;

                // 2. Choca contra el piso?
                if (nuevoY >= CANT_FIL) return false;

                // 3. Choca contra otra pieza ya fijada en el tablero?
                if (nuevoY >= 0 && mat[nuevoY][nuevoX] != 0) return false;
            }
        }
    }
    return true;
}

void fijar_pieza(int mat[CANT_FIL][CANT_COL], tPieza* pieza) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (pieza->matriz_forma[i][j] != 0)
                // Copiamos el número del bloque a la matriz principal
                mat[pieza->posY + i][pieza->posX + j] = pieza->matriz_forma[i][j];
}

void intentar_rotar(int mat[CANT_FIL][CANT_COL], tPieza* pieza) {
    int matriz_temporal[4][4] = {0};

    // Algoritmo para rotar 90 grados a la derecha
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            matriz_temporal[j][3 - i] = pieza->matriz_forma[i][j];

    // Verificamos si la pieza ROTADA cabe en la posición actual
    if (es_movimiento_valido(mat, pieza, 0, 0, matriz_temporal))
        // Si es válido, aplicamos los cambios a la pieza real
        memcpy(pieza->matriz_forma, matriz_temporal, sizeof(int) * 16);
}

void eliminar_filas_llenas(int mat[CANT_FIL][CANT_COL], tUsuario* usuario) {
    int filas_borradas = 0;

    for (int i = CANT_FIL - 1; i >= 0; i--) {
        bool fila_llena = true;
        for (int j = 0; j < CANT_COL; j++) {
            if (mat[i][j] == 0) {
                fila_llena = false; // Hay un hueco, no está llena
                break;
            }
        }

        if (fila_llena) {
            // Bajar todas las filas que están arriba de 'i'
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < CANT_COL; j++) {
                    mat[k][j] = mat[k-1][j];
                }
            }
            // La primera fila (índice 0) se llena de ceros
            for (int j = 0; j < CANT_COL; j++) mat[0][j] = 0;

            filas_borradas++;
            i++; // Volvemos a revisar esta misma fila porque bajó una nueva
        }
    }

    // Lógica para sumar puntos al usuario (ej. 100 puntos por línea, o más si hace combo de 4)
    if (filas_borradas > 0) {
        usuario->puntos += (filas_borradas * 100);
    }
}

int mostrar_menu_interactivo() {
    int opcion_seleccionada = 0;
    int tecla;

    while (1) {
        system("cls");
        gotoxy(35, 5);
        printf("T E T R I S");

        gotoxy(30, 8);
        if (opcion_seleccionada == 0) {
            printf("%c JUGAR %c", 175, 174);
        } else {
            printf("  JUGAR  ");
        }

        gotoxy(30, 10);
        if (opcion_seleccionada == 1) {
            printf("%c TABLA HISTORICA DE PUNTOS %c", 175, 174);
        } else {
            printf("  TABLA HISTORICA DE PUNTOS  ");
        }

        gotoxy(30, 12);
        if (opcion_seleccionada == 2) {
            printf("%c SALIR %c", 175, 174);
        } else {
            printf("  SALIR  ");
        }

        tecla = getch();

        if (tecla == 224 || tecla == 0) {
            tecla = getch();
            if (tecla == 72) {
                opcion_seleccionada--;
                if (opcion_seleccionada < 0) opcion_seleccionada = 2;
            } else if (tecla == 80) {
                opcion_seleccionada++;
                if (opcion_seleccionada > 2) opcion_seleccionada = 0;
            }
        } else if (tecla == 13) {
            return opcion_seleccionada + 1;
        }
    }
}
