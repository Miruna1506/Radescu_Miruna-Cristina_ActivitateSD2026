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
typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;

struct NodPrincipal
{
	Film info;
	NodPrincipal* next;
	NodSecundar* listaS;
};
struct NodSecundar
{
	NodSecundar* next;
	NodPrincipal* nodP;
};
//adaugari la final pt ambele
void adaugareInLP(NodPrincipal** cap, Film f)
{
	NodPrincipal* nou = malloc(sizeof(NodPrincipal));
	nou->info = f;
	nou->next = NULL;
	nou->listaS = NULL;
	if (*cap == NULL) //e goala
	{
		*cap = nou;
	}
	else
	{
		NodPrincipal* p = *cap;
		while (p->next)
		{
			p = p->next;
		}
		p->next = nou;
	}
}
void adaugareInLS(NodSecundar** cap, NodPrincipal* nodNou)
{
	NodSecundar* nou = malloc(sizeof(NodSecundar));
	nou->nodP = nodNou;
	nou->next = NULL;
	if (*cap == NULL)
	{
		*cap = nou;
	}
	else
	{
		NodSecundar* p = *cap;
		while (p->next)
		{
			p = p->next;
		}
		p->next = nou;
	}
}
NodPrincipal* citireListaPrincipala(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	NodPrincipal* lista = NULL;
	if (f)
	{
		while (!feof(f))
		{
			adaugareInLP(&lista, citireFilmDinFisier(f));
		}
		fclose(f);
	}
	return lista;
}
NodPrincipal* cautNodDupaId(NodPrincipal* cap, int id)
{
	while (cap && cap->info.id != id)
	{
		cap = cap->next;
	}
	return cap;
}
void adaugaMuchie(NodPrincipal **cap, int idStart, int idStop)
{
	NodPrincipal* nodStart = cautNodDupaId(*cap, idStart);
	NodPrincipal* nodStop = cautNodDupaId(*cap, idStop);

	if (nodStart && nodStop)
	{
		adaugareInLS(&(nodStart)->listaS, nodStop);
		adaugareInLS(&(nodStop)->listaS, nodStart);
	}
}

void citireMuchiiDinFisier(NodPrincipal * lista, const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	int idStart;
	int idStop;
	if (file)
	{
		while (!feof(file))
		{
			fscanf(file, "%d %d", &idStart, &idStop);
			adaugaMuchie(&lista, idStart, idStop);
		}
		fclose(file);
	}
}
void afisareLista(NodPrincipal* lista, int id)
{
	NodPrincipal* nodCautat = cautNodDupaId(lista, id);

	if (nodCautat)
	{
		NodSecundar* nodS = nodCautat->listaS;
		while (nodS)
		{
			afisareFilm(nodS->nodP->info);
			nodS = nodS->next;
		}
	}
}
void dezalocare(NodPrincipal** cap)
{
	while (*cap)
	{
		NodPrincipal* nodPDeSters = *cap;
		
		NodSecundar* capS = nodPDeSters->listaS;

		while (capS)
		{
			NodSecundar* nodSDeSters = capS;
			capS = capS->next;
			free(nodSDeSters);
		}
		
		free(nodPDeSters->info.regizor);
		free(nodPDeSters->info.titlu);

		*cap = (*cap)->next;
		free(nodPDeSters);
	}
}
int main()
{
	NodPrincipal* lista = citireListaPrincipala("filme.txt");
	citireMuchiiDinFisier(lista, "Muchii.txt");

	afisareLista(lista, 10);

	dezalocare(&lista);
}