%union{
	char *String;
}

%{
#include "include/symbolsTable.h"
#include "include/quadruplets.h"
#include "include/semantic.h"
#include "include/routine.h"
#include "include/arith.h"
#include "include/stack.h"

int yylex();
int yyerror(char *msg);

int nbLigne=1, nbColonne=1; /* Use : Specify the location of the syntactic or semantic error */

int EXISTERR = false; /* Use : Return value of the semantic analyzer (0: no error, 1: there is at least one error) */

TS ts; /* The symbol table (Identifier information) */

QR q; /* The table of quadruplets */

Biblio biblio = { false, false, false }; /* Use : Verification of the import of the necessary libraries + uniqueness */

int numLigneBIB = 0; /* Use : Line number of a library import (Import each library in one line) */

char *infoIDF_type, *infoIDF_constante; /* Use : Update of the TS with the information of the declared variable */

InfoTS infoIDF; /* Use : Get information of the current variable (semantic error checking) */

bool estDECLARATION = false; /* Use : Check if it is a declaration (TRUE) or not (FALSE) */
bool estAFFECTATION = false; /* Use : Check whether it is an ASSIGNMENT (TRUE) or not (FALSE) */
bool estCONDITION = false; /* Use : Check if it is a CONDITION [IF] (TRUE) or not (FALSE) */
bool estBOUCLE = false; /* Use : Check if it is a LOOP [WHILE] (TRUE) or not (FALSE) */

CONSTAFF caf;

bool estDebutAFFECT = false; /* Use : Differentiate the variable to which we assign the operation of the operation itself */

char *sauvType; /* Use : Save the type of the current variable (semantic error checking) */

int indiceIDF; /* Use : Save the size of the current table (semantic error checking) */

bool ignorerINST = false; /* Use : Ignore the current instruction (ASSIGN or COMPARISON [IF or WHILE]) due to a serious error */

ARITH a; /* Use : Save arithmetic expressions as postfix (Reverse Polish Notation) */

int qc=0; /* Use : Indicate the number of the next quadruplet (Quadruplet Counter) */
int tmpc=0; /* Use : Indicate the number of the next temporary (Temporary Counter) */

Stack s; /* Use : LIFO list, save the number of a quadruplet (to come back later) */

char cote1COMP[30], cote2COMP[30]; /* Use : Save both sides of a comparison (Number, variable or temporary) */

char chTemp[30]; /* Use : Temporary character string (multiple uses) */

%}

%token MC_PRG <String>NOM_PROGRAMME <String>IDF <String>BIB_CALCULE <String>BIB_BOUCLE <String>BIB_TAB <String>MC_INTEGER <String>MC_REAL <String>MC_CONST MC_WHILE MC_EXECUT MC_IF <String>NOMBRE
%token '{' '}' ';' '(' ')' '[' ']'
%token SEPARATEUR "||"
%token <String>AFFECTER ":="
%token <String>COMP_EQUAL "=="
%token <String>COMP_LESS "<"
%token <String>COMP_GRATHER ">"
%token <String>COMP_LESS_OR_EQUAL "<="
%token <String>COMP_GRATHER_OR_EQUAL ">="
%token <String>COMP_NOT_EQUAL "!="

%type <String>SIGNE
%type <String>VARIABLE
%type <String>AVANT_OPERATION
%type <String>OPERATEUR_COMP

%left '+' '-'
%left '*' '/'

%%
S: BIB MC_PRG NOM_PROGRAMME '{' DECLARATION LISTE_INST '}' { print_SyntaxOK(); return(EXISTERR); };

BIB: BLOC_BIB_CALCULE BIB | BLOC_BIB_BOUCLE BIB | BLOC_BIB_TAB BIB |;

BLOC_BIB_CALCULE: BIB_CALCULE {
								if(numLigneBIB == nbLigne){ /* Syntactic error : TWO library imports in a single line */
									yyerror(NULL); EXISTERR = true; return(EXISTERR);
								}

								numLigneBIB = nbLigne;

								ROUTINE_Une_Seule_Bibliotheque(&biblio, $1, nbLigne, nbColonne);
							};

BLOC_BIB_BOUCLE: BIB_BOUCLE {
								if(numLigneBIB == nbLigne){ /* Syntactic error : TWO library imports in a single line */
									yyerror(NULL); EXISTERR = true; return(EXISTERR);
								}

								numLigneBIB = nbLigne;

								ROUTINE_Une_Seule_Bibliotheque(&biblio, $1, nbLigne, nbColonne);
							};

BLOC_BIB_TAB: BIB_TAB {
						if(numLigneBIB == nbLigne){ /* Syntactic error : TWO library imports in a single line */
							yyerror(NULL); EXISTERR = true; return(EXISTERR);
						}

						numLigneBIB = nbLigne;

						ROUTINE_Une_Seule_Bibliotheque(&biblio, $1, nbLigne, nbColonne);
					};



DECLARATION: DEBUT_DECLARATION TYPE CONSTANTE LISTE_VAR ';' DECLARATION | FIN_DECLARATION;

DEBUT_DECLARATION: { ROUTINE_Marquer_DECLARATION(&estDECLARATION, true); };
FIN_DECLARATION: { ROUTINE_Marquer_DECLARATION(&estDECLARATION, false); };



TYPE: MC_INTEGER { ROUTINE_Marquer_TYPE_DECLARATION(&infoIDF_type, strdup($1)); } | MC_REAL { ROUTINE_Marquer_TYPE_DECLARATION(&infoIDF_type, strdup($1)); };



CONSTANTE: MC_CONST { ROUTINE_Marquer_CONST_DECLARATION(&infoIDF_constante, "true"); } | { ROUTINE_Marquer_CONST_DECLARATION(&infoIDF_constante, "false"); };



LISTE_VAR: VARIABLE "||" LISTE_VAR | VARIABLE;



VARIABLE: IDF {
				infoIDF = lookUp(ts, $1); /* Get information about the current variable from the TS */

				if(estDECLARATION){
					if(! infoIDF.type){ /* If the type of the current variable is not defined. So, this is his first statement, updated TS */
						updateTS(ts, $1, TS_TYPE, infoIDF_type); /* Mettre à jour le type de la variable courante */
						updateTS(ts, $1, TS_LENGTH, "0"); /* Update the size of the current variable ("0" for a single variable) */
						updateTS(ts, $1, TS_CONSTANT, infoIDF_constante); /* indicate if it is a constant */
					}
					else{
						printf(COLOR_RED"\n- Semantic error (%d:%d) : Double declaration of \"%s\"."COLOR_RESET, nbLigne, nbColonne, $1);
						EXISTERR = true;
					}
					goto Fin_Variable_Id_Simple; /* Finish the routine */
				}

				if(! infoIDF.type){ /* In other cases (excluding DECLARATION) : If the type does not exist. So, the variable was not declared */
					printf(COLOR_RED"\n- Semantic error (%d:%d) : Variable \"%s\" not declared."COLOR_RESET, nbLigne, nbColonne, $1);
					ignorerINST = true;
					EXISTERR = true;
					goto Fin_Variable_Id_Simple; /* Semantic error detected : Finish the routine */
				}

				if(! ignorerINST){
					if(sauvType == NULL){ /* This is the beginning of the assignment : Save the type of the variable (to check the incompatibility) */
						sauvType = infoIDF.type;
						if(infoIDF.constante && estDebutAFFECT && existeCONSTAFF(&caf, $1)){
							printf(COLOR_RED"\n- Semantic Error (%d:%d) : \"%s\" is a constant, other assignment not permitted."COLOR_RESET, nbLigne, nbColonne, $1);
							ignorerINST = true;
							EXISTERR = true;
							goto Fin_Variable_Id_Simple; /* Semantic error detected : Finish the routine */
						}
					}
					else{
						if(!sameString(infoIDF.type, sauvType)){ /* Note: We are sure that "sauvType" is defined (definition in "estDebutAFF" in the rule VARIABLE) */
							printf(COLOR_RED"\n- Semantic error (%d:%d) : Incompatibility of type in ", nbLigne, nbColonne);
							ignorerINST = true; /* Semantic error in the ASSIGNMENT, ignore the current statement (you must first fix this error) */
							EXISTERR = true;
							if(estAFFECTATION)
								printf("the assignment (\"%s\" to \"%s\")."COLOR_RESET, infoIDF.type, sauvType);
							else{
								if(! estBOUCLE)
									printf("the condition (comparison \"%s\" whih \"%s\")."COLOR_RESET, infoIDF.type, sauvType);
								else
									printf("the loop (comparison \"%s\" with \"%s\")."COLOR_RESET, infoIDF.type, sauvType);
							}
						}
					}

					if(infoIDF.taille != 0){
						printf(COLOR_RED"\n- Semantic error (%d:%d) : \"%s\" is an array, not a simple variable."COLOR_RESET, nbLigne, nbColonne, $1);
						EXISTERR = true;
						goto Fin_Variable_Id_Simple; /* Semantic error detected : Finish the routine */
					}
				}

				Fin_Variable_Id_Simple:
					$$ = $1; /* Save the variable in the non-terminal "VARIABLE" (to recover it in another rule) */
			}
		| IDF '[' NOMBRE ']' {
								if(! biblio.tab) /* For a variable of the "array" type, the "TAB" library must be imported */
									printf(COLOR_RED"\n- Semantic error (%d:%d) : Library \"TAB\" not imported."COLOR_RESET, nbLigne, nbColonne);

								if(sameString(number_type($3), "REAL")){ /* The index of the table is of real type (positive or negative) */
									printf(COLOR_RED"\n- Semantic error (%d:%d) : Size of the array \"%s\" must be an integer."COLOR_RESET, nbLigne, nbColonne, $1);
									ignorerINST = true;
									EXISTERR = true;
									goto Fin_Variable_Id_Tab; /* Semantic error detected : Finish the routine */
								}

								indiceIDF = atoi(convert_INTEGER_to_char($3)); /* Save the array index (we are sure that it is an integer) */

								if((strlen($3) > 5) || (indiceIDF > 32768)){ /* The size of the array must be in the range [0; 32768] */
									printf(COLOR_RED"\n- Semantic error (%d:%d) : Size of the array \"%s\" too long (overflow)."COLOR_RESET, nbLigne, nbColonne, $3);
									ignorerINST = true;
									EXISTERR = true;
									goto Fin_Variable_Id_Tab; /* Semantic error detected : Finish the routine */
								}

								if(indiceIDF <= 0){ /* NEGATIVE or NULL array index : Semantic error */
										printf(COLOR_RED"\n- Semantic error (%d:%d) : Size of the array \"%s\" must be positive non-zero."COLOR_RESET, nbLigne, nbColonne, $1);
										ignorerINST = true;
										EXISTERR = true;
										goto Fin_Variable_Id_Tab; /* Semantic error detected : Finish the routine */
								}

								infoIDF = lookUp(ts, $1); /* Get the current variable information from the TS */

								if(estDECLARATION){
									if(! infoIDF.type){ /* If the type of the current variable is not defined. So it's his first statement, update the TS */
										updateTS(ts, $1, TS_TYPE, infoIDF_type); /* Mettre à jour le type de la variable courante */
										updateTS(ts, $1, TS_LENGTH, convert_INTEGER_to_char($3)); /* Update the type of the current variable */
										updateTS(ts, $1, TS_CONSTANT, infoIDF_constante); /* Indicate if it is a constant */
										quad(&q, "bOUNDS", "1", $3, NULL);		qc++;
										quad(&q, "ADEC", $1, NULL, NULL);		qc++;
									}
									else{
										printf(COLOR_RED"\n- Semantic error (%d:%d) : Double declaration of \"%s\"."COLOR_RESET, nbLigne, nbColonne, $1);
										EXISTERR = true;
									}
									goto Fin_Variable_Id_Tab; /* Finish the routine */
								}

								if(! infoIDF.type){ /* In other cases (excluding DECLARATION) : If the type does not exist. So, the variable was not declared */
									printf(COLOR_RED"\n- Semantic error (%d:%d) : Variable \"%s\" not declared."COLOR_RESET, nbLigne, nbColonne, $1);
									ignorerINST = true;
									EXISTERR = true;
									goto Fin_Variable_Id_Tab; /* Semantic error detected : Finish the routine */
								}

								if(! ignorerINST){
									if(sauvType == NULL){ /* This is the beginning of the assignment : Save the type of the variable (to check the incompatibility) */
										sauvType = infoIDF.type;
										if(infoIDF.constante && estDebutAFFECT && existeCONSTAFF(&caf, $1)){
											printf(COLOR_RED"\n- Semantic error (%d:%d) : \"%s\" is a constant, other assignment not permitted."COLOR_RESET, nbLigne, nbColonne, $1);
											ignorerINST = true;
											EXISTERR = true;
											goto Fin_Variable_Id_Tab; /* Semantic error detected : Finish the routine */
										}
									}
									else{
										if(!sameString(infoIDF.type, sauvType)){ /* Note: We are sure that "sauvType" is defined (definition in "estDebutAFF" in the rule VARIABLE) */
											printf(COLOR_RED"\n- Semantic error (%d:%d) : Incompatibility of type in ", nbLigne, nbColonne);
											ignorerINST = true; /* Semantic error in the ASSIGNMENT, ignore the current statement (you must first fix this error) */
											EXISTERR = true;
											if(estAFFECTATION)
												printf("the assignment (\"%s\" to \"%s\")."COLOR_RESET, infoIDF.type, sauvType);
											else{
												if(! estBOUCLE)
													printf("the condition (comparison \"%s\" with \"%s\")."COLOR_RESET, infoIDF.type, sauvType);
												else
												printf("the loop (comparison \"%s\" with \"%s\")."COLOR_RESET, infoIDF.type, sauvType);
											}
										}
									}

									if(infoIDF.taille == 0){
										printf(COLOR_RED"\n- Semantic error (%d:%d) : \"%s\" is a simple variable, not an array."COLOR_RESET, nbLigne, nbColonne, $1);
										EXISTERR = true;
										goto Fin_Variable_Id_Tab; /* Semantic error detected : Finish the routine */
									}

									if(indiceIDF > infoIDF.taille){
										printf(COLOR_RED"\n- Semantic error (%d:%d) : Exceeding the size of the table \"%s\"."COLOR_RESET, nbLigne, nbColonne, $1);
										EXISTERR = true;
										goto Fin_Variable_Id_Tab; /* Semantic error detected : Finish the routine */
									}
								}

								Fin_Variable_Id_Tab:
									sprintf(chTemp, "%s[%s]", $1, $3);
									$$ = strdup(chTemp); /* Save the variable in the non-terminal "VARIABLE" (to recover it in another rule) */
							};



LISTE_INST: INST LISTE_INST |;



INST: AVANT_INST_AFFECTATION AFFECTATION | AVANT_INST_BOUCLE BOUCLE | CONDITION;

AVANT_INST_AFFECTATION: { if(!biblio.calcul) { printf(COLOR_RED"\n- Semantic error (%d:%d) : Library \"CALCUL\" not imported."COLOR_RESET, nbLigne, nbColonne); EXISTERR = true; } };
AVANT_INST_BOUCLE: { if(!biblio.boucle) { printf(COLOR_RED"\n- Semantic error (%d:%d) : Library \"BOUCLE\" not imported."COLOR_RESET, nbLigne, nbColonne); EXISTERR = true; } };



AFFECTATION: AVANT_PT_VIRGULE ';';

AVANT_VARIABLE: { estAFFECTATION = true; estDebutAFFECT = true; };
AVANT_OPERATION: AVANT_VARIABLE VARIABLE { estDebutAFFECT = false; $$ = $2; };



AVANT_PT_VIRGULE: AVANT_OPERATION ":=" OPERATION
		{
			if(!ignorerINST)
				createQR_ARITH(&a, &q, &qc, &tmpc, $1);

			destroyARITH(&a);

			estAFFECTATION = false;
			ignorerINST = false;
			sauvType = NULL;
		};



OPERATION: OPERATION '+' OPERATION { if(!ignorerINST) addARITH(&a, "+"); }
		| OPERATION '-' OPERATION { if(!ignorerINST) addARITH(&a, "-"); }
		| OPERATION '*' OPERATION { if(!ignorerINST) addARITH(&a, "*"); }
		| OPERATION '/' OPERATION { if(!ignorerINST) addARITH(&a, "/"); }
		| OPERANDE;



BOUCLE: MC_WHILE { pushStack(&s, qc); estBOUCLE = true; } COMPARAISON INST_BOUCLE {
																					if(! ignorerINST){
																						if(EXISTERR == 0){
																						sprintf(chTemp, "%d", qc+1);
																						updateQR(q, popStack(&s), QR_POS_2, strdup(chTemp));

																						sprintf(chTemp, "%d", popStack(&s));
																						quad(&q, "BR", strdup(chTemp), NULL, NULL);

																						qc++;
																						}
																					}
																				};
INST_BOUCLE: '{' LISTE_INST '}' | INST;



CONDITION: MC_EXECUT { quad(&q, "BR", NULL, NULL, NULL); pushStack(&s, qc); pushStack(&s, qc); qc++; } LISTE_INST MC_IF COMPARAISON;


COMPARAISON: {
				estCONDITION = true;
				if(!ignorerINST){
					if(! estBOUCLE){ /* CONDITION */
						sprintf(chTemp, "%d", qc+1);
						updateQR(q, popStack(&s), QR_POS_2, strdup(chTemp));

						quad(&q, "BR", NULL, NULL, NULL);
						pushStack(&s, qc);
						qc++;
					}
				}
			}
			'(' OPERATION
			{
				if(!ignorerINST){ /* COMPARAISON (CONDITION or BOUCLE) */
						sprintf(cote1COMP, "%s", (a.head)->info);
						if(createQR_ARITH(&a, &q, &qc, &tmpc, NULL))
							sprintf(cote1COMP, "_T%d_", tmpc-1);
				}
			}
			OPERATEUR_COMP OPERATION ')'
			{ 	if(!ignorerINST){
					if(!estBOUCLE){ /* CONDITION */
						sprintf(cote2COMP, "%s", (a.head)->info);
						if(createQR_ARITH(&a, &q, &qc, &tmpc, NULL))
							sprintf(cote2COMP, "_T%d_", tmpc-1);

						sprintf(chTemp, "%d", qc+1);
						updateQR(q, popStack(&s), QR_POS_2, strdup(chTemp));

						sprintf(chTemp, "%d", popStack(&s)+1);

						switch($5[0]){
							case '=':
								quad(&q, "BE", strdup(chTemp), strdup(cote1COMP), strdup(cote2COMP));
								break;
							case '!':
								quad(&q, "BNE", strdup(chTemp), strdup(cote1COMP), strdup(cote2COMP));
								break;
							case '<':
								strlen($5) == 1 ? quad(&q, "BL", strdup(chTemp), strdup(cote1COMP), strdup(cote2COMP)) : quad(&q, "BLE", strdup(chTemp), strdup(cote1COMP), strdup(cote2COMP));
								break;
							case '>':
								strlen($5) == 1 ? quad(&q, "BG", strdup(chTemp), strdup(cote1COMP), strdup(cote2COMP)) : quad(&q, "BGE", strdup(chTemp), strdup(cote1COMP), strdup(cote2COMP));
						}

						qc++;
					}
					else{ /* BOUCLE (LOOP) */
						sprintf(cote2COMP, "%s", (a.head)->info);
						if(createQR_ARITH(&a, &q, &qc, &tmpc, NULL))
							sprintf(cote2COMP, "_T%d_", tmpc-1);

						switch($5[0]){
							case '=':
								quad(&q, "BNE", NULL, strdup(cote1COMP), strdup(cote2COMP));
								break;
							case '!':
								quad(&q, "BE", NULL, strdup(cote1COMP), strdup(cote2COMP));
								break;
							case '<':
								strlen($5) == 1 ? quad(&q, "BGE", NULL, strdup(cote1COMP), strdup(cote2COMP)) : quad(&q, "BG", NULL, strdup(cote1COMP), strdup(cote2COMP));
								break;
							case '>':
								strlen($5) == 1 ? quad(&q, "BLE", NULL, strdup(cote1COMP), strdup(cote2COMP)) : quad(&q, "BL", NULL, strdup(cote1COMP), strdup(cote2COMP));
						}

						pushStack(&s, qc);
						qc++;
					}
				}

				ignorerINST = false;
				estCONDITION = false;
				estBOUCLE = false;
				sauvType = NULL;
				destroyARITH(&a);
			};



OPERANDE: VARIABLE
		{
			if(!estDebutAFFECT && !ignorerINST)
				addARITH(&a, $1);
		}
		| SIGNE NOMBRE
		{
			sprintf(chTemp, "%s%s", $1, $2); /* Save current number and his sign */
			/* An INTEGER must be in the range [-32767; 32768] */
			if((!sameString(number_type($2), "REAL")) && ((strlen($2) > 5) || (sameString($1, "-") && atoi(convert_INTEGER_to_char($2)) > 32767) || (atoi(convert_INTEGER_to_char($2)) > 32768)) ){
					printf(COLOR_RED"\n- Semantic error (%d:%d) : Size of integer \"%s%s\" too long (overflow).", nbLigne, nbColonne, $1, $2);
					ignorerINST = true; /* Semantic error, ignore the current statement (we must first fix this error) */
					EXISTERR = true;
			}
			else
				if(!estDebutAFFECT && !ignorerINST) { /* OPERANDE called INSTRUCTION (which is valid for the moment) */

					addARITH(&a, strdup(chTemp));

					if(sauvType == NULL){ /* "sauvType" is not defined (first use of a number in this comparison [If it's an assignment, "savType" will surely already be defined]) */
						sauvType = number_type($2);
					}
					else{ /* "sauvType" is already defined */
						if(! sameString(sauvType, number_type($2))){
							printf(COLOR_RED"\n- Semantic error (%d:%d) : Incompatibility of type in ", nbLigne, nbColonne);
							ignorerINST = true; /* Semantic error, ignore the current statement (we must first fix this error) */
							EXISTERR = true;
							if(estAFFECTATION)
								printf("the assignment (\"%s\" to \"%s\")."COLOR_RESET, number_type($2), sauvType);
							else{
								if(!estBOUCLE)
									printf("the condition (comparison \"%s\" with \"%s\")."COLOR_RESET, sauvType, number_type($2));
								else
									printf("the loop (comparison \"%s\" with \"%s\")."COLOR_RESET, sauvType, number_type($2));
							}
						}
					}
				}
		};



SIGNE: '+' { $$ = "+"; }
	| '-' { $$ = "-"; }
	| { $$ = ""; };



OPERATEUR_COMP: "==" { $$ = $1; }
			| "<" { $$ = $1; }
			| ">" { $$ = $1; }
			| "<=" { $$ = $1; }
			| ">=" { $$ = $1; }
			| "!=" { $$ = $1; };

%%
int main(){
	ts = initTS();
	q = initQR();
	a = initARITH();
	s = initStack();
	caf = initCONSTAFF();

	if(yyparse() == 1)
		return 0;

	displayTS(ts);

	printf(COLOR_RESET"\t\tQuadruplets \"BEFORE\" optimisation :\n");
	displayQR(q);

	simplificationAlgebrique(&q, qc/2);
	propagationDeConstantes(&q, &ts);

	printf(COLOR_RESET"\t\tQuadruplets \"AFTER\" optimisation :\n");
	displayQR(q);
}

int yywrap(){
}

int yyerror(char *msg){
	printf(COLOR_RED"\n\n\t- Syntactic error at line %d, column %d.\n\n"COLOR_RESET, nbLigne, nbColonne);
}

