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

typedef struct Nod Nod;
struct Nod {
	Angajat info;
	Nod* next;
};

Angajat citireAngajatDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Angajat a;
	aux = strtok(buffer, sep);
	a.id = atoi(aux);
	a.varsta = atoi(strtok(NULL, sep));
	a.salariu = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	a.nume = (char*)malloc(strlen(aux) + 1);
	strcpy(a.nume, aux);

	aux = strtok(NULL, sep);
	a.prenume = (char*)malloc(strlen(aux) + 1);
	strcpy(a.prenume, aux);

	aux = strtok(NULL, sep);
	a.departament = (char*)malloc(strlen(aux) + 1);
	strcpy(a.departament, aux);

	return a;
}

void afisareAngajat(Angajat angajat) {
	printf("Id: %d\n", angajat.id);
	printf("Varsta: %d\n", angajat.varsta);
	printf("Salariu: %.2f\n", angajat.salariu);
	printf("Nume: %s\n", angajat.nume);
	printf("Prenume: %s\n", angajat.prenume);
	printf("Departament: %s\n\n", angajat.departament);
}

void afisareListaAngajati(Nod* cap) {
	while (cap != NULL) {
		afisareAngajat(cap->info);
		cap = cap->next;
	}
}

void adaugaAngajatInLista(Nod** cap, Angajat angajatNou) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = angajatNou;
	nodNou->next = NULL;
	if (*cap == NULL) {
		*cap = nodNou;
	}
	else {
		Nod* aux = *cap;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nodNou;
	}
}

void adaugaLaInceputInLista(Nod** cap, Angajat angajatNou) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = angajatNou;
	nodNou->next = *cap;
	*cap = nodNou;
}

Nod* citireListaAngajatiDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	if (file) {
		Nod* cap = NULL;
		while (!feof(file)) {
			adaugaAngajatInLista(&cap, citireAngajatDinFisier(file));
		}
		fclose(file);
		return cap;
	}
	return NULL;
}

void dezalocareListaAngajati(Nod** cap) {
	while (*cap != NULL) {
		Nod* p = *cap;
		*cap = p->next;
		if (p->info.nume != NULL) {
			free(p->info.nume);
		}
		if (p->info.prenume != NULL) {
			free(p->info.prenume);
		}
		if (p->info.departament != NULL) {
			free(p->info.departament);
		}
		free(p);
	}
}

float calculeazaSalariuMediu(Nod* cap) {
	float suma = 0;
	int contor = 0;
	while (cap != NULL) {
		suma += cap->info.salariu;
		contor++;
		cap = cap->next;
	}
	if (contor != 0) {
		return suma / contor;
	}
	return 0;
}

void stergeAngajatiDinDepartament(Nod** cap, const char* departamentCautat) {
	while (*cap && strcmp((*cap)->info.departament, departamentCautat) == 0) {
		Nod* temp = *cap;
		*cap = temp->next;
		if (temp->info.nume != NULL) {
			free(temp->info.nume);
		}
		if (temp->info.prenume != NULL) {
			free(temp->info.prenume);
		}
		if (temp->info.departament != NULL) {
			free(temp->info.departament);
		}
		free(temp);
	}
	Nod* p = *cap;
	while (p != NULL) {
		while (p->next && strcmp(p->next->info.departament, departamentCautat) != 0) {
			p = p->next;
		}
		if (p->next) {
			Nod* temp = p->next;
			p->next = temp->next;
			if (temp->info.nume != NULL) {
				free(temp->info.nume);
			}
			if (temp->info.prenume != NULL) {
				free(temp->info.prenume);
			}
			if (temp->info.departament != NULL) {
				free(temp->info.departament);
			}
			free(temp);
		}
		else {
			p = p->next;
		}
	}
}

float calculeazaSalariileAngajatilorCuAcelasiPrenume(Nod* cap, const char* prenumeCautat) {
	float suma = 0;
	while (cap != NULL) {
		if (strcmp(cap->info.prenume, prenumeCautat) == 0) {
			suma += cap->info.salariu;
		}
		cap = cap->next;
	}
	return suma;
}

Angajat* gasesteAngajatCuSalariuMaxim(Nod* cap) {
	if (cap == NULL) {
		return NULL;
	}
	Nod* max = cap;
	cap = cap->next;
	while (cap != NULL) {
		if (cap->info.salariu > max->info.salariu) {
			max = cap;
		}
		cap = cap->next;
	}
	return &max->info;
}

int main() {
	Nod* cap = NULL;
	cap = citireListaAngajatiDinFisier("angajati.txt");
	afisareListaAngajati(cap);

	float medie = calculeazaSalariuMediu(cap);
	printf("\nSalariul mediu este: %.2f\n", medie);

	printf("\nSuma salariilor angajatilor cu prenumele Andrei este: %.2f\n", calculeazaSalariileAngajatilorCuAcelasiPrenume(cap, "Andrei"));

	Angajat* maxim = gasesteAngajatCuSalariuMaxim(cap);
	if (maxim != NULL) {
		printf("\nAngajatul cu salariul maxim este:\n");
		afisareAngajat(*maxim);
	}

	stergeAngajatiDinDepartament(&cap, "IT");
	printf("\nDupa stergerea angajatilor din departamentul IT:\n");
	afisareListaAngajati(cap);

	dezalocareListaAngajati(&cap);
	return 0;
}