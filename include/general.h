#ifndef general_h_
#define general_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define COLOR_RED		"\x1b[31m"
#define COLOR_GREEN		"\x1b[32m"
#define COLOR_YELLOW	"\x1b[33m"
#define COLOR_BLUE		"\x1b[34m"
#define COLOR_MAGENTA	"\x1b[35m"
#define COLOR_CYAN		"\x1b[36m"
#define COLOR_RESET		"\x1b[0m"


#define INTEGER_POSITIVE 1
#define INTEGER_NEGATIVE -1
#define FLOAT_POSITIVE 2
#define FLOAT_NEGATIVE -2


#define YYLVAL_STRING yylval.String = strdup(yytext)


typedef struct Biblio{
	bool calcul;
	bool boucle;
	bool tab;
}Biblio;


void updateComment(char *comment, int *lines, int *columns);

void print_SyntaxOK();

int number_type_sign(char *number);

char *number_type(char *number);

char *convert_INTEGER_to_char(char *number);

bool sameString(char *string_1, char *string_2);

#endif
