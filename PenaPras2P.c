/*
Javier Pena: javier.pena-castano@universite-paris-saclay.fr
Baptiste Pras: baptiste.pras@universite-paris-saclay.fr

Compilé sur Jupyter et XCode pour vérifier la compilo-dépendance
(On avait eu des problèmes de ce genre avec Jupyter au pré-rendu)
*/

#include <stdio.h>
#include <stdlib.h>

/*************************************************/
/*                                               */
/*                sucre syntaxique               */
/*                                               */
/*************************************************/

#define AND &&
#define OR ||
#define ISNOT !=
#define NOT !
#define then

typedef enum{FALSE, TRUE} bool;
int malloc_Liste;
int malloc_ListedeListe;

/*************************************************/
/*                                               */
/*          definition type liste                */
/*                                               */
/*************************************************/

typedef struct Bloc {
    int nombre;
    struct Bloc *suivant;
} Bloc;

typedef Bloc *Liste;

/*************************************************/
/*                                               */
/*                predeclarations                */
/*                                               */
/*************************************************/

/* initialise une Liste � vide */
void initVide(Liste *L);

/* renvoie 1 si la Liste en parametre est vide, 0 sinon */
bool estVide(Liste l);

/* renvoie le premier element de la Liste en parametre */
int premier(Liste l);

/* renvoie une nouvelle Liste correspondant a celle en parametre, avec l'element x ajoute en haut de la pile */
Liste ajoute(int x, Liste l);

/* modifie la Liste en parametre: x est ajoute comme premier element */
void empile(int x, Liste* L);

/* renvoie une nouvelle Liste correspondant a celle en parametre sans son premier element */
Liste suite(Liste l);

/* modifie la Liste en parametre: le premier element est retire */
void depile(Liste* l);

/* affichage simple en recursif et en iteratif */
void affiche_rec(Liste l);
void affiche_iter(Liste l);

/* longueur en recursif et en iteratif */
int longueur_rec(Liste l);
int longueur_iter(Liste l);

/*  Elimination du dernier element en recursif et en iteratif  */
/*  VD est la sousprocedure utilitaire de la version recursive */
void VD (Liste *L);
void VireDernier_rec(Liste *L);
void VireDernier_iter(Liste *L);


/*************************************************/
/*                                               */
/*                briques de base                */
/*                                               */
/*************************************************/


void initVide(Liste *L) {
    *L = NULL;
}

bool estVide(Liste l) {
    return l == NULL;
}

int premier(Liste l) {
    return l->nombre;
}

Liste ajoute(int x, Liste l) {
    Liste tmp = (Liste) malloc(sizeof(Bloc));
    malloc_Liste++;
    tmp->nombre = x;
    tmp->suivant = l;
    return tmp;
}

void empile(int x, Liste *L) {
    *L = ajoute(x,*L);
}

Liste suite(Liste l) {
    return l->suivant;
}


void depile(Liste *L) {
    Liste tmp = *L;
    *L = suite(*L);
    free(tmp);
}

/*************************************************/
/*                                               */
/*     Affiche, avec les briques de base         */
/*                                               */
/*************************************************/

void affiche_rec_bis(Liste l) {
    if (estVide(l))
        printf("]\n");
    else {
        if (estVide(suite(l)))
            printf("%d", premier(l));
        else
            printf("%d, ", premier(l));
        affiche_rec_bis(suite(l));
    }
}

void affiche_rec(Liste l) {
    if (estVide(l))
        printf("[]\n");
    else {
        printf("[");
        affiche_rec_bis(l);
    }
}

void affiche_iter(Liste l) {
    Liste L2 = l;
    while(!estVide(L2))
    {
        printf("%d ", premier(L2));
        L2 = suite(L2);
    }
    printf("\n");
}


/*************************************************/
/*                                               */
/*     Longueur, sans les briques de base        */
/*                                               */
/*************************************************/

int longueur_rec (Liste l) {
    if (l == NULL)
        return 0;
    else return (1 + longueur_rec(l->suivant));
}


int longueur_iter (Liste l) {
    Liste P = l;
    int cpt = 0;
    while (P ISNOT NULL) {   
        P = P->suivant;
        cpt++;
    }
    return cpt;
}


/*************************************************/
/*                                               */
/*       VireDernier,                            */
/*               sans les briques de base,       */
/*               ni le "->"                      */
/*                                               */
/*************************************************/

void VD(Liste *L) {
    if (((**L).suivant) == NULL)  // *L non NULL ie liste non vide
        depile(L);  // moralement : depile(& (*L)) ;
    else VD(&((**L).suivant ));
}

void VireDernier_rec(Liste *L) {
    if ((*L) ISNOT NULL)
        VD(L);        // moralement : VD(& (*L)) ;
}

void VireDernier_iter(Liste *L) {
    if ( (*L) ISNOT NULL) {
        while (((**L).suivant) ISNOT NULL)
            L = &((**L).suivant );
        free(*L);
        *L = NULL;
    }
}


/*************************************************/
/*                                               */
/*       Libere la memoire                       */
/*                                               */
/*************************************************/

void VideListe(Liste *L) {
    if(NOT(estVide(*L))) {
        depile(L);
        VideListe(L);
    }
}


/*************************************************/
/*                                               */
/*           Permutations                        */
/*                                               */
/*************************************************/

/* Pour limiter la fuite mémoire, il faut vider les listes intermédiaires qu'on crée dans les fonctions auxiliaires et qui ne sont donc plus utilisées pour la suite, car actuellement elles restent inutilisées en mémoire */

typedef struct BlocdeListe {
    Liste liste;  // Liste d'entiers contenue dans le bloc
    struct BlocdeListe *suivant;  // Pointeur vers le bloc suivant
} BlocdeListe;

typedef BlocdeListe *ListedeListe;

/* initialise une ListedeListe vide */
void initVideL(ListedeListe *L) {
    *L = NULL;
}

/* renvoie 1 si la ListedeListe en parametre est vide, 0 sinon */
bool estVideL(ListedeListe L) {
    return L == NULL;
}

/* renvoie le premier element de la ListedeListe en parametre */
Liste premierL(ListedeListe L) {
    return L->liste;
}

/* renvoie une nouvelle ListedeListe correspondant a celle en parametre, avec la liste l ajoute en haut de la pile */
ListedeListe ajouteL(Liste l, ListedeListe L) {
    ListedeListe tmp = (ListedeListe) malloc(sizeof(BlocdeListe));
    malloc_ListedeListe++;
    tmp->liste = l;
    tmp->suivant = L;
    return tmp;
}

/* modifie la ListedeListe en parametre: l est ajoute comme premier element */
void empileL(Liste l, ListedeListe *L) {
    *L = ajouteL(l, *L);
}

/* renvoie une nouvelle ListedeListe correspondant a celle en parametre sans son premier element */
ListedeListe suiteL(ListedeListe L) {
    return L->suivant;
}

/* affichage simple en recursif */
void affiche_rec_bis_pourLdL(Liste l) {
    if (estVide(l))
        printf("]");
    else {
        if (estVide(suite(l)))
            printf("%d", premier(l));
        else
            printf("%d, ", premier(l));
        affiche_rec_bis_pourLdL(suite(l));
    }
}

void affiche_rec_pourLdL(Liste l) {  // Affichage spécial des listes au sein des ListedeListe
    if (estVide(l))
        printf("[]");
    else {
        printf("[");
        affiche_rec_bis_pourLdL(l);
    }
}

void affiche_recL_bis(ListedeListe L) {
    if(estVideL(L))
        printf("]\n");
    else {
        affiche_rec_pourLdL(premierL(L));
        if (!(estVideL(suiteL(L))))
            printf(", ");
        affiche_recL_bis(suiteL(L));
    }
}

void affiche_recL(ListedeListe L) {  // Affiche des ListedeListe
    if(estVideL(L))
        printf("\n");
    else {
        printf("[");
        affiche_recL_bis(L);
    }
}

/* modifie la ListedeListe en parametre: le premier element est retire */
void depileL(ListedeListe *L) {
    ListedeListe tmp = *L;
    //depile((*L)->liste);  // Est censé supprimé la liste contenue dans L, mais fait un seg fault
    *L = suiteL(*L);
    free(tmp);
}

/* libère la mémoire */
void VideListeL(ListedeListe *L) {
    if(NOT(estVideL(*L)))
    {
        depileL(L);
        VideListeL(L);
    }

}

/*************************************************/

ListedeListe AETTL(int x, ListedeListe L) {
    if (estVideL(L)) {
        ListedeListe P;
        initVideL(&P);
        return P;
    }
    else
        return ajouteL(ajoute(x, premierL(L)), AETTL(x, suiteL(L)));
}

ListedeListe ATP (int x, Liste L) {
    if (estVide(L)) {
        ListedeListe P; Liste p;
        initVideL(&P); initVide(&p);
        empile(x, &p); empileL(p, &P);
        return P;
    }
    else
        return ajouteL(ajoute(x, L), AETTL(premier(L), ATP(x, suite(L))));
}

ListedeListe Concat(ListedeListe L1, ListedeListe L2) {
    if (estVideL(L1))
        return L2;
    else
        return ajouteL(premierL(L1), Concat(suiteL(L1), L2));
}

ListedeListe ATLTP(int x, ListedeListe L) {
    if (estVideL(L)) {
        ListedeListe P;
        initVideL(&P);
        return P;
    } else
        return Concat(ATP(x, premierL(L)), ATLTP(x, suiteL(L)));
}

ListedeListe Permutations(int n) {
    if (n == 0) {
        ListedeListe P; Liste p;
        initVideL(&P); initVide(&p);
        empileL(p, &P);
        return P;
    } else
        return ATLTP(n, Permutations(n-1));
}


/*************************************************/
/*                                               */
/*           Main                                */
/*                                               */
/*************************************************/

int main(int argc, char** argv) {
    
    printf("\nTests de Permutations:\n");
    malloc_Liste = 0;
    malloc_ListedeListe = 0;
    ListedeListe p1 = Permutations(3);
    printf("Pour Permutations(3), on a fait %d malloc pour Liste et %d malloc pour ListedeListe\n",
           malloc_Liste, malloc_ListedeListe);
    ListedeListe p2 = Permutations(0);
    ListedeListe p3 = Permutations(1);
    printf("Permutations(3) doit renvoyer [[3, 2, 1], [2, 3, 1], [2, 1, 3], [3, 1, 2], [1, 3, 2], [1, 2, 3]]:\n");
    affiche_recL(p1); printf("Permutations(0) doit renvoyer [[]]: "); affiche_recL(p2);
    printf("Permutations(1) doit renvoyer [[1]]: "); affiche_recL(p3); printf("\n");
    VideListeL(&p1); VideListeL(&p2); VideListeL(&p3);
    
    return 0;
}