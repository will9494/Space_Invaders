#include <ncurses.h>
#include <string.h>

/*****DEFINICIONES*****/

void pantalla_bienvenida();
void pantalla_menu();
void pantalla_espera();
void pantalla_juego();
void pantalla_fin();

/*****FUNCIONES*****/

int main() {
    initscr();
    curs_set(FALSE);
    noecho();
    start_color();

    pantalla_bienvenida();

    pantalla_menu();

    pantalla_espera();

    pantalla_juego();

    pantalla_fin();

    endwin();

    return 0;
}
