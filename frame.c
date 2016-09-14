#include <ncurses.h>
#include <string.h>

char *choices[] = {
    "       JUGADOR ROJO       ",
    "       JUGADOR AZUL       ",
    "           SALIR          ",
};
int n_choices = sizeof(choices) / sizeof(char *);
void print_menu(WINDOW *menu_win, int highlight);

void pantalla_bienvenida();
void pantalla_menu();
void pantalla_espera();
void pantalla_juego();
void pantalla_fin();

void pantalla_bienvenida(){
    /***
     *       _____ _____        _____ ______    _____ _   ___      __     _____  ______ _____   _____
     *      / ____|  __ \ /\   / ____|  ____|  |_   _| \ | \ \    / /\   |  __ \|  ____|  __ \ / ____|
     *     | (___ | |__) /  \ | |    | |__       | | |  \| |\ \  / /  \  | |  | | |__  | |__) | (___
     *      \___ \|  ___/ /\ \| |    |  __|      | | | . ` | \ \/ / /\ \ | |  | |  __| |  _  / \___ \
     *      ____) | |  / ____ \ |____| |____    _| |_| |\  |  \  / ____ \| |__| | |____| | \ \ ____) |
     *     |_____/|_| /_/    \_\_____|______|  |_____|_| \_|   \/_/    \_\_____/|______|_|  \_\_____/
     *
     *
     */


    WINDOW *margen;
    refresh();
    int x_m = (COLS - 85) / 2;
    int y_m = (LINES - 36) / 2;
    margen = newwin(36, 85, y_m, x_m);
    box(margen, 0 , 0);

    //Etiquetas
    char logo1[] ="  _____                         _____                     _               ";
    char logo2[] =" / ____|                       |_   _|                   | |              ";
    char logo3[] ="| (___  _ __   __ _  ___ ___     | |  _ ____   ____ _  __| | ___ _ __ ___ ";
    char logo4[] =" \\___ \\| '_ \\ / _` |/ __/ _ \\    | | | '_ \\ \\ / / _` |/ _` |/ _ \\ '__/ __|";
    char logo5[] =" ____) | |_) | (_| | (_|  __/   _| |_| | | \\ V / (_| | (_| |  __/ |  \\__ \\";
    char logo6[] ="|_____/| .__/ \\__,_|\\___\\___|  |_____|_| |_|\\_/ \\__,_|\\__,_|\\___|_|  |___/";
    char logo7[] ="       | |                                                                ";
    char logo8[] ="       |_|                                                                ";

    char msg1[] = "Bienvenido";
    char msg2[] = "Presiona INTRO para continuar";
    char msg3[] = "William Fernando Valladares Munoz";
    char msg4[] = "201314510";

    int xl = (COLS - strlen(logo4))/2;
    int yl1 = LINES/2 - 10;
    int yl2 = LINES/2 - 9;
    int yl3 = LINES/2 - 8;
    int yl4 = LINES/2 - 7;
    int yl5 = LINES/2 - 6;
    int yl6 = LINES/2 - 5;
    int yl7 = LINES/2 - 4;
    int yl8 = LINES/2 - 3;


    int x2 = (COLS - strlen(msg1))/2;
    int y2 = LINES/2;
    int x3 = (COLS - strlen(msg2))/2;
    int y3 = LINES/2 + 6;
    int x4 = 1;
    int y4 = LINES - 2;
    int x5 = COLS - strlen(msg4) - 1;
    int y5 = LINES - 2;

    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    attron(COLOR_PAIR(1));
    mvprintw(yl1,xl,"%s",logo1);
    mvprintw(yl2,xl,"%s",logo2);
    mvprintw(yl3,xl,"%s",logo3);
    mvprintw(yl4,xl,"%s",logo4);
    mvprintw(yl5,xl,"%s",logo5);
    mvprintw(yl6,xl,"%s",logo6);
    mvprintw(yl7,xl,"%s",logo7);
    mvprintw(yl8,xl,"%s",logo8);
    attroff(COLOR_PAIR(1));

    mvprintw(y2,x2,"%s",msg1);
    mvprintw(y3,x3,"%s",msg2);
    mvprintw(y4,x4,"%s",msg3);
    mvprintw(y5,x5,"%s",msg4);

    WINDOW *margen2;
    int x_m2 = x3 - 2;
    int y_m2 = y3 - 2;
    margen2 = newwin(5, 33, y_m2, x_m2);
    box(margen2, 0 , 0);

    wrefresh(margen);
    wrefresh(margen2);

    //Tecla Enter

    while(1) {
        char c = getch();

        if(c == '\n')
            break;
    }

    delwin(margen2);
    delwin(margen);
}

void pantalla_menu(){
    WINDOW *menu_win;
    int highlight = 1;
    int choice = 0;
    int c;

    clear();
    cbreak();

    int startx = (85 - 30) / 2;
    int starty = (36 - 7) / 2;
    menu_win = newwin(7, 30, starty, startx);

    keypad(menu_win, TRUE);
    mvprintw(1, 10, "Utilizar las flechas arriba y abajo, luego presionar INTRO");

    char msg[] = "Selecciona tu lado";
    int x2 = (COLS - strlen(msg))/2;
    int y2 = LINES/2 - 5;
    mvprintw(y2,x2,"%s",msg);

    char msg1[] = "IMPORTANTE:";
    char msg2[] = "El juego iniciara hasta que ambos lados tengan un jugador. De";
    char msg3[] = "lo contrario debe esperar a que alguien entre a jugar contra";
    char msg4[] = "ti.";

    mvprintw(30, 10, "%s",msg1);
    mvprintw(31, 10, "%s",msg2);
    mvprintw(32, 10, "%s",msg3);
    mvprintw(33, 10, "%s",msg4);

    refresh();
    print_menu(menu_win, highlight);

    while(1) {
        c = wgetch(menu_win);
        switch(c){
            case KEY_UP:
                if(highlight == 1)
                    highlight = n_choices;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if(highlight == n_choices)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10:
                choice = highlight;
                break;
            default:
                break;
        }

        print_menu(menu_win, highlight);

        if(choice != 0)	/* User did a choice come out of the infinite loop */
            break;
    }

    //mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
    //clrtoeol();
    //refresh();

    if(choice == 3){
        endwin();
    }else{
        getch();
    }
}

void print_menu(WINDOW *menu_win, int highlight) {
    box(menu_win, 0, 0);
    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
    init_pair(3, COLOR_BLACK, COLOR_WHITE);

    int x = 2;
    int y = 2;
    for(int i = 0; i < n_choices; ++i) {
        /* High light the present choice */
        if(highlight == i + 1) {
            wattron(menu_win,COLOR_PAIR(i+1));
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win,COLOR_PAIR(i+1));
        }else {
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        }
        ++y;
    }

    wrefresh(menu_win);
}

void pantalla_espera(){
    WINDOW *margen;
    refresh();
    margen = newwin(36, 85, 0, 0);
    wrefresh(margen);

    char msg[] = "Esperando al otro jugador...";
    int x = (COLS - strlen(msg))/2;
    int y = LINES/2;
    mvprintw(y,x,"%s",msg);

    WINDOW *margen2;
    int x_m2 = x - 2;
    int y_m2 = y - 2;
    margen2 = newwin(5, 33, y_m2, x_m2);
    box(margen2, 0 , 0);
    wrefresh(margen2);

    getch();

    delwin(margen);
    delwin(margen2);
}

void pantalla_juego(){
    WINDOW *margen;
    refresh();
    margen = newwin(36, 60, 0, 0);
    box(margen, 0 , 0);
    wrefresh(margen);
     keypad(margen, TRUE);

    WINDOW *margen2;
    refresh();
    margen2 = newwin(12, 25, 0, 60);
    box(margen2, 0 , 0);
    wrefresh(margen2);

    WINDOW *margen3;
    refresh();
    margen3 = newwin(12, 25, 12, 60);
    box(margen3, 0 , 0);
    wrefresh(margen3);

    WINDOW *margen4;
    refresh();
    margen4 = newwin(12, 25, 24, 60);
    box(margen4, 0 , 0);
    wrefresh(margen4);

    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
    init_pair(3, COLOR_WHITE, COLOR_GREEN);

    //Jugador Rojo
    char msg1[] = "Vida: ";
    mvprintw(3,65,"%s",msg1);
    char msg2[] = "Puntos: ";
    mvprintw(6,65,"%s",msg2);
    attron(COLOR_PAIR(1));
    char msg3[] = " Jugador Rojo ";
    mvprintw(8,67,"%s",msg3);
    attroff(COLOR_PAIR(1));

    //Tiempo
    attron(COLOR_PAIR(3));
    char msgt1[] = " T I E M P O ";
    mvprintw(16,66,"%s",msgt1);
    attroff(COLOR_PAIR(3));
    char msgt2[] = "00:00";
    mvprintw(18,70,"%s",msgt2);

    //Jugador Azul
    attron(COLOR_PAIR(2));
    char msg11[] = " Jugador Azul ";
    mvprintw(27,67,"%s",msg11);
    attroff(COLOR_PAIR(2));
    char msg22[] = "Vida: ";
    mvprintw(29,65,"%s",msg22);
    char msg33[] = "Puntos: ";
    mvprintw(31,65,"%s",msg33);





    print_nave(margen,26, 33);

    getch();


    delwin(margen4);
    delwin(margen3);
    delwin(margen2);
    delwin(margen);
}

void pantalla_fin(){

    WINDOW *margen;
    refresh();
    margen = newwin(36, 85, 0, 0);
    wrefresh(margen);

    char logo1[] = "______ _             _       _     ___                        _ ";
    char logo2[] = "|  ___(_)           | |     | |   |_  |                      | |";
    char logo3[] = "| |_   _ _ __     __| | ___ | |     | |_   _  ___  __ _  ___ | |";
    char logo4[] = "|  _| | | '_ \\   / _` |/ _ \\| |     | | | | |/ _ \\/ _` |/ _ \\| |";
    char logo5[] = "| |   | | | | | | (_| |  __/| | /\\__/ / |_| |  __/ (_| | (_) |_|";
    char logo6[] = "\\_|   |_|_| |_|  \\__,_|\\___||_| \\____/ \\__,_|\\___|\\__, |\\___/(_)";
    char logo7[] = "                                                   __/ |        ";
    char logo8[] = "                                                  |___/         ";

    int xl = (COLS - strlen(logo4))/2;
    int yl1 = LINES/2 - 10;
    int yl2 = LINES/2 - 9;
    int yl3 = LINES/2 - 8;
    int yl4 = LINES/2 - 7;
    int yl5 = LINES/2 - 6;
    int yl6 = LINES/2 - 5;
    int yl7 = LINES/2 - 4;
    int yl8 = LINES/2 - 3;

    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    attron(COLOR_PAIR(1));
    mvprintw(yl1,xl,"%s",logo1);
    mvprintw(yl2,xl,"%s",logo2);
    mvprintw(yl3,xl,"%s",logo3);
    mvprintw(yl4,xl,"%s",logo4);
    mvprintw(yl5,xl,"%s",logo5);
    mvprintw(yl6,xl,"%s",logo6);
    mvprintw(yl7,xl,"%s",logo7);
    mvprintw(yl8,xl,"%s",logo8);
    attroff(COLOR_PAIR(1));

    refresh();

    char msg1[] = "Ganador: ";
    char msg2[] = "Puntaje: ";
    char msg3[] = "Tiempo: ";
    char msg4[] = "Presiona INTRO para continuar";

    int x1 = (COLS - strlen(msg1))/2;
    int y1 = LINES/2 - 1;
    int x2 = (COLS - strlen(msg2))/2;
    int y2 = LINES/2 + 2;
    int x3 = (COLS - strlen(msg3))/2;
    int y3 = LINES/2 + 5;
    int x4 = (COLS - strlen(msg4))/2;
    int y4 = LINES/2 + 12;

    mvprintw(y1,x1,"%s",msg1);
    mvprintw(y2,x2,"%s",msg2);
    mvprintw(y3,x3,"%s",msg3);

    mvprintw(y4,x4,"%s",msg4);
   // mvprintw(y4,x4,"%s",msg3);
   // mvprintw(y5,x5,"%s",msg4);



    WINDOW *margen2;
    int x_m2 = x4 - 2;
    int y_m2 = y4 - 2;
    margen2 = newwin(5, 33, y_m2, x_m2);
    box(margen2, 0 , 0);
    wrefresh(margen2);

    getch();

    delwin(margen);
    delwin(margen2);
}

