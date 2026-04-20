#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//coada implementata prin vector

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
struct Coada
{
	Carte* vector;
	int dim;
};
typedef struct Coada Coada;
Carte citireCarteDinFisier(FILE* file)
{
	char sep[3] = ",\n";
	char linie[100];
	fgets(linie, 100, file);

	Carte c;
	c.id = atoi(strtok(linie, sep));
	c.nrPagini = atoi(strtok(NULL, sep));
	c.pret = atof(strtok(NULL, sep));

	char* aux;
	aux = strtok(NULL, sep);
	c.titlu = (char*)malloc(strlen(aux) + 1);
	strcpy(c.titlu, aux);
	aux = strtok(NULL, sep);
	c.autor = (char*)malloc(strlen(aux) + 1);
	strcpy(c.autor, aux);

	c.codRaft = (strtok(NULL, sep))[0];

	return c;
}

void afisareCarte(Carte carte)
{
	printf("Id: %d\n", carte.id);
	printf("Nr. pagini: %d\n", carte.nrPagini);
	printf("Pret: %.2f\n", carte.pret);
	printf("Titlu: %s\n", carte.titlu);
	printf("Autor: %s\n", carte.autor);
	printf("Categorie: %c\n\n", carte.codRaft);
}
void enqueue(Coada* coada, Carte c)
{
	Carte* aux = (Carte*)malloc(sizeof(Carte) * (coada->dim + 1));
	for (int i = 0;i < coada->dim;i++) //copiem elementele
	{
		aux[i] = coada->vector[i];
	}
	aux[coada->dim] = c;
	free(coada->vector);//eliberez memoria
	coada->vector = aux;
	coada->dim++;
}
Carte dequeue(Coada* coada)
{
	//scot de la inceput
	Carte carte;
	carte.id = -1;

	if (coada->dim > 0)
	{
		carte = coada->vector[0];//primul element
		Carte* aux = NULL;
		if (coada->dim > 1)
		{
			aux = (Carte*)malloc(sizeof(Carte) * (coada->dim - 1));
			for (int i = 1;i < coada->dim;i++)//copiem
			{
				aux[i - 1] = coada->vector[i];
			}
		}
		free(coada->vector);
		coada->vector = aux;
		coada->dim--;
	}
	return carte;
}
Coada citireCoadaDeCartiDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	Coada coada;
	coada.dim = 0;
	coada.vector = NULL;
	if (file)
	{
		while (!feof(file))
		{
			Carte c = citireCarteDinFisier(file);
			enqueue(&coada, c);
		}
		fclose(file);
	}
	return coada;
}
void afisareCoadaDeCarti(Coada c)
{
	for (int i = 0;i < c.dim;i++) 
	{
		afisareCarte(c.vector[i]);
    }
}
void dezalocareCoadaDeCarti(Coada* coada)
{
	while (coada->vector != NULL)
	{
		Carte carte = dequeue(coada);
		if (carte.autor != NULL)
			free(carte.autor);
		if (carte.titlu != NULL)
			free(carte.titlu);
	}
}
Carte getCarteByID(Coada* queue, int id)
{
	Carte rezultat;
	rezultat.id = -1;
	Coada aux;
	aux.dim = 0;
	aux.vector = NULL;

	while (queue->vector)
	{
		Carte carte = dequeue(queue);
		if (carte.id == id)
		{
			rezultat = carte;
			rezultat.titlu = (char*)malloc(strlen(carte.titlu) + 1);
			strcpy(rezultat.titlu, carte.titlu);

			rezultat.autor = (char*)malloc(strlen(carte.autor) + 1);
			strcpy(rezultat.autor, carte.autor);
		}
		enqueue(&aux, carte);
	}
	//refacem coada initiala
	while (aux.vector)
	{
		enqueue(queue, dequeue(&aux));
	}
	return rezultat;
}

int main()
{
	Coada coada = citireCoadaDeCartiDinFisier("carti.txt");
	afisareCoadaDeCarti(coada);
	Carte c = getCarteByID(&coada, 3);
	printf("\nCartea cu id-ul cerut este:\n");
	afisareCarte(c);
	if (c.id != -1)
	{
		if (c.autor)
		{
			free(c.autor);
		}
		if (c.titlu)
		{
			free(c.titlu);
		}
	}
	else
	{
		printf("Nu exista o carte cu acest id.\n");
	}
	return 0;
}