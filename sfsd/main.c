#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 3

typedef struct {
    int cle;
    char info[200];
   int efface;

} etudiant;

typedef etudiant tab [MAX];
typedef struct {
    int num_Btete;
    int nb_enreg;
    int nb_bloc;
} TEn_Tete;

typedef struct TBloc {
    char t[MAX];
    int suiv;
    int nb;

    struct TBloc* svt;
} TBloc;



typedef struct {
    TBloc* r;
    TEn_Tete* en_tete;
    FILE* f;
    }Lnovnc;



/***************************OVERIR*************************/

   void ouvrir (Lnovnc *fichier,char nom_fich[30],const char mode)//ouvrir un fichier de type LNOF en mode voulu
{
fichier->en_tete = malloc(sizeof(TEn_Tete));//allouer dynamiquement une zone en m�mo centrale pour l'en_tete
if ((mode=='n')||(mode=='N'))//si le mode est nouveau ie:le fichier n'existe pas d�j� alors
    {   fichier->f=fopen(nom_fich,"wb+");//ouvrir un fichier en mode ecriture binaire
        fichier->en_tete->num_Btete=-1;//initialiser les champs 1 ET 3 de l'en_tete � -1
        fichier->en_tete->nb_enreg=0;//initialiser le nombre d'enregistrements � 0
        fichier->en_tete->nb_bloc=-1;
        fwrite(fichier->en_tete,sizeof(TEn_Tete),1,fichier->f);//ecrire l'en_tete ds le fichier binaire
    }
    else
    {
        if ((mode=='a')||(mode=='A'))//si le mode est ancien ie:le fichier existe
        {
          fichier->f=fopen(nom_fich,"rb+");//ouvrir le fichier en mode lecture binaire
          if (fichier->f==NULL) printf("\n\t\t<<Le fichier n'existe pas...>>");//si erreur d'ouverture
          else//sinon
         {fread(fichier->en_tete,sizeof(TEn_Tete),1,fichier->f);}//recuperer le contenu de l'entete ds la variable en_tete

        }
        else printf("le mode errone.");
    }
}


  /****************AFF_ENTETE*******************/
void af_entete(Lnovnc *fichier,int num_caract,int val)//affecter � la caracteristique num_caract la val
{
  switch(num_caract)
  {
      case 1:fichier->en_tete->num_Btete=val; break;
      case 2:fichier->en_tete->nb_enreg=val; break;
      case 3:fichier->en_tete->nb_bloc=val; break;
      default: printf("\n\t\t<<Le numero errone>>");
  }
}


             /*******************EN_TETE*********************/
int en_tete(Lnovnc *fichier,int num_caract )//retourner la cracterstique num_caract ds val
{int val;
    switch(num_caract)
  {
      case 1:val=fichier->en_tete->num_Btete; break;
      case 2:val=fichier->en_tete->nb_enreg; break;
      case 3:val=fichier->en_tete->nb_bloc; break;
      default: printf("\n\t\t<<Le numero errone>>");
  }
  return val;

/***************ECRITURE DIRECTE***********/
void ecrire_dire(Lnovnc *fichier,int i,TBloc *buf)//ecriture directe du contenu de buf ds le fichier � la position i
{   rewind(fichier->f);
    fseek(fichier->f,sizeof(TEn_Tete)+i*sizeof(TBloc),SEEK_SET);//se positionner � la place exacte
    fwrite(buf,sizeof(TBloc),1,fichier->f);//ecriture
}


/****************LECTURE DIRECTE*************/


void lire_dire(Lnovnc *fichier,int i,TBloc *buf)//lecture directe du contenu de fichier � la position i ds le buf
{   rewind(fichier->f);
    fseek(fichier->f,sizeof(TEn_Tete)+i*sizeof(TBloc),SEEK_SET);//se positionner � la place exacte
    fread(buf,sizeof(TBloc),1,fichier->f);//lecture



/****************FERMITUR******************/


void fermer (Lnovnc *fichier)//fermer un fichier
{   rewind(fichier->f);//on se positionne au debut de fichier
    fwrite(fichier->en_tete,sizeof(TEn_Tete),1,fichier->f);//on enregistre les modifications effectu�es sur l'en_tete
    fclose(fichier->f);//on ferme le fichier
    free(fichier->en_tete);//liberer la zone en_tete reserv�e

 /****************les demandes************************/






 /************************************recherch**********************************/




  void recherche (Lnovnc *fichier,char nom_fich[30],int cle,int *num_bloc,int *pos,int *trouv)//module de recherche de la cle ds un fichier
{
    TBloc buf;//declarer un buffer
int i,j,n;//donne le nummero de bloc courant

ouvrir(fichier,nom_fich,'a');//ouvrir le fichier en mode ancien
    if (fichier->f!=NULL)//tester si le fichier existe
       {i=en_tete(fichier,1);//recuperer le numero du 1 er bloc
       n=en_tete(fichier,2);//recuperer le nombre d'enregistrements ds le fichier
        if((i==-1)||(n==0)) {printf("\n \t\t<<Le fichier est vide>>");//tester si le fichier n'est pas vide
        trouv=-1;} //retourner trouv � faux
        else
        {if((i!=-1)&&(n!=0))//sinon

        {
            *trouv=0;*pos=0;j=0;//initialiser les paramettres
            while((i!=-1)&&(*trouv==0))//tq on est pas arriv� � la fin de fichier et on a pas trouv� la cl�
            {
                lire_dire(fichier,i,&buf);//lire le bloc i
                if(buf.nb!=0)//tester s'il n'est pas vide
                {


    while((j<=MAX)&&(*trouv==0))//tq c'est pas la fin de tab d'enregistrements et non trouv
    {if((buf.t[j].cle==cle)&&(buf.t[j].efface==0))//verifier chaque case si elle n'est pas effac�e
                        {//si v�rifi�
                            *trouv=1;//mettre trouv � vrai
                            *num_bloc=i;//retourner le numero de bloc
                            *pos=j;
                        }
                            j++;//aller � la prochaine case

                    }
                    if((j>MAX)&&(*trouv==0))//si on est arriv� � la fin du tableau sans trouver la cl�
                    {
                        j=0;//initialiser la position � 0
                    }
                }
                i=buf.suiv;//aller au bloc suivant
            }
        }}}
fermer(fichier);//fermer le fichier
}




/*********************suppression*************************/



void supprimer(Lnovnc *fichier,char nom_fich[30],int cle) //suppression logique d'un enregistrement
{int num_bloc,pos,trouv,i;

TBloc buf;

ouvrir(fichier,nom_fich,'a');//ouvrir le fichier en mode ancien



fermer(fichier);//le fermer

if (fichier->f!=NULL)

    {recherche(fichier,nom_fich,cle,&num_bloc,&pos,&trouv);//on recherche cette cle est ce qu'elle existe d�j�

    if(trouv==0)//si elle n'existe pas
    {
        printf("\n\t\t<<La cle n'existe pas>>");//le dire
    }
    else
        {if(trouv==1)//sinon
    {   ouvrir(fichier,nom_fich,'a');
        lire_dire(fichier,num_bloc,&buf);//lire le bloc correspendant
        buf.t[pos].efface=1;//mettre le efface de la case convenable � vrai
        buf.nb--;//decrementer le nmbre d'articles ds le bloc
        i=en_tete(fichier,2);//lire la dexieme caract
        i--;
        af_entete(fichier,2,i);//decrementer le nmbre d'articles ds le fichier
        ecrire_dire(fichier,num_bloc,&buf);//enregistrer ces modifications ds le fichier
        fermer(fichier);//fermer le fichier
    }}}
}


/***************affichage*********************/


void affichage (Lnovnc *fichier,char nom_fich[30])
{int i,j,n;
TBloc buf;
    ouvrir(fichier,nom_fich,'A');//ouvrir le fichier en mode ancien
    if(fichier->f!=NULL)//si le fichier existe
    {i=en_tete(fichier,1);//recup�rer le num�ro du premier bloc
     n=en_tete(fichier,2);//recup�rer le nombre d'enregistrements

    if (n!=0)//si le fichier n'est pas vide
    {
    printf("\t\t\t==============\n");
	printf("\t\t\t  L'Affichage  \n");
	printf("\t\t\t==============\n");
	printf("\t\t+----------+----------+\n");
	printf("\t\t| matric  | etudiant|\n");
	printf("\t\t+----------+----------+\n");
        while(i!=-1)//tant qu'on est pas arriv� � la fin de fichier
    {
        lire_dire(fichier,i,&buf);//lire le buf i
        if (buf.nb!=0)
        {
            for (j=0;j<MAX;j++)//parcourir tout le tableau
            {
                if (buf.t[j].efface==0)//chercher les positions non �ffac�es pour les afficher
                {printf("\t\t| %-8d | %-8s |\n",buf.t[j].cle,buf.t[j].nom);
                printf("\t\t+----------+----------+\n");
                }
            }
        }
        i=buf.suiv;//aller au prochain bloc
    }
    }
    else printf("\n \t\t<<Le fichier est vide>>");}
    fermer(fichier);
}


/**********************affichage d'entete********************/

void affichage_entete(Lnovnc *fichier,char nom_fich[30])//afficher les caracteristiques de fichier

{ouvrir(fichier,nom_fich,'a');//ouvrir le fichier en mode ancien

if(fichier->f)//si le fichier existe on affiche toutes les caracteristiques

    {printf("\n\t\tAFFICHAGE EN_TETE");
    printf("\n\t***Numero du premier bloc=%d",en_tete(fichier,1));
    printf("\n\t***Le nombre des enregistrements=%d",en_tete(fichier,2));
    printf("\n\t***Numero du dernier bloc=%d",en_tete(fichier,3));}
    fermer(fichier);
}



/*************MENU***********/
void menu (void)

		{printf("\n\t\t               <<<<<<<LES FICHIERS Lnovnc>>>>>>>>");
		    printf ("\n\t\t ________________________________________________________________\n");
		printf ("\t\t|\t                                                        |\n");
	    printf ("\t\t|                   <<<< MENU PRINCIPALE >>>>                   |\n");
	    printf ("\t\t|\t                                                        |\n");
	    printf ("\t\t|_______________________________________________________________|\n");
	    printf ("\t\t|\t                                                        |\n");
		printf ("\t\t|\t -1. Creer un nouveau fichier vide.                     |\n");
		printf ("\t\t|\t                                                        |\n");
		printf ("\t\t|\t -2. Insertion dans un fichier existant ou non.         |\n");
		printf ("\t\t|\t                                                        |\n");
		printf ("\t\t|\t -3. Supprimer logiquement un etudiant.           |\n");
		printf ("\t\t|\t                                                        |\n");
		printf ("\t\t|\t -4. Affichage de toute les etudiant.                       |\n");
		printf ("\t\t|\t                                                        |\n");
		printf ("\t\t|\t -5. Affichage de l'entete.                             |\n");
		printf ("\t\t|\t                                                        |\n");
		printf ("\t\t|\t -6. Recherche d'un etudiant.                     |\n");
		printf ("\t\t|\t                                                        |\n");
		printf ("\t\t|\t                                                        |\n");
		printf ("\t\t|\t -7 quitter                                            |\n");
		printf ("\t\t|\t                                                        |\n");
        printf ("\t\t|_______________________________________________________________|\n");


		}


		int main()
{  TBloc buf;
char nom_fich[30];
int choix,continu,nb_enreg,i,num_bloc,pos,trouv,confirm;
TArticle n;
LNOF *fichier;

do
{menu();
printf("\nTAPEZ UN CHOIX SVP: ");
confirm=scanf("%d",&choix);
while((confirm!=1)||(choix<1)||(choix>8))
{reglage();
 printf("\nTAPEZ UN CHOIX SVP(entre 1 et 8): ");
confirm=scanf("%d",&choix);
}
switch(choix)
{
    case 1:
    {
        printf("\n\t\t<<CREER UN NOUVEAU FICHIER>>");
     printf("\nDonnez le nom de fichier a creer:");
     scanf("%s",&nom_fich);
     ouvrir(fichier,nom_fich,'N');
     fermer(fichier);
    }break;

    case 2:
    {
        printf("\n\t\t<<INSERER DANS UN FICHIER>>");
        printf("\nDonnez le nom de fichier vous allez inserer:");
     scanf("%s",&nom_fich);
        printf("\ncombien des etudiants voulez vous inserer?");
        printf("\n le nombre= ");
       confirm=scanf("%d",&nb_enreg);
       while((confirm!=1)||(nb_enreg<0))
       {reglage();
           printf("\n Le nombre est errone retapez le svp");
        printf("\n le nombre= ");
        confirm=scanf("%d",&nb_enreg);
       }
        if (nb_enreg>0)
        {printf("\nNB:si le nom est compose ecrivez le partie1_partie2");
            for(i=0;i<nb_enreg;i++)
        {
            printf("\n donner l'enregistrement");
            printf("\n*La cle= ");
            confirm=scanf("%d",&n.cle);
            while((confirm!=1)||(n.cle<0))
            {reglage();
            printf("\nDonnez la cle a nouveau elle fausse");
            printf("\n*La cle= ");
            confirm=scanf("%d",&n.cle);
            }
            printf("\n*Le nom= ");
            scanf("%s",&n.nom);
            insertion(fichier,nom_fich,&n);
        }}
    }break;

    case 3:
    {printf("\n\t\t<<SUPPRESSION LOGIQUE>>");
        printf("\nDonnez le nom de fichier d'ou vous supprimerez:");
     scanf("%s",&nom_fich);
     printf("\n Donnez le matric a supprimer");
     printf("\n*le matric=");
     confirm=scanf("%d",&n.cle);
     while((n.cle<0)||(confirm!=1))
     {reglage();
         printf("\nle matric est errone");
      printf("\n*le matric=");
     confirm=scanf("%d",&n.cle);
     }
     supprimer(fichier,nom_fich,n.cle);
     }break;

    case 4:
    {
      printf("\n\t\t<<AFFICHAGE>>");
      printf("\nDonnez le nom de fichier a afficher:");
     scanf("%s",&nom_fich);
      affichage(fichier,nom_fich);
    }break;

    case 5:
    {
     printf("\n\t\t<<AFFICHAGE EN_TETE>>");
      printf("\nDonnez le nom de fichier a afficher son en_tete:");
     scanf("%s",&nom_fich);
     affichage_entete(fichier,nom_fich);
    }break;


   case 6:
    {
      printf("\n\t\t<<RECHERCHE D'UN ENREGISTREMENT>>");
      printf("\nDonnez le nom de fichier ou vous allez chercher la cle:");
      scanf("%s",&nom_fich);
      printf("\n Donnez le  matic a rechercher");
     printf("\n*le matric=");
     confirm=scanf("%d",&n.cle);
     while((n.cle<0)||(confirm!=1))
     {reglage();
         printf("le matric est errone");
      printf("\n*La cle=");
    confirm= scanf("%d",&n.cle);
     }
     recherche(fichier,nom_fich,n.cle,&num_bloc,&pos,&trouv);
     if (trouv==0) printf("\nCette enregistrement n'existe pas");
     else printf("\n L'enregistrement existe dans le bloc %d a la position %d",num_bloc,pos);

    }break

    case 7: ;return 0   break;
    default: printf("\nLe choix est errone");

}
printf("\nTapez 1 pour continuer :");
scanf("%d",&continu);
system("cls");
}
while (continu==1);


}

