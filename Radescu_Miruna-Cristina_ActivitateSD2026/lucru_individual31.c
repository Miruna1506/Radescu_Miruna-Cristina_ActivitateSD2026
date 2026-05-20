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
//Heap
typedef struct Heap Heap;
struct Heap
{
	Film* vector;
	int lungime;
	int nrElem;
};
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
Heap initializareHeap(int lungime)
{
	Heap h;
	h.lungime = lungime;
	h.nrElem = 0;
	h.vector = (Film*)malloc(sizeof(Film) * h.lungime);
	return h;
}
void filtreazaHeap(Heap h, int pozitie)
{
	int pozFiuSt = 2 * pozitie + 1;
	int pozFiuDr = 2 * pozitie + 2;
	int pozMax = pozitie;

	if (pozFiuDr < h.nrElem && h.vector[pozMax].id < h.vector[pozFiuDr].id)
	{
		pozMax = pozFiuDr;
	}
	if (pozFiuSt < h.nrElem && h.vector[pozMax].id < h.vector[pozFiuSt].id)
	{
		pozMax = pozFiuSt;
	}

	if (pozMax != pozitie)
	{
		Film aux = h.vector[pozitie];
		h.vector[pozitie] = h.vector[pozMax];
		h.vector[pozMax] = aux;

		if (pozMax <= (h.nrElem - 2) / 2)
		{
			filtreazaHeap(h, pozMax);
		}
	}
}
Heap citireHeap(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	Heap h = initializareHeap(10);
	if (file)
	{
		while (!feof(file))
		{
			h.vector[h.nrElem++] = citireFilmDinFisier(file);
		}
		fclose(file);
	}
	for (int i = (h.nrElem - 2) / 2;i >= 0;i--)
	{
		filtreazaHeap(h, i);
	}
	return h;
}
void afisareHeap(Heap h)
{
	for (int i = 0;i < h.nrElem;i++)
	{
		afisareFilm(h.vector[i]);
	}
}
Film extrageDinHeap(Heap* h)
{
	if (h->nrElem > 0)
	{
		Film aux = h->vector[0];
		h->vector[0] = h->vector[h->nrElem - 1];
		h->vector[h->nrElem - 1] = aux;

		h->nrElem--;

		//am stricat ordinea => filtram din nou

		for (int i = (h->nrElem - 2) / 2;i >= 0;i--)
		{
			filtreazaHeap(*h, i);
		}

		return aux;
	}
}
void dezalocareHeap(Heap* h)
{
	for (int i = 0;i < h->lungime;i++)
	{
		free(h->vector[i].regizor);
		free(h->vector[i].titlu);
	}
	free(h->vector);

	h->vector = NULL;
	h->nrElem = 0;
	h->lungime = 0;
}
int main()
{
	Heap heap = citireHeap("filme.txt");
	afisareHeap(heap);
	printf("Filmul extras din heap este:\n");
	afisareFilm(extrageDinHeap(&heap));
	dezalocareHeap(&heap);
}