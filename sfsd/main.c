
void Recuperer_chaine(int n, int i, int j, int *longeur, char *chaine) {
    *longeur = n;
    strncpy(chaine, buf.tab + j - 1, n);
    chaine[n] = '\0';
}
