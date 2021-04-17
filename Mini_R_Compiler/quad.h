#include<stdio.h>
#include <stdlib.h>
#include<string.h>

typedef struct eltQuad {  int numQuad;  char *op; char *op1; char *op2; char *res; struct eltQuad* suivant; }eltQuad;

typedef struct eltRacFus{int val;struct eltRacFus* suivant;}eltRacFus;


//****************************************************************************************************

eltQuad *teteQuad=NULL;


// Insérer à la fin
eltQuad *insererQuad(eltQuad *tete,int numQuad,char*op,char *op1,char *op2,char *res)
{


    eltQuad *PtrInt=(eltQuad*) malloc(sizeof(eltQuad));
	if (PtrInt==NULL)
        printf ("Pas d'espace pour l'allocation\n");
	PtrInt->numQuad=numQuad;
	PtrInt->op=malloc (strlen(op)+1);
	strcpy(PtrInt->op,op);
        PtrInt->op1=malloc (strlen(op1)+1);
	strcpy(PtrInt->op1,op1);
	PtrInt->op2=malloc (strlen(op2)+1);
	strcpy(PtrInt->op2,op2);
	PtrInt->res=malloc (strlen(res)+1);
	strcpy(PtrInt->res,res);
	PtrInt->suivant=NULL;

	if(tete == NULL) return PtrInt;	

	eltQuad *teteInt=(eltQuad*) malloc(sizeof(eltQuad));
	teteInt=tete;
	while (teteInt->suivant!=NULL)
		teteInt=teteInt->suivant;

	teteInt -> suivant = PtrInt;


	return tete;
}
//******************************************************************************************************
//afficher quadruplet
void afficherQuad(eltQuad* tete)
{
	eltQuad *PtrInt=tete;
	if (PtrInt==NULL)
	printf ("La liste est vide\n");
	else
	while (PtrInt!=NULL)
	{
		printf ("%2d - ( %3s , %5s , %5s , %5s )\n",PtrInt->numQuad,PtrInt->op,PtrInt->op1,PtrInt->op2,PtrInt->res);
		PtrInt=PtrInt->suivant;
	}
}

//******************************************************************************************************
//modifier la position du quadruplet
void modifierPositionQuad(eltQuad* tete,int numQuad,int position)
{

	eltQuad *PtrInt=tete;
	if (PtrInt==NULL)
	printf ("La liste est vide\n");
	else
	while (PtrInt!=NULL)
	{
                if(PtrInt->numQuad==numQuad)
                {
                 char t[10]; 
                 sprintf(t,"%d",position); 
                PtrInt->res=malloc (strlen(t)+1);
	        strcpy(PtrInt->res,t);
                }
		PtrInt=PtrInt->suivant;
	}
}

//******************************************************************************************************
//les branchements
char *branchement(char *chaine)
{
if (strcmp(chaine,"!=")==0) return "BE";
if (strcmp(chaine,"==")==0) return "BNE";
if (strcmp(chaine,"<")==0) return "BGE";
if (strcmp(chaine,"<=")==0) return "BG";
if (strcmp(chaine,">")==0) return "BLE";
if (strcmp(chaine,">=")==0) return "BL";
}


//******************************************************************************************
eltRacFus *insererRacFus(eltRacFus *tete,int val)
{	
        eltRacFus *PtrInt=(eltRacFus*) malloc(sizeof(eltRacFus));
	if (PtrInt==NULL)
	printf ("Pas d'espace pour l'allocation\n");
	PtrInt->val=val;
	PtrInt->suivant=tete;
	tete=PtrInt;
	return tete;
}
//*******************************************************************************************
//Raccorder et RaccorderELT pour mettre a jour les quadruplets

void Raccorder (eltRacFus* tete, int numQuad)
{

	eltRacFus *PtrInt=tete;
	if (PtrInt==NULL)
	printf ("La liste est vide\n");
	else
	while (PtrInt!=NULL)
	{
	     	modifierPositionQuad(teteQuad,PtrInt->val,numQuad);
		PtrInt=PtrInt->suivant;
	}
}

void RaccorderELT (int elt, int numQuad)
{
	modifierPositionQuad(teteQuad,elt,numQuad);
}


//******************************************************************************************

eltRacFus *Fusionner (eltRacFus* tete1, eltRacFus* tete2)
{

	if (tete1 == NULL && tete2 == NULL ) return NULL;
	if (tete1 == NULL) {return tete2;}
	if (tete2 == NULL) return tete1;

        eltRacFus *PtrInt=(eltRacFus*) malloc(sizeof(eltRacFus));
	PtrInt=tete1;

	while (PtrInt->suivant!=NULL)
		PtrInt=PtrInt->suivant;
	PtrInt->suivant = tete2;
	return tete1;
}

eltRacFus *FusionnerELT (eltRacFus* tete1, int elt)
{
	eltRacFus *tete2 = (eltRacFus*) malloc(sizeof(eltRacFus));
	tete2 = NULL;
	tete2 = insererRacFus(tete2,elt);

	tete2->suivant = tete1;
	return tete2;
}

