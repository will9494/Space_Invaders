#include <ncurses.h>
#include <string.h>

/*****DEFINICIONES*****/

void pantalla_bienvenida();
void pantalla_menu();
void pantalla_espera();
void pantalla_juego();
void pantalla_fin();

void die(char *s);
void start_share();
void write_share(int n_dato, int dato);
void free_share();

void inicializar_semaforo();
void eliminar_semaforo();
void up_semaforo();
void down_semaforo();

int get_semaforo();


/*****GLOBALES*****/

extern int *s_activo1;
extern int *s_activo2;
extern int *s_color1;
extern int *s_color2;

/*****FUNCIONES*****/

int main() {

    inicializar_semaforo();
    start_share();

    initscr();
    curs_set(FALSE);
    noecho();
    start_color();

    pantalla_bienvenida();
    pantalla_menu();
    pantalla_espera();

    if(*s_activo1 == -1)
        *s_activo1 = 1;
    else if(*s_activo2 == -1)
        *s_activo2 = 1;

    if(*s_activo1 != -1 && *s_activo2 != -1)
        up_semaforo();
    else
        down_semaforo();

    eliminar_semaforo();
    free_share();

    pantalla_juego();
    pantalla_fin();
    endwin();

    return 0;
}
