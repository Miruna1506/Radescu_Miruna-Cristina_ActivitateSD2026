#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
Telefon citireTelefonDinFisiser(FILE* f)
{
	char buff[255];
	char sep[4] = "\n,;";

	fgets(buff, 255, f);

	Telefon t;

	t.id = atoi(strtok(buff, sep));
	t.memorie = atoi(strtok(NULL, sep));
	t.pret = atof(strtok(NULL, sep));
	char* aux = strtok(NULL, sep);
	t.brand = malloc(strlen(aux) + 1);
	strcpy(t.brand, aux);
	aux = strtok(NULL, sep);
	t.model = malloc(strlen(aux) + 1);
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
struct Heap
{
	Telefon* vector;
	int lungime;
	int nrElem;
};
typedef struct Heap Heap;
Heap initializare(int lungime)
{
	Heap h;
	h.lungime = lungime;
	h.vector = (Telefon*)malloc(sizeof(Telefon) * lungime);
	h.nrElem = 0;
	return h;
}
void filtreazaHeap(Heap h, int pozitie)
{
	int pozFiuSt = pozitie * 2 + 1;
	int pozFiuDr = pozitie * 2 + 2;
	int pozMin = pozitie;

	if (pozFiuDr < h.nrElem && h.vector[pozMin].id > h.vector[pozFiuDr].id)
	{
		pozMin = pozFiuDr;
	}
	if (pozFiuSt < h.nrElem && h.vector[pozMin].id > h.vector[pozFiuSt].id)
	{
		pozMin = pozFiuSt;
	}
	if (pozitie != pozMin)
	{
		//interschimbare
		Telefon aux = h.vector[pozMin];
		h.vector[pozMin] = h.vector[pozitie];
		h.vector[pozitie] = aux;

		if (pozMin <= (h.nrElem - 2) / 2)
		{
			filtreazaHeap(h, pozMin);
		}
	}
}
Heap citireHeapDinFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	Heap h = initializare(10);
	if (f)
	{
		while (!feof(f))
		{
			h.vector[h.nrElem++] = citireTelefonDinFisiser(f);
		}
		
		fclose(f);
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
		afisareTelefon(h.vector[i]);
	}
}
void afisareHeapAscuns(Heap h)
{
	for (int i = h.nrElem;i < h.lungime;i++)
	{
		afisareTelefon(h.vector[i]);
	}
}
Telefon extrageDinHeap(Heap* heap)
{
	if (heap->nrElem > 0)
	{
		Telefon aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrElem - 1];
		heap->vector[heap->nrElem - 1] = aux;

		heap->nrElem--;

		for (int i = (heap->nrElem - 2) / 2;i >= 0;i--)
		{
			filtreazaHeap(*heap, i);
		}

		return aux;
	}
}
void dezalocare(Heap* h)
{
	for (int i = 0;i < h->lungime;i++)
	{
		free(h->vector[i].brand);
		free(h->vector[i].model);
	}
	free(h->vector);
	h->vector = NULL;
	h->lungime = 0;
	h->nrElem = 0;
}
int main()
{
	Heap minHeap = citireHeapDinFisier("telefoane.txt");
	afisareHeap(minHeap);
	dezalocare(&minHeap);
}