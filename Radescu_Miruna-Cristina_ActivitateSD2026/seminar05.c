#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod
{
	struct Nod* prev;
	struct Nod* next;
	Masina info;
};
typedef struct Nod Nod;

struct ListaDubla {
	Nod* primul;
	Nod* ultimul;
};
typedef struct ListaDubla ListaDubla;
Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(ListaDubla lista) {
	Nod* aux = lista.primul;
	while (aux)
	{
		afisareMasina(aux->info);
		aux = aux->next;
	}
}
void afisareListaMasiniDeLaSfarsit(ListaDubla lista)
{
	Nod* aux = lista.ultimul;
	while (aux)
	{
		afisareMasina(aux->info);
		aux = aux->prev;
	}
}

void adaugaMasinaInLista(ListaDubla* lista, Masina masinaNoua) {
	//la final
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = NULL;
	nodNou->prev = lista->ultimul;
	if (lista->ultimul)
	{
		lista->ultimul->next = nodNou;
	}
	else
	{
		lista->primul = nodNou;
	}
	lista->ultimul = nodNou;
}

void adaugaLaInceputInLista(ListaDubla* lista, Masina masinaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = lista->primul;
	nodNou->prev = NULL;
	if (lista->primul)
	{
		lista->primul->prev = nodNou;
	}
	else
	{
		lista->ultimul = nodNou;
	}
	lista->primul = nodNou;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	if (file)
	{
		ListaDubla lista;
		lista.primul = NULL;
		lista.ultimul = NULL;
		while (!feof(file))
		{
			adaugaMasinaInLista(&lista, citireMasinaDinFisier(file));
		}
		fclose(file);
		return lista;
	}
}

void dezalocareLDMasini(ListaDubla* lista) {

	Nod* p = lista->primul;
	while (p)
	{
		Nod* aux = p;
		p = p->next;
		if (aux->info.model != NULL)
			free(aux->info.model);
		if (aux->info.numeSofer != NULL)
			free(aux->info.numeSofer);
		free(aux);
	}
	lista->primul = NULL;
	lista->ultimul = NULL;
}

float calculeazaPretMediu(ListaDubla lista) {
	Nod* p = lista.primul;
	float suma = 0;
	int contor = 0;
	while (p)
	{
		suma += p->info.pret;
		contor++;
		p = p->next;
	}
	if (contor != 0)
	{
		return suma / contor;
	}
	return 0;
}

void stergeMasinaDupaID(ListaDubla* lista, int id) {
	if (lista->primul == NULL)
		return;
	Nod* p = lista->primul;
	while (p != NULL && p->info.id != id)
	{
		p = p->next;
	}
	if (p == NULL)
		return;
	if (p->prev == NULL) //e primul nod
	{
		lista->primul = p->next;
		if (lista->primul != NULL)
		{
			lista->primul->prev = NULL;
		}
	}
	else
	{
		p->prev->next = p->next;
	}
	if (p->next != NULL)//nu e ultimul nod 
	{
		p->next->prev = p->prev;
	}
	else //este ultimul
	{
		lista->ultimul = p->prev;
	}
	if (p->info.model != NULL)
		free(p->info.model);
	if (p->info.numeSofer != NULL)
		free(p->info.numeSofer);
	free(p);
}

char* getNumeSoferMasinaScumpa(ListaDubla lista) {

	if (lista.primul != NULL)
	{
		Nod* Max = lista.primul;
		Nod* p = lista.primul;
		while (p)
		{
			if (p->info.pret > Max->info.pret)
			{
				Max = p;
			}
			p = p->next;
		}
		char* nume = (char*)malloc(strlen(Max->info.numeSofer) + 1);
		strcpy(nume, Max->info.numeSofer);
		return nume;
	}
	else return NULL;
}

int main() {

	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);
	printf("\n---------------\n");

	printf("\nPretul mediu al masinilor este %.2f\n\n", calculeazaPretMediu(lista));

	stergeMasinaDupaID(&lista, 1);
	afisareListaMasiniDeLaSfarsit(lista);

	char* numeSofer = getNumeSoferMasinaScumpa(lista);
	printf("Soferul cu cea mai scumpa masina este: %s\n", numeSofer);
	free(numeSofer);
	dezalocareLDMasini(&lista);
	return 0;
}
