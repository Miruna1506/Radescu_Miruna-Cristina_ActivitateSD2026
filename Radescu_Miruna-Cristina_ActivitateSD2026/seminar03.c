#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina; //peste tot unde este termenul masina inlocuieste cu struct StructuraMasina 

void afisareMasina(Masina masina) {
	printf("\nID: %d\n", masina.id);
	printf("Numar usi: %d\n", masina.nrUsi);
	printf("Pret: %5.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n", masina.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	if (masini == NULL || nrMasini == 0)
	{
		printf("Vectorul de masini este gol\n");
		return;
	}
	for (int i = 0;i < nrMasini;i++)
	{
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int * nrMasini, Masina masinaNoua) {
	Masina* vectorNou = malloc(((*nrMasini) + 1) * sizeof(Masina));
	for (int i = 0;i < *nrMasini;i++)
	{
		vectorNou[i] = (*masini)[i];
	}
	vectorNou[*nrMasini] = masinaNoua;
	free(*masini);
	*masini = vectorNou;
	(*nrMasini)++;
}

Masina citireMasinaFisier(FILE* file) {
	char linie[255];
	fgets(linie, 255, file);
	char delimitatori[3] = ",\n";
	Masina masina;
	masina.id=atoi(strtok(linie, delimitatori)); //atoi=ascii to integer
	masina.nrUsi = atoi(strtok(NULL, delimitatori));
	masina.pret = atof(strtok(NULL, delimitatori));
	char * aux= strtok(NULL, delimitatori);
	masina.model = (char*)malloc(sizeof(char)*(strlen(aux) + 1));
	strcpy(masina.model, aux);
	char* temp = strtok(NULL, delimitatori);
	masina.numeSofer = (char*)malloc(sizeof(char) * (strlen(temp) + 1));
	strcpy(masina.numeSofer, temp);
	masina.serie= strtok(NULL, delimitatori)[0]; 
	return masina;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	FILE* file = fopen(numeFisier, "r");
	if (!file)
	{
		printf("Eroare la deschidere\n");
		return;
	}
	else
	{
		Masina* vectorMasini = NULL;
		*nrMasiniCitite = 0;
		while (!feof(file))
		{
			Masina masina = citireMasinaFisier(file);
			adaugaMasinaInVector(&vectorMasini, nrMasiniCitite, masina);
		}
		return vectorMasini;
	}
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	//este dezalocat intreg vectorul de masini
}

int main() {
	int nrMasini = 0;
	Masina *masini=citireVectorMasiniFisier("masini.txt",&nrMasini);
	afisareVectorMasini(masini, nrMasini);

	return 0;
}