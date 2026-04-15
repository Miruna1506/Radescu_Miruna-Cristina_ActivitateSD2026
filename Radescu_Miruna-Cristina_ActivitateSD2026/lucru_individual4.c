#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraCarte
{
	int id;
	int nrPagini;
	float pret;
	char* titlu;
	char* autor;
	char codRaft;
};
typedef struct StructuraCarte Carte;
void afisare_Carte(Carte carte)
{
	printf("\nID: %d", carte.id);
	printf("\nNumar pagini: %d",carte.nrPagini);
	printf("\nPret: %.2f", carte.pret);
	printf("\nTitlu: %s", carte.titlu);
	printf("\nAutor: %s", carte.autor);
	printf("\nCod raft: %c\n", carte.codRaft);
}
void afisare_vector(Carte* carti, int nrCarti)
{
	if (carti == NULL || nrCarti == 0)
	{
		printf("Vectorul de carti este gol.\n");
	}
	for (int i = 0; i < nrCarti; i++)
	{
		afisare_Carte(carti[i]);
	}
}
void adaugareCarte(Carte** carti, int *nrCarti, Carte carteNoua)
{
	//aloc spatiu
	Carte* temp = (Carte*)malloc(((*nrCarti) + 1) * sizeof(Carte)); //cate carti aveam inainte +1
	for (int i = 0; i < (*nrCarti); i++) //aduc elemente din vechiul vector in noul vector
	{
		temp[i] = (*carti)[i];
	}
	temp[*nrCarti] = carteNoua;
	free(*carti);//nu mai tine o adresa
	(*carti) = temp;
	(*nrCarti)++;
}
Carte citireCarteDinFisier(FILE* file)
{
	Carte c;
	char buffer[255];
	char sep[4] = ",;\n";
	fgets(buffer, 255, file);
	c.id = atoi(strtok(buffer, sep)); //strtok returneaza char* => ascii to integer
	c.nrPagini = atoi(strtok(NULL, sep));//nu ii dam buffer pt ca vrem sa plece de unde a ramas la apelul precedent
	c.pret = atof(strtok(NULL, sep));
	char* aux;
	aux = strtok(NULL, sep); 
	c.titlu = (char*)malloc(strlen(aux) + 1);
	strcpy(c.titlu, aux);
	aux = strtok(NULL, sep);
	c.autor = (char*)malloc(strlen(aux) + 1);
	strcpy(c.autor, aux);
	c.codRaft = strtok(NULL, sep)[0]; // codRaft e un char, iar strtok returneaza un sir de caractere
	return c;
}
Carte* citireVectorCartiDinFisier(const char* numeFisier, int* nrCartiCitite)
{
	FILE* file = fopen(numeFisier, "r"); //stream-ul
	Carte* carti = NULL; //e alocat in adaugareCarte()
	(*nrCartiCitite) = 0;
	while (!feof(file))
	{
		adaugareCarte(&carti, nrCartiCitite, citireCarteDinFisier(file));
	}
	fclose(file);
	return carti;
}
void dezalocareVectorCarti(Carte** carti,int *nrCarti)
{
	for (int i = 0; i < *nrCarti; i++)
	{
		//dezalocam campuri alocate dinamic (nu mai pointeaza spre nimic)
		if ((*carti)[i].autor != NULL)
		{
			free((*carti)[i].autor);
			(*carti)[i].autor = NULL;
		}
		if ((*carti)[i].titlu != NULL)
		{
			free((*carti)[i].titlu);
			(*carti)[i].titlu = NULL;
		}
	}
	free(*carti); //arata spre o zona de memorie goala acum
	(*carti) = NULL; 
	(*nrCarti) = 0;
}
int main() {
	int nrCarti = 0;
	Carte* carti = citireVectorCartiDinFisier("carti.txt", &nrCarti);
	afisare_vector(carti, nrCarti);
	dezalocareVectorCarti(&carti, &nrCarti);
	return 0;
}