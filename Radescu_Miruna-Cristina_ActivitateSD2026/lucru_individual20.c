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
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;

Carte citireCarteDinFisier(FILE* f)
{
	char buff[255];
	char sep[4] = "\n;,";

	fgets(buff, 255, f);

	Carte c;
	c.id = atoi(strtok(buff, sep));
	c.nrPagini = atoi(strtok(NULL, sep));
	c.pret = atof(strtok(NULL, sep));
	char* aux = strtok(NULL, sep);
	c.titlu = malloc(strlen(aux) + 1);
	strcpy(c.titlu, aux);
	aux = strtok(NULL, sep);
	c.autor = malloc(strlen(aux) + 1);
	strcpy(c.autor, aux);
	c.codRaft = strtok(NULL, sep)[0];
	return c;
}
void afisareCarte(Carte c)
{
		printf("Id: %d\n", c.id);
		printf("Nr. pagini: %d\n", c.nrPagini);
		printf("Pret: %.2f\n", c.pret);
		printf("Titlu: %s\n", c.titlu);
		printf("Autor: %s\n", c.autor);
		printf("Cod: %c\n\n", c.codRaft);
}
void adaugaCarteInArbore(Nod** radacina, Carte carteNoua)
{
	if (*radacina)
	{
		if ((*radacina)->info.id < carteNoua.id) //daca noua carte are id mai mare decat radacina atunci o adaugam pe dreapta
		{
			adaugaCarteInArbore(&(*radacina)->dreapta, carteNoua);
		}

		if ((*radacina)->info.id > carteNoua.id)//daca noua carte are id mai mic decat radacina atunci o adaugam pe stanga
		{
			adaugaCarteInArbore(&(*radacina)->stanga, carteNoua);
		}
	}
	else //la inceput arborele fiind gol
	{
		Nod* nou = malloc(sizeof(Nod)); //creez radacina

		nou->info = carteNoua;

		nou->dreapta = NULL;
		nou->stanga = NULL;

		*radacina = nou; //pointerul spre radacina arata acum spre nodul nou (el fiind acum radacina)
	}
}
Nod* citireArboreDeCartiDinFisier(const char* numeFisier)
{
	Nod* radacina = NULL; //pointer la radacina arborelui
	
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f))
	{
		adaugaCarteInArbore(&radacina, citireCarteDinFisier(f));
	}
	fclose(f);

	return radacina;
}
void afisareCartiDinArborePreordine(Nod* radacina)
{
	if (radacina)
	{
		afisareCarte(radacina->info); //afisez mai intai nodul curent

		//afisam subarbore din stanga
		afisareCartiDinArborePreordine(radacina->stanga);

		//afisam subarbore din dreapta
		afisareCartiDinArborePreordine(radacina->dreapta);
	}
}
void afisareCartiDinArboreInordine(Nod* radacina)
{
	if (radacina)
	{
		afisareCartiDinArboreInordine(radacina->stanga);

		afisareCarte(radacina->info);

		afisareCartiDinArboreInordine(radacina->dreapta);
	}
}
void dezalocareArboreDeCarti(Nod** radacina)
{
	if (*radacina)
	{
		//dezalocam copiii
		dezalocareArboreDeCarti(&(*radacina)->stanga);

		dezalocareArboreDeCarti(&(*radacina)->dreapta);

		//dezaloc parintele
		free((*radacina)->info.autor);
		free((*radacina)->info.titlu);

		free(*radacina);

		*radacina = NULL;
	}
}

Carte getCarteById(Nod* radacina, int id)
{
	if (radacina)
	{
		if (radacina->info.id == id) //daca are acelasi id cu nodul curent
		{
			Carte c = radacina->info;
			c.autor = malloc(strlen(radacina->info.autor) + 1);
			strcpy(c.autor, radacina->info.autor);
			c.titlu = malloc(strlen(radacina->info.titlu) + 1);
			strcpy(c.titlu, radacina->info.titlu);
			return c;
		}
		if (radacina->info.id < id) //daca e id-ul mai mare decat radacina => cautam in dreapta radacinii
		{
			return getCarteById(radacina->dreapta, id);
		}//altfel caut in stanga
		if (radacina->info.id > id)
		{
			return getCarteById(radacina->stanga, id);
		}
		//nu s-a gasit
		Carte c;
		c.id = -1;
		return c;
	}
}
int determinaNrDeNoduri(Nod* radacina)
{
	if (radacina)
	{
		return determinaNrDeNoduri(radacina->stanga) + determinaNrDeNoduri(radacina->dreapta) + 1;
	}
	return 0;
}
int determinaInaltime(Nod* radacina)
{
	if (radacina)
	{
		int inaltimeStanga = determinaInaltime(radacina->stanga);

		int inaltimeDreapta = determinaInaltime(radacina->dreapta);

		return 1 + fmax(inaltimeDreapta, inaltimeStanga);
	}
}
float calculeazaPretTotal(Nod* radacina)
{
	if (radacina)
	{
		return calculeazaPretTotal(radacina->stanga) + calculeazaPretTotal(radacina->dreapta) + radacina->info.pret;
	}
	return 0;
}
float calculeazaPretulCartilorUnuiAutor(Nod* radacina, const char* nume)
{
	float suma;
	if (radacina)
	{
		suma = calculeazaPretulCartilorUnuiAutor(radacina->stanga, nume) + calculeazaPretulCartilorUnuiAutor(radacina->dreapta, nume);
		if (strcmp(radacina->info.autor, nume) == 0)
		{
			suma += radacina->info.pret;
		}
		return suma;
	}
	return 0;
}
int main()
{
	Nod* arbore = citireArboreDeCartiDinFisier("carti.txt");
	printf("Parcurgerea arborelui in Inordine\n");
	afisareCartiDinArboreInordine(arbore);
	printf("Parcurgerea arborelui in Preordine\n");
	afisareCartiDinArborePreordine(arbore);
	afisareCarte(getCarteById(arbore, 1));
	printf("Numarul de noduri din arbore %d\n",determinaNrDeNoduri(arbore));
	printf("Inaltimea arborelui %d\n", determinaInaltime(arbore));
	printf("Pretul total al cartilor din arbore este %.2f\n", calculeazaPretTotal(arbore));
	printf("Pretul cartilor unui autor este %.2f\n", calculeazaPretulCartilorUnuiAutor(arbore, "Creanga"));
	dezalocareArboreDeCarti(&arbore);

}