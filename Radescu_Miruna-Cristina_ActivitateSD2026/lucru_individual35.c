#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Produs
{
	int id;
	float pret;
	char* denumire;
	char categorie;
};
typedef struct Produs Produs;
void afisareProdus(Produs p)
{
	printf("Id: %d\n", p.id);
	printf("Pret: %.2f\n", p.pret);
	printf("Denumire: %s\n", p.denumire);
	printf("Categorie: %c\n\n", p.categorie);
}
Produs citireProdusDinFisier(FILE* f)
{
	char buff[255];
	char sep[4] = "\n;,";
	fgets(buff, 255, f);
	Produs p;
	p.id = atoi(strtok(buff, sep));
	p.pret = atof(strtok(NULL, sep));
	char* aux = strtok(NULL, sep);
	p.denumire = malloc(strlen(aux) + 1);
	strcpy(p.denumire, aux);
	p.categorie = strtok(NULL, sep)[0];
	return p;
}
//AVL
typedef struct Nod Nod;
struct Nod
{
	Produs info;
	Nod* stanga;
	Nod* dreapta;
};
int calculeazaInalitme(Nod* radacina)
{
	if (radacina)
	{
		int inaltimeSt = calculeazaInalitme(radacina->stanga);
		int inaltimeDr = calculeazaInalitme(radacina->dreapta);

		return 1 + max(inaltimeSt, inaltimeDr);
	}
	return 0;
}
int calculeazaEchilibru(Nod* radacina)
{
	if (radacina)
	{
		int inaltimeSt = calculeazaInalitme(radacina->stanga);
		int inaltimeDr = calculeazaInalitme(radacina->dreapta);

		return inaltimeSt - inaltimeDr;
	}
	return 0;
}
void rotireStanga(Nod** radacina)
{
	Nod* aux = (*radacina)->dreapta;

	(*radacina)->dreapta = aux->stanga;

	aux->stanga = *radacina;

	*radacina = aux;
}
void rotireDreapta(Nod** radacina)
{
	Nod* aux = (*radacina)->stanga;

	(*radacina)->stanga = aux->dreapta;

	aux->dreapta = *radacina;

	*radacina = aux;
}
void adaugareInArbore(Nod** radacina, Produs p)
{
	if (*radacina == NULL)
	{
		Nod* aux = malloc(sizeof(Nod));
		aux->info = p;
		aux->dreapta = NULL;
		aux->stanga = NULL;
		*radacina = aux;
	}
	else
	{
		if ((*radacina)->info.id < p.id)
		{
			adaugareInArbore(&(*radacina)->dreapta, p);
		}
		if ((*radacina)->info.id > p.id)
		{
			adaugareInArbore(&(*radacina)->stanga, p);
		}
		int echilibru = calculeazaEchilibru(*radacina);
		if (echilibru == 2) //mai greu pe stanga
		{
			if (calculeazaEchilibru((*radacina)->stanga) == -1)
			{
				rotireStanga(&(*radacina)->stanga);
			}
			rotireDreapta(radacina);
		}
		if (echilibru == -2)
		{
			if (calculeazaEchilibru((*radacina)->dreapta) == 1)
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
	Nod* arbore = NULL;
	if (f)
	{
		while (!feof(f))
		{
			adaugareInArbore(&arbore, citireProdusDinFisier(f));
		}
		fclose(f);
	}
	return arbore;
}
void afisareArborePreordine(Nod* radacina)
{
	if (radacina)
	{
		afisareProdus(radacina->info);

		afisareArborePreordine(radacina->stanga);

		afisareArborePreordine(radacina->dreapta);
	}
}
void afisareArboreInordine(Nod* radacina)
{
	if (radacina)
	{
		afisareArboreInordine(radacina->stanga);

		afisareProdus(radacina->info);

		afisareArboreInordine(radacina->dreapta);
	}
}
void dezalocare(Nod** radacina)
{
	if (*radacina)
	{

		dezalocare((*radacina)->stanga);
		dezalocare((*radacina)->dreapta);

		free((*radacina)->info.denumire);

		free(*radacina);
		*radacina = NULL;
	}
}
Produs deep_copy(Produs p)
{
	Produs copie;

	copie.id = p.id;
	copie.pret = p.pret;
	copie.denumire = malloc(strlen(p.denumire) + 1);
	strcpy(copie.denumire, p.denumire);
	copie.categorie = p.categorie;
	return copie;
}
Produs getProdusById(Nod* radacina, int id)
{
	if (radacina)
	{
		if (radacina->info.id == id)
		{
			Produs p = deep_copy(radacina->info);
			return p;
		}
		if (radacina->info.id < id)
		{
			return getProdusById(radacina->dreapta, id);
		}
		if (radacina->info.id > id)
		{
			return getProdusById(radacina->stanga, id);
		}
	}
	Produs p;
	p.id = -1;
	return p;
}
int main()
{
	Nod* arbore = citireArboreDinFisier("produse.txt");
	afisareArboreInordine(arbore);

	Produs rezultat = getProdusById(arbore, 1);
	afisareProdus(rezultat);
	if (rezultat.id !=-1)
	{
		free(rezultat.denumire);
	}

	dezalocare(&arbore);
}