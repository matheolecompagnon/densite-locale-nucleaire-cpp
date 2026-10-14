/**
 * @file main.cpp
 * @author Mathéo GLESSMER
 * @brief Ce fichier décrit la troncvation de base
 * @version 0.1
 * @date 2026-09-09
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef __SIMULATION_HPP__
#define __SIMULATION_HPP__

#include <armadillo>

arma::mat rho_naif(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);

#endif