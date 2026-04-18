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
//stiva
typedef struct Nod NodSimplu;
struct Nod
{
	Carte info;
	NodSimplu* next;
};
void pushStack(NodSimplu** stiva, Carte c)
{
	//adaug la inceput
	NodSimplu* nou = (NodSimplu*)malloc(sizeof(NodSimplu));
	nou->info = c;
	nou->next = *stiva;
	*stiva = nou;
}
Carte popStack(NodSimplu** stiva)
{
	Carte rezultat;
	rezultat.id = -1;
	if (*stiva)
	{
		NodSimplu* aux = *stiva; //primul din lista
		*stiva = aux->next; //ne mutam 

		//shallow copy
		rezultat = aux->info; //retin info de la nodul pe care urmeaza sa-l streg
		free(aux);//am dezalocat nodul, fara a sterge zonele de memorie spre care pointeaza titlu si autor (evit dangling pointer)
	}
	return rezultat;
}
char emptyStack(NodSimplu *stiva)  //verific daca e goala
{
	return stiva == NULL; // 1 sau 0(fals)
}
NodSimplu* citireStivaCartiDinFisier(const char* numeFisier) // pun in stiva cartile (care au info din fisier)
{
	NodSimplu* stiva = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f)
	{
		while (!feof(f))
		{
			pushStack(&stiva, citesteCarteDinFisier(f));
		}
		fclose(f);
	}
	return stiva;
}
void dezalocareStivaDeMasini(NodSimplu **stiva) {
	
		while (*stiva)
		{
			NodSimplu* aux = *stiva; //ce voi sterge
			*stiva = (*stiva)->next;//ma deplasez
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
	
}

int size(NodSimplu * stiva) {
	int counter = 0;
	while (stiva)
	{
		counter++;
		stiva = stiva->next;
	}
	return counter;
}
void afisareStivaDeCarti(NodSimplu* stiva)
{
	while (stiva)
	{
		afisareCarte(stiva->info);
		stiva = stiva->next;
	}
}
int main()
{
	NodSimplu* stiva = (NodSimplu*)malloc(sizeof(NodSimplu));
	stiva = citireStivaCartiDinFisier("carti.txt");
	afisareStivaDeCarti(stiva);
	printf("\nNumarul elementelor din stiva este: %d\n", size(stiva));
	Carte c = popStack(&stiva);
	printf("\nCartea extrasa este:\n");
	afisareCarte(c);
	c = popStack(&stiva);
	printf("\nCartea extrasa este:\n");
	afisareCarte(c);
	printf("\nNumarul elementelor din stiva este: %d\n", size(stiva));
	return 0;
}