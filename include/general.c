#include "general.h"


void updateComment(char *comment, int *lines, int *columns){
	for(int i=0; i<strlen(comment); i++){
		(*columns)++;
		if(comment[i] == '\n'){
			*columns = 1;
			(*lines)++;
		}
	}
}


void print_SyntaxOK(){
	printf(COLOR_GREEN"\n\n\t\t╭───────────────────────────────╮");
	printf("\n\t\t│ Syntactically correct program │");
	printf("\n\t\t╰───────────────────────────────╯\n"COLOR_RESET);
}


int number_type_sign(char *number){
	bool negative = false, real = false;

	for(int i=0; i<strlen(number); i++)
		switch(number[i]){
			case '-':
				negative = true;
				break;
			case '.':
				real = true;
				goto result;
		}

	result: return((real ? 2 : 1)*(negative ? -1 : 1));
}


char *number_type(char *number){
	for(int i=0; i<strlen(number); i++)
		if(number[i] == '.')
			return("REAL");

	return("INTEGER");
}


char* convert_INTEGER_to_char(char *number){
	char* result_number = strdup(number);

	if(result_number[0] == '+')
		result_number[0] = ' ';

	return(result_number);
}


bool sameString(char *string_1, char *string_2){
	return(strcmp(string_1, string_2) ? false : true);
}
