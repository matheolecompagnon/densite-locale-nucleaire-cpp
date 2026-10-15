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

arma::mat rho_naif(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);
arma::mat rho_kronecker(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);
arma::mat rho_factorized(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);
arma::mat rho_linearized(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);
arma::mat rho_linearized_again(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);
arma::mat rho_optimized_m(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);
arma::mat rho_symmetric(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);
arma::mat rho_symmetric_optimized(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);
arma::mat rho_memory_max(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);
arma::mat rho_fabulous(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);
arma::mat rho_ultimate(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);

#endif