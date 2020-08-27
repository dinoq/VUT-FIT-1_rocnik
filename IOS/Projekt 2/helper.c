#include "hlava.h"

int helper(int argv[])
{

	int rid=argv[0];
	int art=argv[1];
	int bus_capacity=argv[2];


	srand((unsigned) time(NULL));
	int i = 0;

	int args[2]={0, bus_capacity};
	//Cyklus, ktery se bude provadet, dokud nebudou vytvoreni vsichni rideri
	while(i<rid){
		rand_int = rand()%(art+1);
		//Vytvori proces rider
		int pid=fork();
		if(pid==0){
			rider(args);
		}else{
			//uspi proces na nahodnou dobu v zadanem rozsahu
			usleep(rand_int*1000);
			i++;

		}
	}

	exit(0);
}

