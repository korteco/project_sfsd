#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    int num_Btete;
    int nb_enreg;
    int nb_bloc;
} TEn_Tete;

typedef struct TBloc {
    char info[20];
    int next;
    int nb;

    struct TBloc* svt;
} TBloc;

typedef struct {
    int cle;
    char info[200];
   int deletecle;

} etudiant;

typedef struct {
    TBloc* r;
    TEn_Tete* en_tete;
    FILE* physicalFile;
    }Lnovnc;

   TBloc buf;


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


void lire_dire(LNOF *fichier,int i,TBloc *buf)//lecture directe du contenu de fichier � la position i ds le buf
{   rewind(fichier->f);
    fseek(fichier->f,sizeof(TEn_Tete)+i*sizeof(TBloc),SEEK_SET);//se positionner � la place exacte
    fread(buf,sizeof(TBloc),1,fichier->f);//lecture



/****************FERMITUR******************/


void fermer (LNOF *fichier)//fermer un fichier
{   rewind(fichier->f);//on se positionne au debut de fichier
    fwrite(fichier->en_tete,sizeof(TEn_Tete),1,fichier->f);//on enregistre les modifications effectu�es sur l'en_tete
    fclose(fichier->f);//on ferme le fichier
    free(fichier->en_tete);//liberer la zone en_tete reserv�e

 /****************les demandes************************/




 /************************************recherch**********************************/

  void recherche (LNOF *fichier,char nom_fich[30],int cle,int *num_bloc,int *pos,int *trouv)//module de recherche de la cle ds un fichier
{TBloc buf;//declarer un buffer
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
                if(buf.NbArticle!=0)//tester s'il n'est pas vide
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


void Recuperer_chaine(int n, int i, int j, int *longeur, char *chaine) {
    *longeur = n;
    strncpy(chaine, buf.tab + j - 1, n);
    chaine[n] = '\0';
}
