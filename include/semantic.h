#ifndef semantic_h_
#define semantic_h_

#include "general.h"

#define BIBLIO_CALCUL 0
#define BIBLIO_BOUCLE 1
#define BIBLIO_TAB 2


bool addBiblio(Biblio *biblio, int biblioType);
bool existBiblio(Biblio biblio, int biblioType);
void ERROR_BIBLIO_EXIST(int numLine, int numColumn, int biblioType);


#endif
