#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

typedef struct Nod Nod;

struct Nod {
	Masina info;
	Nod* stanga;
	Nod* dreapta;
};


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


void adaugaMasinaInArbore(Nod** radacina, Masina masinaNoua) {
	if (*radacina) {
		if ((*radacina)->info.id < masinaNoua.id) {
			adaugaMasinaInArbore(&(*radacina)->dreapta, masinaNoua);
		}
		if ((*radacina)->info.id > masinaNoua.id) {
			adaugaMasinaInArbore(&(*radacina)->stanga, masinaNoua);
		}
	}
	else {
		Nod* nou = malloc(sizeof(Nod));
		nou->info = masinaNoua;
		nou->dreapta = NULL;
		nou->stanga = NULL;
		*radacina = nou;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	Nod* radacina = NULL;
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		adaugaMasinaInArbore(&radacina, citireMasinaDinFisier(f));
	}
	fclose(f);
	return radacina;
}

void afisareMasiniDinArborePreordine(Nod* radacina) {
	if (radacina) {
		afisareMasina(radacina->info);
		afisareMasiniDinArborePreordine(radacina->stanga);
		afisareMasiniDinArborePreordine(radacina->dreapta);
	}
}

void afisareMasiniDinArboreInordine(Nod* radacina) {
	if (radacina) {
		afisareMasiniDinArboreInordine(radacina->stanga);
		afisareMasina(radacina->info);
		afisareMasiniDinArboreInordine(radacina->dreapta);
	}
}

void dezalocareArboreDeMasini(Nod** radacina) {
	if (*radacina) {
		dezalocareArboreDeMasini(&(*radacina)->stanga);
		dezalocareArboreDeMasini(&(*radacina)->dreapta);
		if ((*radacina)->info.model)
			free((*radacina)->info.model);
		if ((*radacina)->info.numeSofer)
			free((*radacina)->info.numeSofer);

		free(*radacina);
		*radacina = NULL;
	}
}

Masina getMasinaByID(Nod* radacina, int id) {
	if (radacina) {
		if (radacina->info.id == id) {
			Masina m = radacina->info;
			m.model = malloc(strlen(radacina->info.model) + 1);
			strcpy(m.model, radacina->info.model);
			m.numeSofer = malloc(strlen(radacina->info.numeSofer) + 1);
			strcpy(m.numeSofer, radacina->info.numeSofer);
			return m;
		}
		if (radacina->info.id < id)
			return getMasinaByID(radacina->dreapta, id);
		if (radacina->info.id > id)
			return getMasinaByID(radacina->stanga, id);
	}
	Masina m;
	m.id = -1;
	return m;
}

int determinaNumarNoduri(Nod* radacina) {
	if (radacina) {
		return determinaNumarNoduri(radacina->stanga) + determinaNumarNoduri(radacina->dreapta) + 1;
	}
	return 0;
}

int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina) {
		int inaltimeStanga = calculeazaInaltimeArbore(radacina->stanga);
		int inaltimeDreapta = calculeazaInaltimeArbore(radacina->dreapta);
		return 1 + fmax(inaltimeDreapta, inaltimeStanga);
	}
	return 0;
}

float calculeazaPretTotal(Nod* radacina) {
	if (radacina) {
		return calculeazaPretTotal(radacina->stanga) + calculeazaPretTotal(radacina->dreapta) + radacina->info.pret;
	}
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* numeSofer) {
	float suma;
	if (radacina) {
		suma = calculeazaPretulMasinilorUnuiSofer(radacina->stanga, numeSofer) + calculeazaPretulMasinilorUnuiSofer(radacina->dreapta, numeSofer);
		if (strcmp(radacina->info.numeSofer, numeSofer) == 0)
			suma += radacina->info.pret;
		return suma;
	}
	return 0;
}

int main() {

	Nod* radacina = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	afisareMasiniDinArborePreordine(radacina);
	//dezalocareArboreDeMasini(&radacina);
	//afisareMasiniDinArborePreordine(radacina);
	Masina m = getMasinaByID(radacina, 7);
	if (m.id != -1)
	{
		printf("\nMasina cautata:\n");
		afisareMasina(m);
		free(m.model);
		free(m.numeSofer);
	}
	else printf("Nu exista masina cu acest id!\n");
	printf("Nr noduri : %d", determinaNumarNoduri(radacina));
	dezalocareArboreDeMasini(&radacina);
	return 0;
}