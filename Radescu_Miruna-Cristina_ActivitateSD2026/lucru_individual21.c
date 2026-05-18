#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraAngajat {
	int id;
	int varsta;
	float salariu;
	char* nume;
	char* departament;
};
typedef struct StructuraAngajat Angajat;
struct Nod
{
	Angajat info;
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;

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

void adaugareAngajatInArbore(Nod** radacina, Angajat a)
{
	if (*radacina)
	{
		if ((*radacina)->info.id < a.id)
		{
			adaugareAngajatInArbore(&(*radacina)->dreapta, a);
		}
		if ((*radacina)->info.id > a.id)
		{
			adaugareAngajatInArbore(&(*radacina)->stanga, a);
		}
	}
	else //daca arborele e gol
	{
		Nod* nou = malloc(sizeof(Nod));
		nou->info = a;
		nou->dreapta = NULL;
		nou->stanga = NULL;

		*radacina = nou;
	}
}
Nod* citireArboreDeAngajatiDinFisier(char* numeFisier)
{
	Nod* radacina = NULL;
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f))
	{
		adaugareAngajatInArbore(&radacina, citireAngajatDinFisier(f));
	}
	fclose(f);
	return radacina;
}
void parcurgerePreordine(Nod* radacina)
{
	if (radacina)
	{
		afisareAngajat(radacina->info);

		parcurgerePreordine(radacina->stanga);

		parcurgerePreordine(radacina->dreapta);
	}
}
void parcurgereInordine(Nod* radacina)
{
	if (radacina)
	{
		parcurgereInordine(radacina->stanga);

		afisareAngajat(radacina->info);

		parcurgereInordine(radacina->dreapta);
	}
}
void dezalocare(Nod** radacina)
{
	if (*radacina)
	{
		dezalocare(&(*radacina)->stanga);
		dezalocare(&(*radacina)->dreapta);

		free((*radacina)->info.departament);
		free((*radacina)->info.nume);
		
		free(*radacina);

		*radacina = NULL;
	}
}
Angajat getAngajatById(Nod* radacina, int id)
{
	if (radacina)
	{
		if (radacina->info.id == id)
		{
			Angajat a = radacina->info;
			a.departament = malloc(strlen(radacina->info.departament) + 1);
			strcpy(a.departament, radacina->info.departament);
			a.nume = malloc(strlen(radacina->info.nume) + 1);
			strcpy(a.nume, radacina->info.nume);
			return a;
		}
		if (radacina->info.id < id)
		{
			return getAngajatById(radacina->dreapta, id);
		}
		if (radacina->info.id > id)
		{
			return getAngajatById(radacina->stanga, id);
		}
		//daca nu am gasit
		Angajat a;
		a.id = -1;
		return a;
	}
}
int determinaNrNoduri(Nod* radacina)
{
	if (radacina)
	{
		return determinaNrNoduri(radacina->stanga) + determinaNrNoduri(radacina->dreapta) + 1;
	}
	return 0;
}
int determinaInaltimeArbore(Nod* radacina)
{
	if (radacina)
	{
		int inaltimeStanga = determinaInaltimeArbore(radacina->stanga);
		int inaltimeDreapta = determinaInaltimeArbore(radacina->dreapta);
		return 1 + fmax(inaltimeDreapta, inaltimeStanga);
	}
}
float calculeazaSalariuTotal(Nod * radacina)
{
	if (radacina)
	{
		return calculeazaSalariuTotal(radacina->stanga) + calculeazaSalariuTotal(radacina->dreapta) + radacina->info.salariu;
	}
	return 0;
}
int main()
{
	Nod* arbore = citireArboreDeAngajatiDinFisier("angajati_arbore.txt");
	printf("Parcurgere Inordine\n");
	parcurgereInordine(arbore);
	printf("Parcurgere Preordine\n");
	parcurgerePreordine(arbore);
	printf("Angajatul cu id-ul cerut este: \n");
	afisareAngajat(getAngajatById(arbore, 1));
	printf("Numarul de noduri al arborelui este %d\n", determinaNrNoduri(arbore));
	printf("Inaltimea arborelui este: %d\n", determinaInaltimeArbore(arbore));
	printf("Salariul total al angajatilor este %.2f\n", calculeazaSalariuTotal(arbore));
	
}