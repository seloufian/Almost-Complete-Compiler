#include "routine.h"

void ROUTINE_Une_Seule_Bibliotheque(Biblio *biblio, char *nomBiblio, int numLigne, int numColonne){

	if( (nomBiblio[1] == 'c') || (nomBiblio[1] == 'C') ){
		if((*biblio).calcul)
			printf(COLOR_RED"\n- Semantic error (%d:%d) : Library \"CALCUL\" already exists."COLOR_RESET, numLigne-1, numColonne);
		else
			(*biblio).calcul = true;
		return;
	}
	if( (nomBiblio[1] == 'b') || (nomBiblio[1] == 'B') ){
		if((*biblio).boucle)
			printf(COLOR_RED"\n- Semantic error (%d:%d) : Library \"BOUCLE\" already exists."COLOR_RESET, numLigne-1, numColonne);
		else
			(*biblio).boucle = true;
		return;
	}
	if( (nomBiblio[1] == 't') || (nomBiblio[1] == 'T') ){
		if((*biblio).tab)
			printf(COLOR_RED"\n- Semantic error (%d:%d) : Library \"TAB\" already exists."COLOR_RESET, numLigne-1, numColonne);
		else
			(*biblio).tab = true;
	}
}


void ROUTINE_Marquer_DECLARATION(bool *estDeclaration, bool etat){
	(*estDeclaration) = etat;
}


void ROUTINE_Marquer_TYPE_DECLARATION(char **type, char *nomType){
	(*type) = nomType;
}


void ROUTINE_Marquer_CONST_DECLARATION(char **type, char *estDeclaration){
	(*type) = estDeclaration;
}


CONSTAFF initCONSTAFF(){
	CONSTAFF caf = { NULL, NULL };
	return caf;
}

bool existeCONSTAFF(CONSTAFF *caf, char *nom){
	CONSTAFF f = *caf;

	while(f.tete){
		if(sameString((f.tete)->nom, nom))
			return true;
		f.tete = (f.tete)->suiv;
	}

	ElementCONSTAFF *e = malloc(sizeof(ElementCONSTAFF));

	e->nom = nom;
	e->suiv = NULL;

	if(caf->tete == NULL)
		caf->tete = e;
	else
		(caf->queue)->suiv = e;

	caf->queue = e;

	return false;
}


typedef struct InfoSIMPALG {
	char* nomTemp;
	char* remplacerPar;
	int numQuadTemp;
}InfoSIMPALG;

typedef struct ElementSIMPALG {
	InfoSIMPALG info;
	struct ElementSIMPALG *suiv;
}ElementSIMPALG;

typedef struct SIMPALG {
	ElementSIMPALG *tete, *queue;
}SIMPALG;



SIMPALG initSIMPALG(){
	SIMPALG slg = { NULL, NULL };
	return slg;
}

void addSIMPALG(SIMPALG *s, char* nomTemp, char* remplacerPar, int numQuadTemp){

	ElementSIMPALG *e = malloc(sizeof(ElementSIMPALG));

	(e->info).nomTemp = nomTemp;
	(e->info).remplacerPar = remplacerPar;
	(e->info).numQuadTemp = numQuadTemp;
	e->suiv = NULL;

	if(s->tete == NULL)
		s->tete = e;
	else
		(s->queue)->suiv = e;

	s->queue = e;
}

void destroySIMPALG(SIMPALG *s){
	ElementSIMPALG *e;

	while(s->tete){
		e = s->tete;
		s->tete = (s->tete)->suiv;
		free(e);
	}
}


void displaySIMPALG(SIMPALG s){
	printf("\nDisplay SIMPALG : \n");
	while(s.tete){
		printf("\tNomTemp : %s ... ReplaceWith : %s ... NumQR : %d\n", (s.tete)->info.nomTemp,  (s.tete)->info.remplacerPar, (s.tete)->info.numQuadTemp);
		s.tete = (s.tete)->suiv;
	}
	printf("\n\n");
}


void simplificationAlgebrique(QR *q, int nombreIter){
	QR r;
	InfoSIMPALG infoS;
	int quadCourant;

	SIMPALG slg = initSIMPALG();

	while(nombreIter--){

		r = *q;
		quadCourant = 0;

		while(r.tete){

			infoS.remplacerPar = NULL;

			if(((r.tete)->info.position_4 != NULL) && (r.tete)->info.position_4[0] == '_'){ /* Il s'agit d'une sauvegrade à un temporaire */

				infoS.nomTemp = (r.tete)->info.position_4; /* Save the name of the temporary */
				infoS.numQuadTemp = quadCourant; /* Save the name of the temporary (to remove it later) */

				if(sameString((r.tete)->info.position_1, "+")){ /* An addition */
					if(sameString((r.tete)->info.position_2, "0"))
						infoS.remplacerPar = (r.tete)->info.position_3;

					if(sameString((r.tete)->info.position_3, "0"))
						infoS.remplacerPar = (r.tete)->info.position_2;

					if(sameString((r.tete)->info.position_2, "0.0"))
						infoS.remplacerPar = (r.tete)->info.position_3;

					if(sameString((r.tete)->info.position_3, "0.0"))
						infoS.remplacerPar = (r.tete)->info.position_2;
				}

				if(sameString((r.tete)->info.position_1, "*")){ /* A multiplication */
					if(sameString((r.tete)->info.position_2, "0") || sameString((r.tete)->info.position_3, "0"))
						infoS.remplacerPar = "0";
					if(sameString((r.tete)->info.position_2, "0.0") || sameString((r.tete)->info.position_3, "0.0"))
						infoS.remplacerPar = "0.0";
				}

				if(sameString((r.tete)->info.position_1, "-")){ /* A Subtraction */
					if(sameString((r.tete)->info.position_3, "0"))
						infoS.remplacerPar = (r.tete)->info.position_2;
					if(sameString((r.tete)->info.position_3, "0.0"))
						infoS.remplacerPar = (r.tete)->info.position_2;
				}

				if(sameString((r.tete)->info.position_1, "/")){ /* A division */
					if(sameString((r.tete)->info.position_2, "0"))
						infoS.remplacerPar = "0";
					if(sameString((r.tete)->info.position_2, "0.0"))
						infoS.remplacerPar = "0.0";
				}
			}

			if(infoS.remplacerPar != NULL){ /* There is an algebraic simplification to do */
				addSIMPALG(&slg, infoS.nomTemp, infoS.remplacerPar, infoS.numQuadTemp);
				break; /* We found an algebraic simplification, execute it on the quadruplets */
			}

			quadCourant++;
			r.tete = (r.tete)->suiv;
		}

		char chTemp[5];

		while(slg.tete){ /* Browse the list of temporary to simplify */

			r = *q;
			quadCourant = 0;

			infoS.nomTemp = (slg.tete)->info.nomTemp;
			infoS.remplacerPar = (slg.tete)->info.remplacerPar;
			infoS.numQuadTemp = (slg.tete)->info.numQuadTemp;

			while(r.tete){ /* Browse the list of quadruplets */

				if(quadCourant == infoS.numQuadTemp){
					r.tete = (r.tete)->suiv;
					deleteQR(&(*q), quadCourant);
				}
				else{
					if(((r.tete)->info.position_1[0] == 'B') && (atoi((r.tete)->info.position_2) > infoS.numQuadTemp)){
						sprintf(chTemp, "%d", atoi((r.tete)->info.position_2)-1);
						(r.tete)->info.position_2 = strdup(chTemp);
					}

					if(((r.tete)->info.position_2 != NULL) && sameString((r.tete)->info.position_2, infoS.nomTemp))
						(r.tete)->info.position_2 = infoS.remplacerPar;

					if(((r.tete)->info.position_3 != NULL) && sameString((r.tete)->info.position_3, infoS.nomTemp))
						(r.tete)->info.position_3 = infoS.remplacerPar;

					if(((r.tete)->info.position_4 != NULL) && sameString((r.tete)->info.position_4, infoS.nomTemp))
						(r.tete)->info.position_4 = infoS.remplacerPar;

					r.tete = (r.tete)->suiv;
				}

				quadCourant++;
			}

			slg.tete = (slg.tete)->suiv;
			if(slg.tete)
				(slg.tete)->info.numQuadTemp--; /* An item has been deleted, decrement the number of quadruplet */
		}

		destroySIMPALG(&slg);
	}
}



bool isNumber(char *string){

	if((string[0] != '+') && (string[0] != '-') && (string[0] < '0') && (string[0] > '9'))
		return false;

	int i = 1;

	while((string[i] != '.') && (i<strlen(string))){
		if((string[i] < '0') || (string[i] > '9'))
			return false;
		i++;
	}

	for(int j=i+1; j<strlen(string); j++)
		if((string[j] < '0') || (string[j] > '9'))
			return false;

	return true;
}

void propagationDeConstantes(QR *q, TS *t){

	QR d = *q, r;
	TS s;

	int numQuadRech = 0, numQuadPropag;

	bool estCONST = false;

	char *remplacer, *remplacerPar;
	char chTemp[10];

	while(d.tete){ /* Browse all quadruplets (search for possible constants propagation) */

		if(sameString((d.tete)->info.position_1, ":=") && isNumber((d.tete)->info.position_2)){ /* Quadruplet type (:= , <number> ,  , <element>) */

			s = *t;

			while(s.tete){ /* Check (in the "Table of Symbols") that it is a constant */
				if(sameString((d.tete)->info.position_4, (s.tete)->info.nom) && ((s.tete)->info.constante)){ /* The variable is a constant */
					estCONST = true;
					break;
				}
				(s.tete) = (s.tete)->suiv;
			}

			if(estCONST){

				remplacer = (d.tete)->info.position_4; /* Save the constant */
				remplacerPar = (d.tete)->info.position_2; /* Save the value by which the constant will be replaced */

				d.tete = (d.tete)->suiv; /* Advance in the general browse of quadruplets (the current quadruplet will surely be removed) */
				numQuadPropag = 0;
				r = *q;

				while(r.tete){

					if(numQuadPropag == numQuadRech){
						r.tete = (r.tete)->suiv;
						deleteQR(&(*q), numQuadRech);
					}
					else{
						if(((r.tete)->info.position_1[0] == 'B') && (atoi((r.tete)->info.position_2) > numQuadRech)){
							sprintf(chTemp, "%d", atoi((r.tete)->info.position_2)-1);
							(r.tete)->info.position_2 = strdup(chTemp);
						}

						if(((r.tete)->info.position_2 != NULL) && sameString((r.tete)->info.position_2, remplacer))
							(r.tete)->info.position_2 = remplacerPar;

						if(((r.tete)->info.position_3 != NULL) && sameString((r.tete)->info.position_3, remplacer))
							(r.tete)->info.position_3 = remplacerPar;

						r.tete = (r.tete->suiv);
					}

					numQuadPropag++;
				}
			}
		}

		if(! estCONST){ /* If we have not found a constant to propagate (otherwise, the pointer has already advanced) */
			d.tete = (d.tete)->suiv;
			numQuadRech++;
		}
		else
			estCONST = false;
	}
}
