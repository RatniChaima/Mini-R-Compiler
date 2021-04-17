%{
/*Déclaration (en C ) de variables, constante ,includes ,...*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ts.h"
#include "quad.h"
#include "codeMachine.h"

extern FILE* yyin ;
extern int ligne;
extern int colonne;
int type;
int numQuad=1;
int numT=0;

int yylex();
int yyerror(char* msg) /* permet d’afficher l’erreur générée*/
{msg = "ERREUR SYNTAXIQUE : ";
printf("%s Ligne : %d , Colonne : %d",msg,ligne,colonne); 
return 1;
}
%}
%union
{
/*Déclarations des unités lexicales utilisées */
char *chaine;
int entier;
float reel;
char caractere;
struct list_idf *def_liste_idf;
struct str2{int vrai;int faux;}str2;
struct eltRacFus *liste_next;
struct str1{char *val;int type;}str1;
}
%start S
%token INTEGER NUMERIC CHARACTER LOGICAL IF ELSE FOR IN WHILE ',' '=' '[' ']' '(' ')' '{' '}' '+' '-' '*' '/' '%' '<' '>' Eg NonEg InfEg SupEg and or Aff
%token <chaine>Idf
%token <entier>Entier
%token <reel>Reel
%token <caractere>Caractere
%token <chaine>Booleen
%type <str1>Affectation
%type <str1>exp1
%type <str1>exp2
%type <str1>exp3
%type <str1>Valeur
%type <chaine>Op_Comp_Log
%type <def_liste_idf>ListeIdfs
%type <entier>Type
%type <str2>Condition
%type <entier>M
%type <liste_next>ConditionIf
%type <liste_next>N
%type <liste_next>BlocInst
%type <liste_next>Instruction
%type <liste_next>BlocElseIf
%type <liste_next>BoucleFOR
%type <liste_next>BoucleWHILE
%%
S:BlocDec BlocInst
;
BlocDec:Dec BlocDec
|Dec
;
Dec:Type ListeIdfs {majPos($2,$1);}
|Affectation {if (rechercher($1.val)==-1) {inserer($1.val,0,$1.type,1);} else{{printf ("ERREUR SEMANTIQUE : idf déja déclaré , Ligne : %d , Colonne : %d , Entité : %s \n",ligne,colonne,$1.val);}}} 
|Type Affectation {if (rechercher($2.val)==-1) {inserer($2.val,0,$2.type,1);} else{{printf ("ERREUR SEMANTIQUE : idf déja déclaré , Ligne : , %d Colonne : %d , Entité : %s \n",ligne,colonne,$2.val);}}} 
|DecTableau
;
Type:INTEGER {type = 0; $$=0;}
|NUMERIC {type = 1; $$=1;}
|CHARACTER {type = 2; $$=2;}
|LOGICAL {type = 3; $$=3;}
;
ListeIdfs:Idf{if(rechercher($1)!=-1){printf ("ERREUR SEMANTIQUE : idf déja déclaré , Ligne : %d , Colonne : %d , Entité : %s \n",ligne,colonne,$1); }else {$$=NULL; inserer($1,0,-1,1);} $$=insererPos($$,rechercher($1));} 
|ListeIdfs ',' Idf{if (rechercher($3)!=-1){printf("ERREUR SEMANTIQUE : IDF DEJA DECLARE : %s\n",$3); } else {inserer($3,0,-1,1); $$=insererPos($$,rechercher($3));}}
;
DecTableau:Type Idf '[' Entier ']'{if (rechercher($2)!=-1){printf ("ERREUR SEMANTIQUE : idf déja déclaré , Ligne : %d , Colonne : %d , Entité : %s \n",ligne,colonne,$2); } else {inserer($2,2,type,$4);}}
;
Valeur:Entier {$$.type=0;  char t[10]; sprintf(t,"%d",$1); $$.val=strdup(t);}
|Reel  {$$.type=1;  char t[10]; sprintf(t,"%f",$1); $$.val=strdup(t);}
|Caractere {$$.type=2;  char t[10]; sprintf(t,"%c",$1); $$.val=strdup(t);}
|Booleen {$$.type=3;  char t[10]; sprintf(t,"%s",$1); $$.val=strdup(t);}
;
Affectation :Idf Aff exp1{$$.val = $1; $$.type = $3.type;teteQuad=insererQuad(teteQuad,numQuad,"<-",$3.val," ",$1);numQuad++;}
;
exp1:exp1 '+' exp2{if ($1.type!=$3.type) {printf ("ERREUR SEMANTIQUE : incompatibilité des types , Ligne : %d , Colonne : %d , Type 1 : %s , Type 2 : %s n",ligne,colonne,typeToString($1.type),typeToString($3.type)); } else {$$.type=$1.type;}char t[10]; sprintf(t,"T%d",numT); numT++;teteQuad=insererQuad(teteQuad,numQuad,"+",$1.val,$3.val,t);numQuad++;
$$.val=strdup(t);}
|exp1 '-' exp2{if ($1.type!=$3.type) {printf ("ERREUR SEMANTIQUE : incompatibilité des types , Ligne : %d , Colonne : %d , Type 1 : %s , Type 2 : %s \n",ligne,colonne,typeToString($1.type),typeToString($3.type)); }else {$$.type=$1.type;}char t[10]; sprintf(t,"T%d",numT);numT++;teteQuad=insererQuad(teteQuad,numQuad,"-",$1.val,$3.val,t);numQuad++;
$$.val=strdup(t);}
|exp2 {$$.val=$1.val;}
;
exp2:exp2 '*' exp3{if ($1.type!=$3.type) {printf ("ERREUR SEMANTIQUE : incompatibilité des types , Ligne : %d , Colonne : %d , Type 1 : %s , Type 2 : %s \n",ligne,colonne,typeToString($1.type),typeToString($3.type)); } else {$$.type=$1.type;}char t[10]; sprintf(t,"T%d",numT); numT++;teteQuad=insererQuad(teteQuad,numQuad,"*",$1.val,$3.val,t);numQuad++;
$$.val=strdup(t);}
|exp2 '/' exp3{if ($1.type!=$3.type) {printf ("ERREUR SEMANTIQUE : incompatibilité des types , Ligne : %d , Colonne : %d , Type 1 : %s , Type 2 : %s \n",ligne,colonne,typeToString($1.type),typeToString($3.type)); } else {$$.type=$1.type;}char t[10]; sprintf(t,"T%d",numT); numT++;teteQuad=insererQuad(teteQuad,numQuad,"/",$1.val,$3.val,t);numQuad++;$$.val=strdup(t);}
|exp2 '%' exp3{if ($1.type!=$3.type) {printf ("ERREUR SEMANTIQUE : incompatibilité des types , Ligne : %d , Colonne : %d , Type 1 : %s , Type 2 : %s \n",ligne,colonne,typeToString($1.type),typeToString($3.type)); } else {$$.type=$1.type;}char t[10]; sprintf(t,"T%d",numT); numT++;teteQuad=insererQuad(teteQuad,numQuad,"%",$1.val,$3.val,t);numQuad++;$$.val=strdup(t);}
|exp3 {$$.val=$1.val;}
;
exp3:Valeur {$$.type=$1.type; $$.val=$1.val;}
|Idf {if (rechercher($1)==-1) {printf ("ERREUR SEMANTIQUE : idf non déclaré , Ligne : %d , Colonne : %d , Entité : %s \n",ligne,colonne,$1);} $$.type=getType($1); $$.val=strdup($1);} 
|'(' exp1 ')' {$$.type=$2.type; $$.val=$2.val;}
;
Op_Comp_Log:and {$$=strdup("∧");}
|or {$$=strdup("∨");}
|'<' {$$=strdup("<");}
|'>'{$$=strdup(">");}
|Eg {$$=strdup("=");}
|NonEg {$$=strdup("!=");}
|InfEg {$$=strdup("<=");}
|SupEg {$$=strdup(">=");}
;

ConditionIf:IF Condition M '{' BlocInst '}'{RaccorderELT ($2.vrai,$3); $$=FusionnerELT ($5,$2.faux);}
|IF Condition M '{' BlocInst '}' N M ELSE '{' BlocInst '}'{RaccorderELT ($2.vrai,$3); RaccorderELT ($2.faux,$8); $$=Fusionner($7,$11);}
|IF Condition M '{' BlocInst '}' N M BlocElseIf N M ELSE '{' BlocInst '}'{RaccorderELT ($2.vrai,$3); RaccorderELT ($2.faux,$8);  $$=NULL; $$=Fusionner($$,$10); $$=Fusionner($$,$7);  $$=Fusionner($$,$9);};
Condition:'(' exp1 Op_Comp_Log exp1 ')'{teteQuad=insererQuad(teteQuad,numQuad,$3,$2.val,$4.val," ");$$.vrai=numQuad; numQuad++;teteQuad=insererQuad(teteQuad,numQuad,"BR"," "," "," "); $$.faux=numQuad; numQuad++; }
;
BlocElseIf:ELSE IF M Condition '{' BlocInst '}'{RaccorderELT ($4.vrai,$3); RaccorderELT ($4.faux,numQuad+1); $$=NULL;}
|BlocElseIf N M ELSE IF M Condition '{' BlocInst '}'{RaccorderELT ($7.vrai,$6); RaccorderELT ($7.faux,numQuad+1); $$=NULL; $$=Fusionner($1,$2);}
;

BoucleFOR : FOR M '(' Idf IN Valeur ':' Valeur ')' '{' BlocInst '}' {
{if (rechercher($4)==-1) {printf ("ERREUR SEMANTIQUE : idf non déclaré , Ligne : %d , Colonne : %d , Entité : %s \n",ligne,colonne,$4);}}; Raccorder($11,$2);}
;
BoucleWHILE: WHILE M Condition '{' M BlocInst '}'{RaccorderELT ($3.vrai,$5); Raccorder($6,$2); $$=FusionnerELT ($6,$3.faux); }
;

M:{$$=numQuad;}
;
N:{teteQuad=insererQuad(teteQuad,numQuad,"BR"," "," "," "); $$=NULL; $$=insererRacFus($$,numQuad); numQuad++;}
;
BlocInst:Instruction{$$=$1;}
|Instruction BlocInst {$$=$1;}
;
Instruction:Affectation{if (rechercher($1.val)==-1) {inserer($1.val,0,$1.type,1);} else {if (getType($1.val) != $1.type) 
{printf ("ERREUR SEMANTIQUE : incompatibilité des types , Ligne : %d , Colonne : %d , Type 1 : %s , Type 2 : %s \n",ligne,colonne,typeToString(getType($1.val)),typeToString($1.type));}} $$=NULL; }
|ConditionIf{ $$=$1;}
|BoucleFOR{ $$=$1;}
|BoucleWHILE{ $$=$1;}
;
%%
int main()
{
initialiserTS();
yyin=fopen("MyCode.txt","r");
yyparse();/* permet de lancer l’analyseur syntaxique*/
fclose(yyin);

printf("\n\n********************* La table des symboles ***********************\n\n");
afficherTS();
printf("\n\n********************* Les Quadruplets ***********************\n\n");
afficherQuad(teteQuad);
printf("\n\n********************* Le code machine ***********************\n\n");
afficherCodeMachine(teteQuad);
fclose(yyin);
return 0;
}
