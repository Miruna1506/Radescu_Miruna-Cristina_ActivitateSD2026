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
struct Heap
{
	int lungime;
	int nrCarti;
	Carte* vector;
};
typedef struct Heap Heap;

Carte citireCarteDinFisier(FILE* file)
{
	char buff[255]; //buffer in care citesc o linie din fisier
	char sep[4] = "\n;,";

	fgets(buff, 255, file); // citim o linie intreaga

	Carte c;

	c.id = atoi(strtok(buff, sep));
	c.nrPagini = atoi(strtok(NULL,sep));
	c.pret = atof(strtok(NULL,sep));
	char* aux;
	aux = strtok(NULL, sep);
	c.titlu = malloc(strlen(aux) + 1);
	strcpy(c.titlu, aux);
	aux = strtok(NULL, sep);
	c.autor = malloc(strlen(aux) + 1);
	strcpy(c.autor, aux);
	c.codRaft = strtok(NULL, sep)[0];

	return c;
}
void afisareCarte(Carte carte)
{
		printf("Id: %d\n", carte.id);
		printf("Nr. pagini: %d\n", carte.nrPagini);
		printf("Pret: %.2f\n", carte.pret);
		printf("Titlu: %s\n", carte.titlu);
		printf("Autor: %s\n", carte.autor);
		printf("Cod: %c\n\n", carte.codRaft);
	
}
Heap initializareHeap(int lungime)
{
	Heap h;
	h.lungime = lungime; //setez capacitatea heap-ului
	h.nrCarti = 0;
	//aloc vector
	h.vector = (Carte*)malloc(sizeof(Carte) * lungime);
	return h;
}
void filtreazaHeap(Heap h, int pozitie)
{
	int pozFiuSt = pozitie * 2 + 1;
	int pozFiuDr = pozitie * 2 + 2;
	int pozMax = pozitie; //pp. ca max este chiar nodul curent (tatal)
	//verificam daca fii au id mai mare decat maximul 
	if (pozFiuSt < h.nrCarti && h.vector[pozMax].id < h.vector[pozFiuSt].id)
	{
		pozMax = pozFiuSt;
	}
	if (pozFiuDr < h.nrCarti && h.vector[pozMax].id < h.vector[pozFiuDr].id)
	{
		pozMax = pozFiuDr;
	}
	//daca maximul nu este nodul curent => interschimbare!!
	if (pozMax != pozitie)//daca s-a schimbat maximul
	{
		//interschimbare
		Carte aux = h.vector[pozitie]; //tatal
		//schimb cu fiul
		h.vector[pozitie] = h.vector[pozMax]; 
		h.vector[pozMax] = aux;

		//filtram in cazul in care elementul coborat are copii
		if (pozMax <= (h.nrCarti - 2) / 2)
		{
			filtreazaHeap(h, pozMax);//autoapel
		}
	}
}
Heap citireHeapCartiDinFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);
	while (!feof(f))
	{
		heap.vector[heap.nrCarti++] = citireCarteDinFisier(f);
	}
	fclose(f);
	for (int i = (heap.nrCarti - 2) / 2;i >= 0;i--) //parcurg parintii
	{
		filtreazaHeap(heap, i);
	}
	return heap;
}
void afisareHeap(Heap h)
{
	for (int i = 0;i < h.nrCarti;i++)
	{
		afisareCarte(h.vector[i]);
	}
}
Carte extrageCarte(Heap* h)
{
	if (h->nrCarti > 0) //daca am carti care nu sunt ascunse
	{
		Carte aux = h->vector[0]; //primul elem
		//schimb cu ultimul element
		h->vector[0] = h->vector[h->nrCarti - 1]; 
		h->vector[h->nrCarti - 1] = aux;
		h->nrCarti--; //ascund ultimul element

		//filtrez din nou heap-ul (Ca in citireHeap)
		for (int i = (h->nrCarti - 2) / 2;i >= 0;i--)
		{
			filtreazaHeap(*h, i);
		}
		return aux;
	}
}
void dezalocareHeap(Heap* h)
{
	for (int i = 0; i < h->lungime;i++)
	{
		free(h->vector[i].autor);
		free(h->vector[i].titlu);
	}
	free(h->vector);
	h->vector = NULL;
	h->lungime = 0;
	h->nrCarti = 0;
}
void afisareHeapAscuns(Heap h)
{
	for (int i = h.nrCarti;i < h.lungime;i++)
	{
		afisareCarte(h.vector[i]);
	}
}
int main()
{
	Heap heap = citireHeapCartiDinFisier("carti.txt");
	afisareHeap(heap);
	printf("Carti extrase\n");
	int nrInitial = heap.nrCarti; //retin ca sa pot parcurge toate Cartile
	for (int i = 0;i < nrInitial; i++)
	{
		afisareCarte(extrageCarte(&heap));//trimis prin adresa deci se va modifica nr. de Carti din memorie
	}
	dezalocareHeap(&heap);
}