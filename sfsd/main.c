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



void Recuperer_chaine(int n, int i, int j, int *longeur, char *chaine) {
    *longeur = n;
    strncpy(chaine, buf.tab + j - 1, n);
    chaine[n] = '\0';
}
