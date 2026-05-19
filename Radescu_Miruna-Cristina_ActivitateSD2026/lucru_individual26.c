#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
Telefon citireTelefonDinFisier(FILE* file)
{
	Telefon t;
	char buffer[255];
	char sep[4] = ",\n";

	fgets(buffer, 255, file);

	t.id = atoi(strtok(buffer, sep));
	t.memorie = atoi(strtok(NULL, sep));
	t.pret = (float)atof(strtok(NULL, sep));

	char* aux = strtok(NULL, sep);

	t.brand = (char*)malloc(strlen(aux) + 1);
	strcpy(t.brand, aux);

	aux = strtok(NULL, sep);
	t.model = (char*)malloc(strlen(aux) + 1);
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
typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;

struct NodPrincipal
{
	Telefon info;
	NodPrincipal* next;
	NodSecundar* listaS;
};
struct NodSecundar
{
	NodSecundar* next;
	NodPrincipal* nodP;
};

//inserari la sfarsit
void inserareInListaPrincipala(NodPrincipal** cap, Telefon t)
{
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));

	nou->info = t;
	nou->next = NULL;
	nou->listaS = NULL;
	
	if (*cap)
	{
		NodPrincipal* p = *cap;
		while (p->next) //ma duc la final
		{
			p = p->next;
		}
		p->next = nou; //leg
	}
	else //daca nu am lista
	{
		*cap = nou;
	}
}
void inserareInListaSecundara(NodSecundar** cap, NodPrincipal* nodP)
{
	NodSecundar* nou = (NodSecundar*)malloc(sizeof(NodSecundar));

	nou->nodP = nodP;
	nou->next = NULL;
	

	if (*cap)
	{
		NodSecundar* p = *cap;
		while (p->next) //ma duc la final
		{
			p = p->next;
		}
		p->next = nou; //leg
	}
	else //daca nu am lista
	{
		*cap = nou;
	}
}
NodPrincipal* cautaNodDupaId(NodPrincipal* cap, int id)
{
	while (cap && cap->info.id != id) //daca nu e finalul si pana cand gasesc id-ul
	{
		cap = cap->next; 
	}
	return cap;
}
//o muchie este inserata cand inseram in lista secundara cele 2 noduri
void inserareMuchie(NodPrincipal* cap, int idStart, int idStop)
{
	if (cap)
	{
		//nodurile ce sunt legate unul de altul, asadar trebuie sa adaugam in lista secundara a fiecaruia 
		NodPrincipal* nodStart = cautaNodDupaId(cap, idStart);
		NodPrincipal* nodStop = cautaNodDupaId(cap, idStop);

		if (nodStart && nodStop)
		{
			inserareInListaSecundara(&nodStart->listaS, nodStop);
			inserareInListaSecundara(&nodStop->listaS, nodStart);
		}
	}
}
NodPrincipal* citireListaPrincipalaDinFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	NodPrincipal* lista = NULL;
	if (f)
	{
		while (!feof(f))
		{
			inserareInListaPrincipala(&lista, citireTelefonDinFisier(f));
		}
		fclose(f);
	}
	return lista;
}
void citireMuchiiDinFisier(NodPrincipal *cap, const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
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
void afisareNod(NodPrincipal* cap, int id)
{
	NodPrincipal* nod = cautaNodDupaId(cap, id);

	if (nod)
	{
		NodSecundar* capS = nod->listaS;

		while (capS)
		{
			afisareTelefon(capS->nodP->info);
			capS = capS->next;
		}
	}
}
void dezalocare(NodPrincipal** cap)
{
	while (*cap)
	{
		NodPrincipal* nodDeSters = *cap;

		NodSecundar* capS = nodDeSters->listaS;

		while (capS)
		{
			NodSecundar* nodSecundarDeSters = capS;
			capS = capS->next;
			free(nodSecundarDeSters);
		}

		free(nodDeSters->info.brand);
		free(nodDeSters->info.model);

		*cap = (*cap)->next;

		free(nodDeSters);
	}
}
int main()
{
	NodPrincipal* lista = citireListaPrincipalaDinFisier("telefoane.txt");
	citireMuchiiDinFisier(lista, "Muchii.txt");

	afisareNod(lista, 10);

	dezalocare(&lista);
}