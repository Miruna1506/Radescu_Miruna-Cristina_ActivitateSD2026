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

	if (masina.id == -1)
	{
		 printf("Masina nu exista");
		 return;
	}
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

struct NodSimplu
{
	Masina info;
	struct NodSimplu* next;
};
typedef struct NodSimplu NodSimplu;
void pushStack(NodSimplu** stack,Masina masina) 
{
	//adaugam la inceput
	NodSimplu* nod = (NodSimplu*)malloc(sizeof(NodSimplu));

	nod->info = masina;
	nod->next = *stack;
	*stack = nod;
}

Masina popStack(NodSimplu**stack) 
{
	//extragem + stergere de la inceput
	Masina rezultat;
	rezultat.id = -1;
	if (*stack != NULL)
	{
		NodSimplu* aux = *stack;
		*stack = aux->next;

		rezultat=aux->info;
		free(aux);
	}
	return rezultat;
}

char emptyStack(NodSimplu*stack) 
{
	return stack == NULL;
}

NodSimplu* citireStackMasiniDinFisier(const char* numeFisier) 
{
	NodSimplu* stack = NULL;
	FILE* file = fopen(numeFisier, "r");
	while (!feof(file))
	{
		pushStack(&stack, citireMasinaDinFisier(file));
	}
	fclose(file);
	return stack;
}

void dezalocareStivaDeMasini(/*stiva*/) {
	//sunt dezalocate toate masinile si stiva de elemente
}

int size(/*stiva*/) {
	//returneaza numarul de elemente din stiva
}

//QUEUE
struct Nod
{
	Masina info;
	struct Nod* prev;
	struct Nod* next;
};
typedef struct Nod Nod;
struct Queue //lista dubla inlantuita
{
	Nod* start;
	Nod* end;
};
typedef struct Queue Queue;
void enqueue(Queue* queue,Masina masina) 
{
	//adaugam la inceput
	Nod* nod=(Nod*)malloc(sizeof(Nod)); 

	nod->info = masina;
	nod->next = queue->start;
	nod->prev = NULL;

	if (queue->start != NULL) 
	{
		queue->start->prev = nod;
	}
	else
	{
		queue->end = nod;
	}
	queue->start = nod;
}

Masina dequeue(Queue* queue) 
{
	//extragem de la final
	if (queue->start == NULL)
	{
		Masina masina;
		masina.id = -1;
		return masina;
	}

	Masina masina = queue->end->info;

	if (queue->start == queue->end) //if(queue->end->prev==NULL)
	{
		free(queue->start); 

		queue->start = NULL;
		queue->end = NULL;
	}
	else
	{
		queue->end = queue->end->prev;
		free(queue->end->next);
		queue->end->next = NULL;
	}
	return masina;
}

Queue citireCoadaDeMasiniDinFisier(const char* numeFisier) 
{
	Queue queue;
	queue.end = NULL;
	queue.start = NULL;

	FILE* file = fopen(numeFisier, "r");
	while (!feof(file))
	{
		Masina masina = citireMasinaDinFisier(file);
		enqueue(&queue, masina);
	}
	fclose(file);
	return queue;
}

void dezalocareCoadaDeMasini(/*coada*/) 
{
	//sunt dezalocate toate masinile si coada de elemente
}

//metode de procesare
Masina getMasinaByID(Queue* queue,int id)
{
	Queue aux;
	aux.end = NULL;
	aux.start = NULL;
	
	Masina rezultat;
	rezultat.id = -1;
	while (queue->start != NULL)
	{
		Masina masina = dequeue(queue);
		if (masina.id == id)
		{
			rezultat = masina;
			rezultat.model = (char*)malloc(strlen(masina.model)+1);
			strcpy(rezultat.model, masina.model);
			rezultat.numeSofer = (char*)malloc(strlen(masina.numeSofer)+1);
			strcpy(rezultat.numeSofer, masina.numeSofer);
		}
		enqueue(&aux, masina);
	}
	queue->end = aux.end;
	queue->start = aux.start;
	return rezultat;
}
float calculeazaPretTotal(NodSimplu**stack)
{
	NodSimplu* aux = NULL;
	float total = 0;
	while (!emptyStack(&stack))
	{
		Masina masina = popStack(stack);
		total += masina.pret;
		pushStack(&aux, masina);
	}
	//pentru a reintoarce stiva
	while (!emptyStack(&aux))
	{
		pushStack(stack, popStack(&aux));
	}
	return total;
}

int main() 
{
	Queue queue;
	queue.end = NULL;
	queue.start = NULL;
	queue = citireCoadaDeMasiniDinFisier("masini.txt");
	/*afisareMasina(dequeue(&queue));
	afisareMasina(dequeue(&queue));*/

	Masina masina = getMasinaByID(&queue, 9);
	afisareMasina(getMasinaByID(&queue, 9));
	free(masina.model);
	free(masina.numeSofer);

	//afisareMasina(dequeue(&queue));
	return 0;
}