// graful = multime de noduri si muchii care le leaga

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

typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;

struct NodPrincipal {
	Masina masina;
	NodPrincipal* next;
	NodSecundar* vecini;
};

struct NodSecundar {
	NodSecundar* next;
	NodPrincipal* info;
};

//2.
//functii de inserare in liste
//si in principala si in secundara
void inserareListaP(NodPrincipal** graf, Masina masinaNoua) {
	NodPrincipal* nodNou = malloc(sizeof(NodPrincipal));
	nodNou->masina = masinaNoua;
	nodNou->next = NULL;
	nodNou->vecini = NULL;

	if (*graf)
	{
		NodPrincipal* aux = *graf;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nodNou;
	}
	else {
		*graf = nodNou;
	}
}

void inserareListaS(NodSecundar** listaSec, NodPrincipal* info) {
	NodSecundar* nou = malloc(sizeof(NodSecundar));
	nou->info = info;
	nou->next = NULL;

	if (*listaSec) {
		NodSecundar* aux = *listaSec;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		*listaSec = nou;
	}
}

//3.
//functie de cautarea in lista principala dupa ID
NodPrincipal* cautaNodDupaID(NodPrincipal* listaPrincipala, int id) {
	if (listaPrincipala == NULL) {
		return NULL;
	}
	else {
		while (listaPrincipala && listaPrincipala->masina.id != id) {
			listaPrincipala = listaPrincipala->next;
		}
		return listaPrincipala;
	}
}

//4.
//inserare muchie
void inserareMuchie(NodPrincipal* listaPrincipala, int idStart, int idStop) {
	if (listaPrincipala) {
		NodPrincipal* nodStart = cautaNodDupaID(listaPrincipala, idStart);
		NodPrincipal* nodStop = cautaNodDupaID(listaPrincipala, idStop);

		if (nodStop && nodStart) {
			inserareListaS(&nodStart->vecini, nodStop);
			inserareListaS(&nodStop->vecini, nodStart);
		}
	}
}

NodPrincipal* citireNoduriMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodPrincipal* listaPrincipala = NULL;

	if (f) {
		while (!feof(f)) {
			inserareListaP(&listaPrincipala, citireMasinaDinFisier(f));
		}

		fclose(f);
	}
	else {
		printf("Fisierul %s nu s-a deschis.\n", numeFisier);
	}

	return listaPrincipala;
}

void citireMuchiiDinFisier(NodPrincipal* listaPrincipala, const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	int idStart;
	int idStop;

	if (f) {
		while (!feof(f)) {
			fscanf(f, "%d %d", &idStart, &idStop);
			inserareMuchie(listaPrincipala, idStart, idStop);
		}

		fclose(f);
	}
	else {
		printf("Fisierul %s nu s-a deschis.\n", numeFisier);
	}
}

void dezalocareNoduriGraf(NodPrincipal** listaPrincipala) {
	while (*listaPrincipala) {
		NodPrincipal* nodDeSters = *listaPrincipala;

		NodSecundar* vecin = nodDeSters->vecini;
		while (vecin) {
			NodSecundar* vecinDeSters = vecin;
			vecin = vecin->next;
			free(vecinDeSters);
		}

		free(nodDeSters->masina.model);
		free(nodDeSters->masina.numeSofer);

		*listaPrincipala = (*listaPrincipala)->next;
		free(nodDeSters);
	}
}

void afisareLista(NodPrincipal* graf, int id) {
	NodPrincipal* nod = cautaNodDupaID(graf, id);

	if (nod) {
		NodSecundar* cap = nod->vecini;
		while (cap) {
			afisareMasina(cap->info->masina);
			cap = cap->next;
		}
	}
	else {
		printf("Nu exista masina cu id-ul %d.\n", id);
	}
}

int main() {

	NodPrincipal* graf = NULL;

	graf = citireNoduriMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier(graf, "Muchii.txt");

	afisareLista(graf, 10);

	dezalocareNoduriGraf(&graf);

	return 0;
}