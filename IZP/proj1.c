/*
 * Soubor:  proj1.c
 * Datum:   2017/11/04
 * Autor:   Petr Marek, xmarek69@stud.fit.vutbr.cz
 * Projekt: Práce s textem, projekt č. 1 pro předmět IZP
 */
 
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define VELPOLE 100 //definuji makro standardní velikosti polí používaných v programu

char porovnej(char *argument, char mesto[]);
int pridejpism(char pismeno, char pismena[], int index);
void serad(char *pismena);
void vypis(char pismena[], char znaky[], char mesto[], int nalezeno, int enable);
void velke(char *argument);
int platnyarg(char arg[]);
void prekopiruj(char znaky[], char mesto[], char mesta[], int nalezeno);
void founded(int *nalezeno, char mesto[], char mesta[]);

int main(int argc, char *argv[]){
	char argument[VELPOLE];// Pole pro uložení zadaného argumentu
	char mesta[VELPOLE];// Pole, do kterého se budou ukládat názvy měst ze souboru
	char pismena[VELPOLE];// Pole, do kterého se budou ukládat dostupné znaky
	int index = 0;// Slouží jako index pro zápis do pole 'pismena'
	int ix = -1;// Slouží jako index pro zápis do pole 'znaky'
	char znak = '\0';// Slouží pro ukládání možného následujícího písmena
	char znaky[VELPOLE];// Slouží pro uložení všech možných následujících znaků (včetně duplicitních)
	int nalezeno = 0;// Slouží jako příznak, který určuje, zda se 'argument' shoduje s názvem města
	int enable = 0;// Slouží jako příznak, který určuje, zda eixstuje nějaký možný následující znak
	char mesto[VELPOLE];// slouží pro uložení názvu města v případě nalezení
	argument[0] = '\0';
	
	/* Pokud je argument zadaný, tak se překopíruje do pole 'argument' */
 	if(argc == 2)
		strcpy(argument, argv[1]);
	
	/* Pokud není zadaný platný argument (povolena jsou jen písmena), ukončí se program */
	if(platnyarg(argument) == 0)
		return 0;
	
	/* V následujícím cyklu se opakovně načítá název města ze souboru a ukládá do pole 'mesta' */
	while(scanf("%s",mesta) != EOF){
		velke(mesta); // Název města v poli 'mesta' změní na velká písmena
		znak = porovnej(argument, mesta);// Porovná argument a název města, návratovou hodnotu funkce uloží do proměnné 'znak'
		if((znak != '+')&&(znak != '-')){/* Pokud existuje nějaký následující znak -> nebude 'znak' roven ani '+' ani '-' a vykoná se tento blok */
			index = pridejpism(znak, pismena, index); /* Do proměnné index se uloží návratová hodnota z funkce. 'index' se dále používá pro ukončení pole 'pismena' */
			znaky[++ix] = znak; // Do pole znaky uloží písmeno, včetně těch duplicitních
			znaky[ix+1] = '\0'; // Ukončí pole (string)
			enable = 1;	// Nastaví příznak 'enable', který později zajišťuje výpis dostupných znaků
			prekopiruj(znaky, mesto, mesta, nalezeno); // Za vhodných podmínek překopíruje název města do pole 'mesto' pro pozdější použití
		}else if(znak == '+') /* Pokud se nstavil znak na '+', znamená to, že bylo nalezeno město a proto se (funkcí 'founded()') nastaví příznak 'nalezeno' na '1' a překopíruje se název města do pole 'mesto' pro pozdejší použití */
			founded(&nalezeno, mesto, mesta);
	}
	
	pismena[index] = '\0'; // Ukončí pole písmen
	serad(pismena); // Seřadí písmena v poli 'pismena'
	vypis(pismena, znaky, mesto, nalezeno, enable); // Vypíše výsledek programu
	return 0; // Ukončí program
}

/* Funkce porovnává 2 pole (argument a název města) a vrací znak, který je '-' v případě neshody polí, '+' v případě plné shody (pokud už nejsou další možné znaky a nějaké písmeno v případě, že jsou pole shodná, ale název města může pokračovat(vrátí se následující možný znak) */
char porovnej(char argument[], char mesto[]){
	int argvelikost = strlen(argument); // Uchovává délku pole 'argument'
	int mestovelikost = strlen(mesto); // Uchovává délku pole 'mesto'
	int stejne = 1; // Příznak udávající, zda jsou porovnávaná pole stejná
	/*V následujícím cyklu se postupně projdou znaky na jednotlivých pozicích argumentu a názvu města (pouze po konec argumentu) a v případě neshody na jakékoliv pozici se proměnná 'stejne' nastaví na '0'. V opačném případě zůstane '1'.*/
	for(int i = 0; i < argvelikost; i++)
	{
		if(argument[i] != mesto[i])
			stejne = 0;
	}
	
	// Pokud jsou obě pole (o délce argumentu) stejné, porovná se délka názvu města s délkou argumentu. Pokud jsou stejné, funkce vrátí znak '+', jinak vrátí následující možný znak. Pokud první podmínka nenastane (pole nejsou stejná), funkce vrátí znak '-'
	if(stejne == 1)
	{
		if((mestovelikost==argvelikost))
			return '+';		
		return mesto[argvelikost];
	}else return '-';

}

/* Funkce přidává do pole 'pismena' písmeno 'pismeno', pokud tam ještě není */
int pridejpism(char pismeno, char pismena[], int index)
{	
	int zapsane = 0; // Proměnná slouží pro informaci, zda už je 'pismeno' zapsané v poli 'pismena'
	// Cyklus kontroluje, zda už je někde v poli 'pismena' zapsané písmeno 'pismeno'
	for(int i = 0; pismena[i] != '\0' ; i++){
		if(pismena[i] == pismeno)
			zapsane = 1; // Pokud je už písmeno zapsané, nastaví se proměnná 'zapsane' na hodnotu '1'
	}
	if (zapsane == 0) // Pokud písmeno ještě není zapsané, zapíše se
	{
		if(((int)pismena[index-1] == (int)NULL) && (index > 0)) // Pokud by už na předchozí pozici indexu bylo NULL, sníží se index o 1
			index--;
		pismena[index++] = pismeno;	// Zapíše písmeno	
	}
	return index; // Funkce vrací pozici posledně zapsaného písmena v poli 'pismena', zvýšenou o 1 (pro příští zápis)
}

/* Funkce seřadí písmena v poli 'pismena' */
void serad(char *pismena){
	for(int i=0;pismena[i]!='\0';i++)	
	{
		for(int j=0;pismena[j]!='\0';j++)
		{
			char tmp; //slouží pro dočasné uložení jedné hodnoty
			if(pismena[j]>pismena[i])
			{
			// Následující 3 instrukce prohodí písmena na pozicích 'j' a 'i'
			tmp = pismena[i];
			pismena[i] = pismena[j];
			pismena[j] = tmp;
			}
		}
	}
}

/* Funkce vypíše výsledek programu */
void vypis(char pismena[], char znaky[], char mesto[], int nalezeno, int enable){
	if((strlen(znaky) == 1) || (nalezeno == 1)) // Pokud délka všech zapsaných písmen do pole 'znaky' (včetně duplicitních) je rovno '1', nebo pokud je příznak 'nalezeno' nastaven na '1', znamená to, že bylo nalezeno město, jehož název se vypíše
	{
		printf("Found: %s\n", mesto); // Vypíše název nalezeného města
	}
	if(((strlen(znaky) != 1)||(nalezeno==1)) && (enable == 1)){ // Pokud je nastavený příznak 'enable' na '1' a zároveň je buď příznak 'nalezeno' nastaven na '1', nebo se délka pole 'znaky' nerovná '1' (našlo se  víc měst), tak se vypíší jednotlivé znaky
		printf("Enable: ");
			for(int i = 0; pismena[i] != '\0' ; i++){
				printf("%c", pismena[i]); 
			}
			printf("\n");
	}
	if((nalezeno == 0) && (enable != 1)) // Pokud je příznak 'nalezeno' nastaven na 0 (název města se nenašel), a zároveň není příznak 'enable' nastaven na 1 (nejsou k dispozici žádné následující znaky), tak se vypíše "Not found"
	{
		printf("Not found");
	}
}

/* Malá písmena změní na velké */
void velke(char *argument)
{
	for(int i=0;*(argument+i)!='\0';i++)
	{
		*(argument+i) = toupper(*(argument+i));
	}	
}

/* Zkontroluje, zda je argument platný (zda se jedná o písmeno). V případě neplatnosti vrátí '0', což způsobí ukončení programu ve funkci 'main()'. V opačném případě se malá písmena změní na velké a program pokračuje. */
int platnyarg(char argument[]){
	for(int i = 0; i < (int)strlen(argument);i++)
	{
		if((argument[i]<'A') || (argument[i] > 'z') || ((argument[i] < 'a') && (argument[i] > 'Z'))) // Zkontroluje zda jsou zadány jen povolené znaky
		{
			printf("Wrong input!\n");
			return 0;
		}

	}
	
	velke(argument); // Funkce změní všechna malá písmena na velké
	return 1;
}

/* Kopíruje název města do pole 'mesto' pro pozdější využítí (pro výpis nalezeného města ve funkci 'vypis()') */
void prekopiruj(char znaky[], char mesto[], char mesta[], int nalezeno){
	if((strlen(znaky) == 1) && (nalezeno != 1)) /* Pokud se do pole 'znaky' uložilo jen jedno písmeno, znamená to, že existuje jen jedno město, které odpovídá zadanému argumentu a proto se překopíruje název města do pole 'mesto' pro pozdější použití */
		strcpy(mesto, mesta);
}

/* V případě, že je nalezená přesná shoda mezi argumentem a názvem města, změní příznak 'nalezeno' na '1' a překopíruje název města do pole 'mesto' pro pozdější použití */
void founded(int *nalezeno, char mesto[], char mesta[]){
	*nalezeno = 1;
	strcpy(mesto, mesta);
}