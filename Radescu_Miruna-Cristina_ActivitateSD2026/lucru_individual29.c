#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Angajat
{
	int id;
	int varsta;
	float salariu;
	char* nume;
	char* departament;
};
typedef struct Angajat Angajat;

Angajat citireAngajatDinFisier(FILE* f)
{
	char buff[255];
	char sep[4] = "\n;,";
	fgets(buff, 255, f);

	Angajat a;
	a.id = atoi(strtok(buff, sep));
	a.varsta = atoi(strtok(NULL, sep));
	a.salariu = atof(strtok(NULL, sep));
	char* aux = strtok(NULL, sep);
	a.nume = malloc(strlen(aux) + 1);
	strcpy(a.nume, aux);
	aux = strtok(NULL, sep);
	a.departament = malloc(strlen(aux) + 1);
	strcpy(a.departament, aux);

	return a;
}
void afisareAngajat(Angajat a)
{
	printf("Id: %d\n", a.id);
	printf("Varsta: %d\n", a.varsta);
	printf("Salariu: %.2f\n", a.salariu);
	printf("Nume: %s\n", a.nume);
	printf("Departament: %s\n\n", a.departament);
}

typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;

struct NodPrincipal
{
	Angajat info;
	NodPrincipal* next;
	NodSecundar* listaS;
};
struct NodSecundar
{
	NodPrincipal* nodP;
	NodSecundar* next;
};

//adaugare la final in liste

void adaugareInLP(NodPrincipal** cap, Angajat a)
{
	NodPrincipal* nou = malloc(sizeof(NodPrincipal));
	nou->info = a;
	nou->next = NULL;
	nou->listaS = NULL;

	if (*cap == NULL)//e goala
	{
		*cap = nou;
	}
	else
	{
		NodPrincipal* p = *cap;
		while (p->next)
		{
			p = p->next;
		}
		p->next = nou;
	}
}
void adaugareInLS(NodSecundar** cap, NodPrincipal* nodNou)
{
	NodSecundar* nou = malloc(sizeof(NodSecundar));
	nou->nodP = nodNou;
	nou->next = NULL;

	if (*cap == NULL)//e goala
	{
		*cap = nou;
	}
	else
	{
		NodSecundar* p = *cap;
		while (p->next)
		{
			p = p->next;
		}
		p->next = nou;
	}
}
NodPrincipal* citireListaPrincipala(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	NodPrincipal* lista = NULL;
	if (f)
	{
		while (!feof(f))
		{
			adaugareInLP(&lista, citireAngajatDinFisier(f));
		}
		fclose(f);
	}
	return lista;
}
NodPrincipal* cautaNodDupaId(NodPrincipal* cap, int id)
{
	while (cap && cap->info.id != id)
	{
		cap = cap->next;
	}
	return cap;
}
void adaugaMuchie(NodPrincipal ** cap, int idStart, int idStop)
{
	NodPrincipal* nodStart = cautaNodDupaId(*cap, idStart);
	NodPrincipal* nodStop = cautaNodDupaId(*cap, idStop);

	if (nodStart && nodStop)
	{
		adaugareInLS(&nodStart->listaS, nodStop);
		adaugareInLS(&nodStop->listaS, nodStart);
	}
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
			adaugaMuchie(&cap, idStart, idStop);
		}
		fclose(f);
	}
}
void afisareGraf(NodPrincipal* cap, int id)
{
	NodPrincipal* nodCautat = cautaNodDupaId(cap, id);
	
	if (nodCautat)
	{
		NodSecundar* capS = nodCautat->listaS;
		while (capS)
		{
			afisareAngajat(capS->nodP->info);
			capS = capS->next;
		}
		
	}
}
void dezalocare(NodPrincipal** cap)
{
	while (*cap)
	{
		NodPrincipal* nodPDeSters = *cap;

		NodSecundar* p = nodPDeSters->listaS;

		while (p)
		{
			NodSecundar* nodSDeSters = p;

			p = p->next;
			
			free(nodSDeSters);
		}
		free(nodPDeSters->info.departament);
		free(nodPDeSters->info.nume);
		*cap = (*cap)->next;

		free(nodPDeSters);
	}
}
int main()
{
	NodPrincipal* graf = citireListaPrincipala("angajati.txt");
	citireMuchiiDinFisier(graf, "angajati_muchii.txt");

	afisareGraf(graf, 4);

	dezalocare(&graf);

}