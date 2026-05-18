#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Heap {
	int lungime; //cat spatiu aloc
	int nrMasini;//cate elem. am in vector
	Masina* vector; //vector de masini
};
typedef struct Heap Heap;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);
	m1.serie = strtok(NULL, sep)[0];
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrMasini = 0;
	heap.vector = (Masina*)malloc(sizeof(Masina) * lungime);//aloc spatiu pt vector
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) 
{
	int pozFiuSt = pozitieNod * 2 + 1;
	int pozFiuDr = pozitieNod * 2 + 2;
	int pozMax = pozitieNod; // pp ca tatal este si maxim
	if (pozFiuSt < heap.nrMasini && heap.vector[pozMax].id < heap.vector[pozFiuSt].id) //daca e mai mare decat tatal, retin maximul
	{
		pozMax = pozFiuSt;
	}

	if (pozFiuDr < heap.nrMasini && heap.vector[pozMax].id < heap.vector[pozFiuDr].id)
	{
		pozMax = pozFiuDr;
	}
	if (pozMax != pozitieNod)//daca s a schimbat maximul
	{
		Masina aux = heap.vector[pozMax];//maximul
		heap.vector[pozMax] = heap.vector[pozitieNod]; 
		heap.vector[pozitieNod] = aux; //maxim la tata
		if (pozMax <= (heap.nrMasini - 2) / 2)//daca maximul meu este si parinte
		{
			filtreazaHeap(heap, pozMax); //autoapel
		}
	}
}
 
Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r"); //stream-ul
	Heap heap = initializareHeap(10);

	while (!feof(file))
	{
		//pun datele, nrmasini = 0 initial
		heap.vector[heap.nrMasini++] = citireMasinaDinFisier(file);
	}
	fclose(file);
	for (int i = (heap.nrMasini - 2) / 2;i >= 0;i--)//parintii
	{
		filtreazaHeap(heap, i);
	}
	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0;i < heap.nrMasini;i++)
	{
		afisareMasina(heap.vector[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	for (int i = heap.nrMasini;i < heap.lungime;i++)
	{
		afisareMasina(heap.vector[i]);
	}
}

Masina extrageMasina(Heap* heap) {
	//extragem valoarea maxima
	//duc elemetul de pe prima pozitie, pe ultima si il ascund scazand :nrMasini-1
	if (heap->nrMasini > 0)
	{
		Masina aux = heap->vector[0]; //maximul
		heap->vector[0] = heap->vector[heap->nrMasini - 1];
		heap->vector[heap->nrMasini - 1] = aux;
		heap->nrMasini--;//am ascuns 

		for(int i=(heap->nrMasini-2)/2;i>=0;i--)
		{
			filtreazaHeap(*heap, i);
		}
		return aux;//shallow copy
	}
}


void dezalocareHeap(Heap* heap) {
	//dezalocarea unui vector
	for (int i = 0;i < heap->lungime;i++)
	{
		free(heap->vector[i].model);
		free(heap->vector[i].numeSofer);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrMasini = 0;
}

int main() {

	Heap heap = citireHeapDeMasiniDinFisier("masini.txt");
	afisareHeap(heap);
	printf("Masini extrase:\n");
	afisareMasina(extrageMasina(&heap));
	printf("Heap-ul ascuns:\n");
	afiseazaHeapAscuns(heap);
	dezalocareHeap(&heap);
	return 0;
}