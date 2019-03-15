#include "semantic.h"

bool addBiblio(Biblio *biblio, int biblioType){
	switch(biblioType){
		case 0:
			if((*biblio).calcul)
				return false;
			else
				(*biblio).calcul = true;
			return true;
		case 1:
			if((*biblio).boucle)
				return false;
			else
				(*biblio).boucle = true;
			return true;
		case 2:
			if((*biblio).tab)
				return false;
			else
				(*biblio).tab = true;
		return true;
	}
}


bool existBiblio(Biblio biblio, int biblioType){
	switch(biblioType){
		case 0:
			if(biblio.calcul)
				return true;
			return false;
		case 1:
			if(biblio.boucle)
				return true;
			return false;
		case 2:
			if(biblio.tab)
				return true;
			return false;
	}
}


void ERROR_BIBLIO_EXIST(int numLine, int numColumn, int biblioType){
	printf(COLOR_RED"\n- Semantic error (%d:%d) : The library ", numLine, numColumn);

	switch(biblioType){
		case 0:
			printf("\"CALCUL\" already exists."COLOR_RESET);
			break;
		case 1:
			printf("\"BOUCLE\" already exists."COLOR_RESET);
			break;
		case 2:
			printf("\"TAB\" already exists."COLOR_RESET);
			break;
	}
}
