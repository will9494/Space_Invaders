#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

/*****STRUCTS*****/

typedef union {
    int val;
    struct semid_ds *buf;
    ushort *array;
}  semun;

typedef struct {
    int x;
    int y;
    int tipo;
    int estado;
    int rango;
    int direccion;
} enemigo;

/*****DEFINICIONES*****/

void die(char *s);
void start_share();
void write_share(int n_dato, int dato);
void free_share();

void inicializar_semaforo();
void eliminar_semaforo();
void up_semaforo();
void down_semaforo();
int get_semaforo();
void pre_region_critica();
void post_region_critica();

/*****GLOBALES*****/

int shmid;
int shmid2;

key_t key1 = 5678;
key_t key2 = 7856;
key_t key3 = 5786;

extern int jugador;

int *s_tiempo;
int *s_activo1;
int *s_activo2;
int *s_color1;
int *s_color2;
int *s_x1;
int *s_x2;
int *s_dx1;
int *s_dy1;
int *s_ds1;
int *s_dx2;
int *s_dy2;
int *s_ds2;
int *s_p1pe;
int *s_p2pe;
int *s_turno;
int *s_v1;
int *s_v2;
int *s_p1;
int *s_p2;
enemigo *s_invasores;

/*****MEMORIA COMPARTIDA(sizeof(int)*19) + (sizeof(int) * 6 * 20)*****/

void die(char *s) {
    perror(s);
    exit(1);
}

void start_share() {
    shmid = shmget(key1, (sizeof(int)*4), IPC_CREAT | 0666);
    //STATUS
    s_activo1 = (int *)shmat(shmid, NULL, 0);
    s_activo2 = s_activo1 + (sizeof(int) * 1);
    s_color1 = s_activo1 + (sizeof(int) * 2);
    s_color2 = s_activo1 + (sizeof(int) * 3);
    //BARRAS
    s_x1 = s_activo1 + (sizeof(int)) * 4;
    s_x2 = s_activo1 + (sizeof(int)) * 5;
    s_dx1 = s_activo1 + (sizeof(int)) * 6;
    //DISPAROS
    s_dy1 = s_activo1 + (sizeof(int)) * 7;
    s_ds1 = s_activo1 + (sizeof(int)) * 8;
    s_dx2 = s_activo1 + (sizeof(int)) * 9;
    s_dy2 = s_activo1 + (sizeof(int)) * 10;
    s_ds2 = s_activo1 + (sizeof(int)) * 11;
    //DEKKER
    s_p1pe = s_activo1 + (sizeof(int)) * 12;
    s_p2pe = s_activo1 + (sizeof(int)) * 13;
    s_turno = s_activo1 + (sizeof(int)) * 14;
    //DETALLES
    s_v1 = s_activo1 + (sizeof(int)) * 15;
    s_v2 = s_activo1 + (sizeof(int)) * 16;
    s_p1 = s_activo1 + (sizeof(int)) * 17;
    s_p2 = s_activo1 + (sizeof(int)) * 18;

    if(*s_activo1 == 0){
        *s_activo1 = -1;
        *s_activo2 = -1;
        *s_color1 = -1;
        *s_color2 = -1;
        *s_x1 = -1;
        *s_x2 = -1;
        *s_dx1 = -1;
        *s_dy1 = -1;
        *s_ds1 = -1;
        *s_dx2 = -1;
        *s_dy2 = -1;
        *s_ds2 = -1;
        *s_p1pe = 0;
        *s_p2pe = 0;
        *s_turno = 1;
        *s_v1 = 5;
        *s_v2 = 5;
    }

    //INVASORES
    shmid2 = shmget(key3, (sizeof(enemigo)*20), IPC_CREAT | 0666);
    s_invasores = (enemigo *)shmat(shmid2, NULL, 0);
}

void write_share(int n_dato, int dato) {
    int *shm;

    shmid = shmget(key1, (sizeof(int)*4), IPC_CREAT | 0666);
    shm = (int *)shmat(shmid, NULL, 0);

    shm += (sizeof(int) * n_dato);

    *shm = dato;

    shmdt(shm);
}

void free_share() {
    shmctl(shmid, IPC_RMID, NULL);
}

/*****SEMAFOROS*****/

void inicializar_semaforo() {
    int id_semaforo;
    id_semaforo = semget( key2, 1, 0666 | IPC_CREAT);

    semun sem;
    sem.val = 0;
    semctl(id_semaforo, 0, SETVAL, &sem);
}

void eliminar_semaforo(){
    int id_semaforo;
    id_semaforo = semget( key2, 1, 0666 | IPC_CREAT);
    semctl(id_semaforo, 0, IPC_RMID);

}

void up_semaforo() {
    int id_semaforo;
    id_semaforo = semget( key2, 1, 0);

    struct sembuf accion;
    accion.sem_num = 0;
    accion.sem_op = 1;
    accion.sem_flg = 0;

    semop ( id_semaforo, &accion, 1 );
}

void down_semaforo() {
    int id_semaforo;
    id_semaforo = semget( key2, 1, 0);

    struct sembuf accion;
    accion.sem_num = 0;
    accion.sem_op = -1;
    accion.sem_flg = 0;

    semop( id_semaforo, &accion, 1);
}

int get_semaforo() {
    int id_semaforo;
    int valorActual;
    id_semaforo = semget( key2, 1, 0);
    valorActual = semctl( id_semaforo, 0, GETVAL );

    return valorActual;
}

/*****DEKKER*****/

void pre_region_critica(){
    if(jugador == 1){
        *s_p1pe = 1;
        while(*s_p2pe){
            if(*s_turno == 2){
                *s_p1pe = 0;
                while(*s_turno == 2){}
                *s_p1pe = 1;
            }
        }

    }else{
        *s_p2pe = 1;
        while(*s_p1pe){
            if(*s_turno == 1){
                *s_p2pe = 0;
                while(*s_turno == 1){}
                *s_p2pe = 1;
            }
        }
    }
}

void post_region_critica(){

    if(jugador == 1){
        *s_turno = 2;
        *s_p1pe = 0;
    }else{
        *s_turno = 1;
        *s_p2pe = 0;
    }

}
