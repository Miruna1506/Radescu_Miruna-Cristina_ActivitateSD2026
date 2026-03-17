#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraAngajat {
	int id;
	int varsta;
	float salariu;
	char* nume;
	char* prenume;
	char* departament;
};

typedef struct StructuraAngajat Angajat;

void afisareAngajat(Angajat angajat) {
	printf("\nID: %d\n", angajat.id);
	printf("Varsta: %d\n", angajat.varsta);
	printf("Salariu: %.2f\n", angajat.salariu);
	printf("Nume: %s\n", angajat.nume);
	printf("Prenume: %s\n", angajat.prenume);
	printf("Departament: %s\n", angajat.departament);
}

void afisareVectorAngajati(Angajat* angajati, int nrAngajati) {

	if (angajati==NULL || nrAngajati==0) {
		printf("Vectorul de angajati este gol\n");
		return;
	}

	for (int i = 0; i<nrAngajati; i++) {
		afisareAngajat(angajati[i]);
	}
}

void adaugaAngajatInVector(Angajat** angajati, int* nrAngajati, Angajat angajatNou) {

	Angajat* vectorNou = (Angajat*)malloc(((*nrAngajati) + 1) * sizeof(Angajat));

	for (int i = 0; i<*nrAngajati; i++) {
		vectorNou[i]=(*angajati)[i];
	}

	vectorNou[*nrAngajati]=angajatNou;

	free(*angajati);
	*angajati=vectorNou;
	(*nrAngajati)++;
}

Angajat citireAngajatFisier(FILE* file) {

	char linie[255];
	fgets(linie, 255, file);

	char delimitatori[3]=",\n";

	Angajat angajat;

	angajat.id=atoi(strtok(linie, delimitatori));
	angajat.varsta=atoi(strtok(NULL, delimitatori));
	angajat.salariu=atof(strtok(NULL, delimitatori));

	char* aux=strtok(NULL, delimitatori);
	angajat.nume=(char*)malloc(strlen(aux) + 1);
	strcpy(angajat.nume, aux);

	char* pren=strtok(NULL, delimitatori);
	angajat.prenume=(char*)malloc(strlen(pren) + 1);
	strcpy(angajat.prenume, pren);

	char* temp=strtok(NULL, delimitatori);
	angajat.departament=(char*)malloc(strlen(temp) + 1);
	strcpy(angajat.departament, temp);

	return angajat;
}

Angajat* citireVectorAngajatiFisier(const char* numeFisier, int* nrAngajatiCititi) {

	FILE* file=fopen(numeFisier, "r");

	if (!file) {
		printf("Eroare la deschiderea fisierului\n");
		return NULL;
	}

	Angajat* vectorAngajati = NULL;
	*nrAngajatiCititi = 0;

	while (!feof(file)) {
		Angajat angajat=citireAngajatFisier(file);
		adaugaAngajatInVector(&vectorAngajati, nrAngajatiCititi, angajat);
	}

	fclose(file);
	return vectorAngajati;
}

void dezalocareVectorAngajati(Angajat** vector, int* nrAngajati) {

	for (int i = 0; i<*nrAngajati; i++) {
		free((*vector)[i].nume);
		free((*vector)[i].prenume);
		free((*vector)[i].departament);
	}

	free(*vector);
	*vector=NULL;
	*nrAngajati=0;
}
float calculSalariuMediu(Angajat* angajati, int nrAngajati) {

	float suma = 0;
	for (int i = 0; i<nrAngajati; i++) {
		suma += angajati[i].salariu;
	}
	if (nrAngajati>0) {
		return suma/nrAngajati;
	}

	return 0;
}
void afisareAngajatiSalariuMare(Angajat* angajati, int nrAngajati, float prag) {

	printf("\nAngajatii cu salariu mai mare decat %5.2f:\n", prag);
	for (int i = 0; i<nrAngajati; i++) 
	{
		if (angajati[i].salariu>prag) {
			afisareAngajat(angajati[i]);
		}

	}
}
Angajat* cautaAngajatDupaId(Angajat* angajati, int nrAngajati, int id)
{
	for (int i = 0; i<nrAngajati; i++)
	{
		if (angajati[i].id==id)
		{
			return &angajati[i];
		}
	}
	return NULL;
}
int main() {

	int nrAngajati = 0;

	Angajat* angajati = citireVectorAngajatiFisier("angajati.txt", &nrAngajati);

	afisareVectorAngajati(angajati, nrAngajati);

	float medie = calculSalariuMediu(angajati, nrAngajati);
	printf("\nSalariul mediu este: %5.2f\n", medie);

	afisareAngajatiSalariuMare(angajati, nrAngajati, 4000);

	int idCautat = 2;

	Angajat* rezultat = cautaAngajatDupaId(angajati, nrAngajati, idCautat);

	if (rezultat != NULL)
	{
		printf("\nAngajat gasit:\n");
		afisareAngajat(*rezultat);
	}
	else
	{
		printf("\nNu exista angajat cu id-ul %d\n", idCautat);
	}
	dezalocareVectorAngajati(&angajati, &nrAngajati);
	return 0;
}