#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>   
#include <errno.h>

//Konstanty pro vytvoreni semaforu
#define CREATE_RID_SEM "/sem_1_xmarek69"
#define COUNTER_SEM "/sem_2_xmarek69"
#define BUS_STOP_SEM "/sem_3_xmarek69"
#define BUS_SEM "/sem_4_xmarek69"
#define DEPART_SEM "/sem_5_xmarek69"
#define END_BOARDING_SEM "/sem_6_xmarek69"
#define BUS_FINISH_RID_SEM1 "/sem_7_xmarek69"
#define BUS_FINISH_RID_SEM2 "/sem_8_xmarek69"
#define KONEC_FINISH_RIDERU_SEM "/sem_9_xmarek69"
#define CAN_FINISH_RIDER_SEM "/sem_10_xmarek69"

#define UNLOCKED 1
#define LOCKED 0
//Pouzivam pri definovani velikosti sdilene pameti
#define SHM_SIZE 8

#define KEY 20

//Následuji kostanty pro pristup do sdilene pameti
//Sdileny citac (pro cislo operace pri zapisu do souboru)
#define SHARED_COUNTER 0
//Celkovy pocet rideru co maji jet busem, na tomto indexu se sdilena pamet dekrementuje v procesu bus
#define POCET_RID 1
//Pocet rideru na zastavce
#define POCET_RID_ZASTAVKA 2
//Pocet rideru v buse
#define POCET_RID_BUS 3
//Pocet rideru co maji vypsat finish a skoncit
#define RID_FIN 4
//Pocet rideru co maji vypsat finish a skoncit, na tomto indexu se sdilena pamet dekrementuje
#define RID_FIN_DEC 5
//Pocet rideru v buse, na tomto indexu se sdilena pamet dekrementuje
#define POCET_RID_BUS_DEC 6
#define RID_ID 7

//Sdilena pamet
int shmem_ID;
int *shmem;

//Promenne pro semafory
sem_t *create_rid_sem;
sem_t *counter_sem;
sem_t *bus_stop_sem;
sem_t *bus_sem;
sem_t *depart_sem;
sem_t *end_boarding_sem;
sem_t *bus_finish_rid_sem1;
sem_t *bus_finish_rid_sem2;
sem_t *konec_finish_rideru_sem;
sem_t *can_finish_rider_sem;



FILE *file;
int rand_int; 


void initMem();
void initSem();
void cleanMem();
void deleteSem();
int rider();
int bus();
int helper();
void boardRiders();
void finishRiders();
int zkontrolujArgumenty();