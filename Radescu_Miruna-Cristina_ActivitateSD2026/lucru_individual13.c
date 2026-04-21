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
struct Nod
{
	Carte info;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;
struct Stiva
{
	Nod* first;
	Nod* last;
};
typedef struct Stiva Stiva;
Carte citireCarteDinFisier(FILE* f)
{
	char buff[255];
	char sep[4] = ";,\n";
	fgets(buff, 255, f);
	Carte c;
	c.id = atoi(strtok(buff, sep));
	c.nrPagini = atoi(strtok(NULL, sep));
	c.pret = atof(strtok(NULL, sep));
	char* aux = strtok(NULL, sep);
	c.titlu = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(c.titlu, aux);
	aux = strtok(NULL, sep);
	c.autor = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(c.autor, aux);
	c.codRaft = strtok(NULL, sep)[0];
	return c;
}
void afisareCarte(Carte c)
{
	printf("\nId: %d", c.id);
	printf("\nNumar pagini: %d", c.nrPagini);
	printf("\nPret: %.2f", c.pret);
	printf("\nTitlu: %s", c.titlu);
	printf("\nAutor: %s", c.autor);
	printf("\nCod Raft: %c\n", c.codRaft);
}
void pushStack(Stiva* stiva, Carte c)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = c;
	nou->prev = NULL;
	nou->next = stiva->first;
	if (stiva->first)
	{
		stiva->first->prev = nou;
	}
	else //daca e goala
	{
		stiva->last = nou;
	}
	stiva->first = nou;
}
Carte popStack(Stiva* stiva)
{
	Carte c;
	c.id = -1;
	c.autor = NULL;
	c.titlu = NULL;
	if (stiva->first == NULL)
	{
		return c;
	}
	if (stiva->first->next) //daca avem mai mult de 1 element
	{
		Nod* aux = stiva->first;//nodul de extras
		stiva->first = stiva->first->next; //ma mut pe urmatorul nod
		c = aux->info;
		//c.autor = (char*)malloc(strlen(aux->info.autor) + 1);
		//strcpy(c.autor, aux->info.autor);
		//c.titlu = (char*)malloc(strlen(aux->info.titlu) + 1);
		//strcpy(c.titlu, aux->info.titlu);
		free(aux); //nu eliberez si campurile alocate dinamic
		stiva->first->prev = NULL;
	}
	else //daca am doar un elem.
	{
		Nod* aux = stiva->first;
		c = aux->info;
		free(aux);
		stiva->first = NULL;
		stiva->last = NULL;
	}
	return c;
}
Stiva citireStivaDeCartiDinFisier(const char* numeFisier)
{
	Stiva stiva;
	//nu uita!!!
	stiva.first = NULL;
	stiva.last = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f)
	{
		while (!feof(f))
		{
			Carte c = citireCarteDinFisier(f);
			pushStack(&stiva, c);
		}
		fclose(f);
	}
	return stiva;
}
void afisareStivaDeCarti(Stiva stiva)
{
	while (stiva.first)
	{
		afisareCarte(stiva.first->info);
		stiva.first = stiva.first->next;
	}
}
void dezalocareStivaDeCarti(Stiva* stiva)
{
	while (stiva->first)
	{
		Carte aux = popStack(stiva);
		if (aux.autor)
		{
			free(aux.autor);
		}
		if (aux.titlu)
		{
			free(aux.titlu);
		}
	}
}
Carte getCarteById(Stiva *stiva, int id)
{
	Carte rez;
	rez.id = -1;
	rez.autor = NULL;
	rez.titlu = NULL;
	Stiva aux;
	aux.first = NULL;
	aux.last = NULL;
	while (stiva->first)
	{
		Carte c = popStack(stiva);
		if (c.id == id)
		{
			rez = c;
			rez.autor =(char*) malloc(strlen(c.autor) + 1);
			strcpy(rez.autor, c.autor);
			rez.titlu = (char*)malloc(strlen(c.titlu) + 1);
			strcpy(rez.titlu, c.titlu);
		}
		pushStack(&aux, c);
	}
	while (aux.first)
	{
		pushStack(stiva, popStack(&aux));
	}
	return rez;
}
int main()
{
	Stiva stiva = citireStivaDeCartiDinFisier("carti.txt");
	afisareStivaDeCarti(stiva);
	Carte c = getCarteById(&stiva, 2);
	printf("\nCartea cu id-ul cautat este:\n");
	afisareCarte(c);
	if (c.autor)
	{
		free(c.autor);
	}
	if (c.titlu)
	{
		free(c.titlu);
	}
	dezalocareStivaDeCarti(&stiva);
}