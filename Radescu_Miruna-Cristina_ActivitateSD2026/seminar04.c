#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-o lista simplu inlantuita
struct Nod {
	Masina info;
	struct Nod* next;
};
typedef struct Nod Nod;
Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
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

void afisareListaMasini(Nod* cap) {
	//afiseaza toate elemente de tip masina din lista simplu inlantuita
	//prin apelarea functiei afisareMasina()
	while (cap)
	{
		afisareMasina(cap->info);//dereferentiem
		cap = cap->next;
	}
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* temp = (Nod*)malloc(sizeof(Nod));
	temp->info = masinaNoua;
	temp->next = NULL;
	if (!(*cap))
	{
		*cap = temp;
	}
	else
	{
		Nod* p = *cap;
		while (p->next)
		{
			p = p->next;
		}
		p->next = temp;
	}
}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = *cap;
	*cap = nodNou;
}

void* citireListaMasiniDinFisier(const char* numeFisier) {
	
	FILE* file = fopen(numeFisier, "r");
	Nod* cap = NULL;

	while(!feof(file))
	{
		adaugaMasinaInLista(&cap, citireMasinaDinFisier(file));
		
	}
	fclose(file);
	return cap;
}

void dezalocareListaMasini(Nod** cap) {
	while ((*cap))
	{
		free((*cap)->info.numeSofer);
		free((*cap)->info.model);
		Nod* aux = *cap;
		aux = (*cap)->next;
		free(*cap);
		*cap = aux;
	}
}

float calculeazaPretMediu(Nod* cap) {
	float suma = 0;
	int contor = 0;
	while (cap != NULL)
	{
		suma = suma + cap->info.pret;
		contor++;
		cap = cap->next;
	}
	if (contor != 0)
		return suma / contor;
	return 0;
}

void stergeMasiniDinSeria(Nod** cap, char serieCautata) {
	while (*cap && (*cap)->info.serie == serieCautata)
	{
		Nod* temp = *cap;
		(*cap) = temp->next;
		if (temp->info.numeSofer != NULL)
			free(temp->info.numeSofer);
		if (temp->info.model != NULL)
			free(temp->info.model);
		free(temp);
	}
	Nod* p = *cap;
	while (p != NULL)
	{
		while (p->next && p->next->info.serie != serieCautata)
		{
			p = p->next;
		}
		if (p->next)
		{
			Nod* temp = p->next;
			p->next = temp->next;
			if (temp->info.numeSofer != NULL)
			{
				free(temp->info.numeSofer);
			}
			if (temp->info.model != NULL)
			{
				free(temp->info.model);
			}
			free(temp);
		}
		else
		{
			p = p->next; //primeste null
		}
	}
}

float calculeazaPretulMasinilorUnuiSofer(Nod* cap, const char* numeSofer) {
	float suma = 0;
	while (cap != NULL)
	{
		if (strcmp(cap->info.numeSofer, numeSofer) == 0)
		{
			suma += cap->info.pret;
		}
		cap = cap->next;
	}
	return suma;
}

int main() {
	Nod* lista = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);
	float medie = calculeazaPretMediu(lista);
	printf("\nPretul mediu este: %.2f\n", medie);
	printf("\nPretul masinilor unui sofer este:%.2f\n", calculeazaPretulMasinilorUnuiSofer(lista, "Gigel"));
	stergeMasiniDinSeria(&lista, 'A');
	printf("\nDupa stergerea seria A:\n");
	afisareListaMasini(lista);
	dezalocareListaMasini(&lista);
	return 0;
}