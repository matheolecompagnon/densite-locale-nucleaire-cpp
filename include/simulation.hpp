/**
 * @file simulation.hpp
 * @author Mathéo LECOMPAGNON
 * @author Jean-Marie ASSOGBA
 * @brief Ce fichier décrit l'entête des fonctions de simulation.
 * @version 0.1
 * @date 2026-09-09
 * @copyright Copyright (c) 2026
 */

#ifndef __SIMULATION_HPP__
#define __SIMULATION_HPP__

#include <armadillo>

/**
 * @brief Calcul naïf de la densité (6 boucles imbriquées)
 * @param rVals Vecteur des coordonnées r
 * @param zVals Vecteur des coordonnées z
 * @param nbR Nombre de points sur r
 * @param nbZ Nombre de points sur z
 * @return arma::mat 
 */
arma::mat rho_naif(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);

/**
 * @brief Calcul optimisé exploitant le produit de Kronecker / symétrie m = mp
 * @param rVals Vecteur des coordonnées r
 * @param zVals Vecteur des coordonnées z
 * @param nbR Nombre de points sur r
 * @param nbZ Nombre de points sur z
 * @return arma::mat 
 */
arma::mat rho_kronecker(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);

/**
 * @brief Calcul factorisé de la densité
 * @param rVals Vecteur des coordonnées r
 * @param zVals Vecteur des coordonnées z
 * @param nbR Nombre de points sur r
 * @param nbZ Nombre de points sur z
 * @return arma::mat 
 */
arma::mat rho_factorized(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);

/**
 * @brief Calcul linéarisé avec double boucle (i et j) et table des triplets quantiques
 * @param rVals Vecteur des coordonnées r
 * @param zVals Vecteur des coordonnées z
 * @param nbR Nombre de points sur r
 * @param nbZ Nombre de points sur z
 * @return arma::mat 
 */
arma::mat rho_linearized(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);

/**
 * @brief Calcul linéarisé avec  boucle (i et j) linearisé en une seul boucle et table des triplets quantiques
 * @param rVals Vecteur des coordonnées r
 * @param zVals Vecteur des coordonnées z
 * @param nbR Nombre de points sur r
 * @param nbZ Nombre de points sur z
 * @return arma::mat 
 */
arma::mat rho_linearized_again(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);


/**
 * @brief Calcul linéarisé avec  precalcul des i et j compatible pour eviter les tour de boucle inutile
 * @param rVals Vecteur des coordonnées r
 * @param zVals Vecteur des coordonnées z
 * @param nbR Nombre de points sur r
 * @param nbZ Nombre de points sur z
 * @return arma::mat 
 */
arma::mat rho_optimized_m(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);

/**
 * @brief utilisation de la symetrie de la matrice
 * @param rVals Vecteur des coordonnées r
 * @param zVals Vecteur des coordonnées z
 * @param nbR Nombre de points sur r
 * @param nbZ Nombre de points sur z
 * @return arma::mat 
 */
arma::mat rho_symmetric(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);

/**
 * @brief utilisation de la symetrie de la matrice ainsi que minimisation des alocation mémoire
 * @param rVals Vecteur des coordonnées r
 * @param zVals Vecteur des coordonnées z
 * @param nbR Nombre de points sur r
 * @param nbZ Nombre de points sur z
 * @return arma::mat 
 */
arma::mat rho_symmetric_optimized(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);

/**
 * @brief sauvegarde maximal de data et allocation minimal
 * @param rVals Vecteur des coordonnées r
 * @param zVals Vecteur des coordonnées z
 * @param nbR Nombre de points sur r
 * @param nbZ Nombre de points sur z
 * @return arma::mat 
 */
arma::mat rho_memory_max(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);

arma::mat rho_fabulous(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);

#endif