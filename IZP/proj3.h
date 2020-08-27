/** @file proj3.h
* @brief Dokumentace ke 3. projektu v predmetu IZP
*/


 /** 
  *@brief Struktury cluster_t a obj_t. 
  *@defgroup Struktury
  *@{
  */
	///Struktura typu objekt. 
 struct obj_t {
	///Identifikacni cislo objektu.
	int id;
	///Souradnice objektu X.
    float x;
	///Souradnice objektu Y.
    float y;
};

	///Struktura typu cluster obsahujici objekty.
struct cluster_t {
	///Aktualni pocet objektu v clusteru.
    int size;
	///Kapacita clusteru pro objekty. V pripade nutnosti je mozne ji zvetsit funkci resize_cluster.
    int capacity;
	///Ukazatel na prvni objekt v clusteru.
    struct obj_t *obj;
};
  /** 
  *@}
  */
 
  /**
  *@brief Funkce pracující se strukturami cluster_t a obj_t. 
  *@defgroup Funkce
  *@{
  *@details Následuje seznam funkcí s jejich popisem. Jedná se o funkce, které pracují se strukturami cluster_t a obj_t.
  */
  
 /** 
  *@brief Inicializuje cluster a alokuje dostatecnou pamet pro objekty podle pozadovane kapacity.
  *
  *@param c Ukazatel na cluster, ktery chceme inicializovat.
  *@param cap Pozadovana kapacita clusteru.
  *@pre	Cluster "c" musi byt alokovan.
  *@pre Parametr "cap" nesmi byt zaporny.
  *@post Velikost clusteru je nula.
  *@post Kapacita clusteru se nastaví parametrem "cap".
  *@returns Funkce nic nevraci.
  */
void init_cluster(struct cluster_t *c, int cap);

 /** 
  *@brief Odstrani vsechny objektu clusteru a inicializuje je na prazdny cluster.
  *
  *@param c Ukazatel na cluster, ze ktereho chceme odstranit vsechny objekty a inicializovat na prazdny cluster.
  *@pre	Cluster "c" musi byt alokovan.
  *@post Velikost a kapacita clusteru je nula.
  *@post Ukazatel na objekty je nastaven na NULL
  *@returns Funkce nic nevraci.
  */
void clear_cluster(struct cluster_t *c);

///@brief Pocet objektu o ktery se ma zvetsit kapacita clusteru pri realokaci.
extern const int CLUSTER_CHUNK;

 /** 
  *@brief Zmeni kapacitu clusteru "c" na kapacitu "new_cap".
  *
  *@param c Ukazatel na cluster, kteremu chceme zmenit kapacitu.
  *@param new_cap Kapacita, na kterou se ma realokovat pamet pro objekty (nova kapacita).
  *@pre	Cluster "c" musi byt alokovan.
  *@pre	Kapacita clusteru "c" musi byt mensi nez zadana kapacita "new_cap".
  *@post Kapacita clusteru se zvetsila na kapacitu "new_cap".
  *@returns Funkce vraci puvodni cluster s novou kapacitou.
  */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
  *@brief Prida do clusteru objekt. Pokud uz se do nej nevejde, tak se clusteru rozsiri.
  *
  *@param c Cluster do ktereho ma byt vlozen objekt.
  *@param obj Objekt ktery ma byt vlozen do clusteru.
  *@returns Funkce nic nevraci.
  */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
  *@brief Do clusteru "c1" prida objekty ze clusteru "c2" a seradi je podle ID. Pokud se do nej objekty nevejdou, tak se rozsiri.
  *
  *@param c1 Cluster do ktereho se pridavaji objekty.
  *@param c2 Cluster ze ktereho se kopiruji objekty. 
  *@pre Cluster "c1" musi byt alokovan.
  *@pre Cluster "c2" musi byt alokovan.
  *@post Cluster "c1" je vzestupne serazen podle hodnot id.
  *@post Cluster "c2" je nezmenen.
  *@returns Funkce nic nevraci.
  */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
  *@brief Odstrani cluster z pole clusteru.
  *
  *@param carr Ukazatel na prvni cluster v poli.
  *@param narr Pocet prvku v poli.
  *@param idx Index clusteru, ktery se ma odstranit.
  *@pre Parametr "idx" musi byt mensi nez velikost pole.
  *@pre Velikost pole musi byt vetsi nez nula.
  *@post Velikost pole se snizi o jedna.
  *@returns Funkce vraci novy pocet clusteru v poli.
  */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
  *@brief Pocita Euklidovskou vzdalenost dvou objektu.
  *
  *@param o1 Prvi objekt.
  *@param o2 Druhy objekt.
  *@pre objekt "o1" musi byt alokovany.
  *@pre objekt "o2" musi byt alokovany.
  *@post Vzdalenost je vetsi nez nula.
  *@returns Funkce vraci vzdalenost mezi dvema objekty.
  */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
  *@brief Pocita vzdalenost dvou clusteru.
  *
  *@param c1 Prvni cluster.
  *@param c2 Druhy cluster.
  *@pre Cluster "c1" musi byt alokovany a obsahovat aspon jeden objekt.
  *@pre Cluster "c2" musi byt alokovany a obsahovat aspon jeden objekt.
  *@post Vzdalenost ma kladnou, nenulovou hodnotu.
  *@returns Vzdalenost mezi dvema clustery.
  */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
  *@brief Hleda dva nejblizsi clustery v poli clusteru.
  *
  *@param carr Ukazetel na prvni prvek pole clusteru.
  *@param narr Velikost pole clusteru.
  *@param c1 Ukazatel na prvni ze dvou nejblizsich prvku.
  *@param c2 Ukazatel na druhy ze dvou nejblizsich prvku.
  *@pre Velikost pole musi byt vetsi nez nula.
  *@post Do parametru "c1" a "c2" se zapisi indexy nejblizsich clusteru.
  *@returns Funkce nic nevraci.
  */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
  *@brief Vytiskne cluster "c" na vystup stdout.
  *
  *@param c Cluster ktery ma byt vytisknut.
  *@pre Cluster "c" musi byt alokovan a obsahovat objekt.
  *@returns Funkce nic nevraci.
  */
void print_cluster(struct cluster_t *c);

/**
  *@brief Nacte objekty ze souboru, kazdy objekt ulozi do jineho clusteru. Clustery jsou pote ulozeny do pole clusteru.
  *
  *@param filename Nazev souboru ze ktereho se cte.
  *@param arr Ukazatel na prvni prvek pole clusteru.
  *@pre Ukazatel arr nesmi mit hodnotu NULL.
  *@post Soubor se po cteni zavre.
  *@returns Pocet nactenych objektu.
  */
int load_clusters(char *filename, struct cluster_t **arr);

/**
  *@brief Vytiskne pole clusteru.
  *
  *@param carr Ukazatel na prvni polozku pole.
  *@param narr Pocet prvku ktery se vytiskne.
  *@returns Funkce nic nevraci.
  */
void print_clusters(struct cluster_t *carr, int narr);
  /**
  *@} 
  */