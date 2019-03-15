#ifndef routine_h_
#define routine_h_

#include "general.h"
#include "symbolsTable.h"
#include "quadruplets.h"

typedef struct ElementCONSTAFF {
	char *nom;
	struct ElementCONSTAFF *suiv;
}ElementCONSTAFF;

typedef struct CONSTAFF {
	ElementCONSTAFF *tete, *queue;
}CONSTAFF;


void ROUTINE_Une_Seule_Bibliotheque(Biblio *biblio, char *nomBiblio, int numLigne, int numColonne);

void ROUTINE_Marquer_DECLARATION(bool *estDeclaration, bool etat);
void ROUTINE_Marquer_TYPE_DECLARATION(char **type, char *nomType);
void ROUTINE_Marquer_CONST_DECLARATION(char **type, char *estDeclaration);

CONSTAFF initCONSTAFF();
bool existeCONSTAFF(CONSTAFF *caf, char *nom);

void simplificationAlgebrique(QR *q, int nombreIter);

void propagationDeConstantes(QR *q, TS *t);

#endif
