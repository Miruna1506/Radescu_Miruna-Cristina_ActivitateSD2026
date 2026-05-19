#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
struct Angajat
{
	int id;
	int varsta;
	float salariu;
	char* nume;
	char* departament;
};
typedef struct Angajat Angajat;
struct Nod
{
	Angajat info;
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;
Angajat citireAngajatDinFisier(FILE* f)
{
	char buff[255];
	char sep[4] = "\n;,";
	fgets(buff, 255, f);

	Angajat a;
	a.id = atoi(strtok(buff, sep));
	a.varsta = atoi(strtok(NULL, sep));
	a.salariu = atof(strtok(NULL, sep));
	char* aux = strtok(NULL, sep);
	a.nume = malloc(strlen(aux) + 1);
	strcpy(a.nume, aux);
	aux = strtok(NULL, sep);
	a.departament = malloc(strlen(aux) + 1);
	strcpy(a.departament, aux);
	
		return a;
}
void afisareAngajat(Angajat a)
{
	printf("Id: %d\n", a.id);
	printf("Varsta: %d\n", a.varsta);
	printf("Salariu: %.2f\n", a.salariu);
	printf("Nume: %s\n", a.nume);
	printf("Departament: %s\n\n", a.departament);
}
int determinaInaltime(Nod* radacina)
{
	if (radacina)
	{
		int inaltimeSt = determinaInaltime(radacina->stanga);
		int inaltimeDr = determinaInaltime(radacina->dreapta);

		return 1 + fmax(inaltimeDr, inaltimeSt);
	}
	return 0;
}
int verificareEchilibru(Nod* radacina)
{
	int inaltimeSt = determinaInaltime(radacina->stanga);
	int inaltimeDr = determinaInaltime(radacina->dreapta);

	return inaltimeSt - inaltimeDr;
}
//cand arborele este prea greu pe dreapta
void rotireStanga(Nod** radacina)
{
	//copilul din dreapta
	Nod* aux = (*radacina)->dreapta; //tine minte nodul din dreapta pt ca trb sa urce ca radacina
	
	//copilul stang al nodului de mutat (aux), il mutam la dreapta radacinii
	(*radacina)->dreapta = aux->stanga;
	
	//copilul din stanga al nodului de mutat va fi chiar radacina
	aux->stanga = *radacina;

	*radacina = aux; // pointerul arata acum spre noua radacina a arborelui = aux
}
void rotireDreapta(Nod** radacina)
{
	Nod* aux = (*radacina)->stanga; //retin nodul pe care vreau sa il mut

	(*radacina)->stanga = aux->dreapta; //mut in stanga radacinii copilul nodului de mutat(aux), care este in dreapta

	aux->dreapta = *radacina; //acum copilul din dreapta al lui aux este chiar radacina veche

	*radacina = aux; //acum aux e radacina arborelui
}
void adaugaAngajatInAVL(Nod** radacina, Angajat a)
{
	if (*radacina == NULL) //daca nu am arbore
	{
		Nod* nou = (Nod*)malloc(sizeof(Nod));

		nou->dreapta = NULL;
		nou->stanga = NULL;

		nou->info = a;

		*radacina = nou;
	}
	else //daca avem arbore
	{
		if ((*radacina)->info.id < a.id)//pun in dreapta
		{
			adaugaAngajatInAVL(&(*radacina)->dreapta, a);
		}
		else //pun in stanga
		{
			adaugaAngajatInAVL(&(*radacina)->stanga, a);
		}
		//verificam daca dupa inserare arborele este dezechilibrat
		int gradEchilibru = verificareEchilibru(*radacina);
		if (gradEchilibru == -2) //e mai greu pe dreapta
		{
			//verfic daca copilul din dreapta este mai greu pe stanga, daca e => il rotesc pe dreapta 
			if (verificareEchilibru((*radacina)->dreapta) == 1) 
			{
				rotireDreapta(&(*radacina)->dreapta);//prima data rotesc la dreapta copilul din dreapta => avem dreapta-dreapta acum
			}
			rotireStanga(radacina); //rotirea stanga
		}
		if (gradEchilibru == 2)
		{
			if (verificareEchilibru((*radacina)->stanga) == -1)//daca copilul din stanga este mai greu pe dreapta
			{
				rotireStanga(&(*radacina)->stanga);
			}
			rotireDreapta(radacina);
		}
	}
}
Nod* citireArboreDinFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");

	Nod* radacina = NULL;

	if (f)
	{
		while (!feof(f))
		{
			adaugaAngajatInAVL(&radacina, citireAngajatDinFisier(f));
		}
		fclose(f);
	}
	return radacina;
}
void afisareAngajatiDinArbore(Nod* radacina)
{
	//preordine
	if (radacina)
	{
		afisareAngajat(radacina->info);
		
		afisareAngajatiDinArbore(radacina->stanga);

		afisareAngajatiDinArbore(radacina->dreapta);

	}
}
void dezalocare(Nod** radacina)
{
	if (*radacina)
	{
		dezalocare(&(*radacina)->stanga);
		dezalocare(&(*radacina)->dreapta);

		free((*radacina)->info.departament);
		free((*radacina)->info.nume);

		free(*radacina);
		*radacina = NULL;
	}
}
Angajat getAngajatByID(Nod* radacina, int id)
{
	if (radacina)
	{
		if (radacina->info.id == id)
		{
			Angajat a = radacina->info;

			a.departament = malloc(strlen(radacina->info.departament) + 1);
			strcpy(a.departament, radacina->info.departament);

			a.nume = malloc(strlen(radacina->info.nume) + 1);
			strcpy(a.nume, radacina->info.nume);
			return a;
		}

		if (radacina->info.id < id)
		{
			return getAngajatByID(radacina->dreapta, id);
		}
		if (radacina->info.id > id)
		{
			return getAngajatByID(radacina->stanga, id);
		}
	}
	Angajat a;
	a.id = -1;
	return a;
}
int determinaNumarNoduri(Nod* radacina)
{
	if (radacina)
	{
		return determinaNumarNoduri(radacina->stanga) + determinaNumarNoduri(radacina->dreapta) + 1;
	}
	return 0;
}
float calculeazaSalariuTotal(Nod* radacina)
{
	if (radacina)
	{
		return calculeazaSalariuTotal(radacina->stanga) + calculeazaSalariuTotal(radacina->dreapta) + radacina->info.salariu;
	}
	return 0;
}
float calculeazaSalariuDupaDepartament(Nod* radacina, const char* dep)
{
	float suma;
	if (radacina)
	{
		suma = calculeazaSalariuDupaDepartament(radacina->stanga, dep) + calculeazaSalariuDupaDepartament(radacina->dreapta, dep);
		if (strcmp(radacina->info.departament, dep) == 0)
		{
			suma += radacina->info.salariu;
		}
		return suma;
	}
	return 0;
}
int main()
{
	Nod* arbore = citireArboreDinFisier("angajati_arbore.txt");
	afisareAngajatiDinArbore(arbore);
	printf("Angajatul cu id-ul cautat\n");
	afisareAngajat(getAngajatByID(arbore, 1));
	printf("Numarul de noduri este: %d\n", determinaNumarNoduri(arbore));
	printf("Salariul total este: %.2f\n", calculeazaSalariuTotal(arbore));
	printf("Suma salarilor unui departament: %.2f", calculeazaSalariuDupaDepartament(arbore, "Marketing"));
	dezalocare(&arbore);
}