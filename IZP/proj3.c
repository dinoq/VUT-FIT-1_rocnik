/**
 * Kostra programu pro 3. projekt IZP 2017/18
 *
 * Jednoducha shlukova analyza
 * Unweighted pair-group average
 * https://is.muni.cz/th/172767/fi_b/5739129/web/web/usrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h>

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

#define CHYBA -1
#define COOR_MAX 1000
#define COOR_MIN 0
#define METHOD_LENGHT 10
int premium_case=0;
/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);
	c->size=0;
	c->capacity = cap;
	c->obj = malloc(sizeof(struct obj_t)*cap); // pozn. Kontrola, zda probehla alokace v poradku je kontrolovaana ve funkci main()
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    free(c->obj);
	c->size = 0;
	c->capacity = 0;
	c->obj = NULL;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{

    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;
    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    if(c->size >= c->capacity)
		resize_cluster( c, c->capacity+CLUSTER_CHUNK);
    c->obj[c->size++] = obj;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);
	for(int i=0;i<c2->size;i++){
		append_cluster(c1,c2->obj[i]);
	}
	sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);
    clear_cluster(&carr[idx]);
    for(int i = idx; i < narr-1; i++){
        carr[i] = carr[i+1];
    }
    return --narr;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);
    if(o1 == NULL || o2 == NULL){
        fprintf(stderr,"Chyba pri pocitani vzdalenosti objektu!");
		return CHYBA;
    }
    float x1,x2,y1,y2;
    x1 = o1->x;
    x2 = o2->x;
    y1 = o1->y;
    y2 = o2->y;
    return sqrtf(labs((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)));
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);
	
	float distance, min, max;
	distance = max = 0;
	min = sqrtf(labs((COOR_MAX-COOR_MIN)*(COOR_MAX-COOR_MIN)+(COOR_MAX-COOR_MIN)*(COOR_MAX-COOR_MIN)));;
	
	if(premium_case==0){
		for(int i=0;i<c1->size;i++){
			for(int j=0;j<c2->size;j++){
				distance += obj_distance(c1->obj+i,c2->obj+j);
			}
		}
		distance/=(c1->size*c2->size);
	}else if(premium_case==1){
		for(int i=0;i<c1->size;i++){
			for(int j=0;j<c2->size;j++){
				distance = obj_distance(c1->obj+i,c2->obj+j);				
				if(distance<min)
					min=distance;
			}
		}
		distance=min;
	}else if(premium_case==2){
		for(int i=0;i<c1->size;i++){
			for(int j=0;j<c2->size;j++){
				distance=obj_distance(c1->obj+i,c2->obj+j);
				if(distance>max)
					max=distance;
			}
		}
		distance=max;
	}
	
	return distance;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);
	float distance, min;
	int idx1, idx2;
	idx1 = idx2 = CHYBA;
	min = sqrtf(labs((COOR_MAX-COOR_MIN)*(COOR_MAX-COOR_MIN)+(COOR_MAX-COOR_MIN)*(COOR_MAX-COOR_MIN)));
	if(carr != NULL && narr>1){
		for( int i=0; i<narr ; i++){
			for(int j=0; j<narr ;j++){
				if( i !=j){
					distance = cluster_distance( &carr[i], &carr[j] );
					if(distance < min){
						if(distance == CHYBA)
							return;
						min = distance;
						idx1 = i;
						idx2 = j;			
					}
				}
			}
		}
	}
	if(idx1 != CHYBA && idx2 != CHYBA)
	{
		*c1 = idx1;
		*c2 = idx2;
	}
	else
		fprintf( stderr, "Chyba - Nenalezeni zadni sousede");
}
// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    int id, count,pocet = 0;
    float x,y;
    FILE *soubor;
    if((soubor = fopen(filename,"r")) == NULL){
        fprintf( stderr, "Chyba - soubor nenacten!\n");
        return CHYBA;
    }
    if(!fscanf(soubor,"count=%d", &count)){
        fprintf( stderr, "Chyba pri cteni prvniho radku souboru!\n");
        fclose(soubor);
        return CHYBA;
    }

    *arr=malloc(count*sizeof(struct cluster_t));

    if(arr == NULL){
        fprintf( stderr, "Chyba - nealokovana pamet pro pole!\n");
        fclose(soubor);
        return CHYBA;
    }

    for(int i = 0; i < count;i++){
        init_cluster(&(*arr)[i], 0);
    }

    for(int i = 0; i<count;i++){
        if(0){
            fprintf( stderr, "Chyba - neni dalsi objekt v souboru!\n");
            fclose(soubor);
            return CHYBA;
        }fscanf(soubor,"%d %f %f", &id, &x, &y);
		if((x<COOR_MIN)||(x>COOR_MAX)||(y<COOR_MIN)||(y>COOR_MAX)){
			fprintf( stderr, "Chyba - souradnice objektu s id %d jsou mimo povoleny rozsah pro %d<x<%d a %d<y%d!",id,COOR_MIN,COOR_MAX,COOR_MIN,COOR_MAX);
			return CHYBA;
		}
        (*arr+i)->obj->id = id;
        (*arr+i)->obj->x = x;
        (*arr+i)->obj->y = y;
        (*arr+i)->size = 1;
        if(!(*arr+i)->capacity)(*arr+i)->capacity = 1;
        pocet++;
    }

    if(fclose(soubor)==CHYBA){
        fprintf( stderr, "Chyba pri zavirani souboru!\n");
        return CHYBA;
    }
    return pocet;
}
/*
 Funkce, která uvolni pamet. 'carr' je ukazatel na pole shluku a 'narr' je velikost pole.
*/
void free_clusters(struct cluster_t *carr, int narr)
{
	for(int i=0 ; i<narr ;i++)
		clear_cluster(&carr[i]);
	free(carr);
}
/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}
/*
	Funkce provadi kontrolu clusteru, zda v nich byla alokovana panet pro objekty (zda je dostatek pameti). V pripade ze nektery objekt nema alokovanou pamet pro objekty (ukazatel na objekt je NULL) vrati konstantu CHYBA (-1). Jinak vrati 1.
*/
int obj_alloc(struct cluster_t **arr, int narr){
	for(int i = 0; i < narr; i++){
		if((*arr+i)->obj == NULL){
			fprintf( stderr, "Chyba - Pamet pro objekty nealokovana!");
			return CHYBA;
		}
	}
	return 1;
}

int main(int argc, char *argv[])
{
	struct cluster_t *clusters;
	int pocet = load_clusters(argv[1], &clusters);
	if (pocet == CHYBA ){
		free_clusters(clusters, pocet);
		return CHYBA;
	}
	int alokovano = obj_alloc(&clusters, pocet);
	if(alokovano == CHYBA){
		free_clusters(clusters, pocet);
		return CHYBA;
	}
	int idx1, idx2, count = 1;
	idx1 = idx2 = CHYBA;
	if (argc > 2){
		int n_cluster = atoi(argv[2]);
		if(n_cluster > 0 && n_cluster <= pocet)
			count = n_cluster;
		else{
			fprintf(stderr,"Chyba - druhy argument musi byt v rozsahu 0 az maximalni pocet clusteru");
			free_clusters(clusters, pocet);
			return CHYBA;
		}
	}
	if(argc > 3){
		char method[METHOD_LENGHT];
		strcpy(method,argv[3]);
		if(!strcmp(method,"--avg"))
			premium_case=0;
		
		else if(!strcmp(method,"--min"))
			premium_case=1;
		
		else if(!strcmp(method,"--max"))
			premium_case=2;
		else
		{
			fprintf(stderr,"Chyba - nespravne zadany argument [METHOD]!");
			return CHYBA;
		}
	}
	for(;count<pocet;){
		find_neighbours(clusters, pocet, &idx1, &idx2);
		merge_clusters(&clusters[idx1], &clusters[idx2]);
		pocet = remove_cluster(clusters, pocet, idx2++);
	}
	print_clusters(clusters, pocet);
	free_clusters(clusters, pocet);
	return 0;
}
