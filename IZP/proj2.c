/*
 * Soubor:  proj2.c
 * Datum:   2017/11/19
 * Autor:   Petr Marek, xmarek69@stud.fit.vutbr.cz
 * Projekt: Iteracni vypocty, projekt c. 2 pro predmet IZP
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_UHEL 1.4 // Definuje konstantu pro maximalni moznou velikost uhlu
#define MIN_UHEL 0 // Definuje konstantu pro minimalni moznou velikost uhlu
#define VYSKA_MERAKU 1.5 // Definuje standardni vysku polohy mericiho pristroje
#define MAX_VYSKA 150 // Definuje konstantu pro maximalni vysku polohy mericiho pristroje
#define MIN_VYSKA 0 // Definuje konstantu pro minimalni vysku polohy mericiho pristroje
#define PRESNOST 0.0000000001 // Definuje konstantu pro pozadovanou presnost pri vypoctu tangens

double taylor_tan(double x, unsigned int n);
double cfrac_tan(double x, unsigned int n);
double porovnej_uhly(double x, int n, int m);
double vzdalenost_objektu(double vyska, double uhel);
double vyska_objektu(double vzdalenost, double uhel, double vyska_pristroje);
int iterace_presnost(double uhel);
void napoveda();

int main(int argc, char *argv[])
{
	char *znak;
	if (argc<2){// Pokud nejsou zadane argumenty programu
		printf("Musis zadat argument! Pro napovedu napis argument --help\n");
		return 0;
	}
	if (strcmp(argv[1], "--help")==0){
		napoveda(); // Vytiskne napovedu
		return 0;
	}
	if (strcmp(argv[1],"--tan")==0){ // Porovna uhly
		if(argc != 5){
			printf("Malo argumentu! Pro napovedu napis argument --help\n");
			return 0;
		}else{
			porovnej_uhly(strtod(argv[2],&znak),strtod(argv[3],&znak),strtod(argv[4],&znak));
		}
	}

	double alfa, vzdalenost, vyska, beta, vyskaobj;
	vyska = VYSKA_MERAKU;

/* Cyklus vypocita vzdalenost, pripadne pri zadanem uhlu "beta" i vysku mereneho objektu */
	for(int i = 0; i < argc; i++){
		if (strcmp(argv[i],"-c")==0){
			if((strtod(argv[i+1],&znak)<MAX_VYSKA)&&(strtod(argv[i+1],&znak)>MIN_VYSKA))
				vyska = strtod(argv[i+1],&znak); // Pokud je nastaveny argument programu "X" a je v rozsahu 0 - 100, nastavi se podle tohoto argumentu vyska meraku. V opacnem pripade zustane 1,5 m (jak byla inicializovana)
        }
		if (strcmp(argv[i],"-m")==0){// Vypocita vzdalenost a vytiskne ji
			alfa = strtod(argv[i+1],&znak);
			if((alfa < MIN_UHEL)||(alfa > MAX_UHEL)){
                printf("Spatne zadany argument! Uhel musi byt v rozsahu 0 - 1,4!\n");
                return 0;
			}
			vzdalenost = vzdalenost_objektu(vyska, alfa);
			if(vzdalenost >= 0.)
                printf("%.10e\n", vzdalenost);
            else
                printf("Spatne zadany uhel alfa\n");
			if(argv[i+2] != NULL){// Pokud je nastaveny argument udavajici uhel "beta", nastavi se promenna "beta"
					beta = strtod(argv[i+2],&znak);
					if((beta < MIN_UHEL) || (beta >= MAX_UHEL)){ // Pokud je uhel "beta" mimo povoleny rozsah, vytiskne chybu a ukonci program. V opacnem pripade vypocita vysku objektu a vytiskne ji.
						printf("Spatne zadany uhel beta");
						return 0;
					}else{
						vyskaobj = vyska_objektu(vzdalenost, beta, vyska);
						printf("%.10e\n", vyskaobj);
					}
			}
		}
	}
    return 0;
}

/* Funkce slouzi pro vypocet funkce tangens uhlu "x" pomoci taylorova polynomu */
double taylor_tan(double x, unsigned int n){
	double vysledek, mocnina, x2;
	double citatel[12]={1, 2, 17, 62, 1382, 21844, 929569, 6404582,
	443861162, 18888466084, 113927491862, 58870668456604};
	double jmenovatel[12]={3, 15, 315, 2835, 155925, 6081075,
	638512875, 10854718875, 1856156927625, 194896477400625, 49308808782358125, 3698160658676859375};

	vysledek = mocnina = x;
	x2 = x*x;
/* Cyklus vypocita vysledek funkce tangens zafaneho uhlu "x"*/
	for(int i = 0; (unsigned)i < n-1;i++){
		mocnina *= x2;
		vysledek += ((mocnina*citatel[i])/jmenovatel[i]);
	}
	return vysledek;
}

/* Funkce slouzi pro vypocet funkce tangens uhlu "x" pomoci zretezenych zlomku */
double cfrac_tan(double x, unsigned int n){

	double vysledek, cf, cislo, mocnina;

	vysledek = 0.;
	cf = 0.;
	mocnina = x*x;
	for(int i = 0;(unsigned)i <= n;i++){
		cislo = (n-i)*2.+1.;
		cf = mocnina /(cislo-cf);
	}
	vysledek = cf/x;
	return vysledek;
}

/* Funkce slouzi pro porovnani vysledku vypoctu tangens uhlu "x". Funkce porovnava funci z matematicke knihovny, tayloruv polynom a zretezene zlomky */
double porovnej_uhly(double x, int n, int m){

	double math, taylor, cfranc;
	math = tan(x);

	for(int i = n; i <= m; i++){
		math = tan(x);
		taylor = taylor_tan(x, i);
		cfranc = cfrac_tan(x, i);
		printf("%d %e %e %e %e %e\n", i, math, taylor, fabs(math - taylor), cfranc,fabs(math - cfranc));// Vypise porovnani jednotlivych metod vypoctu tangens a absolutni chyby mezi funkci z matematicke knihovny a funkcemi pro tayloruv polynom a zretezene zlomky.
	}
	return 0;
}

/* Funkce vypocita a vrati vzdalenost objektu podle zadanych argumentu (vysky a uhlu) */
double vzdalenost_objektu(double vyska, double uhel){
    int iterace = iterace_presnost(uhel);
	if(cfrac_tan(uhel, iterace) == 0.)
        return -1;
	double vzdalenost = vyska/cfrac_tan(uhel, iterace);
	return vzdalenost;
}

/* Funkce vypocita a vrati vysku objektu podle zadanych argumentu (vzdalenosti, uhlu a vysky pristroje) */
double vyska_objektu(double vzdalenost, double uhel, double vyska_pristroje){
    int iterace = iterace_presnost(uhel);
	double vyska = cfrac_tan(uhel, iterace)*vzdalenost + vyska_pristroje;
	return vyska;
}

/* Funkce vrati pocet potrebnych iteraci pro pozadovanou presnost (na 10 desetinnych mist) vypoctu ve funkci cfrac_tan() */
int iterace_presnost(double uhel){
    for(int i = 0; i < 100; i++){
        if(fabs(cfrac_tan(uhel, i)-tan(uhel)) < PRESNOST){
            return i;
        }
    }
    return 10;
}
/* Tato funkce vytiskne napovedu */
void napoveda(){
	printf("Napoveda:\nTento program slouzi pro vypocet uhlu tangens tremi metodami a pro jejich porovnani. Dale take za pomoci vypoctu uhlu tangens vypocita vzdalenost mericiho pristroje od mereneho objektu (prip. vysku mereneho objektu).\nJsou celkem tri moznosti jak zadat argumenty programu:\n1. Pri zadanem argumentu \"--help\" se vytiskne tato napoveda.\n2. Pri zadanych argumentech \"--tan\" \"A\" \"N\" \"M\" se porovna presnost vypoctu tangens zadaneho uhlu pomoci funkce z matematicke knihovny, pomoci taylorova polynomu a pomoci zretezenych zlomku. Za argument \"A\" dosadte uhel alfa. Argumenty N a M udavaji, ve kterych iteracich iteracniho vypoctu ma porovnavani probihat.\n3. Pri zadanych argumentech \"[-c X]\" \"-m\" \"A\" \"[B]\" vypise vzdalenost (prip. i vyska) mereneho objektu. Argumenty v hranatych zavorkach jsou dobrovolne. Pokud je zadan argument -c, nastavi se vyska mericiho pristroje podle argumentu \"X\". Musi byt v rozsahu 0 - 100. Implicitni vyska je 1,5 metru. Argument \"-m\" vypocita a zmeri vzdalenost mereneho objektu podle zadaneho uhlu alfa (argument \"A\"). V pripade zadaneho uhlu \"B\" (ktery stanovuje uhel beta) se vypocita i vyska mereneho objektu.\nVsechny uhly se zadavaji v radianech a musi byt vetsi nez 0 a mensi nez 1,4.\n");
}
