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
         * @brief Calculer les valeurs des polynomes d'Hermite à partir de
         * d'un entier n et d'un vecteur vect et remplir la matrice du polynome
         * 
         * @param n  limite supérieure pour les dégrés (exclusive)
         * @param vect  Le vecteur qui contient les valeurs (les points d'abcisse z)
         * sur lesquels les polynômes doivent etre évalués
         */
        void calcHermite(int n,arma::vec vect);

        /**
         * @brief  Récupère les résultats calculés lors de l'appel 
         * précédent à calcHermite
         * 
         * @param n   Le degré \b n du polynôme souhaité
         * @return Un vecteur (arma::vec) contenant 
         * les valeurs du polynôme d'Hermite de degré \c n 
         */
        arma::vec hermite(int n);
        
        /**
         * @brief Calculer les valeurs des polynômes généralisés de Laguerre à
         * partir de deux entiers n et m et d'un vecteur vect , et remplir la 
         * matrice du polynome
         *
         * @param n limite supérieure pour les dégrés (exclusive)
         * @param m limite supérieure pour les paramètres (exclusive)
         * @param vect Le vecteur qui contient les valeurs (les points d'abcisse z)
         * sur lesquels les polynômes doivent etre évalués
         */
        void calcLaguerre(int n,int m,arma::vec vect);

        /**
         * @brief aucune idée pour le moment
         * 
         * @param n dégré spécifique pour le polynôme à obtenir
         * @param m paramètre spécifique pour le polynôme à obtenir
         * @return  Un vecteur (arma::vec) contenant 
         * les valeurs du polynôme de Laguerre de degré \b n  et de
         * paramètre \b m
         */
        arma::vec laguerre(int n, int m);

        

 }