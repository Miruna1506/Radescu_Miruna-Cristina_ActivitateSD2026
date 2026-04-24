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
	int lungime; 
	Masina* vector;
	int nrElemente;
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

	m1.serie = *strtok(NULL, sep);
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
	Heap h;
	h.lungime = lungime;
	h.nrElemente = 0;
	h.vector = (Masina*)malloc(sizeof(Masina) * lungime);
	return h;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int fiuStg = (2 * pozitieNod) + 1;
	int fiuDr = (2 * pozitieNod) + 2;
	int pMax = pozitieNod;
	if (heap.vector[pMax].pret < heap.vector[fiuStg].pret && fiuStg < heap.nrElemente) {
		pMax = fiuStg;
	}
	if (heap.vector[pMax].pret < heap.vector[fiuDr].pret && fiuDr < heap.nrElemente) {
		pMax = fiuDr;
	}
	if (pMax != pozitieNod) {
		Masina aux = heap.vector[pMax];
		heap.vector[pMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		if (pMax < (heap.nrElemente - 2) / 2) {
			filtreazaHeap(heap, pMax);
		}
	}
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Heap h = initializareHeap(10);
	int i = 0;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		h.vector[i] = m;
		i++;
	}
	h.nrElemente = i;
	fclose(f);
	for (int j = (h.nrElemente - 2) / 2;j >= 0;j--) {
		filtreazaHeap(h, j);
	}
	return h;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrElemente; i++) {
		afisareMasina(heap.vector[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	for (int i = heap.nrElemente; i < heap.lungime; i++) {
		afisareMasina(heap.vector[i]);
	}
}

Masina extrageMasina(Heap* heap) {
	Masina m;
	m.id = -1;
	if (heap->nrElemente != 0) {
		m = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrElemente - 1];
		heap->vector[heap->nrElemente - 1] = m;
		heap->nrElemente--;
		for (int i = (heap->nrElemente - 2) / 2;i >= 0;i--) {
			filtreazaHeap(*heap, i);
		}
	}

	return m;
}


void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->vector[i].model);
		free(heap->vector[i].numeSofer);
	}
	free(heap->vector);
	heap->lungime = 0;
	heap->nrElemente = 0;
	heap->vector = NULL;
}

int main() {
	Heap h = citireHeapDeMasiniDinFisier("masini.txt");
	afisareHeap(h);
	printf("==========================================================\n");
	printf("Extrageri:\n");
	afisareMasina(extrageMasina(&h));
	afisareMasina(extrageMasina(&h));
	afisareMasina(extrageMasina(&h));
	printf("==========================================================\n");
	printf("Heap ascuns:\n");
	afiseazaHeapAscuns(h);
	dezalocareHeap(&h);
	afisareHeap(h);

	return 0;
}