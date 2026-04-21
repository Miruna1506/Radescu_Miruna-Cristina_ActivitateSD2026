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
struct HashTable
{
	Telefon* vector;
	int dim;
};
typedef struct HashTable HashTable;
int calculareCodHash(int dimensiune, int id)
{
	return (id * id) % dimensiune;
}
HashTable  initializareHashTable(int dimensiune)
{
	HashTable ht;
	ht.vector =(Telefon*) malloc(sizeof(Telefon) * dimensiune);
	ht.dim = dimensiune;
	for (int i = 0;i < dimensiune;i++)
	{
		Telefon t;
		t.id = -1;
		t.brand = NULL;
		t.model = NULL;

		ht.vector[i] = t;
	}
	return ht;
}
int adaugareTelefonInTabela(HashTable* ht,Telefon t)
{
	int pozitie = calculareCodHash(ht->dim, t.id);

	if (ht->vector[pozitie].id == -1) //daca nu avem coliziune
	{
		ht->vector[pozitie] = t;
		return 1;
	}
	else
	{
		for (int i = pozitie + 1; i < ht->dim;i++) //verific la dreapta
		{
			if (ht->vector[i].id == -1)
			{
				ht->vector[i] = t;
				return 1;
			}
		}
	}
	return 0;
}
void redimensionareTabelaDeTelefoane(HashTable *ht)
{
	HashTable aux= initializareHashTable(ht->dim + 1);
	for (int i = 0;i < ht->dim;i++)
	{
		if (ht->vector[i].id != -1)
		{
			adaugareTelefonInTabela(&aux, ht->vector[i]);
		}
	}
	free(ht->vector);
	ht->vector = aux.vector;
	ht->dim = aux.dim;
}
void adaugareTelefonInTabelaCuRedimensionare(HashTable* ht, Telefon t)
{
	if (adaugareTelefonInTabela(ht, t) == 0) // nu s-a putut insera
	{
		redimensionareTabelaDeTelefoane(ht);
		adaugareTelefonInTabela(ht, t);
	}
}
HashTable citireTabelaDeTelefoaneDinFisier(const char* numeFisier, int dim)
{
	HashTable ht = initializareHashTable(dim);
	FILE* f = fopen(numeFisier, "r");
	if (f)
	{
		while (!feof(f))
		{
			Telefon t = citireTelefonDinFisier(f);
			adaugareTelefonInTabelaCuRedimensionare(&ht, t);
		}
		fclose(f);
	}
	return ht;
}
void afisareTabelaDeTelefoane(HashTable ht)
{
	for (int i = 0;i < ht.dim;i++)
	{
		if (ht.vector[i].id != -1)
		{
			printf("\nPozitia %d:\n", i);
			afisareTelefon(ht.vector[i]);
		}
		else
		{
			printf("\nPozitia %d este libera. \n ", i);
		}
	}
}
void dezalocareTabelaDeTelefoane(HashTable* ht)
{
	for (int i = 0;i < ht->dim;i++)
	{
		if (ht->vector[i].id != -1)
		{
			if (ht->vector[i].brand)
			{
				free(ht->vector[i].brand);
			}
			if (ht->vector[i].model)
			{
				free(ht->vector[i].model);
			}
		}
	}
}
int main()
{
	HashTable ht = citireTabelaDeTelefoaneDinFisier("telefoane.txt",8);
	afisareTabelaDeTelefoane(ht);
	dezalocareTabelaDeTelefoane(&ht);
}