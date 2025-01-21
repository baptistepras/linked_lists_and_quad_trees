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
/*           UnPlusDeuxEgalTrois                 */
/*                                               */
/*************************************************/

bool UnPlusDeuxEgalTrois(Liste L) {
    int un, deux, trois;
    if (estVide(L))
        return TRUE;
    else {
        if (estVide(suite(L))) {
            if (premier(L) == 0)
                return TRUE;
            else
                return FALSE;
        }
        else {
            if (estVide(suite(suite(L)))) {
                if (premier(L) + premier(suite(L)) == 0)
                    return TRUE;
                else
                    return FALSE;
            }
            else {
                if (premier(suite(suite(L))) == premier(L) + premier(suite(L)))
                    return TRUE;
                else
                    return FALSE;
            }
        }
    }
}


/*************************************************/
/*                                               */
/*           Croissante                          */
/*                                               */
/*************************************************/

bool Croissante(Liste L) {
    if (estVide(L))
        return TRUE;
    
    Liste P = suite(L);
    while (!(estVide(P))) {
        if (premier(P) < premier(L))
            return FALSE;
        L = suite(L); P = suite(P);
    }

    return TRUE;
}


/*************************************************/
/*                                               */
/*           NombreMemePosition                  */
/*                                               */
/*************************************************/

int NombreMemePosition_iter(Liste L, Liste P) {
    int cpt = 0;
    while (!(estVide(P)) && !(estVide(L))) {
        if (premier(L) == premier(P))
            cpt++;
        L = suite(L); P = suite(P);
    }
    return cpt;
}

/*************************************************/

int NombreMemePosition_rec(Liste L, Liste P) {
    if (estVide(P) || estVide(L))
        return 0;
    else {
        if (premier(L) == premier(P))
            return 1 + NombreMemePosition_rec(suite(L), suite(P));
        else
            return NombreMemePosition_rec(suite(L), suite(P));
    }
}

/*************************************************/

int NMPrecSF(Liste L, Liste P, int cpt) {
    if (estVide(P) || estVide(L))
        return cpt;
    else {
        if (premier(L) == premier(P)) {
            cpt++;
            return NMPrecSF(suite(L), suite(P), cpt);
        }
        else
            return NMPrecSF(suite(L), suite(P), cpt);
    }
}

int NombreMemePosition_recSF(Liste L, Liste P) {
    return NMPrecSF(L, P, 0);
}

/*************************************************/

void NMPrecSP(Liste L, Liste P, int* cpt) {
    if (estVide(P) || estVide(L))
        ;
    else {
        if (premier(L) == premier(P)) {
            (*cpt)++;
            NMPrecSP(suite(L), suite(P), cpt);
        }
        else
            NMPrecSP(suite(L), suite(P), cpt);
    }
}

int NombreMemePosition_recSP(Liste L, Liste P) {
    int cpt = 0;
    NMPrecSP(L, P, &cpt);
    return cpt;
}


/*************************************************/
/*                                               */
/*           FonctVireDernier                    */
/*                                               */
/*************************************************/

void FVD_recSP(Liste L, Liste *P) {
    if (estVide(suite(L)))
        ;
    else {
        FVD_recSP(suite(L), P);
        empile(premier(L), P);  // Récursif non terminal pas efficace (on fait une copie de la liste)
    }
}

Liste FonctVireDernier_rec(Liste L) {
    Liste P; initVide(&P);
    if (estVide(L)) {
        ;
    }
    else
        FVD_recSP(L, &P);
    return P;
}

/*************************************************/

Liste FonctVireDernier_it(Liste L) {
    Liste P;
    if (estVide(L) || estVide(suite(L))) {
        initVide(&P);
        return P;
    }

    P = L;  // P pointe vers le premier élément de la liste
    while (!(estVide(suite(suite(L))))) {
        L = suite(L);
    }
    L->suivant = NULL;  // Elimine le dernier élément de la liste

    return P;
}


/*************************************************/
/*                                               */
/*           AjouteDevantPremierZero             */
/*                                               */
/*************************************************/

void bisADPZ(Liste *P, int x) {
    if (estVide(*P))  // Aucun 0 rencontré
        empile(x, P);
    else {
        if (premier(*P) == 0)
            empile(x, P);  // On empile devant le premier 0
        else
            bisADPZ(&((*P)->suivant), x);  // On passe à l'élément suivant, toujours via le pointeur P
    }
}

void AjouteDevantPremierZero(Liste *L, int x) {
    Liste P = *L;  // On garde *L le pointeur du début de la liste, et on itère avec le pointeur P
    if (estVide(*L) || premier(*L) == 0)  // x vient en première position
        empile(x, L);
    else  // Sinon on itère sur le prochain élément de la liste, via P
        bisADPZ(&((P)->suivant), x);
}


/*************************************************/
/*                                               */
/*           AjouteDevantDernierZero             */
/*                                               */
/*************************************************/

void bisADDZ_recS(Liste *P, int x, bool* dernier_zero) {
    if (estVide(*P)) {
        if (*dernier_zero)
            ;
        else
            empile(x, P);  // Il n'y a aucun 0 dans la liste
    }
    else {
        if (premier(*P) == 0)
            *dernier_zero = TRUE;
        bisADDZ_recS(&((*P)->suivant), x, dernier_zero);
        if (premier(*P) == 0 && *dernier_zero) {  // On est devant le dernier 0 de la liste
            empile(x, P);
            *dernier_zero = FALSE;
        }
    }
}

void AjouteDevantDernierZero_recS(Liste *L, int x) {
    if (estVide(*L))
        empile(x, L);
    else {
        Liste P = *L;  // On garde *L le pointeur du début de la liste, et on itère avec le pointeur P
        bool b = FALSE;
        bisADDZ_recS(&P, x, &b);
        if (premier(*L) == 0 && !b)  // Cas où le premier élément de la liste est le dernier 0
            empile(x, L);
    }
}

/*************************************************/

void AjouteDevantDernierZero_it(Liste *L, int x) {
    if (estVide(*L))
        empile(x, L);
    else {
        Liste prec = NULL;  // On sauvegarde l'adresse du dernier élément avec prec
        Liste P = *L;  // On itère avec P
        Liste avant = NULL; Liste apres = NULL;  // On encadre la potentielle position de x avec avant/apres
        while (!(estVide(P))) {
            if (premier(P) == 0) {
                avant = prec;
                apres = P;
            }
            prec = P;
            P = suite(P);
        }
        if (avant == NULL) {
            if (apres == NULL) {  // Cas où il n'y a aucun 0 dans la liste
                empile(x, &P);
                (*L)->suivant = P;
            }
            else  // Cas où le premier élément de la liste est le dernier 0
                empile(x, L);
        }
        else {  // On insère x entre le dernier 0 et l'élément qui le précède
            P = ajoute(x, apres);
            avant->suivant = P;
        }
    }
}

/*************************************************/

void bisADDZ_recT(Liste *L, Liste P, int x, Liste avant, Liste apres, Liste prec) {
    if (estVide(P)) {
        if (avant == NULL) {
            if (apres == NULL) {  // Cas où il n'y a aucun 0 dans la liste
                P = ajoute(x, P);
                (*L)->suivant = P;
            }
            else  // Cas où le premier élément de la liste est le dernier 0
                empile(x, L);
        }
        else {  // On insère x entre le dernier 0 et l'élément qui le précède
            P = ajoute(x, apres);
            avant->suivant = P;
        }
    }
    else {
        if (premier(P) == 0) {
            avant = prec;
            apres = P;
        }
        bisADDZ_recT(L, suite(P), x, avant, apres, P);
    }
}

void AjouteDevantDernierZero_recT(Liste *L, int x) {
    if (estVide(*L))
        empile(x, L);
    else {
        Liste av = NULL; Liste ap = NULL;  // On encadre la potentielle position de x avec avant/apres
        Liste prec = NULL;  // On sauvegarde l'adresse du dernier élément avec prec
        bisADDZ_recT(L, *L, x, av, ap, prec);
        // On garde *L le pointeur du début de la liste, et on itère avec P
    }
}


/*************************************************/
/*                                               */
/*           Tic                                 */
/*                                               */
/*************************************************/

void bis(Liste *L, bool zero_deja_vu, bool check_for_zeros) {
    if (estVide(*L) || zero_deja_vu)
        ;
    else {
        if (check_for_zeros) {  // Après avoir ajouté un 0, on élimine les 0 consécutifs suivants
            while (!(estVide(*L)) && premier(*L) == 0) {
                depile(L);
                zero_deja_vu = TRUE;  // On vient d'enlever un 0, donc d'en voir un
            }
            bis(L, zero_deja_vu, FALSE);
        }
        else {
            if (premier(*L) != 0) {  // Ajoute un 0 si les conditions le permettent
                empile(0, L);
                bis(&((*L)->suivant->suivant), zero_deja_vu, TRUE);
            }
            else  // L'élément actuel est un 0
                bis(&((*L)->suivant), TRUE, check_for_zeros);
        }
    }
}

void Tic(Liste *L) {
    bool b1 = FALSE; bool b2 = FALSE;
    bis(L, b1, b2);
}


/*************************************************/
/*                                               */
/*           Main                                */
/*                                               */
/*************************************************/

void poup (Liste l) {
    printf("Double Affichage \n");
    affiche_rec(l);
    affiche_iter(l);

    printf("Longueur en double %d %d \n\n",
           longueur_rec(l),
           longueur_iter(l)
    );
}

int main(int argc, char** argv) {
    Liste l;  // [8, 7, 6]
    initVide (&l); poup(l);
    empile(4, &l); poup(l);
    empile(5, &l); empile(6, &l); empile(7, &l); empile(8, &l); empile(9, &l);
    poup(l); VireDernier_rec(&l); VireDernier_iter(&l); depile(&l); poup(l);

    Liste l1; initVide(&l1);  // [1, 2, 3]
    empile(3, &l1); empile(2, &l1); empile(1, &l1);

    Liste l1a; initVide(&l1a);  // [3]
    empile(3, &l1a);

    Liste l1b; initVide(&l1b);  // [0, 0]
    empile(0, &l1b); empile(0, &l1b);

    Liste l2; initVide(&l2);  // []

    Liste l3; initVide(&l3);  // [3, 6, 9, 0, 3, 4, 2, 9, 2]
    empile(2, &l3); empile(9, &l3); empile(2, &l3); empile(4, &l3); empile(3, &l3);
    empile(0, &l3); empile(9, &l3); empile(6, &l3); empile(3, &l3);

    Liste l4; initVide(&l4);  // [3, 9, 6, 0, 2, 2, 2, 2, 2, 2, 5]
    empile(5, &l4); empile(2, &l4); empile(2, &l4); empile(2, &l4); empile(2, &l4);
    empile(2, &l4); empile(2, &l4); empile(0, &l4); empile(6, &l4); empile(9, &l4);
    empile(3, &l4);

    Liste l5; initVide(&l5); empile(1, &l5); empile(2, &l5);  // [2, 1]

    Liste l6; initVide(&l6); empile(1, &l6); empile(0, &l6);  // [4, 0, 5, 0, 8, 0, 1]
    empile(8, &l6); empile(0, &l6); empile(5, &l6); empile(0, &l6); empile(4, &l6);

    Liste l7; initVide(&l7); empile(1, &l7); empile(0, &l7);  // [4, 0, 5, 0, 8, 0, 1]
    empile(8, &l7); empile(0, &l7); empile(5, &l7); empile(0, &l7); empile(4, &l7);

    Liste l8; initVide(&l8); empile(1, &l8); empile(0, &l8);  // [4, 0, 5, 0, 8, 0, 1]
    empile(8, &l8); empile(0, &l8); empile(5, &l8); empile(0, &l8); empile(4, &l8);

    Liste l9; initVide(&l9); empile(1, &l9); empile(0, &l9);  // [4, 0, 5, 0, 8, 0, 1]
    empile(8, &l9); empile(0, &l9); empile(5, &l9); empile(0, &l9); empile(4, &l9);

    Liste l10; initVide(&l10);  // [3, 7, 2, 0, 0, 0, 8, 9, 0, 0, 2, 1]
    empile(1, &l10); empile(2, &l10); empile(0, &l10); empile(0, &l10); empile(9, &l10);
    empile(8, &l10); empile(0, &l10); empile(0, &l10); empile(0, &l10); empile(2, &l10);
    empile(7, &l10); empile(3, &l10);

    printf("Tests de UnPlusDeuxEgalTrois:\n");
    printf("l doit renvoyer 0: %d\n", UnPlusDeuxEgalTrois(l));
    printf("l1 doit renvoyer 1: %d\n", UnPlusDeuxEgalTrois(l1));
    printf("l2 doit renvoyer 1: %d\n", UnPlusDeuxEgalTrois(l2));
    printf("l1a doit renvoyer 0: %d\n", UnPlusDeuxEgalTrois(l1a));
    printf("l1b doit renvoyer 1: %d\n\n", UnPlusDeuxEgalTrois(l1b));

    printf("Tests de Croissante:\n");
    printf("l doit renvoyer 0: %d\n", Croissante(l));
    printf("l1 doit renvoyer 1: %d\n", Croissante(l1));
    printf("l2 doit renvoyer 1: %d\n\n", Croissante(l2));

    printf("Tests de NombreMemePosition_iter:\n");
    printf("(l3, l4) doit renvoyer 4: %d\n", NombreMemePosition_iter(l3, l4));
    printf("(l2, l1) doit renvoyer 0: %d\n", NombreMemePosition_iter(l2, l1));
    printf("(l1, l1) doit renvoyer 3: %d\n\n", NombreMemePosition_iter(l1, l1));

    printf("Tests de NombreMemePosition_rec:\n");
    printf("(l3, l4) doit renvoyer 4: %d\n", NombreMemePosition_rec(l3, l4));
    printf("(l2, l1) doit renvoyer 0: %d\n", NombreMemePosition_rec(l2, l1));
    printf("(l1, l1) doit renvoyer 3: %d\n\n", NombreMemePosition_rec(l1, l1));

    printf("Tests de NombreMemePosition_recSF:\n");
    printf("(l3, l4) doit renvoyer 4: %d\n", NombreMemePosition_recSF(l3, l4));
    printf("(l2, l1) doit renvoyer 0: %d\n", NombreMemePosition_recSF(l2, l1));
    printf("(l1, l1) doit renvoyer 3: %d\n\n", NombreMemePosition_recSF(l1, l1));

    printf("Tests de NombreMemePosition_recSP:\n");
    printf("(l3, l4) doit renvoyer 4: %d\n", NombreMemePosition_recSP(l3, l4));
    printf("(l2, l1) doit renvoyer 0: %d\n", NombreMemePosition_recSP(l2, l1));
    printf("(l1, l1) doit renvoyer 3: %d\n\n", NombreMemePosition_recSP(l1, l1));

    printf("Tests de FonctVireDernier_rec:\n");
    printf("l doit renvoyer [8, 7]: "); affiche_rec(FonctVireDernier_rec(l));
    printf("l2 doit renvoyer la liste vide: "); affiche_rec(FonctVireDernier_rec(l2));
    printf("l5 doit renvoyer [2]: "); affiche_rec(FonctVireDernier_rec(l5)); printf("\n");

    printf("Tests de FonctVireDernier_it:\n");
    printf("l doit renvoyer [8, 7]: "); affiche_rec(FonctVireDernier_it(l));
    printf("l2 doit renvoyer la liste vide: "); affiche_rec(FonctVireDernier_it(l2));
    printf("l5 doit renvoyer [2]: "); affiche_rec(FonctVireDernier_it(l5)); printf("\n");

    printf("Tests de AjouteDevantPremierZero:\n");
    Liste lv; initVide(&lv);
    Liste lunA; initVide(&lunA); empile(0, &lunA);
    Liste lunB; initVide(&lunB); empile(1, &lunB);
    AjouteDevantPremierZero(&l6, 7); AjouteDevantPremierZero(&lv, 7);
    AjouteDevantPremierZero(&lunA, 7); AjouteDevantPremierZero(&lunB, 7);
    printf("(l6, 7) doit renvoyer [4, 7, 0, 5, 0, 8, 0, 1]: "); affiche_rec(l6);
    printf("(lv, 7) doit renvoyer [7]: "); affiche_rec(lv);
    printf("(lunA, 7) doit renvoyer [7, 0]: "); affiche_rec(lunA);
    printf("(lunB, 7) doit renvoyer [1, 7]: "); affiche_rec(lunB);
    VideListe(&lv); VideListe(&lunA); VideListe(&lunB); printf("\n");

    printf("Tests de AjouteDevantDernierZero_recS:\n");
    initVide(&lv); initVide(&lunA); empile(0, &lunA); initVide(&lunB); empile(1, &lunB);
    AjouteDevantDernierZero_recS(&l7, 7); AjouteDevantDernierZero_recS(&lv, 7);
    AjouteDevantDernierZero_recS(&lunA, 7); AjouteDevantDernierZero_recS(&lunB, 7);
    printf("(l7, 7) doit renvoyer [4, 0, 5, 0, 8, 7, 0, 1]: "); affiche_rec(l7);
    printf("(lv, 7) doit renvoyer [7]: "); affiche_rec(lv);
    printf("(lunA, 7) doit renvoyer [7, 0]: "); affiche_rec(lunA);
    printf("(lunB, 7) doit renvoyer [1, 7]: "); affiche_rec(lunB);
    VideListe(&lv); VideListe(&lunA); VideListe(&lunB); printf("\n");

    printf("Tests de AjouteDevantDernierZero_it:\n");
    initVide(&lv); initVide(&lunA); empile(0, &lunA); initVide(&lunB); empile(1, &lunB);
    AjouteDevantDernierZero_it(&l8, 7); AjouteDevantDernierZero_it(&lv, 7);
    AjouteDevantDernierZero_it(&lunA, 7); AjouteDevantDernierZero_it(&lunB, 7);
    printf("(l8, 7) doit renvoyer [4, 0, 5, 0, 8, 7, 0, 1]: "); affiche_rec(l8);
    printf("(lv, 7) doit renvoyer [7]: "); affiche_rec(lv);
    printf("(lunA, 7) doit renvoyer [7, 0]: "); affiche_rec(lunA);
    printf("(lunB, 7) doit renvoyer [1, 7]: "); affiche_rec(lunB);
    VideListe(&lv); VideListe(&lunA); VideListe(&lunB); printf("\n");

    printf("Tests de AjouteDevantDernierZero_recT:\n");
    initVide(&lv); initVide(&lunA); empile(0, &lunA); initVide(&lunB); empile(1, &lunB);
    AjouteDevantDernierZero_recT(&l9, 7); AjouteDevantDernierZero_recT(&lv, 7);
    AjouteDevantDernierZero_recT(&lunA, 7); AjouteDevantDernierZero_recT(&lunB, 7);
    printf("(l9, 7) doit renvoyer [4, 0, 5, 0, 8, 7, 0, 1]: "); affiche_rec(l9);
    printf("(lv, 7) doit renvoyer [7]: "); affiche_rec(lv);
    printf("(lunA, 7) doit renvoyer [7, 0]: "); affiche_rec(lunA);
    printf("(lunB, 7) doit renvoyer [1, 7]: "); affiche_rec(lunB);
    VideListe(&lv); VideListe(&lunA); VideListe(&lunB); printf("\n");

    printf("Tests de Tic:\n");
    Tic(&l10); Tic(&l1); Tic(&l2); Tic(&l1b);
    printf("l10 doit renvoyer [0, 3, 0, 7, 0, 2, 8, 9, 0, 0, 2, 1]: "); affiche_rec(l10);
    printf("l1 doit renvoyer [0, 1, 0, 2, 0, 3]: "); affiche_rec(l1);
    printf("l2 doit renvoyer la liste vide: "); affiche_rec(l2);
    printf("l1b doit renvoyer [0, 0]: "); affiche_rec(l1b); printf("\n");

    VideListe(&l); VideListe(&l1); VideListe(&l1a); VideListe(&l1b); VideListe(&l2);
    VideListe(&l3); VideListe(&l4); VideListe(&l5); VideListe(&l6);
    
    return 0;
}
