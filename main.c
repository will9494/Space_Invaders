
#include <ncurses.h>			/* ncurses.h includes stdio.h */
#include <string.h>

#define WIDTH 30
#define HEIGHT 10

int startx = 0;
int starty = 0;

WINDOW *pantalla;

void pantalla_bienvenida(WINDOW *window);

int main() {
    int presionado = 0;

    initscr();
    clear();
    noecho();
    cbreak();	/* Line buffering disabled. pass on everything */
    startx = (80 - WIDTH) / 2;
    starty = (24 - HEIGHT) / 2;

    pantalla = newwin(HEIGHT, WIDTH, starty, startx);
    keypad(pantalla, TRUE);

    while(1){
        pantalla_bienvenida(pantalla);

        presionado = wgetch(pantalla);

        if(presionado != 0)
            break;
    }



    return 0;
}



void pantalla_bienvenida(WINDOW *window){
    char mesg[]="Just a string";		/* message to be appeared on the screen */
    int row,col;                        /* to store the number of rows and the number of colums of the screen */

    initscr();                          /* start the curses mode */
    getmaxyx(stdscr,row,col);           /* get the number of rows and columns */
    mvprintw(window,row/2,(col-strlen(mesg))/2,"%s",mesg);
                                       /* print the message at the center of the screen */
    mvprintw(window,row-2,0,"This screen has %d rows and %d columns\n",row,col);
    printw("Try resizing your window(if possible) and then run this program again");
    refresh();
    getch();

    endwin();
}
