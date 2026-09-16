/**
 * @file Poly.hpp
 * @author Mathéo LECOMPAGNON
 * @author Jean-Marie ASSOGBA
 * @brief Ce fichier décrit l'entête de la classe Poly.
 * @version 0.1
 * @date 2026-09-09
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include <armadillo>
/**
 * @brief Cette classe définit la structure d'un polynome
 * ainsi que les fonctions necessaires pour utiliser des 
 * polynomes d'Hermite ou de laguerre
 */

 class Poly {
    private:
        arma::mat matrice;
    public:
        Poly();

        /**
         * @brief Calculer les polynomes d'Hermite à partir de
         * d'un entier @param n et d'un vecteur @param vect et 
         * remplir la matrice du polynome
         * 
         * @param n 
         * @param vect
         */
        void calcHermite(int n,arma::vec vect);

        /**
         * @brief  aucune idée pour le moment
         * 
         * @param n 
         * @return arma::vec 
         */
        arma::vec hermite(int n);

        // Calculer les polynômes généralisés de Laguerre pour i
        // entre 0 et (n-1)  et j entre 0 et (m-1)
        
        /**
         * @brief Calculer les polynômes généralisés de Laguerre à
         * partir de deux entiers @param n et @param m et d'un vecteur
         * @param vect , et remplir la matrice du polynome
         *
         * @param n 
         * @param m 
         * @param vect 
         */
        void calcLaguerre(int n,int m,arma::vec vect);

        /**
         * @brief aucune idée pour le moment
         * 
         * @param n 
         * @param m
         * @return arma::vec 
         */
        arma::vec laguerre(int n, int m);

        //~Poly();

 }