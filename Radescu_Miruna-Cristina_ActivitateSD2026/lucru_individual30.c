#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
typedef struct Nod Nod;
struct Nod
{
	Carte info;
	Nod* stanga;
	Nod* dreapta;
};
int calculeazaInaltime(Nod* radacina)
{
	if (radacina)
	{
		int inaltimeSt = calculeazaInaltime(radacina->stanga);
		int inaltimeDr = calculeazaInaltime(radacina->dreapta);

		return 1 + max(inaltimeSt, inaltimeDr);
	}
	return 0;
}
int calculeazaEchilibru(Nod* radacina)
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

	aux->stanga = *radacina;

	(*radacina) = aux;
}
void rotireDreapta(Nod** radacina)
{
	Nod* aux = (*radacina)->stanga;

	(*radacina)->stanga = aux->dreapta;

	aux->dreapta = (*radacina);

	(*radacina) = aux;
}
void adaugareInArbore(Nod** radacina, Carte c)
{
	if (*radacina == NULL) //daca e gol
	{
		Nod* nou = malloc(sizeof(Nod));

		nou->info = c;
		nou->dreapta = NULL;
		nou->stanga = NULL;

		(*radacina) = nou;
	}
	else
	{
		if ((*radacina)->info.id < c.id) //daca e mai mare => se duce pe dreapta
		{
			adaugareInArbore(&(*radacina)->dreapta, c);
		}
		if ((*radacina)->info.id > c.id)
		{
			adaugareInArbore(&(*radacina)->stanga, c);
		}
		int echilibru = calculeazaEchilibru((*radacina)); //st-dr
		if (echilibru == 2) //e mai greu pe stanga
		{
			if (calculeazaEchilibru((*radacina)->stanga) == -1) //e mai greu pe dreapta
			{
				rotireStanga(&(*radacina)->stanga);
			}
			rotireDreapta(radacina);
		}
		if (echilibru == -2) //e mai greu pe dreapta
		{
			if (calculeazaEchilibru((*radacina)->dreapta) == 1)
			{
				rotireDreapta(&(*radacina)->dreapta);
			}
			rotireStanga(radacina);
		}
	}
}
Nod* citireArbore(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	if (f)
	{
		while (!feof(f))
		{
			adaugareInArbore(&arbore, citireCarteDinFisier(f));
		}
		fclose(f);
	}
	return arbore;
}
void afisareInPreordine(Nod* radacina)
{
	if (radacina)
	{
		afisareCarte(radacina->info);
		afisareInPreordine(radacina->stanga);
		afisareInPreordine(radacina->dreapta);
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
Carte getCarteById(Nod* radacina, int id)
{
	if (radacina)
	{
		if (radacina->info.id == id)
		{
			Carte c = radacina->info;

			c.autor = malloc(strlen(radacina->info.autor) + 1);
			strcpy(c.autor, radacina->info.autor);
			
			c.titlu = malloc(strlen(radacina->info.titlu) + 1);
			strcpy(c.titlu, radacina->info.titlu);

			return c;
		}
		if (radacina->info.id < id)
		{
			return getCarteById(radacina->dreapta, id);
		}
		if (radacina->info.id > id)
		{
			return getCarteById(radacina->stanga, id);
		}
		Carte c;
		c.id = -1;
		return c;
	}
}
int determinaNrNoduri(Nod* radacina)
{
	if (radacina)
	{
		return determinaNrNoduri(radacina->stanga) + determinaNrNoduri(radacina->dreapta) + 1;
	}
	return 0;
}
float calculeazaPretTotal(Nod* radacina)
{
	if (radacina)
	{
		return calculeazaPretTotal(radacina->dreapta) + calculeazaPretTotal(radacina->stanga) + radacina->info.pret;
	}
	return 0;
}
float calculeazaPretCartiAutor(Nod* radacina, const char* numeAutor)
{
	float suma;
	if (radacina)
	{
		suma = calculeazaPretCartiAutor(radacina->dreapta, numeAutor) + calculeazaPretCartiAutor(radacina->stanga, numeAutor);
		if (strcmp(radacina->info.autor, numeAutor) == 0)
		{
			suma += radacina->info.pret;
		}
		return suma;
	}
	return 0;
}
int main()
{
	Nod* arbore = citireArbore("carti_arbore.txt");
	afisareInPreordine(arbore);
	printf("Cartea cu id-ul cautat este:\n");
	afisareCarte(getCarteById(arbore, 2));
	printf("Numarul de noduri este: %d \n", determinaNrNoduri(arbore));
	printf("Pretul total al cartilor este: %.2f\n", calculeazaPretTotal(arbore));
	printf("Pretul total al cartilor unui autor este: %.2f\n", calculeazaPretCartiAutor(arbore, "Sadoveanu"));
	dezalocare(&arbore);
}