#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-o lista simplu inlantuita
typedef struct Nod Nod;
struct Nod
{
	Masina info;
	Nod* next;
};

//creare structura pentru tabela de dispersie
// aceasta este un vector de liste
struct HashTable {
	int dim;
	Nod** vector;
};
typedef struct HashTable HashTable;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Nod *cap) {
	//prin apelarea functiei afisareMasina()
	while (cap)
	{
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaInLista(Nod**lista,Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = NULL;
	if (*lista)
	{
		Nod* aux = (*lista);
		while (aux->next)
		{
			aux = aux->next;
		}
		aux->next = nodNou;
	}
	else
	{
		(*lista) = nodNou;
	}
}

HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = (Nod**)malloc(dimensiune * sizeof(Nod*));
	for (int i = 0;i < dimensiune;i++)
	{
		ht.vector[i] = NULL;
	}
	return ht;
}

int calculeazaHash(int id,int dimensiune) {
	id = id * 23 + 123;
	return id % dimensiune;

}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	//este folosit mecanismul CHAINING
	int poz = calculeazaHash(masina.id, hash.dim);
	if (hash.vector[poz])
	{
		//avem coliziune
		adaugaMasinaInLista(&(hash.vector[poz]), masina);
	}
	else
	{
		//nu avem coliziune
		adaugaMasinaInLista(&(hash.vector[poz]), masina);
	}
}

HashTable citireMasiniDinFisier(const char* numefisier) {
	FILE* f = fopen(numefisier, "r");
	HashTable tabela = initializareHashTable(3);
	if (!f)
	{
		printf("nu exista!\n");
		return tabela;
	}
	while (!feof(f))
	{
	 inserareMasinaInTabela(tabela, citireMasinaDinFisier(f));
	}
	fclose(f);
	return tabela;
}

void afisareTabelaDeMasini(HashTable ht) {
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			printf("Cluster: %d\n", i+1);
			afisareListaMasini(ht.vector[i]);
			printf("======================\n");
		}
	}
}

void dezalocareTabelaDeMasini(HashTable *ht) {
	//sunt dezalocate toate masinile din tabela de dispersie
	for (int i = 0;i < ht->dim;i++)
	{

	}
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	//calculeaza pretul mediu al masinilor din fiecare cluster.
	//trebuie sa returnam un vector cu valorile medii per cluster.
	//lungimea vectorului este data de numarul de clustere care contin masini
	*nrClustere = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			(*nrClustere)++;
		}
	}
	float* preturi_medii = malloc(sizeof(float) * (*nrClustere));
	int k = 0;
	for (int i = 0;i < ht.dim;i++)
	{
		if (ht.vector[i] != NULL)
		{
			float suma = 0;
			int nrMasini = 0;
			Nod* aux = ht.vector[i];
			while (aux)
			{
				suma += aux->info.pret;
				nrMasini++;
				aux = aux->next;
			}
			preturi_medii[k++] = suma / nrMasini;
		}
	}
	return preturi_medii;
}

Masina getMasinaDupaID(HashTable ht, int id) {
	Masina m;
	//cauta masina dupa valoarea atributului cheie folosit in calcularea hash-ului
	//trebuie sa modificam numele functiei 
	m.id = -1;
	int hash = calculeazaHash(id, ht.dim);
	if (hash >= 0 && hash < ht.dim)
	{
		Nod* aux = ht.vector[hash];
		while (aux)
		{
			if (aux->info.id == id)
			{
				m = aux->info;
				m.numeSofer = malloc(sizeof(char) * strlen(aux->info.numeSofer) + 1);
				strcpy(m.numeSofer, aux->info.numeSofer);
				m.model = malloc(sizeof(char) * strlen(aux->info.model) + 1);
				strcpy(m.model, aux->info.model);
				break;
			}
			aux = aux->next;
		}
	}

	return m;
}

int main() {
	
	HashTable tabela = citireMasiniDinFisier("masini.txt");
	afisareTabelaDeMasini(tabela);
	Masina m = getMasinaDupaID(tabela, 5);
	afisareMasina(m);
	int nrClustere;
	float* vectorPreturi = calculeazaPreturiMediiPerClustere(tabela, &nrClustere);
	for (int i = 0;i < nrClustere;i++)
	{
		printf("Pentru clusterul cu index %d, pretul mediu este: %.2f\n", i, vectorPreturi[i]);
	}
	return 0;
}