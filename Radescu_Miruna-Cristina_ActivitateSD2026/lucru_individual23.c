#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Carte
{
	int id;
	int nrPagini;
	float pret;
	char* titlu;
	char* autor;
	char codRaft;
};
typedef struct Carte Carte;
struct Nod
{
	Carte info;
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;
Carte citireCarteDinFisier(FILE* f)
{
	char buff[255];
	char sep[4] = "\n;,";

	fgets(buff, 255, f);

	Carte c;
	c.id = atoi(strtok(buff, sep));
	c.nrPagini = atoi(strtok(NULL, sep));
	c.pret = atof(strtok(NULL, sep));
	char* aux = strtok(NULL, sep);
	c.titlu = malloc(strlen(aux) + 1);
	strcpy(c.titlu, aux);
	aux = strtok(NULL, sep);
	c.autor = malloc(strlen(aux) + 1);
	strcpy(c.autor, aux);
	c.codRaft = strtok(NULL, sep)[0];
	return c; 
}
void afisareCarte(Carte c)
{
	printf("Id: %d\n", c.id);
	printf("Nr. pagini: %d\n", c.nrPagini);
	printf("Pret: %.2f\n", c.pret);
	printf("Titlu: %s\n", c.titlu);
	printf("Autor: %s\n", c.autor);
	printf("Cod: %c\n\n", c.codRaft);
}
int calculeazaInaltimeArbore(Nod* radacina)
{
	if (radacina)
	{
		int inaltimeDr = calculeazaInaltimeArbore(radacina->dreapta);
		int inaltimeSt = calculeazaInaltimeArbore(radacina->stanga);

		return 1 + fmax(inaltimeDr, inaltimeSt);
	}
	return 0;
}
int esteEchilibrat(Nod* radacina)
{
	if (radacina)
	{
		int inaltimeDr = calculeazaInaltimeArbore(radacina->dreapta);
		int inaltimeSt = calculeazaInaltimeArbore(radacina->stanga);

		return inaltimeSt - inaltimeDr;
	}
	return 0;
}
void rotireStanga(Nod** radacina)
{
	if (radacina)
	{
		Nod* aux = (*radacina)->dreapta;

		(*radacina)->dreapta = aux->stanga;

		aux->stanga = *radacina;

		*radacina = aux;
	}
}
void rotireDreapta(Nod** radacina)
{
	if (radacina)
	{
		Nod* aux = (*radacina)->stanga;

		(*radacina)->stanga = aux->dreapta;

		aux->dreapta = *radacina;

		*radacina = aux;
	}
}
void adaugareInArbore(Nod** radacina, Carte c)
{
	if (*radacina == NULL) //daca e gol arborele
	{
		Nod* nou = malloc(sizeof(Nod));
		nou->info = c;
		nou->dreapta = NULL;
		nou->stanga = NULL;
		*radacina = nou;
	}
	else
	{
		if ((*radacina)->info.id < c.id)
		{
			adaugareInArbore(&(*radacina)->dreapta, c);
		}
		else
		{
			adaugareInArbore(&(*radacina)->stanga, c);
		}
	}
	int echilibru = esteEchilibrat(*radacina);
	if (echilibru == -2)//e mai greu pe dreapta
	{
		if (esteEchilibrat((*radacina)->dreapta) == 1)//copilul din dreapta e mai greu pe stanga
		{
			rotireDreapta(&(*radacina)->dreapta);//il rotim spre dreapta
		}
		rotireStanga(radacina);
	}
	if (echilibru == 2)//e mai greu pe stanga
	{
		if (esteEchilibrat((*radacina)->stanga) == -1)//copilul din stanga e mai greu pe dreapta
		{
			rotireDreapta(&(*radacina)->dreapta);
		}
		rotireDreapta(radacina);
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
			adaugareInArbore(&radacina, citireCarteDinFisier(f));
		}
		fclose(f);
	}
	return radacina;
}
void afisareArbore(Nod* radacina)
{
	if (radacina)
	{
		afisareCarte(radacina->info);

		afisareArbore(radacina->stanga);
		
		afisareArbore(radacina->dreapta);
	}
}
void dezalocare(Nod** radacina)
{
	if (*radacina)
	{
		dezalocare(&(*radacina)->dreapta);
		dezalocare(&(*radacina)->stanga);

		free((*radacina)->info.autor);
		free((*radacina)->info.titlu);

		free(*radacina);
		*radacina = NULL;
	}
}
int main()
{
	Nod* arbore = citireArboreDinFisier("carti_arbore.txt");
	afisareArbore(arbore);
	dezalocare(&arbore);
}