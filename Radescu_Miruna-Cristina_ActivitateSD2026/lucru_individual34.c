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
struct Heap
{
	Produs* vector;
	int lungime;
	int nrElemente;
};
typedef struct Heap Heap;
Heap initializare(int lungime)
{
	Heap h;
	h.lungime = lungime;
	h.nrElemente = 0;
	h.vector = malloc(sizeof(Produs) * h.lungime);
	return h;
}
void filtreazaHeap(Heap h, int pozitie)
{
	int pozFiuSt = pozitie * 2 + 1;
	int pozFiuDr = pozitie * 2 + 2;
	int pozMax = pozitie;

	if (pozFiuDr < h.nrElemente && h.vector[pozMax].id < h.vector[pozFiuDr].id)
	{
		pozMax = pozFiuDr;
	}
	if (pozFiuSt < h.nrElemente && h.vector[pozMax].id < h.vector[pozFiuSt].id)
	{
		pozMax = pozFiuSt;
	}
	if (pozitie != pozMax)
	{
		Produs aux = h.vector[pozitie];
		h.vector[pozitie] = h.vector[pozMax];
		h.vector[pozMax] = aux;

		if (pozMax <= (h.nrElemente - 2) / 2)
		{
			filtreazaHeap(h, pozMax);
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
			h.vector[h.nrElemente++] = citireProdusDinFisier(f);
		}
		fclose(f);
	}
	for (int i = (h.nrElemente - 2) / 2;i >= 0;i--)
	{
		filtreazaHeap(h, i);
	}
	return h;
}
void afisareHeap(Heap h)
{
	for (int i = 0;i < h.nrElemente;i++)
	{
		afisareProdus(h.vector[i]);
	}
}
Produs extrageDinHeap(Heap* h)
{
	if (h->nrElemente > 0)
	{
		Produs aux = h->vector[0];
		h->vector[0] = h->vector[h->nrElemente - 1];
		h->vector[h->nrElemente - 1] = aux;

		h->nrElemente--;

		for (int i = (h->nrElemente - 2) / 2;i >= 0;i--)
		{
			filtreazaHeap(*h, i);
		}
		return aux;
	}
}
void dezalocare(Heap* h)
{
	for (int i = 0;i < h->lungime;i++)
	{
		free(h->vector[i].denumire);
	}
	free(h->vector);
	h->vector = NULL;
	h->lungime = 0;
	h->nrElemente = 0;
}
void afisareHeapAscuns(Heap h)
{
	for (int i = h.nrElemente;i < h.lungime;i++)
	{
		afisareProdus(h.vector[i]);
	}
}
int main()
{
	Heap heap = citireHeapDinFisier("produse.txt");
	afisareHeap(heap);

	printf("Extrage din heap:\n");
	afisareProdus(extrageDinHeap(&heap));
	
	printf("Afisare Heap ascuns:\n");
	afisareHeapAscuns(heap);
	dezalocare(&heap);
}
