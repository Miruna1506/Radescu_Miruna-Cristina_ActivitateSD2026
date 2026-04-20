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
Carte citireCartiDinFisier(FILE* file)
{
	Carte carte;
	char buff[255];
	char sep[4] = ";,\n";
	fgets(buff, 255, file);
	carte.id = atoi(strtok(buff, sep));
	carte.nrPagini = atoi(strtok(NULL, sep));
	carte.pret = atof(strtok(NULL, sep));
	char* aux = strtok(NULL, sep);
	carte.titlu = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(carte.titlu, aux);
	aux = strtok(NULL, sep);
	carte.autor = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(carte.autor, aux);
	carte.codRaft = strtok(NULL, sep)[0];
	return carte;
}
void afisareCarte(Carte carte)
{
	printf("\nID: %d", carte.id);
	printf("\nNumar pagini: %d", carte.nrPagini);
	printf("\nPret: %.2f", carte.pret);
	printf("\nTitlu: %s", carte.titlu);
	printf("\nAutor: %s", carte.autor);
	printf("\nCod raft: %c\n", carte.codRaft);
}
struct HashTable
{
	int dim;
	Carte* tabela;
};
typedef struct HashTable HashTable;
HashTable initializareHashTable(int dimensiune)
{
	HashTable ht;
	ht.dim = dimensiune;
	ht.tabela = (Carte*)malloc(sizeof(Carte) * dimensiune);
	for (int i = 0;i < dimensiune;i++)
	{
		ht.tabela[i].id = -1;
		ht.tabela[i].nrPagini = 0;
		ht.tabela[i].pret = 0;
		ht.tabela[i].titlu = NULL;
		ht.tabela[i].autor = NULL;
		ht.tabela[i].codRaft = '-';
	}
	return ht;
}
int calculeazaHash(const char* cheie, int dimensiune)
{
	int suma = 0;
	for (int i = 0;i < (int)strlen(cheie);i++)
	{
		suma += cheie[i];
	}
	return suma % dimensiune;
}
//Liniar Probing simplu
int inserareCarteInTabela(HashTable* ht, Carte carte)
{
	int pozitie = calculeazaHash(carte.autor, ht->dim); //calculez unde adaug cartea
	for (int i = 0;i < ht->dim;i++)
	{
		//ex: pozitie = 1 , dim = 7 => poz = 1,2,3,4,5,6,0
		int poz = (pozitie + i) % ht->dim;;

		if (ht->tabela[poz].id == -1) //daca nu e ocupata
		{
			ht->tabela[poz] = carte;
			return 1;
		}
	}
	return 0;
}
void redimensionareHashTable(HashTable* ht)
{
	int dimNoua = ht->dim * 2; //aloc mai mult spatiu decat +1
	HashTable htNou = initializareHashTable(dimNoua);
	for (int i = 0;i < ht->dim;i++)
	{
		//copiez elementele din ht in htNou
		if (ht->tabela[i].id != -1)
		{
			inserareCarteInTabela(&htNou, ht->tabela[i]);
		}
	}
	free(ht->tabela);
	ht->tabela = htNou.tabela;
	ht->dim = htNou.dim;
}
//functie de inserare cu redimensiunare (Liniar Probing)
void inserareCarteInTabelaCuRedimensionare(HashTable* ht, Carte carte)
{
	if (inserareCarteInTabela(ht, carte)==0)//nu s-a putut insera
	{
		redimensionareHashTable(ht);
		inserareCarteInTabela(ht, carte);
	}
}
HashTable citireHashTableDeCartiDinFisier(const char* numeFisier, int dimensiune )
{
	HashTable ht = initializareHashTable(dimensiune);
	FILE *f = fopen(numeFisier, "r");
	if (f)
	{
		while (!feof(f))
		{
			Carte c = citireCartiDinFisier(f);
			inserareCarteInTabelaCuRedimensionare(&ht, c);
		}
		fclose(f);
	}
	return ht;
}
void afisareHashTable(HashTable ht)
{
	for (int i = 0;i < ht.dim;i++)
	{
		if (ht.tabela[i].id != -1)
		{
			printf("\nPozitia %d:", i);
			afisareCarte(ht.tabela[i]);
		}
		else
		{
			printf("\nPozitia %d este libera.\n", i);
		}
	}
}
void dezalocareHashTable(HashTable* ht)
{
	for (int i = 0;i < ht->dim;i++)
	{
		if (ht->tabela[i].id != -1)
		{
			if (ht->tabela[i].autor)
			{
				free(ht->tabela[i].autor);
			}
			if (ht->tabela[i].titlu)
			{
				free(ht->tabela[i].titlu);
			}
		}
	}
	free(ht->tabela);
	ht->tabela = NULL;
	ht->dim = 0;
}
int main()
{
	HashTable ht = citireHashTableDeCartiDinFisier("carti.txt", 7);
	afisareHashTable(ht);
	dezalocareHashTable(&ht);
	return 0;
}