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
struct Carte deep_copy(struct Carte c)
{
	struct Carte carte = initializare(c.id, c.nrPagini, c.titlu, c.pret, c.gen); 
	return carte;
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
void dezalocare_vector(struct Carte** vector, int *nrElemente)
{
	for (int i = 0; i < *nrElemente; i++) {
		dezalocare(&vector[i]);
	}
	free(vector);
}
void afisare_vector(struct Carte* vector, int nrElemente)
{
	printf("\n-- Librarie --\n");
	//afisarea elementelor din vector apeland functia afisare
	for (int i = 0;i < nrElemente;i++)
	{
		afisare(vector[i]);
	}
}
struct Carte* copiazaPrimeleNElemente(struct Carte* vector, int nrElemente, int nrElementeCopiate) {
	//copiem intr-un vector nou pe care il vom returna primele nrElementeCopiate
	struct Carte *vectorNou=NULL;
	vectorNou = (struct Carte*)malloc(sizeof(struct Carte) * nrElementeCopiate);
	if (nrElemente < nrElementeCopiate)
	{
		nrElementeCopiate = nrElemente;
	}
		printf("\n-- Dupa copiere --\n");
		for (int i = 0;i < nrElementeCopiate;i++)
		{
			vectorNou[i] = deep_copy(vector[i]); //trebuie facut deep copy, deoarece daca voi dezaloca vectorul original=> dangling pointer
		}
	return vectorNou;
}
void afiseaza_dupa_gen(struct Carte* vector, int n, const char* gen_cautat) {
	printf("\n--- Carti din genul %s ---", gen_cautat);
	for (int i = 0; i < n; i++) {
		if (strcmp(vector[i].gen, gen_cautat) == 0) {
			afisare(vector[i]);
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

	afisare_vector(librarie, nrCarti);
	afiseaza_dupa_gen(librarie,nrCarti,"fantasy");
	calculeaza_valoare_stoc(librarie, nrCarti);

	struct Carte* rezultat;
	int nrCopiate = 2;
	rezultat = copiazaPrimeleNElemente(librarie,nrCarti,nrCopiate);
	afisare_vector(rezultat,nrCopiate);

	dezalocare_vector(&librarie,&nrCarti);
}