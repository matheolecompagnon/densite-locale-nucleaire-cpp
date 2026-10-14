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
     * @brief Calculer mMax, nMax et n_zMax en fonction de N et Q (Troncation)
     */
    mMax = 0;
    double q_pow = std::pow(Q, 2.0 / 3.0);
    double nu = ((double)(N + 2)) * q_pow + 1.0 / 2.0;
    
    // 1. Calcul de mMax
    while(nu >= 1.0) {
        mMax++;
        nu -= Q; // Correction de l'affectation
    }

    // 2. Initialisation du vecteur nMax
    nMax.set_size(mMax);
    for(int m = 0; m < mMax; ++m) {
        nMax(m) = (mMax - m - 1) / 2 + 1;
    }

    // 3. Initialisation directe de la matrice n_zMax (arma::imat)
    int max_n = arma::max(nMax);
    n_zMax.set_size(mMax, max_n);
    n_zMax.fill(0); // Met des zéros dans les cellules inutilisées de la matrice

    for(int m = 0; m < mMax; ++m) {
        for(int n = 0; n < nMax(m); ++n) {
            int i = m + 2 * n + 1;
            // Calcul de nu(i) = (N + 2) * Q^(2/3) + 1/2 - i * Q
            double nu_i = ((double)(N + 2)) * q_pow + 0.5 - ((double)i) * Q;
            
            // Stockage de la valeur entière de nu(i) dans la matrice n_zMax
            n_zMax(m, n) = static_cast<int>(nu_i);
        }
    }
}

static double factoriel(int n) {
    /**
     * @brief Fonction pour calculer la factorielle d'un entier n
     * 
     */
    double f = 1.0;
    for (int k = 2; k <= n; ++k) f *= k;
    return f;
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
    Poly poly;

    //Calculer le facteur de normalisation
    double facteur = (1.0 / (br * std::sqrt(M_PI))) * std::sqrt((double)factoriel(n) / (double)factoriel(n + abs_m));

    //Calculer les puissances et l'exponentielle
    arma::vec puissance = arma::square(r) / (br * br);

    //Calculer l'exponentielle
    arma::vec exponentielle = arma::exp(-puissance / 2.0);

    
    arma::vec fact_vec = arma::pow(r / br, abs_m);

    //Calculer les polynômes de Laguerre
    poly.calcLaguerre(abs_m + 1, n + 1, puissance);
    arma::vec laguerre = poly.laguerre(abs_m, n);

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

    double facteur = 1.0 / (
        std::sqrt(bz * std::pow(2.0, n_z) * (double)factoriel(n_z) * std::sqrt(M_PI)));
    Poly poly;

    //Calculer les puissances et l'exponentielle
    arma::vec base = z / bz;
    arma::vec puissance = arma::square(base);

    //Calculer l'exponentielle
    arma::vec exponentielle = arma::exp(-puissance / 2.0);

    //Calculer les polynômes d'Hermite
    poly.calcHermite(n_z + 1, z / bz);
    arma::vec hermite = poly.hermite(n_z);

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

