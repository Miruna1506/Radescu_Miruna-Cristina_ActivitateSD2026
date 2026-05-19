#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include < math.h>
struct Telefon
{
	int id;
	char* brand;
	char* model;
	float pret;
	int memorie;
	unsigned char sistem;
};
typedef struct Telefon Telefon;
Telefon citireTelefonDinFisier(FILE* file)
{
	Telefon t;
	char buffer[255];
	char sep[4] = ",\n";

	fgets(buffer, 255, file);

	t.id = atoi(strtok(buffer, sep));
	t.memorie = atoi(strtok(NULL, sep));
	t.pret = (float)atof(strtok(NULL, sep));

	char* aux = strtok(NULL, sep);

	t.brand = (char*)malloc(strlen(aux) + 1);
	strcpy(t.brand, aux);

	aux = strtok(NULL, sep);
	t.model = (char*)malloc(strlen(aux) + 1);
	strcpy(t.model, aux);

	t.sistem = strtok(NULL, sep)[0];

	return t;
}
void afisareTelefon(Telefon t)
{
	printf("\nID: %d", t.id);
	printf("\nBrand: %s", t.brand);
	printf("\nModel: %s", t.model);
	printf("\nPret: %.2f", t.pret);
	printf("\nMemorie: %d GB", t.memorie);
	printf("\nSistem: %c\n", t.sistem);
}
struct Nod
{
	Telefon info;
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;
int calculeazaInaltime(Nod * radacina)
{
	if (radacina)
	{
		int inaltimeSt = calculeazaInaltime(radacina->stanga);
		int inaltimeDr = calculeazaInaltime(radacina->dreapta);

		return 1 + max(inaltimeDr, inaltimeSt);
	}
	return 0;
}
int calculeazaEchilibru(Nod * radacina)
{
	if (radacina)
	{
		int inaltimeSt = calculeazaInaltime(radacina->stanga);
		int inaltimeDr = calculeazaInaltime(radacina->dreapta);

		return inaltimeSt - inaltimeDr;
	}
	return 0;
}
void rotireStanga(Nod** radacina)
{
	Nod* aux = (*radacina)->dreapta;
	
	(*radacina)->dreapta = aux->stanga;

	aux->stanga = (*radacina);

	*radacina = aux;
}
void rotireDreapta(Nod** radacina)
{
	Nod* aux = (*radacina)->stanga;

	(*radacina)->stanga = aux->dreapta;

	aux->dreapta = (*radacina);

	*radacina = aux;
}
void adaugaInArbore(Nod** radacina, Telefon t)
{
	if (*radacina == NULL)//daca e gol arborele
	{
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = t;
		nou->dreapta = NULL;
		nou->stanga = NULL;
		*radacina = nou;
	}
	else
	{
		if ((*radacina)->info.id < t.id)
		{
			adaugaInArbore(&(*radacina)->dreapta, t);
		}
		
		if ((*radacina)->info.id > t.id)
		{
			adaugaInArbore(&(*radacina)->stanga, t);
		}

		int echilibru = calculeazaEchilibru(*radacina);

		if (echilibru == 2) //e mai greu pe stanga
		{
			if (calculeazaEchilibru((*radacina)->stanga) == -1) //copilul e greu pe dreapta
			{
				rotireStanga(&(*radacina)->stanga);
			}
			rotireDreapta(radacina);
		}
		if (echilibru == -2) //e mai greu pe dreapta
		{
			if (calculeazaEchilibru((*radacina)->dreapta) == 1) //copilul e greu pe stanga
			{
				rotireDreapta(&(*radacina)->dreapta);
			}
			rotireStanga(radacina);
		}
	}
}
Nod* citireArboreDinFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	Nod* radacina = NULL;
	if (f)
	{
		while (!feof(f))
		{
			adaugaInArbore(&radacina, citireTelefonDinFisier(f));
		}
		fclose(f);
	}
	return radacina;
}
void dezalocare(Nod** radacina)
{
	if (*radacina)
	{
		dezalocare(&(*radacina)->dreapta);

		dezalocare(&(*radacina)->stanga);

		free((*radacina)->info.brand);

		free((*radacina)->info.model);

		free((*radacina));

		*radacina = NULL;
	}
}
//preordine
void afisareArbore(Nod* radacina)
{
	if (radacina)
	{
		afisareTelefon(radacina->info);

		afisareArbore(radacina->stanga);

		afisareArbore(radacina->dreapta);
	}
}
int main()
{
	Nod* arbore = citireArboreDinFisier("telefoane.txt");
	afisareArbore(arbore);
	dezalocare(&arbore);
}