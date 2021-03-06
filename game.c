#include <ncurses.h>
#include <pthread.h>
#include <time.h>

/*****STRUCTS*****/

typedef struct {
    int id;
    int x;
    int y;
    int x2;
    int y2;
    int tipo;
    int estado;
    int rango;
    int direccion;
    int direccion2;
} enemigo;

typedef struct {
    int x;
    int y;
    bool estado;
} disparo;

/*****DEFINICIONES*****/

void *print_nave(void *win);
void print_fila(int x, int y);
void subtract_time (struct timespec * __restrict later, struct timespec * __restrict former);
void start_game(WINDOW *w1, WINDOW *w2, WINDOW *w4);
void actualizar_disparos();
void actualizar_filas();
void check_colision();
void pre_region_critica();
void post_region_critica();

/*****GLOBALES*****/

int x = 26;
int y = 33;
disparo d;
struct timespec start;
struct timespec now;

enemigo fila[25];
int dir = 1;
int ind = 0;
int n = 0;

extern int jugador;

extern int *s_activo1;
extern int *s_activo2;
extern int *s_color1;
extern int *s_color2;
extern int *s_x1;
extern int *s_x2;
extern int *s_dx1;
extern int *s_dy1;
extern int *s_ds1;
extern int *s_dx2;
extern int *s_dy2;
extern int *s_ds2;
extern int *s_v1;
extern int *s_v2;
extern int *s_p1;
extern int *s_p2;
extern enemigo *s_invasores;

/*****FUNCIONES*****/

void start_game(WINDOW* w1, WINDOW* w2, WINDOW* w4){
    clock_gettime(CLOCK_MONOTONIC, &start);
    nodelay (stdscr, TRUE);

    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = 1000000;

    int paused = 0;
    struct timespec prev = start;

    print_fila(12,13);
    print_fila(12,15);
    //print_fila(12,16);
    print_fila(12,19);
    print_fila(12,21);

    int puntos1;
    int vidas1;

    int puntos2;
    int vidas2;

    while(1) {
        pre_region_critica();
        if(jugador == 1){
            vidas1 = *s_v1;
            puntos1 = *s_p1;
            vidas2 = *s_v2;
            puntos2 = *s_p2;
        }else{
            vidas1 = *s_v2;
            puntos1 = *s_p2;
            vidas2 = *s_v1;
            puntos2 = *s_p1;
        }
        post_region_critica();


        //Jugador 1
        char msg22[] = "Vida: ";
        mvwprintw(w4,7,5,"%s%d",msg22, vidas1);
        char msg33[] = "Puntos: ";
        mvwprintw(w4,5,5,"%s%d%s",msg33, puntos1,"  ");
        wrefresh(w4);


        //Jugador 2
        char msg1[] = "Vida: ";
        mvwprintw(w2,3,5,"%s%d",msg1, vidas2);
        char msg2[] = "Puntos: ";
        mvwprintw(w2,6,5,"%s%d%s",msg2, puntos2,"   ");
        wrefresh(w2);

        if (0 == paused) {
            subtract_time (&now, &start);

            if (now.tv_sec != prev.tv_sec) {
                mvprintw (18,70, "%02d:%02d", now.tv_sec / 60 , now.tv_sec % 60);
                refresh();
                prev = now;
            }
        }

        actualizar_disparos();

        pthread_t thread;
        pthread_create(&thread, NULL, print_nave, (void *) w1);
        pthread_join(thread, NULL);

        actualizar_filas();

        clock_nanosleep (CLOCK_MONOTONIC, 0, &delay, NULL);

        if(vidas1 == 0 || vidas2 == 0){
            paused = 1;
            break;
        }
    }

    pre_region_critica();
    post_region_critica();
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

    wclear(win);
    char nave[] = "<====+====>";
    mvwprintw(win,y,x,"%s",nave);
    wrefresh(win);

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
            if(x > 4){
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

    pre_region_critica();

    if(jugador == 1){
        *s_x1 = x;
        mvprintw(3,*s_x2,"%s",nave);
    }else{
        *s_x2 = x;
        mvprintw(3,*s_x1,"%s",nave);
    }

    post_region_critica();

    refresh();

    pthread_exit(NULL);

    return NULL;
}

void actualizar_disparos() {

    if(d.estado == 1 && d.y > 0){
        mvprintw(d.y, d.x + 5, "|");
        refresh();
    }

    struct timespec reqtime;
    reqtime.tv_sec = 0;
    reqtime.tv_nsec = 70000000;
    nanosleep(&reqtime, NULL);

    pre_region_critica();
    if(jugador == 1){
        *s_dx1 = d.x;
        *s_dy1 = 3 + 33 - d.y;
        *s_ds1 = d.estado;

        if(*s_ds2 == 1)
            mvprintw(*s_dy2, *s_dx2 + 5, "|");
    }else {
        *s_dx2 = d.x;
        *s_dy2 = 3 + 33 - d.y;
        *s_ds2 = d.estado;

        if(*s_ds1 == 1)
            mvprintw(*s_dy1, *s_dx1 + 5, "|");
    }
    post_region_critica();

    if(d.estado == 1 && d.y > 0){

        pre_region_critica();
        if(jugador == 1){
            if(d.x >= (*s_x2-5) && d.x < (*s_x2+5) && d.y == 3){
                *s_v2 -= 1;
                *s_p2 /= 2;

                d.estado = 0;
            }

        }else{
            if(d.x >= (*s_x1-5) && d.x < (*s_x1+5) && d.y == 3){
                *s_v1 -= 1;
                *s_p1 /= 2;
                d.estado = 0;
            }
        }
        post_region_critica();
        d.y--;

    }else {
        d.estado = 0;
    }

    check_colision();
}

void print_fila(int x, int y) {
    int n_x = x;

    pre_region_critica();
    for(int i = 0; i < 5; i++){

        int j;
        switch(y){
            case 13:
                j = 21;
                break;
            case 15:
                j = 19;
                break;
            case 19:
                j = 15;
                break;
            case 21:
                j = 13;
                break;
        }

        enemigo e;
        e.estado = 1;
        e.tipo = 0;
        e.x = n_x;
        e.x2 = n_x;
        e.y = j;
        e.y2 = y;
        e.rango = 5;
        e.direccion = dir;
        e.direccion2 = dir;
        e.id = ind;

        if(i == 2){
            e.tipo = 1;
            e.rango = 6;
            n_x += 8;
        }else{
            n_x += 8;
        }

        s_invasores[n] = e;
        n++;
        ind++;
    }

    dir *= -1;

    post_region_critica();
}

void actualizar_filas() {

    pre_region_critica();

    if(jugador == 1) {
        for(int i=0; i<20; i++){
            enemigo e = s_invasores[i];

            if(i%5 == 0)
                mvprintw(e.y2,0,"%s","                                                            ");

            if(e.estado == 0){
                continue;
            }else {
                if(e.tipo == 0)
                    mvprintw(e.y2,e.x2,"%s","\\-.-/");
                else
                    mvprintw(e.y2,e.x2,"%s","(/-\\)");

                if(e.direccion2 == -1)
                    e.x2--;
                else
                    e.x2++;

                if(e.x2 == 0 || e.x2 == 53)
                    e.direccion2 *= -1;

                s_invasores[i] = e;
            }
        }

    }else{
        for(int i=0; i<20; i++){
            enemigo e = s_invasores[i];

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

                s_invasores[i] = e;

            }
        }
    }

    post_region_critica();

    refresh();
}

void check_colision(){

    pre_region_critica();
    if(jugador == 1){

        for(int i=0; i<20; i++){
            enemigo e = s_invasores[i];
            int j;
            switch(e.y){
                case 13:
                    j = 21;
                    break;
                case 15:
                    j = 19;
                    break;
                case 19:
                    j = 15;
                    break;
                case 21:
                    j = 13;
                    break;
            }

            if(e.estado){
                if( d.x <= e.x && d.x > (e.x-e.rango) && d.y == j ){
                    d.estado = 0;
                    d.y = 0;
                    e.estado = 0;

                    s_invasores[i] = e;

                    if(e.tipo == 1){
                        if(jugador == 1)
                            *s_p1 += 15;
                        else
                            *s_p2 += 15;
                    }else{
                        if(jugador == 1)
                            *s_p1 += 10;
                        else
                            *s_p2 += 10;
                    }

                    break;
                }
            }else{
                continue;
            }

        }

    }else {
        for(int i=0; i<20; i++){
            enemigo e = s_invasores[i];

            if(e.estado){
                if( d.x <= e.x && d.x > (e.x - e.rango) && d.y == e.y ){
                    d.estado = 0;
                    d.y = 0;
                    e.estado = 0;

                    s_invasores[i] = e;

                    if(e.tipo == 1){
                        if(jugador == 1)
                            *s_p1 += 15;
                        else
                            *s_p2 += 15;
                    }else{
                        if(jugador == 1)
                            *s_p1 += 10;
                        else
                            *s_p2 += 10;
                    }

                    break;
                }
            }else{
                continue;
            }
        }

    }
    post_region_critica();
}

