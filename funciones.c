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
    gotoxy(35, 21);
    printf("CAIDA ACELERADA: S");
    gotoxy(35, 22);
    printf("CAIDA INSTANTANEA: ESPACIO");
}

void actualizar_puntaje(size_t puntos){
    gotoxy(44, 8);
    printf("%llu", (unsigned long long)puntos);
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
    while(i < TOP_PUNTOS && fread(&usuario, sizeof(tUsuario), 1, pf) == 1){
        gotoxy(5, 9+i);
        printf("%s", usuario.nombre);
        gotoxy(20, 9+i);
        printf("%llu", (unsigned long long)usuario.puntos);
        i++;
    }
    fclose(pf);
    return 0;
}

int actualizar_tabla_puntos(tUsuario* usuario){
    tUsuario aux;
    int insertado = 0;

    if(!usuario) return ERR_AP;

    FILE* pf = fopen(ARCH_PUNTOS, "rb"); // puede no existir en la primera partida
    FILE* pt = fopen(ARCH_TEMP, "wb");
    if(!pt){
        if(pf) fclose(pf);
        return ERR_AP;
    }

    if(pf){
        while(fread(&aux, sizeof(tUsuario), 1, pf) == 1){
            if(!insertado && usuario->puntos > aux.puntos){
                fwrite(usuario, sizeof(tUsuario), 1, pt); // entra antes del primero que tiene menos puntos
                insertado = 1;
            }
            fwrite(&aux, sizeof(tUsuario), 1, pt);
        }
        fclose(pf);
    }
    if(!insertado) // es el puntaje más bajo, o el primero de la tabla
        fwrite(usuario, sizeof(tUsuario), 1, pt);
    fclose(pt);

    remove(ARCH_PUNTOS); // en Windows rename falla si el destino ya existe
    if(rename(ARCH_TEMP, ARCH_PUNTOS) != 0)
        return ERR_AP;
    return 0;
}

tUsuario* sesion_juego(){
    tUsuario* usuario = calloc(1, sizeof(tUsuario));
    if(!usuario) return NULL;

    printf("Ingrese su nombre para esta partida: ");
    scanf("%9s", usuario->nombre);
    while(getchar() != '\n');
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
        default:
            free(pieza);
            return NULL;
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
                mat[pieza->posY + i][pieza->posX + j] = pieza->matriz_forma[i][j];
}

void intentar_rotar(int mat[CANT_FIL][CANT_COL], tPieza* pieza){
    if(pieza->tipo == 1) //si es un cuadrado no se rota, para evitar movimientos raros.
        return;
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
            for (int k = i; k > 0; k--)
                for (int j = 0; j < CANT_COL; j++)
                    mat[k][j] = mat[k-1][j];
            // La primera fila (índice 0) se llena de ceros
            for (int j = 0; j < CANT_COL; j++)
                mat[0][j] = 0;

            filas_borradas++;
            i++; // Volvemos a revisar esta misma fila porque bajó una nueva
        }
    }

    // Lógica para sumar puntos al usuario (ej. 100 puntos por línea, o más si hace combo de 4)
    switch(filas_borradas) {
        case 1:
            usuario->puntos += 100;
            break;
        case 2:
            usuario->puntos += 300;
            break;
        case 3:
            usuario->puntos += 500;
            break;
        case 4:
            usuario->puntos += 1000;
            break;
    }
}

int mostrar_menu_interactivo() {
    const char* opciones[CANT_OPCIONES] = {"JUGAR", "TABLA HISTORICA DE PUNTOS", "SALIR"};
    int opcion_seleccionada = 0;
    int tecla;

    while (1) {
        system("cls");
        gotoxy(35, 5);
        printf("T E T R I S");

        for (int i = 0; i < CANT_OPCIONES; i++) {
            gotoxy(30, 8 + i * 2);
            if (i == opcion_seleccionada)
                printf("%c %s %c", 175, *(opciones + i), 174);
            else
                printf("  %s  ", *(opciones + i));
        }

        tecla = getch();

        if (tecla == TECLA_ESPECIAL || tecla == 0) {
            tecla = getch();
            if (tecla == FLECHA_ARRIBA) {
                opcion_seleccionada--;
                if (opcion_seleccionada < 0) opcion_seleccionada = CANT_OPCIONES - 1;
            } else if (tecla == FLECHA_ABAJO) {
                opcion_seleccionada++;
                if (opcion_seleccionada > CANT_OPCIONES - 1) opcion_seleccionada = 0;
            }
        } else if (tecla == TECLA_ENTER) {
            return opcion_seleccionada + 1;
        }
    }
}
