#ifndef quadruplets_h_
#define quadruplets_h_

#include "general.h"

#define QR_POS_1	1
#define QR_POS_2	2
#define QR_POS_3	3
#define QR_POS_4	4


typedef struct InfoQR {
	char* position_1;
	char* position_2;
	char* position_3;
	char* position_4;
}InfoQR;

typedef struct ElementQR {
	InfoQR info;
	struct ElementQR *suiv;
}ElementQR;

typedef struct QR {
	ElementQR *tete, *queue;
}QR;


QR initQR();

bool emptyQR(QR r);

void quad(QR* r, char* position_1, char* position_2, char* position_3, char* position_4);

int sizeQR(QR r);

bool updateQR(QR r, int numberQR, int position, char* value);

bool deleteQR(QR *q, int numQR);

void displayQR(QR r);

#endif
