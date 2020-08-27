#include "hlava.h"

#define BUS "./bus"
#define HELPER "./helper"


	int capacity;
	int riders;
	int art;
	int abt;
	int pid_bus;
	int pid_helper;
	char *arg_ptr;



int main(int argc, char const *argv[])
{
	setbuf(stdout, NULL);

	if(zkontrolujArgumenty(argc, argv) == -1){
		//Chybne zadane argumenty
		exit(1); 
	}
	
	initMem();
	initSem();


	//Vytvori novy proces pro bus
	pid_bus=fork();
	

	if(pid_bus==0){
		//Zpusobi ze proces "prepne" do funkce busu	
		bus(abt);
	}

	
	int args2[3]={riders, art, capacity};
	//Vytvori proces pro pomocny proces, ktery bude vytvaret ridery
	pid_helper=fork();
	if(pid_helper==0){	
		//Zpusobi ze proces "prepne" do funkce pomocneho procesu, vytvarejiciho ridery
		helper(args2);
	}

	int child;
	//Program ceka na ukonceni procesu
	waitpid(pid_bus, &child, 0);
	waitpid(pid_helper, &child, 0);

	//Program odstrani semafory a sdilenou pamet
	deleteSem();
	cleanMem();

	return 0;
}

void initMem(){
	//Vytvori se klic
	key_t klic = ftok("main.c", KEY);
	//Vytvori se sdilena pamet
	if ((shmem_ID = shmget(klic, SHM_SIZE*sizeof(int), IPC_CREAT | 0666)) == -1)
    {
        fprintf(stderr, "Chyba při získávání sdílené paměti!\n");
        exit(1);
    }

    if ((shmem = shmat(shmem_ID, NULL, 0)) == NULL)
    {        
        fprintf(stderr, "Chyba při získávání sdílené paměti!\n");
        exit(1);
    }

    //inicializuje se sdilena pamet, ktera je implementovana jako pole
    shmem[SHARED_COUNTER]=0;
    shmem[POCET_RID]=riders;
    shmem[POCET_RID_ZASTAVKA]=0;
    shmem[POCET_RID_BUS]=0;
    shmem[RID_FIN]=0;
    shmem[RID_FIN_DEC]=0;
    shmem[POCET_RID_BUS_DEC]=0;
    shmem[RID_ID]=0;
}

void initSem(){	
	create_rid_sem = sem_open(CREATE_RID_SEM, O_CREAT | O_EXCL, 0666, UNLOCKED);
	counter_sem = sem_open(COUNTER_SEM, O_CREAT | O_EXCL, 0666, UNLOCKED);
	bus_stop_sem = sem_open(BUS_STOP_SEM, O_CREAT | O_EXCL, 0666, UNLOCKED);
	bus_sem = sem_open(BUS_SEM, O_CREAT | O_EXCL, 0666, LOCKED);
	depart_sem = sem_open(DEPART_SEM, O_CREAT | O_EXCL, 0666, LOCKED);
	end_boarding_sem = sem_open(END_BOARDING_SEM, O_CREAT | O_EXCL, 0666, LOCKED);
	bus_finish_rid_sem1 = sem_open(BUS_FINISH_RID_SEM1, O_CREAT | O_EXCL, 0666, LOCKED);
	bus_finish_rid_sem2 = sem_open(BUS_FINISH_RID_SEM2, O_CREAT | O_EXCL, 0666, LOCKED);
	konec_finish_rideru_sem = sem_open(KONEC_FINISH_RIDERU_SEM, O_CREAT | O_EXCL, 0666, LOCKED);
	can_finish_rider_sem = sem_open(CAN_FINISH_RIDER_SEM, O_CREAT | O_EXCL, 0666, LOCKED);
}

void deleteSem(){
	sem_close(create_rid_sem);
	sem_unlink(CREATE_RID_SEM);

	sem_close(counter_sem);
	sem_unlink(COUNTER_SEM);

	sem_close(bus_stop_sem);
	sem_unlink(BUS_STOP_SEM);

	sem_close(bus_sem);
	sem_unlink(BUS_SEM);

	sem_close(depart_sem);
	sem_unlink(DEPART_SEM);

	sem_close(end_boarding_sem);
	sem_unlink(END_BOARDING_SEM);

	sem_close(bus_finish_rid_sem1);
	sem_unlink(BUS_FINISH_RID_SEM1);

	sem_close(bus_finish_rid_sem2);
	sem_unlink(BUS_FINISH_RID_SEM2);

	sem_close(konec_finish_rideru_sem);
	sem_unlink(KONEC_FINISH_RIDERU_SEM);

	sem_close(can_finish_rider_sem);
	sem_unlink(CAN_FINISH_RIDER_SEM);
}

void cleanMem(){
	shmdt(&shmem_ID);
	shmctl(shmem_ID,IPC_RMID,NULL);
}

int zkontrolujArgumenty(int argc, char *argv[]){
	//Kontrola argumentů	
	if(argc==5){

		riders=strtol(argv[1], &arg_ptr, 10);
		if(arg_ptr[0]!='\0'){
			fprintf(stderr, "Error: spatne zadany 1. argument!\n");
			return -1;
		}
		capacity=strtol(argv[2], &arg_ptr, 10);
		if(arg_ptr[0]!='\0'){
			fprintf(stderr, "Error: spatne zadany 2. argument!\n");
			return -1;
		}
		art=strtol(argv[3], &arg_ptr, 10);
		if(arg_ptr[0]!='\0'){
			fprintf(stderr, "Error: spatne zadany 3. argument!\n");
			return -1;
		}
		abt=strtol(argv[4], &arg_ptr, 10);
		if(arg_ptr[0]!='\0'){
			fprintf(stderr, "Error: spatne zadany 4. argument!\n");
			return -1;
		}
		if((art<0) | (art>1000) | (abt<0) | (abt>1000) | (riders<=0) | (capacity<=0)){
			fprintf(stderr, "Error: argumenty jsou mimo povoleny rozsah!\n");
			return -1;
		}

	}else{		
		fprintf(stderr, "Error: malo argumentu!\n");
		return -1;
	}

	return 0;
}