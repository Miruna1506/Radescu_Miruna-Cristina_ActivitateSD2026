#include<stdio.h>
#include<stdlib.h>

struct Carte {
	int id;
	int nrPagini;
	char* titlu;
	float pret;
	char* gen;
};
struct Carte initializare(int id,int nrPagini, char * titlu,float pret,char *gen) {
	struct Carte c;
	c.id = id;
	c.nrPagini = nrPagini;
	c.titlu =(char*) malloc(strlen(titlu) + 1);
	strcpy(c.titlu, titlu);
	c.pret = pret;
	c.gen = (char*)malloc(strlen(gen) + 1);
	strcpy(c.gen, gen);
	return c;
}
void afisare(struct Carte c) {
	printf("\nId:%d\n", c.id);
	printf("nrPagini:%d\n", c.nrPagini);
	printf("Titlu:%s\n", c.titlu);
	printf("Pret:%5.2f\n", c.pret);
	printf("Gen:%s\n", c.gen);
}
void dezalocare(struct Carte* c) {
	//dezalocare campuri alocate dinamic
	if (c->titlu != NULL)
	{
		free(c->titlu);
		c->titlu = NULL;
	}
	if (c->gen != NULL)
	{
		free(c->gen);
		c->gen = NULL;
	}

}
void afiseaza_dupa_gen(struct Carte* c, int n, const char* gen_cautat) {
	printf("\n--- Carti din genul %s ---", gen_cautat);
	for (int i = 0; i < n; i++) {
		if (strcmp(c[i].gen, gen_cautat) == 0) {
			afisare(c[i]);
		}
	}
}
void calculeaza_valoare_stoc(struct Carte* c, int nr) {
	float total = 0;
	for (int i = 0; i < nr; i++) {
		total += c[i].pret;
	}
	printf("\nValoarea totala a stocului este de: %5.2f", total);
}
int main() {

	int nrCarti = 3; 
	struct Carte* librarie = (struct Carte*)malloc(nrCarti * sizeof(struct Carte));

	librarie[0] = initializare(1, 100, "Harry Potter", 50, "fantasy");
	librarie[1] = initializare(2, 300, "Dune", 80, "SF");
	librarie[2] = initializare(3, 150, "Hobbitul", 45, "fantasy");
	afiseaza_dupa_gen(librarie,nrCarti,"fantasy");
	calculeaza_valoare_stoc(librarie, nrCarti);

	for (int i = 0; i < nrCarti; i++) {
		dezalocare(&librarie[i]);
	}
	free(librarie);
}