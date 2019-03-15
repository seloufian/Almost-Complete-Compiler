#include "symbolsTable.h"

TS initTS(){
	TS t = { NULL, NULL };
	return(t);
}


bool emptyTS(TS t){
	return(t.tete ? false : true);
}


void addTS(TS* t, char* nom, char* type, int taille, bool constante){
	TS q = *t;

	while(!emptyTS(q)){
		if(strcmp(nom, (q.tete)->info.nom) == 0)
			goto finAddTS;
		q.tete = (q.tete)->suiv;
	}

	ElementTS* e = malloc(sizeof(ElementTS));

	InfoTS info = { nom, type, taille, constante };
	e->info = info;
	e->suiv = NULL;

	if(emptyTS(*t)){
		(*t).tete = e;
		(*t).queue = e;
	}
	else{
		((*t).queue)->suiv = e;
		(*t).queue = e;
	}

	finAddTS: ;
}


int sizeTS(TS t){
	int i = 0;

	while(!emptyTS(t)){
		i++;
		t.tete = (t.tete)->suiv;
	}
	return i;
}


InfoTS lookUp(TS t, char* nom){
	InfoTS info = { .nom = NULL };

	while(!emptyTS(t)){
		if(strcmp(nom, (t.tete)->info.nom) == 0){
			info = (t.tete)->info;
			break;
		}
		t.tete = (t.tete)->suiv;
	}

	return info;
}


bool updateTS(TS t, char* name, int column, char* value){
	ElementTS* e = NULL;

	while(!emptyTS(t)){
		if(strcmp(name, (t.tete)->info.nom) == 0){
			e = t.tete;
			break;
		}
		t.tete = (t.tete)->suiv;
	}

	if(e != NULL){
		switch(column){
			case 2:
				(e->info).type = value;
				break;
			case 3:
				(e->info).taille = atoi(value);
				break;
			case 4:
				if(strcmp(value, "false") == 0)
					(e->info).constante = false;
				else
					(e->info).constante = true;
				break;
		}
		return true;
	}

	return false;
}


void displayTS(TS t){
	InfoTS info;
	int i=0;

	printf(COLOR_YELLOW);
	printf("\n\t┌────────────────────────────────────────────────┐");
	printf("\n\t│                TABLE OF SYMBOLS                │");
	printf("\n\t├──────┬────────────┬────────┬────────┬──────────┤");
	printf("\n\t│Number│    Name    │  Type  │  Size  │ Constant │");
	printf("\n\t├──────┼────────────┼────────┼────────┼──────────┤\n");

	while(!emptyTS(t)){
		info = (t.tete)->info;
		printf("\t│%-6d│%-12s│%-8s│%-8d│%-10s│", i, info.nom, info.type ? info.type : "", info.taille, info.constante ? "Yes" : "No");
		printf("\n\t├──────┼────────────┼────────┼────────┼──────────┤\n");
		i++;
		t.tete = (t.tete)->suiv;
	}

	printf("\n\n"COLOR_RESET);
}
