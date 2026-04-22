#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//LSI

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
};
typedef struct Nod Nod;

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
void adaugareTelefonLaInceputInLista(Nod** cap, Telefon t)
{
	//la inceput
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = t;
	nou->next = *cap;
	*cap = nou;
}
void adaugareTelefonLaSfarsitInLista(Nod** cap, Telefon t)
{
	Nod* p = *cap;
	Nod * nou = (Nod*)malloc(sizeof(Nod));
	nou->info = t;
	nou->next = NULL;
	if (p) //daca avem elemente in lista
	{
		while (p->next)
		{
			p = p->next;
		}
		p->next = nou;
	}
	else
	{
		*cap = nou;
	}
}
Nod* citireListaDeTelefoane(const char* numeFisier)
{
	Nod* lista = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f)
	{
		while (!feof(f))
		{
			Telefon t = citireTelefonDinFisier(f);
			adaugareTelefonLaSfarsitInLista(&lista, t);
		}
		fclose(f);
	}
	return lista;
}
void afisareListaDeTelefoane(Nod* cap)
{
	while (cap)
	{
		afisareTelefon(cap->info);
		cap = cap->next;
	}
}
void dezalocareListaDeTelefoane(Nod** cap)
{
	Nod* p = *cap;
	while (p)
	{
		Nod* aux = p;//de sters
		p = p->next;//ma deplasez
		if (aux->info.brand)
		{
			free(aux->info.brand);
		}
		if (aux->info.model)
		{
			free(aux->info.model);
		}
		free(aux);
	}
	*cap = NULL;
}
float calculeazaPretMediuDupaBrand(Nod* cap, char* brand)
{
	float suma = 0.0;
	int counter = 0;
	float medie = 0.0;
	while (cap)
	{
		if (strcmp(cap->info.brand, brand) == 0)
		{
			counter++;
			suma += cap->info.pret;
		}
		cap = cap->next;
	}
	if (counter != 0)
	{
		medie = suma / counter;
		return medie;
	}
	else
	{
		return 0;
	}
}
void stergeTelefonDupaSistem(Nod** cap, char sistem)
{
	while ((*cap) && (*cap)->info.sistem == sistem)
	{
		Nod* aux = *cap; //nodul de sters
		(*cap) = (*cap)->next;//ajunge pe null daca toate corespund
		if (aux->info.brand)
		{
			free(aux->info.brand);
		}
		if (aux->info.model)
		{
			free(aux->info.model);
		}
		free(aux);
	}
	if (*cap)//daca nu e goala lista
	{
		Nod* p = *cap;
		while (p)
		{
			while (p->next && p->next->info.sistem != sistem) // cand e = ne oprim 
			{
				p = p->next;//inainte de nodul cautat
			}
			if (p->next)
			{
				Nod* aux = p->next;//nodul de sters
				p->next = aux->next;
				//stergem
				if (aux->info.brand)
				{
					free(aux->info.brand);
				}
				if (aux->info.model)
				{
					free(aux->info.model);
				}
				free(aux);
			}
			else
			{
				p = NULL;
			}
		}
	}
}
int main()
{
	Nod* lista = citireListaDeTelefoane("telefoane.txt");
	afisareListaDeTelefoane(lista);
	stergeTelefonDupaSistem(&lista, 'I');
	printf("\nLista dupa stergere\n");
	afisareListaDeTelefoane(lista);
	float medie = calculeazaPretMediuDupaBrand(lista, "Samsung");
	printf("\nPretul mediu al brandului este: %.2f\n", medie);
	dezalocareListaDeTelefoane(&lista);
}