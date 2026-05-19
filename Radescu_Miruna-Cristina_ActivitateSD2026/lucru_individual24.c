#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Angajat
{
	int id;
	int varsta;
	float salariu;
	char* nume;
	char* departament;
};
typedef struct Angajat Angajat;
struct Heap
{
	Angajat* vector;
	int lungime;
	int nrElemente;
};
typedef struct Heap Heap;
Angajat citireAngajatDinFisier(FILE* f)
{
	char buff[255];
	char sep[4] = "\n;,";
	fgets(buff, 255, f);

	Angajat a;
	a.id = atoi(strtok(buff, sep));
	a.varsta = atoi(strtok(NULL, sep));
	a.salariu = atof(strtok(NULL, sep));
	char* aux = strtok(NULL, sep);
	a.nume = malloc(strlen(aux) + 1);
	strcpy(a.nume, aux);
	aux = strtok(NULL, sep);
	a.departament = malloc(strlen(aux) + 1);
	strcpy(a.departament, aux);
	
		return a;
}
void afisareAngajat(Angajat a)
{
	printf("Id: %d\n", a.id);
	printf("Varsta: %d\n", a.varsta);
	printf("Salariu: %.2f\n", a.salariu);
	printf("Nume: %s\n", a.nume);
	printf("Departament: %s\n\n", a.departament);
}
Heap initializareHeap(int lungime)
{
	Heap h;
	h.lungime = lungime;
	h.nrElemente = 0;
	h.vector = (Angajat*)malloc(sizeof(Angajat) * lungime);
	return h;
}
void filteazaHeap(Heap h, int pozitie)
{
	int pozFiuSt = pozitie * 2 + 1;
	int pozFiuDr = pozitie * 2 + 2;
	int pozMax = pozitie; //pp ca nodul initial e maximul
	//pozMax = pozitia unde am gasit maximul
	if (pozFiuDr < h.nrElemente && h.vector[pozMax].id < h.vector[pozFiuDr].id)
	{
		pozMax = pozFiuDr;
	}
	if (pozFiuSt < h.nrElemente && h.vector[pozMax].id < h.vector[pozFiuSt].id)
	{
		pozMax = pozFiuSt; 
	}
	if (pozMax != pozitie)
	{
		//interschimbare
		Angajat aux = h.vector[pozitie]; //retin nodul curent
		h.vector[pozitie] = h.vector[pozMax]; // mut in locul radacinii elementul de pe pozitia cu maximul
		h.vector[pozMax] = aux; //pe pozitia unde era maximul pun fosta radacina

		if (pozMax <= (h.nrElemente - 2) / 2) //daca elementul coborat are copii
		{
			filteazaHeap(h, pozMax);
		}
	}
}
Heap citireHeapDinFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	Heap h = initializareHeap(4);
	if (f)
	{
		while (!feof(f))
		{
			h.vector[h.nrElemente++] = citireAngajatDinFisier(f); //adaug elementele in vector
		}
		fclose(f);
	}
	for (int i = (h.nrElemente - 2) / 2;i >= 0;i--)
	{
		filteazaHeap(h, i);
	}
	return h;
}
void afisareHeap(Heap h)
{
	for (int i = 0;i < h.nrElemente;i++)
	{
		afisareAngajat(h.vector[i]);
	}
}
//extragere a elementului maxim
Angajat extrageElement(Heap* h)
{
	if (h->nrElemente > 0)
	{
		//interschim prima poz cu ultima
		Angajat aux = h->vector[0];
		h->vector[0] = h->vector[h->nrElemente - 1];
		h->vector[h->nrElemente - 1] = aux;
		h->nrElemente--;
		//filtram din nou
		for (int i = (h->nrElemente - 2) / 2;i >= 0;i--)
		{
			filteazaHeap(*h, i);
		}
		return aux;
	}
}
void dezalocareHeap(Heap* h)
{
	for (int i = 0;i < h->lungime;i++)
	{
		free(h->vector[i].departament);
		free(h->vector[i].nume);
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
		afisareAngajat(h.vector[i]);
	}
}
int main()
{
	Heap h = citireHeapDinFisier("angajati.txt");
	afisareHeap(h);
	int dim = h.nrElemente;
	printf("Extragerea tuturor elementelor din heap:\n");
	for (int i = 0;i < dim;i++)
	{
		afisareAngajat(extrageElement(&h));
	}
	printf("Elementele heap-ului ascuns\n");
	afisareHeapAscuns(h);
	dezalocareHeap(&h);
}