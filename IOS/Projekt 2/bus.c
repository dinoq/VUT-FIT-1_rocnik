#include "hlava.h"

int numberOfRiders;


void writeToFileBus();


int bus(int abt_arg)
{
	int abt = abt_arg;

	numberOfRiders = shmem[POCET_RID];

	//Inicializace generatoru nahody
	srand((unsigned) time(NULL));

	//Vypise start...
	writeToFileBus("start", -2);

	//Cyklus simulující opakující se jízdu
	while(numberOfRiders > 0){

		sem_wait(bus_stop_sem);
		
		//Vynuluje aktualni pocet rideru v autobuse
		shmem[POCET_RID_BUS] = 0;
		shmem[POCET_RID_BUS_DEC] = 0;


		//Vypise arrival...
		writeToFileBus("arrival", -1);

		//Zacne nakladat ridery
		boardRiders();
		
		//Pokud je počet rideru na zastavce vetsi nez 0, tak pocka, nez se nalozi (az do kapacity autobusu)
		if(shmem[POCET_RID_ZASTAVKA] > 0){
			sem_wait(depart_sem);
		}
		//Vypise depart...
		writeToFileBus("depart", -1);

		sem_post(bus_stop_sem);


		//následuje simulace jízdy
		numberOfRiders = shmem[POCET_RID];
		rand_int = rand()%(abt+1);
		usleep(rand_int*1000);

		//Vypise end...
		writeToFileBus("end", -1);

		shmem[RID_FIN] = shmem[POCET_RID_BUS];
		shmem[RID_FIN_DEC] = shmem[POCET_RID_BUS];

		//Zaridi aby rideri (kteri jeli minulou jizdu) vypsali finish
		finishRiders();
	}
	//Vypise finish...
	writeToFileBus("finish", -1);


	exit(0);
}

//Funkce zapise do souboru informaci podle predaneho stavu "state", pripadne i cisla "num"
void writeToFileBus(char* state, int num){

	sem_wait(counter_sem);

	
    shmem[SHARED_COUNTER]++;
    if(num != -2){
    	//pokud nebyla funkce zavolana s parametrem num=-2, vytvori se soubor, pripadne pokud existuje, tak se bude k nemu pripisovat
    	file = fopen("proj2.out", "a");
    }else{
    	//pokud byla funkce zavolana s parametrem num=-2, vytvori se soubor, pripadne se prepise, pokud existuje
    	file = fopen("proj2.out", "w");
    }
    
    if(file == NULL){
    	fprintf(stderr, "Chyba při otevírání souboru v bus.c\n");
    }else{

	    //Vytiskne do souboru...
	    if((num != -1) & (num != -2)){
	    	fprintf(file, "%d: BUS: %s: %d\n", *shmem, state, num);		
	    }else{
	    	fprintf(file, "%d: BUS: %s\n", *shmem, state);
	    }
	    
	    //Uzavre soubor
	    fclose(file);
	    file=NULL;

	}

	sem_post(counter_sem);
	return;
}

//Funkce zaridi nalozeni rideru (pokud nejaci cekaji na zastavce)
void boardRiders(){
	//Pokud jsou na zastávce ještě nějací cestující
	if(shmem[POCET_RID_ZASTAVKA] > 0){
		//Vypise start boarding...
		writeToFileBus("start boarding", shmem[POCET_RID_ZASTAVKA]);

		sem_post(bus_sem);

		//Cekej na nastoupeni vsech rideru (kteri mohou jet - at do kapacity busu)
		sem_wait(end_boarding_sem);

		//Pokud ted nekdo nastoupil, tak...
		if(shmem[POCET_RID_BUS] > 0){
			//Uvolni 1. semafor ve finish pro ridery co ted pojedou
			sem_post(bus_finish_rid_sem1);
		}

		//Vypise end boarding...
		writeToFileBus("end boarding", shmem[POCET_RID_ZASTAVKA]);

	}
	return;
}

//Funkce zaridi aby rideri (kteri jeli minulou jizdu) vypsali finish
void finishRiders(){

	//Pokud minulou jizdu nekdo jel, tak...
	if(shmem[RID_FIN] > 0){
		//Ǒtevri 2. semafor ve finish pro ridery co dojeli
		sem_post(bus_finish_rid_sem2);

		//Pokud minulou jizdu někdo jel, tak cekej na finish vsech rideru z predchozi jizdy
		sem_wait(konec_finish_rideru_sem);
		sem_wait(can_finish_rider_sem);
	}
	return;
}