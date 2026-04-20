#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 //coada implementata prin LSI

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
};
typedef struct Nod Nod;
Carte citireCarteDinFisier(FILE* file)
{
	char buff[255];
	char sep[4] = ";,\n";
	Carte c;
	fgets(buff, 255, file);
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
void enqueue(Nod** coada, Carte c)
{
	//adaugare la inceput
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = c;
	nou->next = *coada;
	(*coada) = nou;
}
Carte deep_copy(Carte c)
{
	Carte carte;
	carte = c;
	carte.autor = malloc(strlen(c.autor) + 1);
	strcpy(carte.autor, c.autor);
	carte.titlu = malloc(strlen(c.titlu) + 1);
	strcpy(carte.titlu, c.titlu);
	return carte;
}
Carte dequeue(Nod** coada)
{
	//extragere de la sfarsit
	Carte c;
	c.id = -1;
	c.autor = NULL;
	c.titlu = NULL;
	
	if (*coada == NULL) //coada e goala
	{
		return c;
	}
	if ((*coada)->next == NULL) //daca este doar un element
	{
		Nod* aux = *coada;
		c = deep_copy(aux->info);
		*coada = NULL;

		free(aux->info.autor);
		free(aux->info.titlu);
		free(aux);

		return c;
	}
	Nod* p = *coada;
	while (p->next->next != NULL)
	{
		p = p->next;
	}

	Nod* aux = p->next; // ultimul nod
	c = deep_copy(aux->info);
	p->next = NULL;

	free(aux->info.autor);
	free(aux->info.titlu);
	free(aux);

	return c;
}
void afisareCoadaDeCarti(Nod* coada)
{
	while (coada)
	{
		afisareCarte(coada->info);
		coada = coada->next;
	}
}
Nod* citireCoadaDeCartiDinFisier(const char * numeFisier)
{
	Nod* coada = NULL;
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f))
	{
		Carte c = citireCarteDinFisier(f);
		enqueue(&coada, c); 
	}
	fclose(f);
	return coada;
}
void dezalocareCoadaDeCarti(Nod** coada)
{
	while (*coada)
	{
		Carte c = dequeue(coada);
		if (c.autor)
		{
			free(c.autor);
		}
		if (c.titlu)
		{
			free(c.titlu);
		}
	}
}
Carte* getCarteCuCodRaft(Nod** coada, char codRaft, int *counter)
{
	Nod* aux = NULL;
	Carte* rezultat = NULL;
	(*counter) = 0;
	while ((*coada))
	{
		Carte c = dequeue(coada);
		if (c.codRaft == codRaft)
		{
			(*counter)++; //numar cate sunt cu acel cod
		}
		enqueue(&aux, c);//pun la loc ce am scos
	}
	if ((*counter) == 0) //daca nu gasesc elemente
	{
		while (aux)
		{
			enqueue(coada, dequeue(&aux));
		}
		return NULL;
	}
	rezultat = (Carte*)malloc(sizeof(Carte) * (*counter));
	int j = 0;//pozitia din rezultat
	while (aux)
	{
		Carte c = dequeue(&aux);
		if (c.codRaft == codRaft)
		{
			rezultat[j] = c;
			rezultat[j].autor = (char*)malloc(strlen(c.autor) + 1);
			strcpy(rezultat[j].autor, c.autor);
			rezultat[j].titlu = (char*)malloc(strlen(c.titlu) + 1);
			strcpy(rezultat[j].titlu, c.titlu);
			j++;
		}
		enqueue(coada, c);
	}
	return rezultat;
}
int main()
{
	Nod* coada = citireCoadaDeCartiDinFisier("carti.txt");
	afisareCoadaDeCarti(coada);
	int counter = 0;
	Carte* rezultat = getCarteCuCodRaft(&coada, 'A',&counter);
	printf("\nCartile cu codul raftului cerut sunt:\n");
	for (int i = 0;i < counter;i++)
	{
		afisareCarte(rezultat[i]);
	}
	for (int i = 0;i < counter;i++)
	{
		if (rezultat[i].autor)
		{
			free(rezultat[i].autor);
		}
		if (rezultat[i].titlu)
		{
			free(rezultat[i].titlu);
		}
	}
	printf("\n");
	dezalocareCoadaDeCarti(&coada);
}