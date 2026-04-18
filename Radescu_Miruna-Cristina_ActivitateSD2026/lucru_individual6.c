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
typedef struct Nod Nod;
struct Nod
{
	Carte info;
	Nod* prev;
	Nod* next;

};
struct ListaDubla
{
	Nod* first;
	Nod* last;
	int nrNoduri;
};
typedef struct ListaDubla ListaDubla;
void afisareCarte(Carte carte)
{
	printf("\nId: %d", carte.id);
	printf("\nNumar pagini: %d", carte.nrPagini);
	printf("\nPret: %.2f", carte.pret);
	printf("\nTitlu: %s", carte.titlu);
	printf("\nAutor: %s", carte.autor);
	printf("\nCod raft: %c\n", carte.codRaft);
}
Carte citireCarteDinFisier(FILE* file)
{
	Carte c;
	char buff[255];
	char sep[4] = ";,\n";
	fgets(buff, 255, file);
	c.id = atoi(strtok(buff, sep));
	c.nrPagini = atoi(strtok(NULL, sep));
	c.pret = atof(strtok(NULL, sep));
	char* aux;
	aux = strtok(NULL, sep);
	c.titlu = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(c.titlu, aux);
	aux = strtok(NULL, sep);
	c.autor = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(c.autor, aux);
	c.codRaft = strtok(NULL, sep)[0];
	return c;
}
void afisareListaCartiDeLaInceput(ListaDubla lista)
{
	Nod* p = lista.first; //adresa primului nod din lista
	while (p)
	{
		afisareCarte(p->info);
		p = p->next;
	}
}
void afisareListaCartiDeLaSfarsit(ListaDubla lista)
{
	Nod* p = lista.last; //adresa primului nod din lista
	while (p)
	{
		afisareCarte(p->info);
		p = p->prev;
	}
}
void adaugaCarteInListaLaInceput(ListaDubla* lista, Carte carte)
{
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = carte;
	nodNou->prev = NULL;
	nodNou->next = lista->first;
	if (lista->first)//daca avem nod in lista
	{
		lista->first->prev = nodNou;
	}
	else
	{
		lista->last = nodNou;
	}
	lista->first = nodNou;
	lista->nrNoduri++;
}
void adaugaCarteInListaLaFinal(ListaDubla* lista, Carte carte)
{
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = carte; //shallow copy
	nodNou->prev = lista->last;//tine adresa nodului din spatele sau
	nodNou->next = NULL;//null in fata sa
	if (lista->last) //daca avem nod in lista
	{
		lista->last->next = nodNou;
	}
	else
	{
		lista->first = nodNou;
	}
	lista->last = nodNou; //acum ultimul nod este cel nou
	lista->nrNoduri++;
}
ListaDubla citireListaDublaDeCartiDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");//stream-ul
	ListaDubla lista;
	lista.first = NULL;
	lista.last = NULL;
	lista.nrNoduri = 0;
	while (!feof(file)) //nu e sf. de fisier
	{
		adaugaCarteInListaLaFinal(&lista, citireCarteDinFisier(file));
	}
	fclose(file);
	return lista;
}
void dezalocareListaDeCarti(ListaDubla* lista)
{
	Nod* p = lista->first; //ca sa nu mai scriu lista->first peste tot
	while (p)
	{
		Nod* aux = p;
		p = p->next; //ma deplasez si sterg aux care e in spate
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
	lista->first = NULL;
	lista->last = NULL;
	lista->nrNoduri = 0;
}
float calculeazaPretMediu(ListaDubla lista)
{
	float suma = 0.0;
	float medie = 0.0;
	int count = lista.nrNoduri;
	if (count > 0)
	{
		Nod* p = lista.first;
		while (p)
		{
			suma += p->info.pret;
			p = p->next;
		}
		medie = suma / count;

		return medie;
	}
	return 0;
}
void stergeCarteDupaID(ListaDubla* lista, int id)
{
	if (lista->first == NULL)
		return;

	Nod* p = lista->first;
	while (p && p->info.id != id)
	{
		p = p->next;
	}

	if (p == NULL)
		return;

	//un singur nod
	if (p->prev == NULL && p->next == NULL)
	{
		lista->first = NULL;
		lista->last = NULL;
	}
	//primul nod
	else if (p->prev == NULL)
	{
		lista->first = p->next;
		lista->first->prev = NULL;
	}
	//ultimul nod
	else if (p->next == NULL)
	{
		lista->last = p->prev;
		lista->last->next = NULL;
	}
	//nod din interior
	else
	{
		p->prev->next = p->next;
		p->next->prev = p->prev;
	}

	if (p->info.autor)
	{
		free(p->info.autor);
		p->info.autor = NULL;
	}
	if (p->info.titlu)
	{
		free(p->info.titlu);
		p->info.titlu = NULL;
	}

	free(p);
	lista->nrNoduri--;
}
char* getTitluCarteScumpa(ListaDubla lista)
{
	if (lista.first) //daca avem nod in lista
	{
		Nod* maxi = lista.first;
		Nod* p = lista.first->next;
		while (p)
		{
			if (maxi->info.pret < p->info.pret)
			{
				maxi = p; //retinem adresa nodului
			}
			p = p->next;
		}
		char* titlu = malloc(sizeof(char) * (strlen(maxi->info.titlu) + 1));
		strcpy(titlu, maxi->info.titlu);
		return titlu;
	}
	else
	{
		return NULL;
	}
}
int main()
{
	ListaDubla lista = citireListaDublaDeCartiDinFisier("carti.txt");
	afisareListaCartiDeLaInceput(lista);
	printf("\n=============================");
	afisareListaCartiDeLaSfarsit(lista);
	printf("\n=============================");
	printf("\nPretul mediu al cartilor este: %.2f",calculeazaPretMediu(lista));
	char* titluCarte = getTitluCarteScumpa(lista);
	printf("\nTitlul celei mai scumpe carti este: %s", titluCarte);
	if (titluCarte)
	{
		free(titluCarte);
	}
	stergeCarteDupaID(&lista, 1);
	printf("\n\n==Lista dupa stergere=========\n");
	afisareListaCartiDeLaInceput(lista);
	dezalocareListaDeCarti(&lista);
}