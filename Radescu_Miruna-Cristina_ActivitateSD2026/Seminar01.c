#include<stdio.h>
#include<stdlib.h>

struct Produs {
	int id;
	int stoc;
	char* denumire;
	float pret;
	char categorie;
};

struct Produs initializare(int id, int stoc, char* denumire, float pret, char categorie) {
	struct Produs s;
	s.id = id;
	s.stoc = stoc;
	s.denumire = (char*)malloc(sizeof(char) * (strlen(denumire) + 1));
	strcpy(s.denumire, denumire);
	s.pret = pret;
	s.categorie = categorie;
	return s;
}

void afisare(struct Produs s) {
	printf("\nId:%d\n", s.id);
	printf("Stoc:%d\n", s.stoc);
	printf("Denumire:%s\n", s.denumire);
	printf("Pret:%5.2f\n", s.pret);
	printf("Categorie:%c\n", s.categorie);
	//afisarea tuturor atributelor.
}

void modifica_Denumire(struct Produs* s, char* nouaDenumire) {
	free((*s).denumire);
	s->denumire = (char*)malloc(strlen(nouaDenumire) + 1);
	strcpy(s->denumire, nouaDenumire);
}

void dezalocare(struct Produs* s) {
	//dezalocare campuri alocate dinamic
	if (s->denumire != NULL)
	{
		free(s->denumire);
		s->denumire = NULL;
	}

}
float calculeazaTotal(struct Produs s)
{
	return s.stoc * s.pret;
}
int main() {
	struct Produs s;
	s = initializare(1, 20, "Telefon smart", 2000, 'A');
	afisare(s);
	modifica_Denumire(&s, "Mouse");
	afisare(s);
	printf("\nValoarea totala pentru %s este de %5.2f RON", s.denumire, calculeazaTotal(s));
	/*struct Produs* ps = &s;
	struct Produs* pp = malloc(sizeof(struct Produs));
	free(pp);
	free(ps);*/
	dezalocare(&s);
	return 0;
}