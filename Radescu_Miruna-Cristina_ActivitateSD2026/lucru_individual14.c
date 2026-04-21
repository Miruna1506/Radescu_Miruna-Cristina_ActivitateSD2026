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
struct Nod
{
	Telefon info;
	struct Nod* next;
};
typedef struct Nod Nod;
struct HashTable
{
	Nod** vector;
	int dim;
};
typedef struct HashTable HashTable;
void adaugareTelefonInLista(Nod** lista, Telefon t)
{
	//la sfarsit
	Nod* nou = malloc(sizeof(Nod));
	nou->info = t;
	nou->next = NULL;
	Nod* aux = *lista; //ma deplasez cu copia
	if (*lista)
	{


		while (aux->next) //pana ajung pe ultimul nod
		{
			aux = aux->next;
		}
		aux->next = nou;
	}
	else
	{
		//daca e goala
		*lista = nou;
	}
}
void afisareListaDeTelefoane(Nod* lista)
{
	while (lista)
	{
		afisareTelefon(lista->info);
		lista = lista->next;
	}
}
void dezalocareListaDeTelefoane(Nod** lista)
{
	Nod* p = *lista;
	while (p)
	{
		Nod* aux = p;
		p = p->next;//ma deplasez
		if (aux->info.brand)
		{
			free(aux->info.brand);
		}
		if (aux->info.model)
		{
			free(aux->info.model);
		}
		free(aux);
	}
	*lista = NULL;
}
HashTable initializareTabela(int dimensiune)
{
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = malloc(sizeof(Nod*)*dimensiune);
	for (int i = 0;i < dimensiune;i++)
	{
		ht.vector[i] = NULL;
	}
	return ht;
}
int calculeazaCodHash(int dimensiune, int id)
{
	id += 23;
	return (id*id) % dimensiune;
}
void adaugareInTabelaDeTelefoane(HashTable* ht, Telefon t )
{
	int pozitie = calculeazaCodHash(ht->dim, t.id);
	if (ht->vector[pozitie] == NULL) //nu avem coliziune
	{
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		ht->vector[pozitie] = nou;
		ht->vector[pozitie]->info = t;
		ht->vector[pozitie]->next = NULL;
	}
	else //avem coliziune
	{
		adaugareTelefonInLista((&ht->vector[pozitie]), t);
	}
}
HashTable citireTabelaDeTelefoane(const char* numeFisier, int dim)
{
	HashTable ht = initializareTabela(dim);
	FILE* f = fopen(numeFisier, "r");
	if (f)
	{
		while (!feof(f))
		{
			Telefon t = citireTelefonDinFisier(f);
			adaugareInTabelaDeTelefoane(&ht, t);
		}
	}
	return ht;
}
void afisareTabelaDeTelefoane(HashTable ht)
{
	for (int i = 0; i < ht.dim;i++)
	{
		if (ht.vector[i])
		{
			printf("\nTelefoanele de pe pozitia %d sunt:\n", i);
			afisareListaDeTelefoane(ht.vector[i]);
		}
		else
		{
			printf("\nPe pozitia %d, nu avem telefoane.\n", i);
		}
	}
}
void dezalocareTabela(HashTable* ht)
{
	for (int i = 0;i < ht->dim;i++)
	{
		dezalocareListaDeTelefoane(&ht->vector[i]);
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}
float* calculeazaPreturiMediiPerClustere(HashTable ht, int *counter)
{
	*counter = 0;
	for (int i = 0;i < ht.dim;i++)
	{
		if (ht.vector[i])
		{
			(*counter)++; //cate clustere sunt
		}
	}
	float* preturi = (float*) malloc(sizeof(float) * (*counter));
	int j = 0;//pe pozitia 0 din vector
	for (int i = 0; i < ht.dim;i++)
	{
		float suma = 0.0;
		int nr = 0;
		if (ht.vector[i])
		{
			Nod* aux = ht.vector[i];
			while (aux) // parcurg lista
			{
				nr++;
				suma += aux->info.pret;
				aux = aux->next;
			}
			float medie = suma / nr;
			preturi[j] = (float)medie;
			j++;
		}
	}
	return preturi;
}
Telefon getTelefonDupaId(HashTable ht, int id)
{
	Telefon rezultat;
	rezultat.id = -1;
	rezultat.model = NULL;
	rezultat.brand = NULL;
	int hash = calculeazaCodHash(ht.dim, id);
	if (hash >= 0 && hash < ht.dim)
	{
		Nod* aux = ht.vector[hash]; //am mers la pozitia respectiva
		while (aux)
		{
			if (aux->info.id == id)
			{
				rezultat = aux->info;
				rezultat.brand = (char*)malloc(strlen(aux->info.brand) + 1);
				strcpy(rezultat.brand, aux->info.brand);

				rezultat.model = (char*)malloc(strlen(aux->info.model) + 1);
				strcpy(rezultat.model, aux->info.model);
				break;
			}
			aux = aux->next;
		}
	}
	return rezultat;
}
int main()
{
	HashTable ht = citireTabelaDeTelefoane("telefoane.txt", 7);
	afisareTabelaDeTelefoane(ht);
	int counter = 0;
	float* rezultat = calculeazaPreturiMediiPerClustere(ht, &counter);
	for (int i = 0;i < counter;i++)
	{
		printf("\nPentru clusterul cu index %d, pretul mediu este: %.2f\n", i, rezultat[i]);
	}
	Telefon t = getTelefonDupaId(ht, 2);
	printf("\nTelefonul cu id-ul cautat este:\n");
	afisareTelefon(t);
	if (t.model)
	{
		free(t.model);
	}
	if (t.brand)
	{
		free(t.brand);
	}
	dezalocareTabela(&ht);
	return 0;
}