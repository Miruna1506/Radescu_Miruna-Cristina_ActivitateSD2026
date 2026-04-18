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
Carte citesteCarteDinFisier(FILE* file)
{
	char buff[255];
	char sep[4] = ";,\n";
	Carte carte;
	fgets(buff, 255, file);
	carte.id = atoi(strtok(buff, sep));
	carte.nrPagini = atoi(strtok(NULL, sep));
	carte.pret = atof(strtok(NULL, sep));
	char* aux = strtok(NULL, sep);
	carte.titlu = malloc(strlen(aux) + 1);
	strcpy(carte.titlu, aux);
	aux = strtok(NULL, sep);
	carte.autor = malloc(strlen(aux) + 1);
	strcpy(carte.autor, aux);
	carte.codRaft = strtok(NULL, sep)[0];
	return carte;
}
void afisareCarte(Carte carte)
{
	printf("\nId: %d", carte.id);
	printf("\nNumar pagini: %d", carte.nrPagini);
	printf("\nPret: %.2f", carte.pret);
	printf("\nTitlu: %s", carte.titlu);
	printf("\nAutor: %s", carte.autor);
	printf("\nCod raft: %c\n", carte.codRaft);
}
typedef struct Nod Nod;
struct Nod
{
	Carte info;
	Nod* prev;
	Nod* next;
};
typedef struct Coada Coada;
struct Coada //LDI
{
	Nod* first;
	Nod* last;
};
void enqueue(Coada* coada, Carte carte)
{
	//adaugare la inceput
	Nod* nou = malloc(sizeof(Nod));
	nou->info = carte;
	nou->prev = NULL;
	nou->next = coada->first;
	if (coada->first)// daca am nod in coada
	{
		coada->first->prev = nou;//retin adresa noului nod
	}
	else
	{
		coada->last = nou; //daca nu am nod in coada si last are adresa noului nod
	}
	coada->first = nou;//first are oricum adresa noului nod
}
Carte dequeue(Coada* coada)
{
	if (coada->first == NULL) //daca nu am elemente in coada
	{
		Carte rezultat;
		rezultat.id = -1;
		return rezultat;
	}
	Carte rezultat = coada->last->info; //shallow copy
	if (coada->last == coada->first)//un singur element
	{
		//sterg
		free(coada->first);//nu dezaloc si campurile alocate dinamic din Carte
		coada->first = NULL;
		coada->last = NULL;
	}
	else
	{
		coada->last = coada->last->prev;
		free(coada->last->next);
		coada->last->next = NULL; //pun null la next-ul nodului care acum este ultimul
	}
	return rezultat;
}
Coada citireCoadaDeCartiDinFisier(const char* numeFisier)
{
	Coada coada;
	coada.first = NULL;
	coada.last = NULL;
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f))
	{
	 enqueue(&coada, citesteCarteDinFisier(f));
	}
	fclose(f);
	return coada;
}
/*void dezalocareCoadaDeCarti(Coada* coada) //trimit prin adresa
{
	while (coada->first)
	{
		Nod * aux = coada->first; //ce vreau sa sterg
		coada->first = coada->first->next;//ma deplasez
		if (aux->info.autor)
		{
			free(aux->info.autor);
		}
		if (aux->info.titlu)
		{
			free(aux->info.titlu);
		}
		free(aux);
	}
	coada->first = NULL;
	coada->last = NULL;
}*/
void dezalocareCoadaDeCarti(Coada* coada)
{
	while (coada->first)
	{
		Carte c = dequeue(coada);
		if (c.titlu)
		{
			free(c.titlu);
			c.titlu = NULL;
		}

		if (c.autor)
		{
			free(c.autor);
			c.autor = NULL;
		}
	}
}
void afisareCoadaDeCarti(Coada coada)
{
	while (coada.first)
	{
		afisareCarte(coada.first->info);
		coada.first = coada.first->next;
	}
}
Carte deep_copy(Carte c)
{
	Carte carte;
	carte.id = c.id;
	carte.pret = c.pret;
	carte.autor = malloc(sizeof(char) * (strlen(c.autor) + 1));
	strcpy(carte.autor, c.autor);
	carte.titlu = malloc(sizeof(char) * (strlen(c.titlu) + 1));
	strcpy(carte.titlu, c.titlu);
	carte.nrPagini = c.nrPagini;
	carte.codRaft = c.codRaft;
	return carte;

}
/*Carte getCarteById(Coada* coada, int id)
{
	Carte rezultat;
	rezultat.id = -1;
	rezultat.titlu = NULL;
	rezultat.autor = NULL;
	Nod* p = coada->first;
	while (p && p->info.id != id)//ne oprim pe nodul cu cartea cu id-ul cerut
	{
		p = p->next;
	}
	if (p)
	{
		rezultat = deep_copy(p->info);
	}
	return rezultat;
}*/
Carte getCarteById(Coada* coada, int id)
{
	Coada aux;
	aux.first = NULL;
	aux.last = NULL;
	Carte rezultat;
	rezultat.id = -1;
	while (coada->first)
	{
		Carte c = dequeue(coada); //extrag cartile si 
		if (c.id == id)
		{
			rezultat = deep_copy(c);
		}
		//punem la loc nodurile
		enqueue(&aux, c);
	}
	coada->first = aux.first;
	coada->last = aux.last;
	return rezultat;
}
int main()
{
	Coada coada = citireCoadaDeCartiDinFisier("carti.txt");
	afisareCoadaDeCarti(coada);
	Carte c = dequeue(&coada);
	printf("\nCartea scoasa din coada este:\n");
	afisareCarte(c);
	free(c.autor);
	free(c.titlu);
	Carte rezultat = getCarteById(&coada, 2);
	printf("\nCartea cu id-ul cerut este:\n");
	afisareCarte(rezultat);
	free(rezultat.autor);
	free(rezultat.titlu);
	dezalocareCoadaDeCarti(&coada);
	return 0;
}