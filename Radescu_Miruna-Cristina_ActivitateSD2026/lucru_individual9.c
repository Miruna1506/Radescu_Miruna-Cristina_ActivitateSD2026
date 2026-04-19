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

Carte citireCarteDinFisier(FILE* file)
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
Carte afisareCarte(Carte carte)
{
	printf("\nID: %d", carte.id);
	printf("\nNumar pagini: %d", carte.nrPagini);
	printf("\nPret: %.2f", carte.pret);
	printf("\nTitlu: %s", carte.titlu);
	printf("\nAutor: %s", carte.autor);
	printf("\nCod raft: %c\n", carte.codRaft);
	return carte;
}
struct Nod
{
	Carte info;
	struct Nod* next;
};
typedef struct Nod Nod;

struct HashTable
{
	int dim;
	Nod** tabela; //vector de pointeri la Nod
	//mai poate avea si nr. pozitii ocupate
};
typedef struct HashTable HashTable;
void afisareListaCarti(Nod* cap)
{
	while (cap)
	{
		afisareCarte(cap->info);
		cap = cap->next;
	}
}
void adaugareCarteInLista(Nod* cap, Carte c) //apelata doar daca avem coliziune
{
   //adaug la sfarsit
	while (cap->next)
	{
		cap = cap->next;
	}
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = c;
	nou->next = NULL;
	cap->next = nou;

}
HashTable initializareHashTable(int dimensiune)
{
	HashTable ht;
	ht.dim = dimensiune;
	ht.tabela = (Nod**)malloc(sizeof(Nod*) * dimensiune);
	for (int i = 0;i < dimensiune;i++)
	{
		ht.tabela[i] = NULL; //pointerii = NULL
	}
	return ht;
}
int calculeazaHash(const char* nume, int dimensiune)
{
	//dupa nume autor(cheia) si dimensiune
	int suma = 0;
	for (int i = 0;i < strlen(nume);i++)
	{
		suma += nume[i]; //ia codul ascii si adauga la suma
	}
	return suma % dimensiune;
}
void inserareCarteInTabela(HashTable hash, Carte carte)
{
	int pozitie = calculeazaHash(carte.autor, hash.dim);
	if (hash.tabela[pozitie] == NULL)//daca e libera pozitia
	{
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		hash.tabela[pozitie] = nou;
		hash.tabela[pozitie]->info = carte;
		hash.tabela[pozitie]->next = NULL;
	}
	else //avem coliziune
	{
		adaugareCarteInLista(hash.tabela[pozitie], carte);//adresa primului nod si cartea pe care o inserez in lista
	}
}
HashTable citireCartiDinFisierInTabela(const char* numeFisier, int dimensiune)
{
	//initializez tabela
	HashTable hash = initializareHashTable(dimensiune);
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f))
	{
		Carte c = citireCarteDinFisier(f);
		inserareCarteInTabela(hash, c);
	}
	fclose(f);
	return hash;
}
void afisareTabelaDeCarti(HashTable ht)
{
	//evidentiem si clusterele
	for (int i = 0;i < ht.dim;i++)
	{
		if (ht.tabela[i] != NULL)
		{
			printf("\nMasinile de pe pozitia %d sunt: \n", i);
			afisareListaCarti(ht.tabela[i]);//toata lista
		}
		else
		{
			printf("\nPe pozitia %d nu avem masini\n", i);
		}
	}
}
void dezalocareListaDeCarti(Nod** cap)
{
	Nod* p = *cap;
	while (p)
	{
		Nod* aux = p;
		p = p->next;//ne deplasam
		if(aux->info.titlu)
		{
			free(aux->info.titlu);
		}
		if (aux->info.autor)
		{
			free(aux->info.autor);
		}
		free(aux);
	}
	*cap = NULL;
}
void dezalocareTabelaDeCarti(HashTable* ht)
{
	for (int i = 0;i < ht->dim;i++)
	{
		dezalocareListaDeCarti(&(ht->tabela[i]));
	}
	free(ht->tabela);
	ht->tabela = NULL;
	ht->dim = 0;
}
float* calculeazaPreturiMediiPerClustere(HashTable ht, int *counter)
{
	*counter = 0;
	for (int i = 0; i < ht.dim;i++)
	{
		if (ht.tabela[i])
		{
			(*counter)++; //cate clustere am
		}
	}

	float * preturi = (float*)malloc(sizeof(float) * (*counter));//aloc spatiu
	int j = 0;//sunt pe poz 0 in vector

	for (int i = 0;i < ht.dim;i++) //parcurg tabela
	{
		if (ht.tabela[i]) //am gasit lista
		{
			float suma = 0.0;
			int nr = 0;
			Nod* p = ht.tabela[i];
			while (p) //parcurg lista
			{
				suma += p->info.pret; //fac suma pt cluster
				nr++;
				p = p->next;
			}
			preturi[j] = suma / nr;
			j++;
		}
	}
	return preturi;
}
Carte deep_copy(Carte c)
{
	Carte carte;
	/*carte.id = c.id;
	carte.nrPagini = c.nrPagini;
	carte.codRaft = c.codRaft;*/
	carte = c;
	carte.autor = (char*)malloc(sizeof(char) * (strlen(c.autor) + 1));
	strcpy(carte.autor, c.autor);
	carte.titlu = (char*)malloc(sizeof(char) * (strlen(c.titlu) + 1));
	strcpy(carte.titlu, c.titlu);
	return carte;
}
Carte getCarteDupaCheie(HashTable ht, const char* numeCautat) //numele autorului e cheia
{
	Carte c;
	c.id = -1;
	c.titlu = NULL;
	c.autor = NULL;
	int poz = calculeazaHash(numeCautat, ht.dim); //pozitia unde se afla acea Carte a autorului dat
	if (poz >= 0 && poz < ht.dim)//e ok, a gasit
	{
		Nod* p = ht.tabela[poz]; //lista respectiva
		while (p && (strcmp(p->info.autor,numeCautat)!=0)) // verificam deoarece lista poate contine si alti autori (din cauza coliziunii)
		{
			p = p->next; 
		}
		//s-a oprit la autorul cautat
		if (p)
		{
			c = deep_copy(p->info); //retin cartea
		}
	}
	return c;
}
int main()
{
	HashTable ht = citireCartiDinFisierInTabela("carti.txt", 7);
	afisareTabelaDeCarti(ht);
	int nrClustere = 0;
	float* preturi = calculeazaPreturiMediiPerClustere(ht, &nrClustere);
	printf("\nPreturile medii per clustere sunt:\n");
	for (int i = 0;i < nrClustere;i++)
	{
		printf("%.2f  ", preturi[i]);
	}
	printf("\n");
	Carte rezultat = getCarteDupaCheie(ht, "Rebreanu");
	if (rezultat.id != -1)
	{
		printf("\nCartea dupa numele autorului este:\n");
		afisareCarte(rezultat);
		if (rezultat.autor)
		{
			free(rezultat.autor);
		}
		if (rezultat.titlu)
		{
			free(rezultat.titlu);
		}
	}
	else
	{
		printf("\nAutorul nu are nici o carte\n");
	}
	dezalocareTabelaDeCarti(&ht);
	return 0;
}
