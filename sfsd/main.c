#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    int nbBlocs;
    int nbfir;
    int nblas;
} entete;

typedef struct Record {
    char info[20];
    int next;
    int nb;

    struct Record* svt;
} Record;

typedef struct {
    int cle;
    char info[200];
   int deletecle

} etudiant

typedef struct {
    Record* r;
    entetf t;
    FILE* physicalFile;
    }Lnovnc;


void Recuperer_chaine(int n, int i, int j, int *longeur, char *chaine) {
    *longeur = n;
    strncpy(chaine, buf.tab + j - 1, n);
    chaine[n] = '\0';
}
