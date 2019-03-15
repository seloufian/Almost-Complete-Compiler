#include "arith.h"

ARITH initARITH(){
	ARITH a = { 0, NULL, NULL };
	return(a);
}


void addARITH(ARITH* a, char* infoAdd){

	ElementARITH *e = malloc(sizeof(ElementARITH));

	e->info = infoAdd;
	e->next = NULL;

	if(a->size == 0){ /* "ARITH" is empty */
		a->head = e;
		a->tail = e;
	}
	else{
		(a->tail)->next = e;
		a->tail = e;
	}

	(a->size)++;
}


void destroyARITH(ARITH *a){
	ElementARITH *e;

	a->tail = NULL;

	while(a->head != NULL){
		e = a->head;
		a->head = (a->head)->next;
		(a->size)--;
		free(e);
	}
}


bool createQR_ARITH(ARITH *a, QR *r, int *numQR, int *lastTemp, char* affectTo){

	ElementARITH *pos1, *pos2, *pos3, *h = a->head;

	int size = a->size;
	char tempName[10];

	bool lestTemp_changed = false;

	if(size == 1){
		if(affectTo){
			quad(&(*r), ":=", strdup(h->info), NULL, affectTo);
			(*numQR)++;
		}
	}
	else{
		lestTemp_changed = true;
		while(size > 1){

			pos1 = h;
			pos2 = h->next;
			pos3 = (h->next)->next;

			if(!strcmp(pos3->info, "+") || !strcmp(pos3->info, "-") || !strcmp(pos3->info, "*") ||!strcmp(pos3->info, "/")){

				sprintf(tempName, "_T%d_", *lastTemp);

				quad(&(*r), strdup(pos3->info), strdup(pos1->info), strdup(pos2->info), strdup(tempName));
				(*numQR)++;
				(*lastTemp)++;

				pos1->info = strdup(tempName);
				pos1->next = pos3->next;

				free(pos2);
				free(pos3);

				size -= 2;

				h = a->head;
			}
			else
				h = h->next;
		}

		if(affectTo){
			quad(&(*r), ":=", strdup(tempName), NULL, affectTo);
			(*numQR)++;
		}
	}

	destroyARITH(a);
	*a = initARITH();

	return(lestTemp_changed);
}


void displayARITH(ARITH a){
	printf("\n\n(Size = %d)   ", a.size);
	while(a.head != NULL){
		printf("%s  ", (a.head)->info);
		a.head = (a.head)->next;
	}
	printf("\n\n");
}
