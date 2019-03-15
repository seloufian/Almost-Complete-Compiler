#ifndef arith_h_
#define arith_h_

#include "general.h"
#include "quadruplets.h"


typedef struct ElementARITH {
	char *info;
	struct ElementARITH *next;
}ElementARITH;

typedef struct ARITH {
	int size;
	ElementARITH *head, *tail;
}ARITH;


ARITH initARITH();

void addARITH(ARITH* a, char* infoAdd);

void destroyARITH(ARITH *a);

bool createQR_ARITH(ARITH *a, QR *r, int *numQR, int *lastTemp, char* affectTo);

void displayARITH(ARITH a);

#endif
