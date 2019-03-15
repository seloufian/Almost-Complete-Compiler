#ifndef symbolsTable_h_
#define symbolsTable_h_

#include "general.h"

#define TS_TYPE		2
#define TS_LENGTH 	3
#define TS_CONSTANT 4


typedef struct InfoTS {
	char* nom;
	char* type;
	int taille;
	bool constante;
}InfoTS;

typedef struct ElementTS {
	InfoTS info;
	struct ElementTS *suiv;
}ElementTS;

typedef struct TS {
	ElementTS *tete, *queue;
}TS;


TS initTS();

bool emptyTS(TS t);

void addTS(TS* t, char* nom, char* type, int taille, bool constante);

int sizeTS(TS t);

InfoTS lookUp(TS t, char* nom);

bool updateTS(TS t, char* name, int column, char* value);

void displayTS(TS t);

#endif
