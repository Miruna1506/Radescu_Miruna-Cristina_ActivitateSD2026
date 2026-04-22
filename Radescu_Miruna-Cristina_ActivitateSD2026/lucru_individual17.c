#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//LDI

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
struct Nod
{
	Telefon info;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;
struct ListaDubla
{
	Nod* last;
	Nod* first;
};
typedef struct ListaDubla ListaDubla;
Telefon citireTelefonDinFisier(FILE* f)
{
	Telefon t;
	char buff[255];
	char sep[4] = ";,\n";
	fgets(buff, 255, f);
	t.id = atoi(strtok(buff, sep));
	t.memorie = atoi(strtok(NULL, sep));
	t.pret = atof(strtok(NULL, sep));
	char* aux = strtok(NULL, sep);
	t.brand = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(t.brand, aux);
	aux = strtok(NULL, sep);
	t.model = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
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
void inserareTelefonLaInceput(ListaDubla* lista, Telefon t)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = t;
	nou->prev = NULL;
	nou->next = lista->first;
	if (lista->first)
	{
		lista->first->prev = nou;
	}
	else
	{
		lista->last = nou;
	}
	lista->first = nou;
}
void inserareTelefonLaSfarsit(ListaDubla* lista, Telefon t)
{
	Nod * nou = (Nod*)malloc(sizeof(Nod));
	nou->info = t;
	nou->next = NULL;
	nou->prev = lista->last;
	if (lista->last)
	{
		lista->last->next = nou;
	}
	else
	{
		lista->first = nou;
	}
	lista->last = nou;
}
ListaDubla citireLDDeTelefoaneDinFisier(const char* numeFisier)
{
	ListaDubla lista;
	lista.first = NULL;
	lista.last = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f)
	{
		while (!feof(f))
		{
			Telefon t = citireTelefonDinFisier(f);
			inserareTelefonLaSfarsit(&lista, t);
		}
		fclose(f);
	}
	return lista;
}
void afisareLDDeLaInceput(ListaDubla lista)
{
	while (lista.first)
	{
		afisareTelefon(lista.first->info);
		lista.first = lista.first->next;
	}
}
void afisareLDDeLaSfarsit(ListaDubla lista)
{
	while (lista.last)
	{
		afisareTelefon(lista.last->info);
		lista.last = lista.last->prev;
	}
}
void dezalocareLDDeTelefoane(ListaDubla* lista)
{
	Nod* p = lista->first;
	while (p)
	{
		Nod* aux = p;//ce vom sterge
		p = p->next;
		if (aux->info.model != NULL)
			free(aux->info.model);
		if (aux->info.brand != NULL)
			free(aux->info.brand);
		free(aux);
	}
	lista->first = NULL;
	lista->last = NULL;
}
void stergeTelefonDupaID(ListaDubla* lista, int id) {
	if (lista->first == NULL)
		return;

	Nod* p = lista->first; //copie
	while (p && p->info.id != id)
	{
		p = p->next;
	}
	if (p==NULL)
	{
		return;
	}
     if (p->prev == NULL && p->next==NULL) //daca e doar un nod
	{
		 lista->first = NULL;
		 lista->last = NULL;
	}
	else if (p->prev == NULL)//daca e primul nod
	{
		 lista->first = p->next;
		 lista->first->prev = NULL;
	}
	else if (p->next == NULL)
	{
		 lista->last = p->prev;
		 lista->last->next = NULL;
	}
	else
	 {
		 p->next->prev = p->prev;
		 p->prev->next = p->next;
	 }
	 	if (p->info.brand)
	{
		free(p->info.brand);
		p->info.brand = NULL;
	}
	if (p->info.model)
	{
		free(p->info.model);
		p->info.model = NULL;
	}

	free(p);
}

int main()
{
	ListaDubla lista = citireLDDeTelefoaneDinFisier("telefoane.txt");
	afisareLDDeLaInceput(lista);
	stergeTelefonDupaID(&lista, 10);
	printf("\nLista dupa stergere:\n");
	afisareLDDeLaInceput(lista);
	dezalocareLDDeTelefoane(&lista);
}

