#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Film
{
	int id;
	int durataMinute;
	float pretBilet;
	char* titlu;
	char* regizor;
	char sala;
};
typedef struct Film Film;
void afisareFilm(Film film)
{
	printf("Id: %d\n", film.id);
	printf("Durata minute: %d\n", film.durataMinute);
	printf("Pret bilet: %.2f\n", film.pretBilet);
	printf("Titlu: %s\n", film.titlu);
	printf("Regizor: %s\n", film.regizor);
	printf("Sala: %c\n\n", film.sala);
}
Film citireFilmDinFisier(FILE* file)
{
	char buff[255];
	char sep[4] = "\n;,";

	fgets(buff, 255, file);

	Film f;
	f.id = atoi(strtok(buff, sep));
	f.durataMinute = atoi(strtok(NULL, sep));
	f.pretBilet = atof(strtok(NULL, sep));
	char* aux = strtok(NULL, sep);
	f.titlu = malloc(strlen(aux) + 1);
	strcpy(f.titlu, aux);
	aux = strtok(NULL, sep);
	f.regizor = malloc(strlen(aux) + 1);
	strcpy(f.regizor, aux);
	f.sala = strtok(NULL, sep)[0];

	return f;
}
typedef struct Nod Nod;
struct Nod 
{
	Film info;
	Nod* stanga;
	Nod* dreapta;
};
int calculeazaInaltime(Nod* radacina)
{
	if (radacina)
	{
		int inaltimeDr = calculeazaInaltime(radacina->dreapta);
		int inaltimeSt = calculeazaInaltime(radacina->stanga);

		return 1 + max(inaltimeDr, inaltimeSt);
	}
	return 0;
}
int calculeazaEchilibru(Nod* radacina)
{
	if (radacina)
	{
		int inaltimeDr = calculeazaInaltime(radacina->dreapta);
		int inaltimeSt = calculeazaInaltime(radacina->stanga);

		return inaltimeSt - inaltimeDr;
	}
	return 0;
}
void rotireStanga(Nod** radacina)
{
	Nod* aux = (*radacina)->dreapta;//nodul care va deveni radacina

	(*radacina)->dreapta = aux->stanga;

	aux->stanga = (*radacina);

	*radacina = aux;
}
void rotireDreapta(Nod** radacina)
{
	Nod* aux = (*radacina)->stanga; //nodul care va deveni radacina

	(*radacina)->stanga = aux->dreapta;

	aux->dreapta = (*radacina);

	*radacina = aux;
}
void adaugareInArbore(Nod** radacina, Film film)
{
	if (*radacina == NULL)//e gol
	{
		Nod* nou = malloc(sizeof(Nod));
		nou->info = film;
		nou->dreapta = NULL;
		nou->stanga = NULL;

		*radacina = nou;
	}
	else
	{
		if (film.id > (*radacina)->info.id)
		{
			adaugareInArbore(&(*radacina)->dreapta, film);
		}
		if (film.id < (*radacina)->info.id)
		{
			adaugareInArbore(&(*radacina)->stanga, film);
		}
		int echilibru = calculeazaEchilibru(*radacina);
		if (echilibru == 2) //st-dr => e mai greu pe stanga
		{
			if (calculeazaEchilibru((*radacina)->stanga) == -1) //e mai greu pe dreapta
			{
				rotireStanga(&(*radacina)->stanga);
			}
			rotireDreapta(radacina);
		}
		if (echilibru == -2)//e mai greu pe dreapta
		{
			if (calculeazaEchilibru((*radacina)->dreapta) == 1) // copilul e mai greu pe stanga
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
			adaugareInArbore(&arbore, citireFilmDinFisier(f));
		}
		fclose(f);
	}
	return arbore;
}
void afisareArborePreordine(Nod* radacina)
{
	if (radacina)
	{
		afisareFilm(radacina->info);

		afisareArborePreordine(radacina->stanga);

		afisareArborePreordine(radacina->dreapta);
	}
}
void afisareArboreInordine(Nod* radacina)
{
	if (radacina)
	{
		afisareArboreInordine(radacina->stanga);

		afisareFilm(radacina->info);

		afisareArboreInordine(radacina->dreapta);
	}
}
void dezalocare(Nod** radacina)
{
	if (radacina)
	{
		dezalocare(&(*radacina)->stanga);

		dezalocare(&(*radacina)->dreapta);

		free((*radacina)->info.regizor);
		free((*radacina)->info.titlu);

		free((*radacina));

		*radacina = NULL;
	}
}
int main()
{
	Nod* arbore = citireArboreDinFisier("filme.txt");
	//afisareArborePreordine(arbore);
	afisareArboreInordine(arbore);
	dezalocare(&arbore);
}