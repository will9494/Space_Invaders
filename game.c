#include <ncurses.h>
#include <pthread.h>
#include <time.h>

/*****STRUCTS*****/

typedef struct {
    int x;
    int y;
    int tipo;
    int estado;
    int rango;
    int direccion;
} enemigo;

typedef struct {
    int x;
    int y;
    bool estado;
} disparo;

typedef struct {
    WINDOW *w;
    int x;
    int y;
} thread_data;

/*****DEFINICIONES*****/

void *print_nave(void *win);
void print_fila(int x, int y);
void subtract_time (struct timespec * __restrict later, struct timespec * __restrict former);
void start_game(WINDOW *w1, WINDOW *w2, WINDOW *w3, WINDOW *w4);
void actualizar_disparos();
void actualizar_filas();
void check_colision();

/*****GLOBALES*****/

int x = 26;
int y = 33;
disparo d;
struct timespec start;
struct timespec now;

enemigo fila[25];
int dir = 1;
int n = 0;

int puntos1;
int vidas1 = 5;

int puntos2;
int vidas2 = 5;


/*****FUNCIONES*****/


int pos_filas[5];

void start_game(WINDOW* w1, WINDOW* w2, WINDOW* w3, WINDOW* w4){
    clock_gettime(CLOCK_MONOTONIC, &start);
    nodelay (stdscr, TRUE);

    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = 1000000;

    int paused = 0;
    struct timespec prev = start;

    print_fila(12,12);
    print_fila(10,14);
    //print_fila(12,16);
    print_fila(10,18);
    print_fila(12,20);

    pos_filas[0] = 12;
    pos_filas[1] = 10;
    //pos_filas[2] = 12;
    pos_filas[3] = 10;
    pos_filas[4] = 12;

    while(1) {
        //Jugador 1
        char msg22[] = "Vida: ";
        mvwprintw(w4,5,5,"%s%d",msg22, vidas1);
        char msg33[] = "Puntos: ";
        mvwprintw(w4,7,5,"%s%d",msg33, puntos1);
        wrefresh(w4);

        //Jugador 2
        char msg1[] = "Vida: ";
        mvwprintw(w2,3,5,"%s%d",msg1, vidas2);
        char msg2[] = "Puntos: ";
        mvwprintw(w2,6,5,"%s%d",msg2, puntos2);
        wrefresh(w2);

        if (0 == paused) {
            subtract_time (&now, &start);

            if (now.tv_sec != prev.tv_sec) {
                mvprintw (18,70, "%02d:%02d", now.tv_sec / 60 , now.tv_sec % 60);
                refresh();
                prev = now;
            }
        }else {
            subtract_time (&start, &now);
        }

        actualizar_disparos();

        pthread_t thread;
        pthread_create(&thread, NULL, print_nave, (void *) w1);
        pthread_join(thread, NULL);

        actualizar_filas();

        clock_nanosleep (CLOCK_MONOTONIC, 0, &delay, NULL);
    }
}

void subtract_time (struct timespec * __restrict later, struct timespec * __restrict former) {
    clock_gettime(CLOCK_MONOTONIC, later);
    later->tv_sec -= former->tv_sec;

    if (later->tv_nsec < former->tv_nsec) {
        later->tv_sec--;
        later->tv_nsec = 1000000000 + later->tv_nsec - former->tv_nsec;
    }else {
        later->tv_nsec -= former->tv_nsec;
    }
}

void *print_nave(void *win){
    WINDOW *margen = (WINDOW *) win;
    int xx = x;

    char nave[] = "<====+====>";
    mvprintw(y,x,"%s",nave);
    refresh();

    nodelay(margen, TRUE);
    int c = wgetch(margen);
    switch(c){
        case KEY_RIGHT:
            if(x < 48){
                x++;
                x++;
                xx = x - 2;
            }
            break;
        case KEY_LEFT:
            if(x > 1){
                x--;
                x--;
                xx = x + 11;
            }
            break;
        case 32:
            if(d.estado == 0){
                d.x = x;
                d.y = y;
                d.estado = 1;
            }
            break;
        default:
            break;
    }

    mvprintw(y,x,"%s",nave);

    if(x != xx){
        mvprintw(y,xx,"%s","  ");
    }

    refresh();

    pthread_exit(NULL);

    return NULL;
}

void actualizar_disparos(){
    int delay = 40000;

    if(d.estado == 1 && d.y > 0){
        mvprintw(d.y, d.x + 5, "|");
        refresh();
    }

    usleep(delay);

    if(d.estado == 1 && d.y > 0){
        mvprintw(d.y, d.x + 5, " ");
        refresh();

        d.y--;
    }else{
        d.estado = 0;
    }

    check_colision();
}


void print_fila(int x, int y) {
    int n_x = x;

    for(int i = 0; i < 5; i++){
        enemigo e = fila[n];
        e.estado = 1;
        e.tipo = 0;
        e.x = n_x;
        e.y = y;
        e.rango = 5;
        e.direccion = dir;

        if(i == 2){
            e.tipo = 1;
            e.rango = 6;
            n_x += 8;
        }else{
            n_x += 8;
        }

        fila[n] = e;

        n++;
    }

    dir *= -1;
    n -= 5;

    for(int j = 0; j < 5; j++){
        enemigo e = fila[n];

        if(e.tipo == 0){
            mvprintw(y,e.x,"%s","\\-.-/");
        }else{
            mvprintw(y,e.x,"%s","(/--\\)");
        }
        refresh();

        n++;
    }

}

void actualizar_filas() {

    for(int i=0; i<20; i++){
        enemigo e = fila[i];

        if(i%5 == 0)
            mvprintw(e.y,0,"%s","                                                            ");

        if(e.estado == 0){
            continue;
        }else {
            if(e.tipo == 0)
                mvprintw(e.y,e.x,"%s","\\-.-/");
            else
                mvprintw(e.y,e.x,"%s","(/-\\)");

            if(e.direccion == -1)
                e.x--;
            else
                e.x++;

            if(e.x == 0 || e.x == 53)
                e.direccion *= -1;

            fila[i] = e;

        }
    }

    refresh();
}


void check_colision(){
    for(int i=0; i<20; i++){
        enemigo e = fila[i];

        if(e.estado){
            if( d.x <= e.x && d.x > (e.x-e.rango) && d.y == e.y ){
                d.estado = 0;
                d.y = 0;
                e.estado = 0;

                fila[i] = e;

                if(e.tipo == 1){
                    puntos1 += 15;
                }else{
                    puntos1 += 10;
                }

                break;
            }
        }else{
            continue;
        }
    }
}
