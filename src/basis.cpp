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
 * pour un vecteur r donné, un nombre quantique m et un nombre quantique n_z.
 * 
 * @param r 
 * @param m 
 * @param n_z 
 * @return arma::vec 
 */

arma::vec Basis::rPart(arma::vec r, int m, int n_z) {
    /**
     * @brief Implémentation de la fonction de base pour la partie radiale
     * 
     */
    int abs_m = std::abs(m);

    //Calculer le facteur de normalisation
    double facteur = (1.0 / (br * std::sqrt(M_PI))) * std::sqrt(factoriel(n_z) / factoriel(n_z + abs_m));

    //Calculer les puissances et l'exponentielle
    arma::vec puissance = arma::square(r) / (br * br);

    //Calculer l'exponentielle
    arma::vec exponentielle = arma::exp(-puissance / 2.0);

    
    arma::vec fact_mat = arma::pow(r / br, abs_m);

    //Calculer les polynômes de Laguerre
    poly::calcLaguerre(abs_m + 1, n_z + 1, puissance);
    arma::vec laguerre = poly::laguerre(abs_m, n_z);

    //Combiner tous les éléments pour obtenir la fonction de base radiale
    return facteur * fact_mat % laguerre % exponentielle;
}

/**4
 * @brief Cette fonction calcule la partie z de la fonction de base
 * pour un vecteur z donné et un nombre quantique n.
 * 
 * @param z
 * @param n
 * @return arma::vec
 */
arma::vec Basis::zPart(arma::vec z, int n) {
    /**
     * @brief Implémentation de la fonction de base pour la partie z
     * 
     */
    //Calculer le facteur de normalisation
    double facteur = 1.0 / (std::sqrt(bz) * sqrt(std::pow(2.0, n) * factoriel(n) * std::sqrt(M_PI)));

    //Calculer les puissances et l'exponentielle
    arma::vec puissance = arma::square(z) / (bz * bz);

    //Calculer l'exponentielle
    arma::vec exponentielle = arma::exp(-puissance / 2.0);

    //Calculer les polynômes d'Hermite
    poly::calcHermite(n + 1, puissance);
    arma::vec hermite = poly::hermite(n);

    //Combiner tous les éléments pour obtenir la fonction de base z
    return facteur * hermite % exponentielle;
}

