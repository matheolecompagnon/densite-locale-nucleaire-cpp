/**
 * @file basis.cpp
 * @author Mathéo LECOMPAGNON
 * @brief Ce fichier décrit l'implémentation de la classe Basis.
 * @version 0.1
 * @date 2026-09-09
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#include "basis.hpp"


Basis::Basis(double _br, double _bz, int _N, double _Q) : br(_br), bz(_bz), N(_N), Q(_Q) {
    /**
     * @brief Calculer mmax, nMax et n_zMax en fonction de N et Q (Troncation)
     * 
     */
    //Appel à la fonctions de troncature pour calculer mMax
    Basis::mMax = m_max( _N, _Q);


    //Une fois mMax calculé, on peut calculer nMax et n_zMax
    //On initialise nMax avec de taille mMax et n_zMax a une matrice de taille mMax
    Basis::nMax.zeros(Basis::mMax) ;

    //Calculer nMax pour chaque m
    for(int m = 0; m < Basis::mMax ; ++m) {
        Basis::nMax(m) = n_max(Basis::mMax , m);
    }

    //initialiser n_zMax avec une matrice de taille mMax x nMax.max()
    Basis::n_zMax.zeros(Basis::mMax, Basis::nMax.max());

    //Calculer n_zMax pour chaque combinaison de m et n
    for(int m = 0; m < Basis::mMax ; ++m) {
        for(int n = 0; n <= Basis::nMax(m) ; ++n) {
            Basis::n_zMax(m, n) = nz_max(m, n, _N, _Q);
        }
    }

}

int factoriel(int n) {
    /**
     * @brief Fonction pour calculer la factorielle d'un entier n
     * 
     */
    if (n <= 1) return 1;
    return n * factoriel(n - 1);
}

/**
 * @brief Cette fonction calcule la partie radiale de la fonction de base 
 * pour un vecteur r donné, un nombre quantique m et un nombre quantique n.
 * 
 * @param r 
 * @param m 
 * @param n 
 * @return arma::vec 
 */

arma::vec Basis::rPart(arma::vec r, int m, int n) {
    /**
     * @brief Implémentation de la fonction de base pour la partie radiale
     * 
     */
    int abs_m = std::abs(m);

    //Calculer le facteur de normalisation
    double facteur = (1.0 / (br * std::sqrt(M_PI))) * std::sqrt(factoriel(n) / factoriel(n + abs_m));

    //Calculer les puissances et l'exponentielle
    arma::vec puissance = arma::square(r) / (br * br);

    //Calculer l'exponentielle
    arma::vec exponentielle = arma::exp(-puissance / 2.0);

    
    arma::vec fact_vec = arma::pow(r / br, abs_m);

    //Calculer les polynômes de Laguerre
    Poly::calcLaguerre(abs_m + 1, n + 1, puissance);
    arma::vec laguerre = Poly::laguerre(abs_m, n);

    //Combiner tous les éléments pour obtenir la fonction de base radiale
    return facteur * fact_vec % laguerre % exponentielle;
}

/**4
 * @brief Cette fonction calcule la partie z de la fonction de base
 * pour un vecteur z donné et un nombre quantique n.
 * 
 * @param z
 * @param n
 * @return arma::vec
 */
arma::vec Basis::zPart(arma::vec z, int n_z) {
    /**
     * @brief Implémentation de la fonction de base pour la partie z
     * 
     */
    //Calculer le facteur de normalisation
    double facteur = 1.0 / (std::sqrt(bz) * sqrt(std::pow(2.0, n_z) * factoriel(n_z) * std::sqrt(M_PI)));

    //Calculer les puissances et l'exponentielle
    arma::vec puissance = arma::square(z) / (bz * bz);

    //Calculer l'exponentielle
    arma::vec exponentielle = arma::exp(-puissance / 2.0);

    //Calculer les polynômes d'Hermite
    Poly::calcHermite(n_z + 1, puissance);
    arma::vec hermite = Poly::hermite(n_z);

    //Combiner tous les éléments pour obtenir la fonction de base z
    return facteur * hermite % exponentielle;
}


arma::mat Basis::basisFunc(int m , int n , int n_z , arma::vec z, arma::vec r) {
    //Calcul de r_part et z_part 
    arma::vec r_part = rPart(r, m, n);
    arma::vec z_part = zPart(z, n_z);

    // Produit tensoriel pour obtenir la matrice finale
    return z_part * r_part.t(); 
}