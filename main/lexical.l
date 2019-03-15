%{
#include "synt.tab.h"
#include "include/symbolsTable.h"

extern YYSTYPE yylval;

extern int nbIdf, nbLigne, nbColonne;
extern TS ts;

bool existeNomPr = false;
%}

biblio_calcul #((c|C)alcul|CALCUL)
biblio_boucle #((b|B)oucle|BOUCLE)
biblio_tab #((t|T)ab|TAB)
identificateur [A-Z][_a-zA-Z0-9]*
motCle_programme ((p|P)rogramme|PROGRAMME)
motCle_integer ((i|I)nteger|INTEGER)
motCle_real ((r|R)eal|REAL)
motCle_const ((c|C)onst|CONST)
separateurVariables "||"
commentaire "//"([^/]|\n|"/"[^/])*"//"
motCle_while ((w|W)hile|WHILE)
motCle_execut ((e|E)xecut|EXECUT)
motCle_if ((i|I)f|IF)
affectation ":="
comparateur_EQUAL "=="
comparateur_LESS "<"
comparateur_GRATHER ">"
comparateur_LESS_OR_EQUAL "<="
comparateur_GRATHER_OR_EQUAL ">="
comparateur_NOT_EQUAL "!="
nombre (([1-9][0-9]*)|(0))(\.[0-9]+)?

%%
{commentaire} { updateComment(yytext, &nbLigne, &nbColonne); }
{biblio_calcul} { YYLVAL_STRING; return(BIB_CALCULE); }
{biblio_boucle} { YYLVAL_STRING; return(BIB_BOUCLE); }
{biblio_tab} { nbColonne+=strlen(yytext); YYLVAL_STRING; return(BIB_TAB); }
{motCle_integer} { nbColonne+=strlen(yytext); YYLVAL_STRING; return(MC_INTEGER); }
{motCle_real} { nbColonne+=strlen(yytext); YYLVAL_STRING; return(MC_REAL); }
{motCle_const} { nbColonne+=strlen(yytext); YYLVAL_STRING; return(MC_CONST); }
{motCle_programme} { nbColonne+=strlen(yytext); return(MC_PRG); }
{motCle_while} { nbColonne+=strlen(yytext); return(MC_WHILE); }
{motCle_execut} { nbColonne+=strlen(yytext); return(MC_EXECUT); }
{motCle_if} { nbColonne+=strlen(yytext); return(MC_IF); }
{affectation} { nbColonne+=strlen(yytext); YYLVAL_STRING; return(AFFECTER); }
{comparateur_LESS_OR_EQUAL} { nbColonne+=strlen(yytext); YYLVAL_STRING; return(COMP_LESS_OR_EQUAL); }
{comparateur_LESS} { nbColonne+=strlen(yytext); YYLVAL_STRING; return(COMP_LESS); }
{comparateur_GRATHER_OR_EQUAL} { nbColonne+=strlen(yytext); YYLVAL_STRING; return(COMP_GRATHER_OR_EQUAL); }
{comparateur_GRATHER} { nbColonne+=strlen(yytext); YYLVAL_STRING; return(COMP_GRATHER); }
{comparateur_NOT_EQUAL} { nbColonne+=strlen(yytext); YYLVAL_STRING; return(COMP_NOT_EQUAL); }
{comparateur_EQUAL} { nbColonne+=strlen(yytext); YYLVAL_STRING; return(COMP_EQUAL); }
{separateurVariables} { nbColonne+=strlen(yytext); return(SEPARATEUR); }
[{};+/*)(\]\[-] { nbColonne+=strlen(yytext); return(yytext[0]); }
{nombre} { nbColonne+=strlen(yytext); YYLVAL_STRING; return(NOMBRE); }
{identificateur} { nbColonne+=strlen(yytext); YYLVAL_STRING; if(existeNomPr){ addTS(&ts, strdup(yytext), NULL, -1, false); return(IDF); } else existeNomPr = true; return(NOM_PROGRAMME); }
[ \t] nbColonne++;
\n { nbLigne++;  nbColonne=1; }
. { nbColonne+=strlen(yytext);  printf(COLOR_RED"\n\n\t- Lexical error at line %d, column %d.\n\n"COLOR_RESET, nbLigne, nbColonne); }

%%

