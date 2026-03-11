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

void adaugaLaInceputInLista(/*lista de masini*/ Masina masinaNoua) {
	//adauga la inceputul listei o noua masina pe care o primim ca parametru
}

void* citireListaMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
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
	//sunt dezalocate toate masinile si lista de elemente
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

float calculeazaPretMediu(/*lista de masini*/) {
	//calculeaza pretul mediu al masinilor din lista.
	return 0;
}

void stergeMasiniDinSeria(/*lista masini*/ char serieCautata) {
	//sterge toate masinile din lista care au seria primita ca parametru.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

float calculeazaPretulMasinilorUnuiSofer(/*lista masini*/ const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	return 0;
}

int main() {
	//definim o lista
	Nod* lista = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);
	dezalocareListaMasini(&lista);
	afisareListaMasini(lista);
	return 0;
}