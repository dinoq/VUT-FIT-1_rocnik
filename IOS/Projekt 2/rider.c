#include "hlava.h"

int riderID;
int bus_capacity;

void writeToFileRider();

void start();
void enter();
void boarding();
void finish();

int rider(int argv[])
{
	bus_capacity = argv[1];

	//zaridi start ridera
	start();
	//zaridi vstup ridera na zastavku
	enter();
	//zaridi nastup ridera do busu
	boarding();
	//zaridi ukonceni procesu ridera
	finish();
	
	exit(0);
}


//Funkce zapise do souboru informaci podle predaneho stavu "state", pripadne i cisla "num"
void writeToFileRider(char* state, int num){
	sem_wait(counter_sem);
	
    shmem[SHARED_COUNTER]++;

    file = fopen("proj2.out", "a");
    if(file == NULL){
    	fprintf(stderr, "Chyba při otevírání souboru v rider.c\n");
    }else{
	    //Vytiskne do souboru...
	    if(num != -1){
			fprintf(file, "%d: RID %d: %s: %d\n", *shmem, riderID, state, num);
	    }else{
	    	fprintf(file, "%d: RID %d: %s\n", *shmem, riderID, state);
	    }
	    //Uzavre soubor
	    fclose(file);
	    file=NULL;
	}

	sem_post(counter_sem);

	return;
}

void start(){

	sem_wait(create_rid_sem);
	shmem[RID_ID]++;
	riderID = shmem[RID_ID];
	//Vytiskne start...
    writeToFileRider("start", -1);
	sem_post(create_rid_sem);

	return;
}
void enter(){
	sem_wait(bus_stop_sem);
    shmem[POCET_RID_ZASTAVKA]++;

	//Vytiskne enter...
    writeToFileRider("enter", shmem[POCET_RID_ZASTAVKA]);

	sem_post(bus_stop_sem);

	return;
}
void boarding(){

	sem_wait(bus_sem);
	sem_wait(counter_sem);
	//dekrementuje počet riderů, kteří ještě mají nastoupit	
	(shmem[POCET_RID])--;
	//dektementuje počet riderů, čekajících na zastávce
	shmem[POCET_RID_ZASTAVKA]--;
	//inkrementuje počet riderů v autobuse
	shmem[POCET_RID_BUS]++;
	shmem[POCET_RID_BUS_DEC]++;
	
	sem_post(counter_sem);

	//Vytiskne boarding...
	writeToFileRider("boarding", -1);

	//Pokud je jeste nekdo na zastavce a neni bus plny, tak uvolni semafor pro nastup dalsimu riderovi
	if((shmem[POCET_RID_ZASTAVKA]>0) && (shmem[POCET_RID_BUS] < bus_capacity)){
		sem_post(bus_sem);		
	}else{
		//Jinak otevře busu semafor (tzn. nastal konec nastupovani rideru)
		sem_post(end_boarding_sem);
		sem_post(depart_sem);
	}

	return;
}
void finish(){
	sem_wait(bus_finish_rid_sem1);
	if(--shmem[POCET_RID_BUS_DEC]>0){
		//Pokud je promenna - ktera se dekrementuje (a na zacatku mela hodnotu poctu rideru v buse) - vetsi nez 0, pust dalsiho ridera
		sem_post(bus_finish_rid_sem1);
	}


	sem_wait(bus_finish_rid_sem2);

	if(--shmem[RID_FIN_DEC]>0){
		//Pokud je promenna - ktera se dekrementuje (a na zacatku mela hodnotu poctu rideru, kteri maji vypsat finish) - vetsi nez 0, pust dalsiho ridera
		sem_post(bus_finish_rid_sem2);
	}else{
		//Jinak otevri busu semafor
		sem_post(konec_finish_rideru_sem);
	}

	//Vypise finish
	writeToFileRider("finish", -1);


	if(shmem[RID_FIN_DEC]==0){
		//Pokud je dokonce finish rideu, tak uvolni busu semafor
		sem_post(can_finish_rider_sem);
	}

	return;
}
