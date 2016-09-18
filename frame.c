#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*****DEFINICIONES*****/

void print_menu(WINDOW *menu_win, int highlight);
void pantalla_bienvenida();
void pantalla_menu();
void pantalla_espera();
void pantalla_juego();
void pantalla_fin();
void start_game(WINDOW *w1, WINDOW *w2, WINDOW *w3, WINDOW *w4);

void write_share(int n_dato, int dato);

/*****GLOBALES*****/

int jugador = 0;

char *choices[] = {
    "       JUGADOR ROJO       ",
    "       JUGADOR AZUL       ",
    "           SALIR          ",
};
int n_choices = sizeof(choices) / sizeof(char *);
int choice = 0;

extern struct timespec now;

extern int *s_color1;
extern int *s_color2;
extern int *s_v1;
extern int *s_v2;
extern int *s_p1;
extern int *s_p2;

/*****FUNCIONES*****/

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

    while(1) {
        char c = getch();

        if(c == 10)
            break;
    }

    delwin(margen2);
    delwin(margen);
}

void pantalla_menu(){
    WINDOW *menu_win;
    int highlight = 1;
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

        if(choice != 0)	{
            if(*s_color1 == -1){
                *s_color1 = choice;
                jugador = 1;
            }else if(*s_color1 != choice){
                *s_color2 = choice;
                jugador = 2;
            }else{
                if(choice == 1)
                    choice = 2;
                else
                    choice = 1;
                *s_color2 = choice;
                jugador = 2;
            }
            break;
        }
    }

    if(choice == 3){
        endwin();
        exit(0);
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
    clear();
    WINDOW *margen;
    refresh();
    margen = newwin(36, 85, 0, 0);
    wrefresh(margen);

    char msg[] = "Esperando al otro jugador...";
    int x = (COLS - strlen(msg))/2;
    int y = LINES/2;

    WINDOW *margen2;
    int x_m2 = x - 2;
    int y_m2 = y - 2;
    margen2 = newwin(5, 33, y_m2, x_m2);
    box(margen2, 0 , 0);
    mvwprintw(margen2,2,2,"%s",msg);
    wrefresh(margen2);

    delwin(margen2);
}

void pantalla_juego(){
    WINDOW *margen;
    margen = newwin(36, 60, 0, 0);
    //box(margen, 0 , 0);
    keypad(margen, TRUE);

    WINDOW *margen2;
    margen2 = newwin(12, 25, 0, 60);
    box(margen2, 0 , 0);
    //wrefresh(margen2);

    WINDOW *margen3;
    margen3 = newwin(12, 25, 12, 60);
    box(margen3, 0 , 0);
    //wrefresh(margen3);

    WINDOW *margen4;
    margen4 = newwin(12, 25, 24, 60);
    box(margen4, 0 , 0);
    //wrefresh(margen4);

    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
    init_pair(3, COLOR_WHITE, COLOR_GREEN);

    char nave[] = "<----+---->";
    mvwprintw(margen,33,26,nave);

    //Jugador 2
    char msg1[] = "Vida: ";
    mvwprintw(margen2,3,5,msg1);
    char msg2[] = "Puntos: ";
    mvwprintw(margen2,6,5,"%s",msg2);

    int ch;
    char *msg3;
    if(choice != 1){
        msg3 = " Jugador Rojo ";
        ch = 1;
    }else {
        msg3 = " Jugador Azul ";
        ch = 2;
    }
    wattron(margen2,COLOR_PAIR(ch));
    mvwprintw(margen2,8,7,"%s",msg3);
    wattroff(margen2,COLOR_PAIR(ch));
    refresh();

    //Tiempo
    wattron(margen3,COLOR_PAIR(3));
    char msgt1[] = " T I E M P O ";
    mvwprintw(margen3,4,6,"%s",msgt1);
    wattroff(margen3,COLOR_PAIR(3));
    char msgt2[] = "00:00";
    mvwprintw(margen3,6,10,"%s",msgt2);

    //Jugador 1
    wattron(margen4,COLOR_PAIR(choice));
    char *msg11;
    if(choice == 1)
        msg11 = " Jugador Rojo ";
    else
        msg11 = " Jugador Azul ";
    mvwprintw(margen4,3,7,"%s",msg11);
    wattroff(margen4,COLOR_PAIR(choice));
    char msg22[] = "Vida: ";
    mvwprintw(margen4,7,5,"%s",msg22);
    char msg33[] = "Puntos: ";
    mvwprintw(margen4,5,5,"%s",msg33);

    wrefresh(margen);
    wrefresh(margen2);
    wrefresh(margen3);
    wrefresh(margen4);

    start_game(margen, margen2,margen3,margen4);

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

    char *gsm1;
    char *gsm2;
    int gsm3;
    if(*s_p1 > *s_p2){
        gsm1 = "(Jugador 1)";
        gsm3 = *s_p1;

        if(*s_color1 == 1)
            gsm2 = "ROJO";
        else
            gsm2 = "AZUL";
    }else{
        gsm1 = "(Jugador 2)";
        gsm3 = *s_p2;

        if(*s_color2 == 1)
            gsm2 = "ROJO";
        else
            gsm2 = "AZUL";
    }

    mvprintw(y1,x1-5,"%s %s %s",msg1,gsm1,gsm2);
    mvprintw(y2,x2,"%s %d",msg2,gsm3);
    mvprintw(y3,x3,"%s %02d:%02d", msg3, now.tv_sec / 60 , now.tv_sec % 60);
    mvprintw(y4,x4,"%s",msg4);


    WINDOW *margen2;
    int x_m2 = x4 - 2;
    int y_m2 = y4 - 2;
    margen2 = newwin(5, 33, y_m2, x_m2);
    box(margen2, 0 , 0);
    wrefresh(margen2);

    while(1){
        int ch = getch();
        if(ch == 10)
            break;
    }
    delwin(margen);
    delwin(margen2);
}

