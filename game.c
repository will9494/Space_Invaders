
#include <ncurses.h>
#include <string.h>

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

typedef struct {
    int x;
    int y;
    int tipo;
    int estado;
    int rango;

} enemigo;

void print_nave(WINDOW *margen, int x, int y);
void print_fila(int x, int y);
void *print_beam();

void print_fila(int x, int y){
    enemigo fila[5];
    int n_x = x;

    for(int i = 0; i < 5; i++){
        enemigo e = fila[i];
        e.estado = 1;
        e.tipo = 0;
        e.x = n_x;
        e.y = y;
        e.rango = 5;

        if(i == 2){
            e.tipo = 1;
            e.rango = 6;
            n_x += 9;
        }else{
            n_x += 8;
        }

        fila[i] = e;
    }


    for(int j = 0; j < 5; j++){
        enemigo e = fila[j];


        if(e.tipo == 0){
            mvprintw(y,e.x,"%s","\\-.-/");
        }else{
            mvprintw(y,e.x,"%s","(/-.-\\)");
        }
        refresh();
    }
}


void print_nave(WINDOW *margen, int x, int y){
    char nave[] = "<--------->";

    mvprintw(y,x,"%s",nave);
    refresh();

    char buff[] = " ";
    int xx = x;

    //print_fila(12,12);
    //print_fila(10,14);
    //print_fila(12,16);
    //print_fila(10,18);
    //print_fila(12,20);

    while(1) {
        int c = wgetch(margen);
        switch(c){
            case KEY_RIGHT:{

                if(x < 48){
                    x++;
                    xx = x - 1;
                }
                break;
            }
            case KEY_LEFT:{

                if(x > 1){
                    x--;
                    xx = x + 11;
                }
                break;
            }
            case 32:{
                pthread_t thread;

                pthread_create(&thread, NULL, print_beam, NULL);

                wrefresh(margen);
                //print_beam(x, y - 1);
            }
            default: {
                break;
            }
        }

        mvprintw(y,x,"%s",nave);
        mvprintw(y,xx,"%s",buff);
        refresh();

        if(c == 10)
            break;
    }

}

void *print_beam() {
    int x = 26;
    int y = 32;

    while(1) {
        mvprintw(y, x + 6, "o");
        refresh();

        usleep(35000);

        mvprintw(y, x + 6, " ");
        refresh();

        y--;

        if(y == 0)
            break;
    }

    pthread_exit(NULL);

    return NULL;

}
