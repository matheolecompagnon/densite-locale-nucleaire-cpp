/**
 * @file poly.hpp
 * @author Mathéo LECOMPAGNON
 * @author Jean-Marie ASSOGBA
 * @brief Ce fichier décrit l'entête de la classe Poly.
 * @version 0.1
 * @date 2026-09-09
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef __POLY_HPP__
#define __POLY_HPP__

#include <armadillo>
/**
 * @brief Cette classe définit la structure d'un polynome
 * ainsi que les fonctions necessaires pour utiliser des 
 * polynomes d'Hermite ou de laguerre
 */

 class Poly {


    public:
        /**
         * @brief coefficients est une matrice pour stocker les coefficients 
         * des polynomes calculés avec les fonctions calcHermite et calcLaguerre
         */
        arma::mat coefficients;
        /**
         * @brief evaluated_value est une matrice pour stocker les resultats de l'évaluation
         * des polynomes stocké dans coefficients en des valeurs qui sont passées en parametre
         * des fonctions calcHermite et calcLaguerre par un vecteur.
         */
        arma::mat evaluated_value;
        int n;
        int m;

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
         * précédent à calcHermite et retourne un vecteur calculé
         * 
         * @param n   Le degré \b n du polynôme souhaité
         * @return Un vecteur (arma::vec) calculé pour le dégré n
         * évalué en les points de zvals passés précédemment
         * à calcHermite
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
        void calcLaguerre(int m,int n,arma::vec vect);

        /**
         * @brief aucune idée pour le moment
         * 
         * @param n dégré spécifique pour le polynôme à obtenir
         * @param m paramètre spécifique pour le polynôme à obtenir
         * @return  Un vecteur (arma::vec) contenant 
         * les valeurs de l'évaluation du polynôme de Laguerre de degré \b n  et de
         * paramètre \b m
         */
        arma::vec laguerre(int m, int n);

        /**
         * @brief Calculer les valeurs de la dérivé des polynômes généralisés de Laguerre à
         * partir de deux entiers n et m et d'un vecteur vect , et remplir la 
         * matrice du polynome
         *
         * @param n limite supérieure pour les dégrés (exclusive)
         * @param m limite supérieure pour les paramètres (exclusive)
         * @param vect Le vecteur qui contient les valeurs (les points d'abcisse z)
         * sur lesquels les polynômes doivent etre évalués
         */
        void calcDerivatveLaguerre(int m,int n,arma::vec vect);

        /**
         * @brief aucune idée pour le moment
         * 
         * @param n dégré spécifique pour la dérivée du polynôme à obtenir
         * @param m paramètre spécifique pour le polynôme à obtenir
         * @return  Un vecteur (arma::vec) contenant 
         * les valeurs de l'évaluation du polynôme de Laguerre de degré \b n  et de
         * paramètre \b m
         */
        arma::vec derivativeLaguerre(int m, int n);

 }
#endif
