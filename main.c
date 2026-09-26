#include "biblioteca.h"

int main(int argc, char* argv[]){
    srand(time(NULL));
    system("mode con cols=80 lines=30");

    int opcion_menu;

    while(1) {
        opcion_menu = mostrar_menu_interactivo();

        if (opcion_menu == 3) {
            break; // SALIR
        } else if (opcion_menu == 2) {
            // TABLA HISTORICA DE PUNTOS
            system("cls");
            mostrar_tabla_puntos();
            gotoxy(5, 20);
            printf("Presione cualquier tecla para volver al menu...");
            getch();
        } else if (opcion_menu == 1) {
            // JUGAR
            system("cls");

            tUsuario* usuario = sesion_juego();
            if(!usuario){
                printf("Sin memoria disponible.\n");
                exit(1);
            }

            int cond_victoria = 1;
            tPieza* pieza;
            tPieza* pieza_siguiente;
            int mat[CANT_FIL][CANT_COL]={0};
            int contador_gravedad = 0;
            int velocidad = 10; // La pieza cae cada 10 ciclos (aprox 500ms)
            char tecla;

            mostrar_partida();
            pieza = generar_pieza();
            pieza_siguiente = generar_pieza();

            if(pieza == NULL || pieza_siguiente == NULL){
                    system("cls");
                    printf("Ocurrió un error! lo sentimos...");
                    exit(1);
                }

            if(verificar_derrota(mat, pieza))
                cond_victoria = 0;

            while(cond_victoria==1){
                mostrar_matriz(mat);
                mostrar_pieza(pieza);

                // Limpiamos el área de la próxima pieza y la dibujamos
                for(int i = 0; i < 4; i++){
                    gotoxy(38, 14 + i);
                    printf("           ");
                }
                for(int i = 0; i < 4; i++){
                    for(int j = 0; j < 4; j++){
                        if(pieza_siguiente->matriz_forma[i][j] != 0){
                            gotoxy(38 + j*2, 14 + i);
                            printf("[]");
                        }
                    }
                }

                if(kbhit()){
                    tecla = getch();
                    if (tecla == 224 || tecla == 0) {
                        getch(); // Si el usuario toca flechas en el juego, las ignoramos limpiando el buffer
                    } else {
                        tecla = tolower(tecla);
                        switch(tecla){
                            case 'a':
                                if (es_movimiento_valido(mat, pieza, -1, 0, pieza->matriz_forma))
                                    pieza->posX--;
                                break;
                            case 'd':
                                if (es_movimiento_valido(mat, pieza, 1, 0, pieza->matriz_forma))
                                    pieza->posX++;
                                break;
                            case 's':
                                if (es_movimiento_valido(mat, pieza, 0, 1, pieza->matriz_forma))
                                    pieza->posY++;
                                break;
                            case 'w':
                                intentar_rotar(mat, pieza);
                                break;
                        }
                    }
                }

                // Gravedad automática
                contador_gravedad++;
                if (contador_gravedad >= velocidad) {
                    if (es_movimiento_valido(mat, pieza, 0, 1, pieza->matriz_forma)) {
                        pieza->posY++; // Cae sola
                    } else {
                        // Si no puede caer más, se fija
                        fijar_pieza(mat, pieza);
                        eliminar_filas_llenas(mat, usuario);

                        free(pieza); // Liberamos la pieza que ya se fijó
                        pieza = pieza_siguiente; // La siguiente pasa a ser la actual
                        pieza_siguiente = generar_pieza(); // Generamos una nueva

                        // Si la nueva pieza choca ni bien nace, perdimos
                        if (!es_movimiento_valido(mat, pieza, 0, 0, pieza->matriz_forma)) {
                            cond_victoria = 0;
                        }
                    }
                    contador_gravedad = 0; // Reiniciamos el contador de gravedad
                }

                Sleep(50);
                actualizar_puntaje(usuario->puntos);
            }

            // Mensaje de fin de juego
            gotoxy(10, CANT_FIL + 4);
            printf("--- G A M E  O V E R ---");
            Sleep(2000);

            actualizar_tabla_puntos(usuario);
            free(usuario);
            free(pieza);
            free(pieza_siguiente);
        }
    }

    system("cls");
    gotoxy(0, 5);
    printf("Gracias por jugar!\n");
    return 0;
}

