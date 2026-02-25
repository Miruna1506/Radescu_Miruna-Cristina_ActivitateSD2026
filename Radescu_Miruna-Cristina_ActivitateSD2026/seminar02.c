#include<stdio.h>
#include<malloc.h>

struct Telefon {
	int id;
	int RAM;
	char* producator;
	float pret;
	char serie;
};
struct Telefon initializare(int id,int RAM,char* producator,float pret,char serie) {
	struct Telefon t;
	t.id = id;
	t.RAM = RAM;
	t.producator = (char*)malloc(strlen(producator) + 1);
	strcpy(t.producator, producator);
	t.pret = pret;
	t.serie = serie;
	return t;
}
struct Telefon copy_telefon(struct Telefon t)
{
	struct Telefon tel = initializare(t.id, t.RAM, t.producator, t.pret, t.serie);
	return tel;
}
void afisare(struct Telefon t) {
	//afisarea tuturor atributelor.
	printf("\nId: %d\n", t.id);
	printf("RAM: %d\n", t.RAM);
	printf("Producator: %s\n", t.producator);
	printf("Pret: %5.2f\n", t.pret);
	printf("Serie: %c\n", t.serie);
}

void afisareVector(struct Telefon* vector, int nrElemente) {
	//afisarea elementelor din vector apeland functia afisare
	for (int i = 0;i < nrElemente;i++)
	{
		afisare(vector[i]);
	}
}

struct Telefon* copiazaPrimeleNElemente(struct Telefon* vector, int nrElemente, int nrElementeCopiate) {
	//copiem intr-un vector nou pe care il vom returna primele nrElementeCopiate
	struct Telefon *vectorNou=NULL;
	vectorNou = (struct Telefon*)malloc(sizeof(struct Telefon) * nrElementeCopiate);
	if (nrElemente < nrElementeCopiate)
	{
		nrElementeCopiate = nrElemente;
	}
		printf("\n-- Dupa copiere --\n");
		for (int i = 0;i < nrElementeCopiate;i++)
		{
			vectorNou[i] = copy_telefon(vector[i]);
		}
	
	return vectorNou;
}

void dezalocare(struct Telefon** vector, int* nrElemente) {
	//dezalocam elementele din vector si vectorul
	for (int i = 0;i < *nrElemente;i++)
	{
		free((*vector)[i].producator);
		(*vector)[i].producator = NULL;

	}
	free(*vector);//dezaloc ca sa nu am memory leak
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaTelefoaneScumpe(struct Telefon* vector, char nrElemente, float pret_minim, struct Telefon** vectorNou, int* dimensiune) {
	//parametrul prag poate fi modificat in functie de pret
	*dimensiune = 0;
	for (int i = 0;i < nrElemente;i++)
	{
		if (vector[i].pret >= pret_minim)
		{
			(*dimensiune)++;
		}
	}
	*vectorNou =(struct Telefon*) malloc(*dimensiune * sizeof(struct Telefon));
	for (int i = 0,j=0;i < *dimensiune;i++)
	{
		if (vector[i].pret >= pret_minim)
		{
			(*vectorNou)[j] = copy_telefon(vector[i]);
			j++;
		}
	}
	printf("\nTelefoane scumpe:\n");
	// tipul atributului ales pentru a indeplini o conditie
	//este creat un nou vector cu elementele care indeplinesc acea conditie
}

struct Telefon getPrimulElementConditionat(struct Telefon* vector, int nrElemente, const char* conditie) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	struct Telefon s;
	s.id = 1;

	return s;
}
	

int main() {

	struct Telefon t;
	t = initializare(3, 100, "Samsung", 2500.0, 'S');
	//afisare(t);
	struct Telefon* telefoane;

	int nrTelefoane = 3;

	telefoane = (struct Telefon*)malloc(sizeof(struct Telefon) * nrTelefoane);
	telefoane[0] = initializare(1, 200, "Apple", 3000, 'S');
	telefoane[1] = initializare(2, 200, "Huawei", 3000, 'S');
	telefoane[2] = t;

	afisareVector(telefoane, nrTelefoane);
	struct Telefon* rezultat;
	int nrTelefoaneCopiate = 2;
	rezultat = copiazaPrimeleNElemente(telefoane, nrTelefoane, nrTelefoaneCopiate);
	afisareVector(rezultat,nrTelefoaneCopiate);

	int dimensiune = 0;
	copiazaTelefoaneScumpe(telefoane, nrTelefoane, 2600, &rezultat, &dimensiune);
	afisareVector(rezultat, dimensiune);

	dezalocare(&telefoane, &nrTelefoane);
	dezalocare(&rezultat, &nrTelefoaneCopiate);

	//afisareVector(telefoane, nrTelefoane);
	return 0;
}
