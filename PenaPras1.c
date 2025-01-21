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

typedef enum{false, true} bool;

/*************************************************/
/*                                               */
/*            factorielle                        */
/*                                               */
/*************************************************/

long fact(int n) { 
    if (n==0) return 1;
    else 
        return n * fact(n-1);
}

// itou avec un arg out => passage par adresse

void bisfact(int n, long * r) { 
    if (n==0)
        *r=1.0;
    else { bisfact(n-1,r);
        *r = *r *n;
    }
}

long fact2(int n) {
    long r;
    bisfact(n,&r);
    return r;
}

/*************************************************/
/*                                               */
/*            Calcul de e                        */
/*                                               */
/*************************************************/

// d'après google,
// e = 2,7182818284 5904523536 0287471352 6624977572 4709369995 
//       9574966967 6277240766 3035354759 4571382178 5251664274

// Il est proposé de faire 3 versions pour observer des phénomènes de précision.
// C'est le même code, seul le type change.

/*************************************************/

float Efloat() {
    float e = 1;  // On commence pour n = 1, or 1/0! = 1/1 = 1
    int n_fac = 1;
    int n = 1;
    float epsilon = 0.00000000000000000001;  // Taille d'un float
    float terme = 1./n_fac;

    while (terme > epsilon) {
        e += terme;
        n++;
        n_fac *= n;
        terme = 1./n_fac;
    }

    return e;
}

/*************************************************/

double Edouble() {
    double e = 1;  // On commence pour n = 1, or 1/0! = 1/1 = 1
    int n_fac = 1;
    int n = 1;
    double epsilon = 0.000000000000000000000000000001;  // Taille d'un double
    double terme = 1./n_fac;

    while (terme > epsilon) {
        e += terme;
        n++;
        n_fac *= n;
        terme = 1./n_fac;
    }

    return e;
}

/*************************************************/

long double Elongdouble() {
    long double e = 1;  // On commence pour n = 1, or 1/0! = 1/1 = 1
    int n_fac = 1;
    int n = 1;
    long double epsilon = 0.0000000000000000000000000000000000000001;  // Taille d'un long double
    long double terme = 1./n_fac;

    while (terme > epsilon) {
        e += terme;
        n++;
        n_fac *= n;
        terme = 1./n_fac;
    }

    return e;
}

/*************************************************/
/*                                               */
/*            Suite Y                            */
/*                                               */
/*************************************************/

void afficheYfloat(int n) {
    float y0 = Efloat() - 1;
    float terme = y0;
    printf("Terme n0 = %f\n", terme);

    for (int i = 1; i <= n; i++) {
        terme = i * terme - 1;
        printf("Terme n%d = %f\n", i, terme);
    }
}

/*************************************************/

void afficheYdouble(int n) {
    double y0 = Edouble() - 1;
    double terme = y0;
    printf("Terme n0 = %lf\n", terme);

    for (int i = 1; i <= n; i++) {
        terme = i * terme - 1;
        printf("Terme n%d = %lf\n", i, terme);
    }
}

/*************************************************/

void afficheYlongdouble(int n) {
    long double y0 = Elongdouble() - 1;
    long double terme = y0;
    printf("Terme n0 = %Lf\n", terme);

    for (int i = 1; i <= n; i++) {
        terme = i * terme - 1;
        printf("Terme n%d = %Lf\n", i, terme);
    }
}


/*************************************************/
/*                                               */
/*            Puissance                          */
/*                                               */
/*************************************************/

bool cas_speciaux(double x, long n) {
    if (x == 0 && n < 0) {
        printf("Erreur : 0^n avec n négatif\n");
        return false;
    }
    if (x == 0 && n == 0) {
        printf("Erreur: 0^0 pas défini\n");
        return false;
    }
    if (n < 0) {
        printf("Erreur: n négatif pas pris en charge\n");
        return false;
    }
    return true;
}

double pow1(double x, long n) {
    if (n == 0)
        return 1;

    else {
        if (n < 0)
            return 1. / pow1(x, -n);
            // return power1(x, n+1) / x;
        else
            return pow1(x, n-1) * x;
    }
}

double power1(double x, long n) {
    if (x == 0 && n < 0) {
        printf("Erreur : 0^n avec n négatif\n");
        return -1;
    }
    if (x == 0 && n == 0) {
        printf("Erreur: 0^0 pas défini\n");
        return -1;
    }
    return pow1(x, n);
}

/*************************************************/

double power2a(double x, long n) {
    if (x == 0 && n < 0) {
        printf("Erreur : 0^n avec n négatif\n");
        return -1;
    }
    if (x == 0 && n == 0) {
        printf("Erreur: 0^0 pas défini\n");
        return -1;
    }

    double r = 1;

    if (n > 0) {
        for (int i = 1; i <= n; i++)
            r *= x;
    }

    if (n < 0) {
        for (int i = 1; i <= -n; i++)
            r /= x;
    }

    return r;
}

/*************************************************/

double power2b(double x, long n) {
    if (!(cas_speciaux(x, n)))
        return -1;

    double r = 1;

    while (n > 0) {
        r *= x;
        n--;
    }

    return r;
}

/*************************************************/

void pow3(double x, long n,double* r) {
    if (n != 0) {
        *r *= x;
        pow3(x, n-1, r);
    }
}

double power3(double x, long n) {
    if (!(cas_speciaux(x, n)))
        return -1;

    double r = 1;
    pow3(x, n, &r);
    return r;
}

/*************************************************/

double pow4(double x, long n, double r) {
    if (n == 0)
        return r;
    else
        return pow4(x, n-1, r*x);
}

double power4(double x, long n) {
    if (!(cas_speciaux(x, n)))
        return -1;

    return pow4(x, n, 1);
}

/*************************************************/

//   Observation (1+1/10^k)^(10^k) : rame : 8 en 1/2s, 9 en qqs s, 10 en 1m

double pow5(double x, long n) {
    if (n == 0)
        return 1;
    else  {
        if (n % 2 == 0)
            return pow5(x, n/2) * pow5(x, n/2);
        else
            return (pow5(x, n/2) * pow5(x, n/2)) *x;
    }
}

double power5(double x, long n) {  // Déjà codée (sauf tests des cas spéciaux)
    if (!(cas_speciaux(x, n)))
        return -1;
    return pow5(x, n);
}

/*************************************************/

double pow6(double x, long n) {
    if (n == 0)
        return 1;
    else {
        double y = pow6(x, n / 2);
        if (n % 2 == 0)
            return y*y;
        else
            return y*y*x;
    }
}

double power6(double x, long n) {
    if (!(cas_speciaux(x, n)))
        return -1;
    return pow6(x, n);
}

/*************************************************/

double pow7(double x, long n) {
    if (n == 0)
        return 1;
    else {
        if (n % 2 == 0)
            return pow7(x*x, n/2);
        else
            return pow7(x*x, n/2) * x;
    }
}

double power7(double x, long n) {
    if (!(cas_speciaux(x, n)))
        return -1;
    return pow7(x, n);
}

/*************************************************/

double pow8(double x, long n, double r) {
    if (n == 0)
        return r;
    else {
        if (n % 2 == 0)
            return pow8(x*x, n/2, r);
        else
            return pow8(x*x, n/2, r*x);
    }
}

double power8(double x, long n) {
    if (!(cas_speciaux(x, n)))
        return -1;
    return pow8(x, n, 1);
}

/*************************************************/

void pow9(double x, long n, double* r) {
    if (n != 0) {
        if (n % 2 != 0)
            *r *= x;
        pow9(x*x, n/2, r);
    }
}

double power9(double x, long n) {
    if (!(cas_speciaux(x, n)))
        return -1;

    double r = 1;
    pow9(x, n, &r);
    return r;
}

/*************************************************/

//   Observation (1+1/10^k)^(10^k) : calcul immédiat

double power10(double x, long n) {  // Déjà codée (sauf tests des cas spéciaux)
    if (!(cas_speciaux(x, n)))
        return -1;

    double r = 1.0;
    while (n ISNOT 0) {
        if (n % 2 == 1) then r = r*x; // no else
        n = n / 2;
        x = x * x;
    }
    return r;
}

/*************************************************/

double power(double x, long n, int i) {
    switch (i) {
        case 1 : return power1(x,n); break;
        case 2 : return power2a(x,n); break;  // 2 pour 2a
        case 0 : return power2b(x,n); break;  // 0 pour 2b
        case 3 : return power3(x,n); break;
        case 4 : return power4(x,n); break;
        case 5 : return power5(x,n); break;
        case 6 : return power6(x,n); break;
        case 7 : return power7(x,n); break;
        case 8 : return power8(x,n); break;
        case 9 : return power9(x,n); break;
        case 10 : return power10(x,n); break;
        default : return 0;
    }
}

// remarque : les break sont ici inutiles car les returns font déjà des break

/*************************************************/

// memes versions que la 10 mais avec des long double, puis des floats
// but du jeu : observer des variations de precision

// observation :
//   imprécision à partir de k~5 (float), k~12 (double), k~14 (long double)
//   rend 1 à partir de k=8 (float), k=16 (double)
//   rend 1 non observé sur les long double, maxint atteint avant

long double power11(long double x, long n) {  // Déjà codée (sauf tests des cas spéciaux)
    if (!(cas_speciaux(x, n)))
        return -1;

    long double r = 1.0 ;
    while (n ISNOT 0) {
        if (n % 2 == 1) then r = r*x; // no else
        n = n / 2;
        x = x * x;
    }
    return r;
}

/*************************************************/

float power12(float x, long n) {  // Déjà codée (sauf tests des cas spéciaux)
    if (!(cas_speciaux(x, n)))
        return -1;

    float r = 1.0;
    while (n ISNOT 0) {
        if (n % 2 == 1) then r = r*x; // no else
        n = n / 2;
        x = x * x;
    }
    return r;
}


/*************************************************/
/*                                               */
/*             Ackermann                         */
/*                                               */
/*************************************************/


int A1(int m, int n) {
    if (m == 0)
        return n+1;
    else {
        int tmp = m-1;
        if (n == 0)
            return A1(tmp, 1);
        else {
            n--;
            return A1(tmp, A1(m, n));
        }
    }
}

int Ackermann1(int m) {
    return A1(m, 0);
}

/*************************************************/

int A2(int m, int n) {
    int tmp;
    if (m == 0)
        return n+1;
    else {
        int r = 1;
        for (int i = 1; i <= n+1; i++) {
            tmp = m - 1;
            r = A2(tmp, r);
        }
        return r;
    }
}

int Ackermann2(int m) {
    return A2(m, 0);
}

/*************************************************/

int A3(int m, int n) {
    while (m > 0) {
        int tmp = m-1;
        if (n == 0)
            return A3(tmp, 1);
        else {
            n--;
            return A3(tmp, A3(m, n));
        }
    }
    return n+1;
}

int Ackermann3(int m) {
    return A3(m, 0);
}

/*************************************************/

int* agrandiPile(int *pile, int* tailleMax) {
    *tailleMax *= 2;
    int* tmp = (int*) realloc (pile, *tailleMax * sizeof(int));
    return tmp;
}

int Ackermann4 (int m) {
    // On définit un tableau de taille 16 à la base, et on l'agrandit au besoin en le doublant de taille
    // 16 permet de calculer jusqu'à Ackermann(4, 0) sans avoir à agrandir la pile
    int* pile = (int*) malloc (16 * sizeof(int));
    int tailleMax = 16;  // Garde en mémoire la taille max de la pile
    int taillePile = 0;  // Garde en mémoire la taille actuelle de la pile
    int n = 0; bool flag = true;
    
    while (true) {
        if (m == 0) {
            if (taillePile == 0)
                return n + 1;  // On a fini tous les appels
            else {
                m = pile[taillePile-1];  // On dépile le dernier élément
                taillePile--;
                n++;
            }
        }
        
        else {
            if (n == 0) {
                m--;
                n = 1;
            } else {
                pile[taillePile] = m-1;  // On empile un élément
                taillePile++;
                n--;
                
                if (taillePile == tailleMax)  // On agrandi la pile si besoin
                    pile = agrandiPile(pile, &tailleMax);
            }
        }
    }
}

/*************************************************/

int Ackermann5 (int m) {  // Ackermann 5 pas codée
    return -1;
}

/*************************************************/

int Ackermann (int m, int i) {
    switch (i) {
        case 1 : return Ackermann1 (m);  // break;
        case 2 : return Ackermann2 (m);  // break;
        case 3 : return Ackermann3 (m);  // break;
        case 4 : return Ackermann4 (m);  // break;
        case 5 : return Ackermann5 (m);  // break;
        default : return 0;
    }
}


/*************************************************/
/*                                               */
/*             X(n)                              */
/*                                               */
/*************************************************/

int ln2_it(int n) {  // Version itérative de ln2 
    int cpt = 0;
    while (n > 1) {
        cpt++; 
        n = n/2;
    }
    return cpt;
}

int ln2_rec(int n) {
    if (n == 1)
        return 0;
    else
        return 1 + ln2_rec(n/2);
}

/*************************************************/

int X_it(int n) {
    int elt = 2;
    for (int i = 1; i <= n; i++)
        elt = elt + ln2_it(elt);
    return elt;
}

/*************************************************/

int X_recS(int n) {
    if (n == 0)
        return 2;
    else {
        n--;
        int tmp = X_recS(n);
        return tmp + ln2_rec(tmp);
    }
}

/*************************************************/

int bisSF(int n, int acc) {
    if (n == 0)
        return acc;
    else {
        n--;
        return bisSF(n, acc + ln2_rec(acc));
    }
}

int X_recT_SF(int n) {
    return bisSF(n, 2);
}

/*************************************************/

void bisSP(int n, int* acc) {
    if (n == 0)
        ;
    else {
        n--;
        *acc = *acc + ln2_rec(*acc);
        bisSP(n, acc);
    }
}

int X_recT_SP(int n) {
    int acc = 2;
    bisSP(n, &acc);
    return acc;
}

/*************************************************/

int X(int n, int i) {
    switch(i)  {
        case 1 : return X_it(n);
        case 2 : return X_recS(n);
        case 3 : return X_recT_SF(n);
        case 4 : return X_recT_SP(n);
        default : return 0 ;
    }
}

/*************************************************/
/*                                               */
/*               main                            */
/*                                               */
/*************************************************/

int main(int argc, char** argv) {
    double x;
    long double y;
    float z;

    int cptx;

    long nx;

    int i, j;
    
    printf("\n");

/******************************************************************************/

    // mettre "if true" vs "if false" selon que vous voulez les tests ou non

/****************  petit test sur le fonctionnement du switch  ************/

    if (false) {

        switch (2) {
            case 1 : printf("toc\n") ;  break ;
            case 2 : printf("pata") ;
            case 3 : printf("pouf\n") ;  break ;
            case 4 : printf("youpla") ;
            default : printf("boum\n") ;
        }


        switch (4) {
            case 1 : printf("toc\n");  break;
            case 2 : printf("pata");
            case 3 : printf("pouf\n");  break;
            case 4 : printf("youpla");
            default : printf("boum\n");
        }

        printf("\n") ;
    }


/************************  taille des nombres  *************************/

    if (false) {

        printf("ce programme suppose que les long font 8 octets\n") ;
        printf("S'ils n'en font que 4, il faudra utiliser long long\n") ;

        printf("short : %d octets\n", (int) sizeof(short));
        printf("int : %d octets\n", (int) sizeof(int));
        printf("long : %d octets\n", (int) sizeof(long));
        printf("long long : %d octets\n", (int) sizeof(long long));
        printf("float : %d octets\n", (int) sizeof(float));
        printf("double : %d octets\n", (int) sizeof(double));
        printf("long double : %d octets\n", (int) sizeof(long double));
        printf("\n") ;

        x = 1.0 ;
        cptx = 0 ;
        while ( (1.0 + x) - 1.0 != 0 )
        { x = x/10 ; cptx ++ ; }
        printf("1+1/10^c devient 1 a partir de c=%d pour les double\n", cptx) ;
        printf("et 1+1/10^%d vaut en fait 1+%lf\n", cptx-1, (1.0 + 10*x) - 1.0) ;

        printf("ce programme suppose que les doubles font au moins 8 octets\n") ;
        printf("et que ((double) 1+1/10^-15) n'est pas 1 \n") ;
        printf("Si ce n'est pas le cas, utiliser des long double \n") ;

        printf("\n") ;
    }

/************************  factorielle  *************************/

    if (false) {

        printf("%ld \n",fact(5)) ;
        printf("%ld \n",fact2(5)) ;
        printf("\n") ;
    }


/******************    Autour de e      *******************************/

    // d'après google,
    // e = 2,7182818284 5904523536 0287471352 6624977572 4709369995 
    //       9574966967 6277240766 3035354759 4571382178 5251664274

    if (true) {
        printf(" e1 = %.20f \n", Efloat());
        printf(" e3 = %.30lf \n", Edouble());
        printf(" e3 = %.40Lf \n", Elongdouble());
        printf("\n");
    }


    if (true) {
        afficheYfloat(30);
        printf("\n");
        afficheYdouble(30);
        printf("\n");
        afficheYlongdouble(30);
        printf("\n");
    }

/******************    power     *******************************/

    if (true) {

        //   test simplet, vérifie le B.A. BA, test de 2^10 pour toutes les versions

        for(i=0 ; i<=10 ; i++)
            printf("Power %d dit que power(2,10) vaut %.0lf \n", i, power(2,10, i));
        printf("\n");
        printf("Power 1 dit que power(2, -2) vaut %.2lf \n", power1(2, -2));
        printf("Power 2a dit que power(2, -2) vaut %.2lf \n", power2a(2, -2));
        printf("\n");
    }


    if (false) {

        i=9;  // numéro de la version que l'on teste
        // test de la résistance de power version i, calcul de (1+1/N)^N
        // pour des N puissances de 10 de plus en plus grosses

        x = 1.0;
        nx = 1;
        for(j=0 ; j<=15 ; j++) {
            printf("power %d ((1+10^-%2d)^(10^%2d)) rend %.12lf\n", i, j, j, power(1+x,nx, i));
            x = x/10;
            nx = nx * 10;
        }
        printf("\n");
    }

    if (false) {

        // tests sur la précision que l'on obtient suivant que
        // l'on utilise float, double, long double

        printf("VERSION 12 avec float \n");
        z = 1.0;
        nx = 1;
        for(j=0 ; j<=18 ; j++) {
            printf("power((1+10^-%2d)^(10^%2d)) rend %.12f\n", j, j, power12(1+z, nx));
            z = z/10;
            nx = nx * 10;
        }
        printf("\n");

        printf("VERSION 10 avec double \n");
        x = 1.0;
        nx = 1;
        for(j=0 ; j<=18 ; j++) {
            printf("power((1+10^-%2d)^(10^%2d)) rend %.12lf\n", j, j, power10(1+x, nx));
            x = x/10;
            nx = nx * 10;
        }
        printf("\n");

        printf("VERSION 11 avec long double \n");
        y = 1.0;
        nx = 1;
        for(j=0 ; j<=18 ; j++) {
            printf("power((1+10^-%2d)^(10^%2d)) rend %.12LF\n", j, j, power11(1+y, nx));
            y = y/10;
            nx = nx * 10;
        }
        printf("\n");
    }

/******************    Ackermann    *******************************/

    if (true) {
        for(i=1 ; i<=5 ; i++) {  // numéro de version
            if (i == 5)
                printf("Ackermann 5 pas codée:\n");
            for(j=0 ; j<=5 ; j++)  // test de A(j,0) pour j de 0 à 5
                printf("Ack%d(%d) = %d \n", i, j, Ackermann(j, i));
            printf("\n");
        }
    }

/******************    X(n)    *******************************/

    if (true) {
        for(i=1 ; i<=4 ; i++) {  // numéro de version
            for(j=0 ; j<=8 ; j++)  // test de A(j,0) pour j de 0 à 5
                printf("X%d(%d) = %d\n", i, j, X(j, i));
            printf("...\nX%d(%d) = %d\n\n", i, 100, X(100, i));
        }
    }

/***********************************************************************/
    
    return 0;
}
