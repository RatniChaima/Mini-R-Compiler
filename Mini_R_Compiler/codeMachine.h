#include<stdio.h>
#include <stdlib.h>
#include<string.h>



//****************************************************************************************************
typedef struct elementLabel {  int num; struct elementLabel* suivant; }elementLabel;


typedef struct elementVAR { char* elt; struct elementVAR* suivant;}elementVAR;

int nbEltPile = 0;

//****************************************************************************************************
// inserer label
elementLabel *insererLabel(elementLabel *tete, int label)
{
        elementLabel *PtrInt=(elementLabel*) malloc(sizeof(elementLabel));
	if (PtrInt==NULL)
	printf ("Pas d'espace pour l'allocation\n");
	PtrInt->num = label;
	PtrInt->suivant=tete;
	tete=PtrInt;
	return tete;
}
// il existe une label?
int existeLabel(elementLabel *tete, int label)
{
	elementLabel *PtrInt=(elementLabel*) malloc(sizeof(elementLabel));
	PtrInt = tete;
	while (PtrInt != NULL)
	{
		if(PtrInt->num == label)
			return 1;
		PtrInt = PtrInt->suivant;
	}
	return 0;
}


//****************************************************************************************************
// inserer variable
elementVAR *insererVAR(elementVAR *tete, char* elt)
{
        elementVAR *PtrInt=(elementVAR*) malloc(sizeof(elementVAR));
	if (PtrInt==NULL)
	printf ("Pas d'espace pour l'allocation\n");
	PtrInt->elt=malloc (strlen(elt)+1);
	strcpy(PtrInt->elt,elt);
	PtrInt->suivant=tete;
	tete=PtrInt;
	return tete;
}
// il existe une variable?
int existeVAR(elementVAR *tete, char* elt)
{
	elementVAR *PtrInt=(elementVAR*) malloc(sizeof(elementVAR));
	PtrInt = tete;
	while (PtrInt != NULL)
	{
		if(strcmp(PtrInt->elt,elt)==0)
			return 1;
		PtrInt = PtrInt->suivant;
	}
	return 0;
}

//****************************************************************************************************
// retourner le nombre d'occurances
int nbOccurances (eltQuad* tete, char* temp)
{
	eltQuad *PtrInt=tete;
	int nbOcc=0;
	while (PtrInt!=NULL)
	{
		if (strcmp(PtrInt->op1,temp)==0 || strcmp(PtrInt->op2,temp)==0)
			nbOcc++;
		PtrInt=PtrInt->suivant;
	}
	return nbOcc;
}
int tempToInt (char s[])
{

	int x=0;
	int y=1;

	
	for (int i=1; i<strlen(s); i++)
			{y = y * 10;}

	y = y /10;
	
	for (int i=1; i<strlen(s); i++)
	{
		x = x + ( ( s[i] - 48 ) * y );
		y = y / 10;
	}
	return x;
}
//****************************************************************************************************
// affichage de l'entete

void afficherEnTete(eltQuad* tete)
{
	printf("TITLE prog.asm: Code Objet\n");	// nom du programme
	printf("Pile segment stack;\n"); // déclaration d’un segment de pile 
	printf("\tdw 100 dup(?)\n");	// dw : 1 word (16 bits) (Declare Word)
	printf("Pile ends\n");	// fin de la déclaration de la pile
	printf("\nDATA segment\n"); //déclaration d’un segment de données qui va contenir les variables
	
	printf("\tTAB DW dup(?)\n");

	elementVAR *teteVAR = NULL;

	eltQuad *PtrInt=tete;
	while (PtrInt!=NULL)
	{
		if ((strcmp(PtrInt->op,"+")==0 || strcmp(PtrInt->op,"-")==0 || strcmp(PtrInt->op,"*")==0 || strcmp(PtrInt->op,"/")==0 || strcmp(PtrInt->op,"=")==0) && 
		rechercher(PtrInt->op1)!=-1 && existeVAR(teteVAR,PtrInt->op1)==0)
		{
			teteVAR=insererVAR(teteVAR,PtrInt->op1);
			printf("\t%s DW ?\n",PtrInt->op1); // DW : 1 word (16 bits) (Declare Word)
		}
		if ((strcmp(PtrInt->op,"+")==0 || strcmp(PtrInt->op,"-")==0 || strcmp(PtrInt->op,"*")==0 || strcmp(PtrInt->op,"/")==0) && 
		rechercher(PtrInt->op2)!=-1 && existeVAR(teteVAR,PtrInt->op2)==0)
		{
			teteVAR=insererVAR(teteVAR,PtrInt->op2);
			printf("\t%s DW ?\n",PtrInt->op2);
		}
		if ((strcmp(PtrInt->op,"+")==0 || strcmp(PtrInt->op,"-")==0 || strcmp(PtrInt->op,"*")==0 || strcmp(PtrInt->op,"/")==0 || strcmp(PtrInt->op,"=")==0) && 
		rechercher(PtrInt->res)!=-1 && existeVAR(teteVAR,PtrInt->res)==0)
		{
			teteVAR=insererVAR(teteVAR,PtrInt->res);
			printf("\t%s DW ?\n",PtrInt->res);
		}
		PtrInt=PtrInt->suivant;
	}



	printf("DATA ends\n");	//fin de la déclaration de données
	printf("\nCODE segment\n"); //déclaration du segment de code  qui va contenir le code
	printf("\tMAIN:\n");	
	printf("\t\tASSUME CS:CODE, DS:DATE:\n");	
	printf("\t\tMOV AX , DATA\n");
	printf("\t\tMOV DX , AX\n\n");
}
//************************************************************************************
// affichage du code machine

void afficherCodeMachine(eltQuad* tete)
{
	
	afficherEnTete(tete);
	eltQuad *PtrInt=tete;
	int lastQuad=1;
	elementLabel *teteLabel = NULL;

	if (PtrInt==NULL)
		printf ("La liste est vide\n");
	
	else
	while (PtrInt!=NULL)
	{

		if ( existeLabel(teteLabel, PtrInt->numQuad) == 1)
				printf("\t\tLabel_%d : ", PtrInt->numQuad); //label : identificateur qui permet de désigner un endroit dans le code source (retour de debut de bcls...)

		if (strcmp(PtrInt->op,"=")==0)
		{
			if (rechercher(PtrInt->op1)==-1 )
			{
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n"); // récupère le contenu du sommet de la pile et le transfère dans AX (depiler)
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
				printf ("\t\tMOV %s , AX\n",PtrInt->res);	
			}
			else
				printf ("\t\tMOV %s , %s\n",PtrInt->res,PtrInt->op1);	
		}

		else if (strcmp(PtrInt->op,"+")==0)
		{
			if (rechercher(PtrInt->op1)==-1 && rechercher(PtrInt->op2)==-1 )
			{
				if (nbOccurances(tete,PtrInt->op2)<=1)
					printf ("\t\tPOP BX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op2));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV BX , TAB[SI]\n");
				}
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
				printf ("\t\tADD AX , BX\n");	
			}
			else if (rechercher(PtrInt->op1)==-1 )
			{
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
				printf ("\t\tADD AX , %s\n",PtrInt->op2);
			}
			else if (rechercher(PtrInt->op2)==-1 )
			{
				if (nbOccurances(tete,PtrInt->op2)<=1)
					printf ("\t\tPOP BX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op2));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV BX , TAB[SI]\n");
				}
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);
				printf ("\t\tADD AX , BX\n");	
			}
			else
			{
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);
				printf ("\t\tADD AX , %s\n",PtrInt->op2);
			}
			if (nbOccurances(tete,PtrInt->res)<=1)
				printf ("\t\tPUSH AX\n"); // empile le contenu du registre AX
			else
			{
				printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->res));
				printf ("\t\tADD SI , SI\n");
				printf ("\t\tMOV TAB[SI] , AX\n");
			}	
		}

		else if (strcmp(PtrInt->op,"-")==0)
		{
			if (rechercher(PtrInt->op1)==-1 && rechercher(PtrInt->op2)==-1 )
			{
				if (nbOccurances(tete,PtrInt->op2)<=1)
					printf ("\t\tPOP BX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op2));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV BX , TAB[SI]\n");
				}
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
				printf ("\t\tSUB AX , BX\n");	
			}
			else if (rechercher(PtrInt->op1)==-1 )
			{
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
				printf ("\t\tSUB AX , %s\n",PtrInt->op2);
			}
			else if (rechercher(PtrInt->op2)==-1 )
			{
				if (nbOccurances(tete,PtrInt->op2)<=1)
					printf ("\t\tPOP BX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op2));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV BX , TAB[SI]\n");
				}
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);
				printf ("\t\tSUB AX , BX\n");	
			}
			else
			{
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);
				printf ("\t\tSUB AX , %s\n",PtrInt->op2);
			}
			if (nbOccurances(tete,PtrInt->res)<=1)
				printf ("\t\tPUSH AX\n");
			else
			{
				printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->res));
				printf ("\t\tADD SI , SI\n");
				printf ("\t\tMOV TAB[SI] , AX\n");
			}		
		}

		else if (strcmp(PtrInt->op,"*")==0)
		{
			if (rechercher(PtrInt->op1)==-1 && rechercher(PtrInt->op2)==-1 )
			{
				if (nbOccurances(tete,PtrInt->op2)<=1)
					printf ("\t\tPOP BX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op2));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV BX , TAB[SI]\n");
				}
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
				printf ("\t\tMUL BX\n");	
			}
			else if (rechercher(PtrInt->op1)==-1 )
			{
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
				printf ("\t\tMUL %s\n",PtrInt->op2);
			}
			else if (rechercher(PtrInt->op2)==-1 )
			{
				if (nbOccurances(tete,PtrInt->op2)<=1)
					printf ("\t\tPOP BX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op2));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV BX , TAB[SI]\n");
				}
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);
				printf ("\t\tMUL BX\n");	
			}
			else
			{
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);
				printf ("\t\tMUL %s\n",PtrInt->op2);
			}
			if (nbOccurances(tete,PtrInt->res)<=1)
				printf ("\t\tPUSH AX\n");
			else
			{
				printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->res));
				printf ("\t\tADD SI , SI\n");
				printf ("\t\tMOV TAB[SI] , AX\n");
			}		
		}

		else if (strcmp(PtrInt->op,"/")==0)
		{
			if (rechercher(PtrInt->op1)==-1 && rechercher(PtrInt->op2)==-1 )
			{
				if (nbOccurances(tete,PtrInt->op2)<=1)
					printf ("\t\tPOP BX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op2));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV BX , TAB[SI]\n");
				}
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
				printf ("\t\tDIV BX\n");	
			}
			else if (rechercher(PtrInt->op1)==-1 )
			{
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
				printf ("\t\tDIV %s\n",PtrInt->op2);
			}
			else if (rechercher(PtrInt->op2)==-1 )
			{
				if (nbOccurances(tete,PtrInt->op2)<=1)
					printf ("\t\tPOP BX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op2));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV BX , TAB[SI]\n");
				}
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);
				printf ("\t\tDIV BX\n");	
			}
			else
			{
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);
				printf ("\t\tDIV %s\n",PtrInt->op2);
			}
			if (nbOccurances(tete,PtrInt->res)<=1)
				printf ("\t\tPUSH AX\n");
			else
			{
				printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->res));
				printf ("\t\tADD SI , SI\n");
				printf ("\t\tMOV TAB[SI] , AX\n");
			}		
		}
		
		else if (strcmp(PtrInt->op,"<")==0)
		{
			if (rechercher(PtrInt->op1)==-1)
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
			else
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);	
			printf ("\t\tCMP AX , %s\n",PtrInt->op2);	
			printf ("\t\tJL Label_%s\n",PtrInt->res);
			teteLabel = insererLabel(teteLabel,atoi(PtrInt->res));	
		}

		else if (strcmp(PtrInt->op,"<=")==0)
		{
			if (rechercher(PtrInt->op1)==-1 )
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
			else
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);	
			printf ("\t\tCMP AX , %s\n",PtrInt->op2);	
			printf ("\t\tJLE Label_%s\n",PtrInt->res);
			teteLabel = insererLabel(teteLabel,atoi(PtrInt->res));	
		}

		else if (strcmp(PtrInt->op,">")==0)
		{
			if (rechercher(PtrInt->op1)==-1)
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
			else
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);	
			printf ("\t\tCMP AX , %s\n",PtrInt->op2);	
			printf ("\t\tJG Label_%s\n",PtrInt->res);
			teteLabel = insererLabel(teteLabel,atoi(PtrInt->res));	
		}

		else if (strcmp(PtrInt->op,">=")==0)
		{
			if (rechercher(PtrInt->op1)==-1 )
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
			else
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);	
			printf ("\t\tCMP AX , %s\n",PtrInt->op2);	
			printf ("\t\tJGE Label_%s\n",PtrInt->res);
			teteLabel = insererLabel(teteLabel,atoi(PtrInt->res));	
		}

		else if (strcmp(PtrInt->op,"=")==0)
		{
			if (rechercher(PtrInt->op1)==-1)
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
			else
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);	
			printf ("\t\tCMP AX , %s\n",PtrInt->op2);	
			printf ("\t\tJE Label_%s\n",PtrInt->res);
			teteLabel = insererLabel(teteLabel,atoi(PtrInt->res));	
		}

		else if (strcmp(PtrInt->op,"!=")==0)
		{
			if (rechercher(PtrInt->op1)==-1)
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
			else
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);	
			printf ("\t\tCMP AX , %s\n",PtrInt->op2);	
			printf ("\t\tJNE Label_%s\n",PtrInt->res);
			teteLabel = insererLabel(teteLabel,atoi(PtrInt->res));	
		}

		else if (strcmp(PtrInt->op,"BR")==0)
		{
			printf ("\t\tJMP Label_%s\n",PtrInt->res);
			teteLabel = insererLabel(teteLabel,atoi(PtrInt->res));	
		}
		lastQuad++;
		PtrInt=PtrInt->suivant;
	}
	
	if ( existeLabel(teteLabel, lastQuad) == 1)
	printf("\t\tLabel_%d : \n", lastQuad);
	printf("\tMAIN ends\n");	
	printf("CODE ends\n");	//fin de la déclaration du code
}


