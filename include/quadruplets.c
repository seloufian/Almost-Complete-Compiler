#include "quadruplets.h"

QR initQR(){
	QR r;
	r.tete = NULL;	r.queue = NULL;
	return(r);
}


bool emptyQR(QR r){
	if(r.tete == NULL)
		return true;
	return false;
}


void quad(QR* r, char* position_1, char* position_2, char* position_3, char* position_4){

	ElementQR* e = malloc(sizeof(ElementQR));

	InfoQR info = { position_1, position_2, position_3, position_4 };
	e->info = info;
	e->suiv = NULL;

	if(emptyQR(*r)){
		(*r).tete = e;
		(*r).queue = e;
	}
	else{
		((*r).queue)->suiv = e;
		(*r).queue = e;
	}
}


int sizeQR(QR r){
	int i = 0;

	while(!emptyQR(r)){
		i++;
		r.tete = (r.tete)->suiv;
	}
	return i;
}


bool updateQR(QR r, int numberQR, int position, char* value){
	ElementQR* e = NULL;

	int posQR = 0;
	while(!emptyQR(r)){
		if(posQR == numberQR){
			e = r.tete;
			break;
		}
		r.tete = (r.tete)->suiv;
		posQR++;
	}

	if(e != NULL){
		switch(position){
			case 1:
				(e->info).position_1 = value;
				break;
			case 2:
				(e->info).position_2 = value;
				break;
			case 3:
				(e->info).position_3 = value;
				break;
			case 4:
				(e->info).position_4 = value;
				break;
		}
		return true;
	}

	return false;
}


bool deleteQR(QR *q, int numQR){
	ElementQR *e = q->tete, *prec = NULL;
	QR r = *q;

	while(numQR-- && r.tete){
		prec = r.tete;
		r.tete = (r.tete)->suiv;
		e = r.tete;
	}

	if(! r.tete)
		return false;

	if(! prec){ /* Delete the FIRST item */
		q->tete = (q->tete)->suiv;
		if(! q->tete) /* List with ONE item */
			q->queue = NULL;
	}
	else{
		if(! e->suiv){ /* Delete the LAST item */
			prec->suiv = NULL;
			q->queue = prec; /* Backward tail */
		}
		else{ /* Delete an item that is neither the FIRST nor the LAST */
			prec->suiv = e->suiv;
		}
	}

	free(e);

	return true;
}


void displayQR(QR r){
	InfoQR info;
	int i=0;

	printf(COLOR_CYAN);
	printf("\n\t┌────────────────────────────────────────────────────┐");
	printf("\n\t│                   THE QUADRUPPLES                  │");
	printf("\n\t├──────┬─────────────────────────────────────────────┤");
	printf("\n\t│Number│                  Quadruple                  │");
	printf("\n\t├──────┼──────┬────────────┬────────────┬────────────┤\n");

	while(!emptyQR(r)){
		info = (r.tete)->info;
		printf("\t│%-6d│%-6s│", i, info.position_1);
		info.position_1[0]=='B' ? printf("Quad_%-7s│", info.position_2) : printf("%-12s│", info.position_2 ? info.position_2 : "");
		printf("%-12s│%-12s│", info.position_3 ? info.position_3 : "", info.position_4 ? info.position_4 : "");

		printf("\n\t├──────┼──────┼────────────┼────────────┼────────────┤\n");
		i++;
		r.tete = (r.tete)->suiv;
	}

	printf("\t│%-6d│ End of program...                           │", i);
	printf("\n\t└──────┴─────────────────────────────────────────────┘");

	printf("\n\n"COLOR_RESET);
}
