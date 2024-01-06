#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Etudiant {
    char nom[50];
    char prenom[50];
    int matricule;
    struct Etudiant* suivant;
};

struct ListeEtudiants {
    struct Etudiant* tete;
};

struct Etudiant* creerEtudiant(int tm_nom, int tm_prenom) {
    char nom[tm_nom], prenom[tm_prenom];
    int i = 0;
    struct Etudiant* nouvelEtudiant = (struct Etudiant*)malloc(sizeof(struct Etudiant));

    if (nouvelEtudiant == NULL) {
        perror("Erreur lors de l'allocation de mémoire pour un nouvel étudiant");
        exit(EXIT_FAILURE);
    }

    printf("Nom : ");
    while (i < tm_nom - 1 && scanf("%c", &nom[i]) == 1 && nom[i] != '?') {
        i++;
    }
    nom[i] = '\0';

    i = 0;

    printf("Prenom : ");
    while (i < tm_prenom - 1 && scanf("%c", &prenom[i]) == 1 && prenom[i] != '?') {
        i++;
    }
    prenom[i] = '\0';

    printf("Matricule : ");
    scanf("%d", &(nouvelEtudiant->matricule));

    strcpy(nouvelEtudiant->nom, nom);
    strcpy(nouvelEtudiant->prenom, prenom);
    nouvelEtudiant->suivant = NULL;

    return nouvelEtudiant;
}

void ajouterEtudiant(struct ListeEtudiants* liste, int tm_nom, int tm_prenom) {
    struct Etudiant* nouvelEtudiant = creerEtudiant(tm_nom, tm_prenom);

    if (liste->tete == NULL) {
        liste->tete = nouvelEtudiant;
    } else {
        nouvelEtudiant->suivant = liste->tete;
        liste->tete = nouvelEtudiant;
    }
}

void supprimerEtudiant(struct ListeEtudiants* liste, int matricule) {
    struct Etudiant* courant = liste->tete;
    struct Etudiant* precedent = NULL;

    while (courant != NULL && courant->matricule != matricule) {
        precedent = courant;
        courant = courant->suivant;
    }

    if (courant != NULL) {
        if (precedent != NULL) {
            precedent->suivant = courant->suivant;
        } else {
            liste->tete = courant->suivant;
        }

        free(courant);
    }
}

void supprimerTout(struct ListeEtudiants* liste) {
    while (liste->tete != NULL) {
        struct Etudiant* courant = liste->tete;
        liste->tete = liste->tete->suivant;
        free(courant);
    }
}

struct Etudiant* rechercherEtudiant(struct ListeEtudiants* liste, int matricule) {
    struct Etudiant* courant = liste->tete;

    while (courant != NULL) {
        if (courant->matricule == matricule) {
            return courant;
        }
        courant = courant->suivant;
    }

    return NULL;
}

void afficherDansFichier(struct ListeEtudiants* liste, const char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "w");

    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    struct Etudiant* courant = liste->tete;

    while (courant != NULL) {
        fprintf(fichier, "Nom: %s,\n Prenom: %s,\n Matricule: %d\n", courant->nom, courant->prenom, courant->matricule);
        courant = courant->suivant;
    }

    fclose(fichier);
}

int main() {
    struct ListeEtudiants liste;
    int choix, n, mat;

    liste.tete = NULL;

    while (choix != 5) {
        printf("\n1. Ajouter des étudiants\n");
        printf("2. Supprimer un étudiant\n");
        printf("3. Rechercher un étudiant\n");
        printf("4. Supprimer tout \n");
        printf("5. Quitter\n");

        printf("Choisissez une option : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                printf("Nombre d'étudiants à ajouter : ");
                scanf("%d", &n);
                for (int i = 0; i < n; i++) {
                    ajouterEtudiant(&liste, 20, 20);
                }
                afficherDansFichier(&liste, "sfsd");
                break;

            case 2:
                printf("Matricule de l'étudiant à supprimer : ");
                scanf("%d", &mat);
                supprimerEtudiant(&liste, mat);
                afficherDansFichier(&liste, "sfsd");
                break;

            case 3:
                printf("Matricule de l'étudiant à rechercher : ");
                scanf("%d", &mat);
                struct Etudiant* resultat = rechercherEtudiant(&liste, mat);
                if (resultat != NULL) {
                    printf("Étudiant trouvé: Nom: %s, Prenom: %s, Matricule: %d\n", resultat->nom, resultat->prenom, resultat->matricule);
                } else {
                    printf("Étudiant non trouvé.\n");
                }
                break;
            case 4:
                supprimerTout(&liste);
                afficherDansFichier(&liste, "sfsd");
                break;

            case 5:
                return 0;

            default:
                printf("Option invalide\n");
        }
    }

    return 0;
}
