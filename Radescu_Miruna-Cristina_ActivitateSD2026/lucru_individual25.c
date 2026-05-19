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
typedef struct NodSecundar NodSecundar;
typedef struct NodPrincipal NodPrincipal;
//lista de liste
struct NodSecundar
{
	NodPrincipal* nodP; //ma duce la nodul din lista principala (adresa)
	NodSecundar* next;
};
//LSI dar are si pointer la primul element din lista secundara
struct NodPrincipal
{
	Carte info;
	NodPrincipal* next;
	NodSecundar* listaS;
};
//creez prima data lista principala
void inserareListaPrincipala(NodPrincipal** cap, Carte c)
{
	//inseram la final 
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal)); //nodul de inserat

	nou->info = c;
	nou->next = NULL; 
	nou->listaS = NULL; //momentan n-am adaugat vecini

	if (*cap)
	{
		//ne deplasam
		NodPrincipal* p = *cap; //copie
		while (p->next)
		{
			p = p->next;
		}
		p->next = nou; //adaugam nodul
	}
	else //daca nu am inca elemente in lista
	{
		*cap = nou;//va fi primul nod
	}

}
void inserareListaSecundara(NodSecundar** cap, NodPrincipal* nodP)
{
	//la final
	NodSecundar* nou = malloc(sizeof(NodSecundar));
	nou->nodP = nodP;
	nou->next = NULL;

	if (*cap) {
		NodSecundar* p = *cap; //sa nu ne deplasam cu cap pt ca e trimis prin adresa
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {//daca e goala lista, nodul nou devine primul
		*cap = nou;
	}
}
NodPrincipal* cautaNodDupaId(NodPrincipal* cap, int id)
{
	//caut daca nu s-a terminat lista si pana la nodul cu acel id
	while (cap && cap->info.id != id)
	{
		cap = cap->next; //elementul cautat
	}
	return cap;
}
void inserareMuchie(NodPrincipal* cap, int idStart, int idStop)
{
	if (cap)
	{
		//nodurile cu id-urile
		NodPrincipal* nodStart = cautaNodDupaId(cap, idStart); 
		NodPrincipal* nodStop = cautaNodDupaId(cap, idStop);
		if (nodStart && nodStop) //doar daca exista cele doua noduri
		{
			inserareListaSecundara(&nodStart->listaS, nodStop); //la nodul start pun nodul stop in LS
			inserareListaSecundara(&nodStop->listaS, nodStart); //la nodul stop pun nodul start in LS
		}
	}
}
//citire noduri principale
NodPrincipal* citireNoduriDinFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	NodPrincipal* listaPrincipala = NULL;

	if (f)
	{
		while (!feof(f))
		{
			inserareListaPrincipala(&listaPrincipala, citireCarteDinFisier(f));
		}
		fclose(f);
	}
	return listaPrincipala;
}
void citireMuchiiDinFisier(NodPrincipal* cap, const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	//voi citi aceste id-uri
	int idStart;
	int idStop;

	if (f)
	{
		while (!feof(f))
		{
			fscanf(f, "%d %d", &idStart, &idStop);
			inserareMuchie(cap, idStart, idStop);
		}
		fclose(f);
	}
}
void dezalocare(NodPrincipal** cap)
{
	while (*cap)
	{
		NodPrincipal* nodDeSters = *cap;//retin nodul de sters

		NodSecundar* capS = nodDeSters->listaS; //vrem sa stergem lista secundara aferenta nodului

		while (capS)
		{
			NodSecundar* nodSecundarDeSters = capS; //retin
			capS = capS->next; //ma deplasez
			free(nodSecundarDeSters);//sterg
		}
		//eliberez si campurile alocate dinamic ale informatiei
		free(nodDeSters->info.autor);
		free(nodDeSters->info.titlu);
		//ma deplasez
		*cap = (*cap)->next;
		free(nodDeSters); //sterg in urma
	}
}
void afisareLista(NodPrincipal* cap, int id) //afisez nodul cu id-ul cerut si pe vecinii sai (ce are in LS)
{
	NodPrincipal* nod = cautaNodDupaId(cap, id);
	if (nod) //am gasit
	{
		NodSecundar* capS = nod->listaS;
		//parcurg lista secundara pt a afisa nodurile
		while (capS)
		{
			afisareCarte(capS->nodP->info); //afisez info-ul nodului principal gasit in lista secundara
			capS = capS->next;
		}
	}
	else
	{
		printf("Nu exista carte cu acest id\n");
	}
}
int main()
{
	NodPrincipal* graf = citireNoduriDinFisier("carti.txt");
	citireMuchiiDinFisier(graf, "Muchii.txt");

	//se afiseaza vecinii nodului 10
	afisareLista(graf, 10);

	dezalocare(&graf);
}