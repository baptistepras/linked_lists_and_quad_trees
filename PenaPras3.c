/*
Javier Pena: javier.pena-castano@universite-paris-saclay.fr
Baptiste Pras: baptiste.pras@universite-paris-saclay.fr

Compilé sur Jupyter et XCode pour vérifier la compilo-dépendance
(On avait eu des problèmes de ce genre avec Jupyter au pré-rendu)

InterUnion ne marche pas parfaitement, voir message ligne 308
Idem pour CompteDamiers, voir message ligne 392
Compresse et Dalite pas fait par manque de compréhension
*/

#include <stdio.h>
#include <stdlib.h>

/*************************************************/
/*                                               */
/*                Sucre syntaxique               */
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
/*              Définition du type               */
/*                                               */
/*************************************************/

typedef struct bloc_image {
    bool quatre;
    struct bloc_image *hg, *hd, *bg, *bd;
} bloc_image;
typedef bloc_image *image;


/*************************************************/
/*                                               */
/*            Construction de l'image            */
/*                                               */
/*************************************************/


image Blanc() {
    image tmp = (image) malloc(sizeof(bloc_image));
    tmp->quatre = FALSE;
    tmp->hg = NULL; tmp->hd = NULL;
    tmp->bg = NULL; tmp->bd = NULL;
    return tmp;
}

image Noir() {return NULL;}

image Compose(image i1, image i2, image i3, image i4) {
    image tmp = (image) malloc(sizeof(bloc_image));
    tmp->quatre = TRUE;
    tmp->hg = i1; tmp->hd = i2;
    tmp->bg = i3; tmp->bd = i4;
    return tmp;
}


/*************************************************/
/*                                               */
/*                Affichage                      */
/*                                               */
/*************************************************/

void AffichageBis(image i) {
    if (i == NULL) {
        printf("X");
    } else {
        if (!(i->quatre)) {
            printf("o");
        } else {
            AffichageBis(i->hg); AffichageBis(i->hd);
            AffichageBis(i->bg); AffichageBis(i->bd);
            printf("*");
        }
    }
}

void Affichage(image i) {
    if (i == NULL) {
        printf("X\n");
    } else {
        if (!(i->quatre))
            printf("o\n");
        else {
            AffichageBis(i->hg); AffichageBis(i->hd);
            AffichageBis(i->bg); AffichageBis(i->bd);
            printf("*\n");
        }
    }
}

void AffichageProfondeurBis(image i, int profondeur) {
    if (i == NULL) {
        printf("X%d ", profondeur);
    } else {
        if (!(i->quatre)) {
            printf("o%d ", profondeur);
        } else {
            profondeur++;
            AffichageProfondeurBis(i->hg, profondeur); AffichageProfondeurBis(i->hd, profondeur);
            AffichageProfondeurBis(i->bg, profondeur); AffichageProfondeurBis(i->bd, profondeur);
            printf("*%d ", profondeur-1);
        }
    }
}

void AffichageProfondeur(image i) {
    if (i == NULL) {
        printf("X0\n");
    } else {
        if (!(i->quatre))
            printf("o0\n");
        else {
            AffichageProfondeurBis(i->hg, 1);
            AffichageProfondeurBis(i->hd, 1);
            AffichageProfondeurBis(i->bg, 1);
            AffichageProfondeurBis(i->bd, 1);
            printf("*0\n");
        }
    }
}


/*************************************************/
/*                                               */
/*                Lecture                        */
/*                                               */
/*************************************************/

image LectureBis(char *img[], int *p) {
    switch ((*img)[*p]) {
        case '*' : *p -= 1; image bd = LectureBis(img, p);  image bg = LectureBis(img, p);
                    image hd = LectureBis(img, p); image hg = LectureBis(img, p);
                    return Compose(hg, hd, bg, bd);  // On s'enfonce en profondeur
        case 'o' : *p -= 1; return Blanc();
        case 'X' : *p -= 1; return Noir();
        default : return Noir();  // Simplement pour éviter les warnings, cas jamais atteint
    }
}

image Lecture() {
    // Crée un tableau de char avec dans l'ordre de gauche à droite,
    // les caractères tapés au clavier par l'utilisateur
    int taille = 128;
    char *imageStr = malloc(taille * sizeof(char));
    printf("Tapez sur une ligne votre arbre où 'o' est une image blanche, 'X' une image noire, et '*' "
           "une image qui se divise en 4 sous-images. '!' marquera la fin de votre arbre:\n");
    char c = getchar(); int pointeur = -1;
    while (c != '!') {
        // Agrandi le tableau s'il n'est plus assez grand
        if (pointeur >= taille) {
            taille *= 2;
            imageStr = realloc(imageStr, taille * sizeof(char));
        }
        // Ajoute le caractère correspondant à une couleur
        if (c == 'o' || c == 'X' || c == '*') {
            pointeur++;
            imageStr[pointeur] = c;
        }
        c = getchar();
    }

    // Construit l'image décrite par imageStr
    return LectureBis(&imageStr, &pointeur);
}


/*************************************************/
/*                                               */
/*            estBlanche / estNoire              */
/*                                               */
/*************************************************/

bool estBlanche(image img) {
    if (img == NULL)  // L'image n'est pas une feuille noire
        return FALSE;
    else {
        if (img->quatre)  // L'image n'est pas une feuille blanche
            return estBlanche(img->hg) && estBlanche(img->hd) &&
                   estBlanche(img->bg) && estBlanche(img->bd);
        else
            return TRUE;
    }
}

bool estNoire(image img) {
    if (img == NULL)  // L'image n'est pas une feuille noire
        return TRUE;
    else {
        if (img->quatre)  // L'image n'est pas une feuille blanche
            return estNoire(img->hg) && estNoire(img->hd) &&
                   estNoire(img->bg) && estNoire(img->bd);
        else
            return FALSE;
    }
}


/*************************************************/
/*                                               */
/*                Aire                           */
/*                                               */
/*************************************************/

double Aire(image img) {
    if (img == NULL)
        return 1.;
    else {
        if (img->quatre)
            // Simple moyenne pondérée de chaque sous-arbre qui remonte jusqu'à la racine
            return 0.25 * (Aire(img->hg) + Aire(img->hd) + Aire(img->bg) + Aire(img->bd));
        else
            return 0.;
    }
}


/*************************************************/
/*                                               */
/*               TriangleBD                      */
/*                                               */
/*************************************************/

image TriangleBD(int p) {
    if (p == 0)
        return Blanc();
    else {
        // On remplit en noir seulement le carreau en bas à droite
        p -= 1;
        return Compose(Blanc(), TriangleBD(p), TriangleBD(p), Noir());
    }
}


/*************************************************/
/*                                               */
/*                 Arrondit                      */
/*                                               */
/*************************************************/

void Arrondit(image *img, int p) {
    if (*img == NULL)  // L'image n'est pas une feuille noire
        ;
    else {
        if (!((*img)->quatre))  // L'image n'est pas une feuille blanche
            ;
        else {
            if (p == 0) {
                if (Aire(*img) >= 0.5)  // On colore l'image en noir
                    *img = Noir();
                else  // On colore l'image en blanc
                    *img = Blanc();
            } else {
                // On arrondit chacun des 4 blocs de l'image
                p -= 1;
                Arrondit(&((*img)->hg), p); Arrondit(&((*img)->hd), p);
                Arrondit(&((*img)->bg), p); Arrondit(&((*img)->bd), p);
            }
        }
    }
}


/*************************************************/
/*                                               */
/*                 MemeDessin                    */
/*                                               */
/*************************************************/

bool MemeDessin(image img1, image img2) {
    if (img1 == NULL)
        return estNoire(img2);  // img1 est une image noire, on vérifie que img2 l'est aussi
    if (img2 == NULL)
        return estNoire(img1);  // img2 est une image noire, on vérifie que img1 l'est aussi
    else {
        if (!(img1->quatre))
            return estBlanche(img2);  // img1 est une image blanche, on vérifie que img2 l'est aussi
        if (!(img2->quatre))
            return estBlanche(img1);  // img2 est une image blanche, on vérifie que img1 l'est aussi
        else
            // Appel récursif sur chaque sous-image des deux images
            return MemeDessin(img1->hg, img2->hg) && MemeDessin(img1->hd, img2->hd)
                && MemeDessin(img1->bg, img2->bg) && MemeDessin(img1->bd, img2->bd);
    }
}


/*************************************************/
/*                                               */
/*                InterUnion                     */
/*                                               */
/*************************************************/

// XoXoXooX*ooXX*X**!
void InterUnion(image *img1, image *img2) {
    // La fonction crée trop de sous-images, donc le rendu n'est pas le même
    // Impossible de comprendre pourquoi ou comment régler le problème sans passer par des copies

    if (estNoire(*img1) && estBlanche(*img2)) {
        // Intervertis les couleurs de img1 et img2 (seulement nécessaire dans ce cas)
        *img1 = Blanc(); *img2 = Noir();

    } else {

        // Sauvegarde des valeurs pour gagner en temps d'exécution
        bool n1 = estNoire(*img1); bool n2 = estNoire(*img2);
        bool b1 = estBlanche(*img1); bool b2 = estBlanche(*img2);
        bool flag = FALSE;

        if ((n1 || b1) && !(n2 || b2)) {
            // img1 est une image complète, mais pas img2, donc on agrandit img1
            image tmp = (image) malloc(sizeof(bloc_image));
            tmp->quatre = TRUE;
            flag = TRUE;
            if (n1) {
                tmp->hg = Noir(); tmp->hd = Noir();
                tmp->bg = Noir(); tmp->bd = Noir();
            } else {
                tmp->hg = Blanc(); tmp->hd = Blanc();
                tmp->bg = Blanc(); tmp->bd = Blanc();
            }
            *img1 = tmp;

        } else {
            if ((n2 || b2) && !(n1 || b1)) {
                // img2 est une image complète, mais pas img1, donc on agrandit img2
                image tmp = (image) malloc(sizeof(bloc_image));
                tmp->quatre = TRUE;
                flag = TRUE;
                if (n2) {
                    tmp->hg = Noir(); tmp->hd = Noir();
                    tmp->bg = Noir(); tmp->bd = Noir();
                } else {
                    tmp->hg = Blanc(); tmp->hd = Blanc();
                    tmp->bg = Blanc(); tmp->bd = Blanc();
                }
                *img2 = tmp;
            } else {

                if (!(n2 || b2) && !(n1 || b1))
                    // Si ni img1 ni img2 ne sont des images complètes (ont des sous-images)
                    flag = TRUE;
            }
        }

        if (flag) {  // Appels récursifs seulement si img1 et img2 ont des sous-images
            InterUnion(&(*img1)->hg, &(*img2)->hg);
            InterUnion(&(*img1)->hd, &(*img2)->hd);
            InterUnion(&(*img1)->bg, &(*img2)->bg);
            InterUnion(&(*img1)->bd, &(*img2)->bd);
        }
    }
}


/*************************************************/
/*                                               */
/*                CompteDamiers                   */
/*                                               */
/*************************************************/

bool CompteDamiersBis(image img, int *acc) {
    if (img == NULL) {
        ;
    } else {
        if (!(img->quatre)) {
            ;
        } else {
            if (estNoire(img->hd) && estNoire(img->bg) &&
                estBlanche(img->hg) && estBlanche(img->bd))
                *acc += 1;
            CompteDamiersBis(img->hg, acc);
            CompteDamiersBis(img->hd, acc);
            CompteDamiersBis(img->bg, acc);
            CompteDamiersBis(img->bd, acc);
        }
    }
    return FALSE;
}

int CompteDamiers(image img) {
    // Compte seulement 14, mais pas de succès pour trouver quel damier n'est pas trouvé
    if (img == NULL) {
        return 0;
    } else {
        if (!(img->quatre)) {
            return 0;
        } else {
            int acc = 0;
            CompteDamiersBis(img, &acc);
            return acc;
        }
    }
}

/*************************************************/
/*                                               */
/*             Compresse et Dilate               */
/*                                               */
/*************************************************/

void Compresse(image *img);

void Dilate(image *img);


/*************************************************/
/*                                               */
/*                    main                       */
/*                                               */
/*************************************************/

int main() {
    // Tests de la lecture et de l'affichage
    // oooX*ooXo*oXoo*Xooo**! -> o2 o2 o2 X2 *1 o2 o2 X2 o2 *1 o2 X2 o2 o2 *1 X2 o2 o2 o2 *1 *0
    printf("Tests de la lecture et de l'affichage:\n");
    printf("Entre votre arbre, la première ligne doit renvoyer l'affichage simple,"
           " et la seconde l'affichage avec profondeur de chaque bloc.\n");
    printf("Vous pouvez essayer 'XooXo*oXXXoXoXX**oX**!' qui renverra "
           "'X1 o2 o2 X2 o2 *1 o1 X2 X3 X3 o3 X4 o4 X4 X4 *3 *2 o2 X2 *1 *0'.\n\n");
    image lecture = Lecture(); image b = Blanc(); image n = Noir();
    Affichage(lecture); AffichageProfondeur(lecture);
    printf("Blanc(): "); Affichage(b); printf("Blanc(): "); AffichageProfondeur(b);
    printf("Noir(): "); Affichage(n); printf("Noir(): "); AffichageProfondeur(n);
    printf("\n+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n\n");


    // Tests de estBlanche() et estNoire()
    printf("Tests de estNoire() et estBlanche():\n");
    printf("Entrez successivement 'ooooo*oooo*o*oooooo**!' puis 'XXXXX*XXXX*X*XXXXXX**!'"
           "qui sont respectivement une image blanche et une image noire.\n\n");
    image blanche = Lecture();  //
    image noire = Lecture();  //
    printf("estBlanche():\n -Image blanche:%d\n -Image noire:%d\n -Image lecture:%d\n "
           "-Image Blanc():%d\n -Image Noir():%d\n\n", estBlanche(blanche),
           estBlanche(noire), estBlanche(lecture), estBlanche(b), estBlanche(n));
    printf("estNoire():\n -Image blanche:%d\n -Image noire:%d\n -Image lecture:%d\n "
           "-Image Blanc():%d\n -Image Noir():%d\n", estNoire(blanche), estNoire(noire),
           estNoire(lecture), estNoire(b), estNoire(n));
    printf("\n+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n\n");


    // Tests de Aire
    printf("Tests de Aire():\n");
    printf("Entrez 'XoXoo*XXXXo**!', son aire doit valoir 0.75.\n\n");
    image aire = Lecture();
    printf("Aire():\n -Image aire:%lf\n -Image noire:%lf\n -Image blanche:%lf\n "
           "-Image Blanc():%lf\n -Image Noir():%lf\n", Aire(aire),
           Aire(noire), Aire(blanche), Aire(b), Aire(n));
    printf("\n+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n\n");


    // Tests de TriangleBD
    printf("Tests de TriangleBD():\n");
    image tbd0 = TriangleBD(0); image tbd1 = TriangleBD(1);
    image tbd3 = TriangleBD(3); printf("%d", tbd0->quatre);
    printf("TriangleBD(0) doit renvoyer 'o': "); Affichage(tbd0);
    printf("TriangleBD(1) doit renvoyer 'oooX*': "); Affichage(tbd1);
    printf("TriangleBD(3) doit renvoyer 'oooooX*oooX*X*ooooX*oooX*X*X*': "); Affichage(tbd3);
    printf("\n+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n\n");


    // Tests de MemeDessin
    printf("Tests de MemeDessin():\n");
    printf("Entrez 'Xooooo*ooo*oooo*o*XoooXXXX***!' et 'XXXXXXXXXX***oooo*Xooooo*oX**!' pour créer "
           "'memedessin1' et 'memedessin2'.\n\n");
    image memedessin1 = Lecture(); image memedessin2 = Lecture();
    printf("-MemeDessin(memedessin1, memedessin2) doit renvoyer 1: %d\n",
           MemeDessin(memedessin1, memedessin2));
    printf("-MemeDessin(memedessin2, memedessin1) doit renvoyer 1: %d\n",
           MemeDessin(memedessin2, memedessin1));
    printf("-MemeDessin(noire, blanche) doit renvoyer O: %d\n",
           MemeDessin(noire, blanche));
    printf("-MemeDessin(aire, lecture) doit renvoyer O: %d\n",
           MemeDessin(aire, lecture));
    printf("\n+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n\n");


    // Tests de InterUnion
    printf("Tests de InterUnion() (Voir problème ligne 308):\n");
    printf("Entrez 'oXoXXo*oXoX*oXX**!' et 'XoXoXooX*ooXX*X**!' "
           "pour créer 'interunion1' et 'interunion2'.\n\n");
    image interunion1 = Lecture(); image interunion2 = Lecture();
    InterUnion(&interunion1, &interunion2);
    printf("Après InterUnion(interunion1, interunion2):\n");
    printf(" -interunion1 donne 'oooXXo*ooooXX*X**': "); Affichage(interunion1);
    printf(" -interunion2 donne 'XXXoXoX*XooX*XX**': "); Affichage(interunion2);
    printf("\n+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n\n");


    // Tests de CompteDamiers
    printf("Tests de CompteDamiers() (Voir problème ligne 392):\n");
    printf("Entrez 'oXXo*oXXo*oXXo*oXXo**oXXo*oXXo*oXXo*oXXo**"
           "oXXo*oXXo*oXXo**oXXo*oXXo*XX*oooo*XXoooooo*o***!' "
           "pour créer 'damier'. Doit renvoyer 15.\n\n");
    image damier = Lecture();
    printf("-CompteDamiers(damier): %d\n", CompteDamiers(damier));
    printf("\n+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n\n");


    // Tests de Arrondit (placés à la fin car la fonction modifie les arbres)
    printf("Tests de Arrondit():\n");
    printf("Entrez 'XXXooX*oXXo*oooX**oXo*oXXoooXXX**oX**!' pour créer 'arrondit'.\n\n");
    image arrondit = Lecture();
    printf("-Arrondit(arrondit, 2) doit renvoyer 'XXoXo*oXooX**': ");
    Arrondit(&arrondit, 2); Affichage(arrondit);
    printf("-Arrondit(lecture, 0) doit renvoyer 'o': ");
    Arrondit(&lecture, 0); Affichage(lecture);
    printf("-Arrondit(aire, 0) doit renvoyer 'X': ");
    Arrondit(&aire, 0); Affichage(aire);
    printf("-Arrondit(blanche, 1) doit renvoyer 'oooo*': ");
    Arrondit(&blanche, 1); Affichage(blanche);
    printf("-Arrondit(noire, 1) doit renvoyer 'XXXX*': ");
    Arrondit(&noire, 1); Affichage(noire);
    printf("\n+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n\n");


    return 0;
}