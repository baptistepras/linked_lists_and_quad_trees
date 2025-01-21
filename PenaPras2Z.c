

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
/*           ListeZ                              */
/*                                               */
/*************************************************/

typedef struct BlocdeListeZ {
    int nombre;  // Entier contenue dans le bloc
    struct BlocdeListeZ *suivant;  // Pointeur vers le bloc suivant
    struct BlocdeListeZ **prec;  // Pointeur vers le champ suivant du bloc précédent
} BlocdeListeZ;

typedef BlocdeListeZ *acces;

/* affichage simple en recursif */
void affiche_recZ_SP(acces a, acces debut) {
    if (a->suivant == debut) {
        printf("%d]\n", a->nombre);
        // printf("SUIV: %d\n", a->suivant->nombre);  // Vérifie que la liste est bien cyclique
        // printf("PREC: %d\n", (*(a->prec))->nombre);  // Vérifie que le champ prec est bien implémenté
    }
    else {
        printf("%d, ", a->nombre);
        // printf("\nPREC: %d\n", (*(a->prec))->nombre);  // Vérifie que le champ prec est bien implémenté
        affiche_recZ_SP(a->suivant, debut);
    }
}

void affiche_recZ(acces a) {
    if (a == NULL)
        printf("[]\n");
    else {
        acces debut = a;
        if (a->suivant == debut) {
            printf("[%d]\n", a->nombre);
            // printf("SUIV: %d\n", a->suivant->nombre);  // Vérifie que la liste est bien cyclique
            // printf("PREC: %d\n", (*(a->prec))->nombre);  // Vérifie que le champ prec est bien implémenté
        }
        else {
            printf("[%d, ", a->nombre);
            // printf("\nPREC: %d\n", (*(a->prec))->nombre);  // Vérifie que le champ prec est bien implémenté
            affiche_recZ_SP(a->suivant, debut);
        }
    }
}

/*************************************************/

void ZAjouteAvant(int x, acces *a) {
    acces tmp = (acces) malloc(sizeof(BlocdeListeZ));
    tmp->nombre = x;
    if (*a == NULL) {
        tmp->suivant = tmp;
        tmp->prec = &(tmp->suivant);
    } else {
        tmp->suivant = *a;  // tmp suivant pointe vers l'ancien premier élément
        tmp->prec = (*a)->prec;  // tmp prec pointe vers le prec de l'ancien premier élément
        (*a)->prec = &(tmp->suivant);  // le prec de l'ancien premier élément pointe vers tmp suiv
        int debut = (*a)->nombre;
        while ((*a)->suivant->nombre != debut) {  // On suppose qu'il n'y a aucun doublon dans la liste :( 
            *a = (*a)->suivant;  // le suivant du dernier élément pointe vers le nouveau bloc
        }
        (*a)->suivant = tmp;
    }
    *a = tmp;
}

/*************************************************/

/* initialise une ListeZ vide */
void initVideZ(acces *a) {
    *a = NULL;
}

/* modifie la ListeZ en parametre: le premier element est retire */
void depileZ(acces *a) {
    acces tmp = *a;
    *a = (*a)->suivant;
    free(tmp);
}

void VideListeZ_SP(acces *a, acces debut) {
    if (*a != debut) {
        depileZ(a);
        VideListeZ_SP(&((*a)->suivant), debut);
    }
}

void VideListeZ(acces *a) {
    acces debut = *a;
    if (*a == NULL)
        ;
    else { 
        if (*a != debut)
            VideListeZ_SP(a, debut);
        else
            depileZ(a);
    }
}


/*************************************************/
/*                                               */
/*           Main                                */
/*                                               */
/*************************************************/

int main(int argc, char** argv) {
    
    printf("\nTests de ListeZ:\n");
    printf("La construction de ListeZ doit renvoyer successivement: [], [42], [93, 42], [17, 93, 42], [23, 17, 93, 42]:\n");
    acces a; initVideZ(&a); affiche_recZ(a);
    ZAjouteAvant(42, &a); affiche_recZ(a); ZAjouteAvant(93, &a); affiche_recZ(a); 
    ZAjouteAvant(17, &a); affiche_recZ(a); ZAjouteAvant(23, &a); affiche_recZ(a);
    VideListeZ(&a);
    printf("Décommentez les lignes 44, 45, 49, 61, 62 et 66 pour vérifier la propriété cyclique de la liste et la bonne implémentation du champ prec\n\n");
    
    return 0;
}
