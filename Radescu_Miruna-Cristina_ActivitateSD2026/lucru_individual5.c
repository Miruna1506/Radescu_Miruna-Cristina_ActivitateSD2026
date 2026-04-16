#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct StructuraCarte Carte;
typedef struct Nod Nod;
struct StructuraCarte
{
	int id;
	int nrPagini;
	float pret;
	char* titlu;
	char* autor;
	char codRaft;
};
struct Nod
{
	Carte info;
	Nod* next;
};
void afisare_Carte(Carte carte)
{
	printf("\nID: %d", carte.id);
	printf("\nNumar pagini: %d",carte.nrPagini);
	printf("\nPret: %.2f", carte.pret);
	printf("\nTitlu: %s", carte.titlu);
	printf("\nAutor: %s", carte.autor);
	printf("\nCod raft: %c\n", carte.codRaft);
}
Carte citireCartiDinFisier(FILE* file)
{
	Carte c;
	char buff[255];
	char sep[4] = ";,\n";
	fgets(buff, 255, file); //citeste pana la new line inclusiv
	c.id = atoi(strtok(buff, sep));
	c.nrPagini = atoi(strtok(NULL, sep));
	c.pret = atof(strtok(NULL, sep));
	char* aux;
	aux = strtok(NULL, sep);
	c.titlu = (char*)malloc(strlen(aux) + 1); 
	strcpy(c.titlu, aux);
	aux = strtok(NULL, sep);
	c.autor = (char*)malloc(strlen(aux) + 1); 
	strcpy(c.autor, aux);
	c.codRaft = strtok(NULL, sep)[0];// codRaft e un char, iar strtok returneaza un sir de caractere
	return c;
}
void afisareListaDeCarti(Nod* cap)
{
	while (cap != NULL)
	{
		afisare_Carte(cap->info); //in cap de info avem Carte
		cap = cap->next;
	}
}
Carte deep_copy(Carte c)
{
	Carte copie;
	copie.id = c.id;
	copie.nrPagini = c.nrPagini;
	copie.pret = c.pret;
	copie.codRaft = c.codRaft;

	if (c.titlu != NULL)
	{
		copie.titlu = (char*)malloc(strlen(c.titlu) + 1);
		strcpy(copie.titlu, c.titlu);
	}
	else
	{
		copie.titlu = NULL;
	}

	if (c.autor != NULL)
	{
		copie.autor = (char*)malloc(strlen(c.autor) + 1);
		strcpy(copie.autor, c.autor);
	}
	else
	{
		copie.autor = NULL;
	}

	return copie;
}
void adaugaCarteInListaLaFinal(Nod** cap, Carte carteNoua) //prin adresa daca e goala lista si cap = null
{
	//nodul pe care vreau sa il adaug in lista
	Nod* nou = (Nod*)malloc(sizeof(Nod));

	nou->info = carteNoua; //shallow copy merge
	//nou->info = deep_copy((*cap)->info);

	nou->next = NULL; //va fi ultimul nod
	if (*cap) //verific daca lista e goala sau nu
	{
		//nu e goala
		Nod* p = *cap; //copie la pointerul care arata spre primul elem. din lista
		//vrem sa ajungem pe ultimul element
		while (p->next != NULL) //sa stim daca putem avansa
		{
			p = p->next;
		}
		//am ajuns la ultimul element
		p->next = nou;
		
	}
	else
	{
		//daca e goala
		*cap = nou;
	}
}
void adaugaCarteInListaLaInceput(Nod** cap, Carte carteNoua)
{
	//nodul pe care vreau sa il adaug in lista
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = carteNoua; //shallow copy
	//nou->info = deep_copy((*cap)->info);
	nou->next = *cap; 
	*cap = nou; //adresa nodului pe care-l vom insera la inceput
}
Nod* citireListaCartiDinFisier(const char* numeFisier)
{
	Nod* cap = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f)
	{
		while (!feof(f))
		{
			adaugaCarteInListaLaFinal(&cap, citireCartiDinFisier(f));//adaugam si in lista
		}
		fclose(f);
	}
	return cap;
}
void dezalocareListaCarti(Nod** cap)
{
	while (*cap)
	{
		Nod* p = *cap;
		(*cap) = (*cap)->next; //cu asta ma deplasez
		if (p->info.titlu)
		{
			free(p->info.titlu);
			p->info.titlu = NULL;
		}
		if (p->info.autor)
		{
			free(p->info.autor);
			p->info.autor = NULL;
		}
		free(p);//eliberam tot nodul
	}
}
float calculeazaPretMediu(Nod* cap)
{
	int contor = 0;
	float suma = 0.0;
	float medie = 0.0;
	while (cap)
	{
		contor++;
		suma += cap->info.pret;
		cap = cap->next;
	}
	if (contor > 0) {
		medie = suma / contor;
		return medie;
	}
	return 0;
}
float calculeazaPretulCartilorDupaAutor(Nod* cap, char* numeAutor)
{
	float suma = 0.0;
	while (cap)
	{
		if (strcmp(cap->info.autor, numeAutor) == 0)
		{
			suma += cap->info.pret;
		}
		cap = cap->next;
	}
	return suma;
}
void stergeCarteDupaCodRaft(Nod** cap, char cod)
{
	while ((*cap) && (*cap)->info.codRaft == cod) //pt a sterge primele elemente, care nu dispun de nod anterior
	{
		Nod* aux = *cap;
		*cap = (*cap)->next;//ne deplasam
		if (aux->info.titlu)
		{
			free(aux->info.titlu);
			aux->info.titlu = NULL;
		}
		if (aux->info.autor)
		{
			free(aux->info.autor);
			aux->info.autor = NULL;
		}
		free(aux);
	}
	if ((*cap)) 
	{
		Nod* p = *cap;
		while (p) 
		{
			while (p->next && p->next->info.codRaft != cod) //ma deplasez pana gasesc cel cu codul cautat
			{
				p = p->next;
			}
			if (p->next) //avem ce sa stergem
			{
				Nod* aux = p->next; //adresa nodului de sters
				p->next = aux->next;//adresa nodului de dupa cel sters
				//stergem
				if (aux->info.titlu)
				{
					free(aux->info.titlu);
					aux->info.titlu = NULL;
				}
				if (aux->info.autor)
				{
					free(aux->info.autor);
					aux->info.autor = NULL;
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
	Nod* cap = citireListaCartiDinFisier("carti.txt");
	afisareListaDeCarti(cap);
	float medie=calculeazaPretMediu(cap);
	printf("\nMedia tuturor cartilor din lista este: %.2f\n", medie);
	printf("\nSuma tuturor cartilor ale unui autor este: %.2f\n", calculeazaPretulCartilorDupaAutor(cap, "Rebreanu"));
	stergeCarteDupaCodRaft(&cap, 'A');
	printf("\n==Afisare dupa stergere in functie de codul raftului==========\n");
	afisareListaDeCarti(cap);
	dezalocareListaCarti(&cap);
	return 0;
}