%{
/*Déclarations C (pré-code)*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntaxique.tab.h"
extern YYSTYPE yylval;
int colonne=1;
int ligne=1;
%}
/*Définitions*/
entier (0|[1-9][0-9]*)|"(-"[1-9][0-9]*")"
reel {entier}"."[0-9]*|"(-"[1-9][0-9]*"."[0-9]*")"
caractere \'.\'
bool true|false
idf [A-Z][a-zA-Z0-9_]*[a-zA-Z0-9]*
commentaire "#"(.)*
/*Règles de traduction*/
%%
"INTEGER" {printf ("INTEGER type %s\n",yytext); colonne=colonne+yyleng; return INTEGER;}
"NUMERIC" {printf ("NUMERIC type %s\n",yytext); colonne=colonne+yyleng; return NUMERIC;}
"CHARACTER" {printf ("CHARACTER type %s\n",yytext); colonne=colonne+yyleng; return CHARACTER;}
"LOGICAL" {printf ("LOGICAL type %s\n",yytext); colonne=colonne+yyleng; return LOGICAL;}
"IF" {printf ("if %s\n",yytext); colonne=colonne+yyleng; return IF;}
"ELSE" {printf("else %s\n",yytext);colonne=colonne+yyleng;return ELSE;}
"FOR" {printf ("for %s\n",yytext); colonne=colonne+yyleng; return FOR;}
"IN" {printf ("in %s\n",yytext); colonne=colonne+yyleng; return IN;}
"WHILE" {printf ("while %s\n",yytext); colonne=colonne+yyleng; return WHILE;}
"," {printf ("virgule %s\n",yytext); colonne=colonne+yyleng; return ',';}
"[" {printf ("crochet %s\n",yytext); colonne=colonne+yyleng; return '[';}
"]" {printf ("crochet %s\n",yytext); colonne=colonne+yyleng; return ']';}
"(" {printf ("parenthese %s\n",yytext); colonne=colonne+yyleng; return '(';}
")" {printf ("parenthese %s\n",yytext); colonne=colonne+yyleng; return ')';}
"{" {printf ("accolade %s\n",yytext); colonne=colonne+yyleng; return '{';}
"}" {printf ("accolade %s\n",yytext); colonne=colonne+yyleng; return '}';}
":" {printf ("deux points %s\n",yytext); colonne=colonne+yyleng; return ':';}
"+" {printf ("addition %s\n",yytext); colonne=colonne+yyleng; return '+';}
"-" {printf ("soustraction %s\n",yytext); colonne=colonne+yyleng; return '-';}
"*" {printf ("multiplication %s\n",yytext); colonne=colonne+yyleng; return '*';}
"/" {printf ("division %s\n",yytext); colonne=colonne+yyleng; return '/';}
"%" {printf ("reste %s\n",yytext); colonne=colonne+yyleng; return '%';}
"<" {printf ("comparaison %s\n",yytext); colonne=colonne+yyleng; return '<';}
">" {printf ("comparaison %s\n",yytext); colonne=colonne+yyleng; return '>';}
"=" {printf ("egalite %s\n",yytext); colonne=colonne+yyleng; return Eg;}
"!=" {printf ("comparaison %s\n",yytext); colonne=colonne+yyleng; return NonEg;}
"<=" {printf ("comparaison %s\n",yytext); colonne=colonne+yyleng; return InfEg;}
">=" {printf ("comparaison %s\n",yytext); colonne=colonne+yyleng; return SupEg;}
"∧" {printf ("et %s\n",yytext); colonne=colonne+yyleng; return and;}
"∨" {printf ("ou %s\n",yytext); colonne=colonne+yyleng; return or;}
"<-" {printf ("Affectation %s\n",yytext); colonne=colonne+yyleng; return Aff;}
{idf} {if (yyleng<=10) {printf ("idf %s\n",yytext); yylval.chaine=strdup(yytext); return Idf;} else {printf ("ERREUR LEXICALE : taille IDF trop longue , Ligne : %d , Colonne : %d , Entité : %s \n",ligne,colonne,yytext); colonne=colonne+yyleng;}}
{entier} {if ( atoi(yytext) < -32768 || atoi(yytext) > 32767 ) {printf("l'entier %d n'est pas reconnu\n",atoi(yytext)); }  else{ printf("Entier %d \n", atoi(yytext));} 
yylval.entier=atoi(yytext); return Entier;}
{reel} {printf ("Reel %s\n",yytext); colonne=colonne+yyleng; yylval.reel=atof(yytext); return Reel;}
{caractere} {printf ("Caractère %s\n",yytext); colonne=colonne+yyleng; yylval.chaine=strdup(yytext); return Caractere;}
{bool} {printf("Booleen %s\n",yytext); colonne=colonne+yyleng; yylval.chaine=strdup(yytext); return Booleen;}
{commentaire} {printf ("commentaire\n"); colonne=colonne+yyleng;}
"\n"+ {printf ("Saut de ligne\n");ligne++; colonne=0;}
" " {colonne=colonne+yyleng;}
"\t" {colonne=colonne+yyleng;}
. {printf(" Erreur Lexicale entite: %s , ligne: %d , colonne: %d\n",yytext,ligne,colonne);}
%%
