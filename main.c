
#include <ncurses.h>
#include <string.h>


void pantalla_bienvenida();
void pantalla_menu();
void pantalla_espera();
void pantalla_juego();
void pantalla_fin();

int main() {
    initscr();
    curs_set(FALSE);
    noecho();
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);

    pantalla_bienvenida();

    pantalla_menu();

    pantalla_espera();

    pantalla_juego();

    pantalla_fin();

    endwin();

    return 0;

}
