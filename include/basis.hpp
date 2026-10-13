/**
 * @file basis.hpp
 * @author Mathéo LECOMPAGNON
 * @author Jean-Marie ASSOGBA
 * @brief Ce fichier décrit l'entête de la classe Basis.
 * @version 0.1
 * @date 2026-09-09
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef __BASIS_HPP__
#define __BASIS_HPP__

#include <armadillo>
#include "poly.hpp"
#include <complex>

class Basis {
    public:

        /**
         * @brief Conctructeur pur la classe basis
         * @param br
         * @param bz
         * @param N
         * @param Q
         */
        Basis(double br, double bz, int N, double Q); 

        /**
         * @brief Maximum value matrix for n_z quantum number
         * 
         */
        arma::imat n_zMax;
        /**
         * @brief Maximum value vector for n quantum number
         * 
         */
        arma::ivec nMax;
        /**
         * @brief Maximum value for m quantum number
         * 
         */
        int mMax;

        /**
         * @brief Basis function for the radius part
         * 
         * @param r radius coordinate
         * @param m quantum number
         * @param n_z quantum number
         * @return arma::vec 
         */
        arma::vec rPart(arma::vec r, int m, int n);
        /**
         * @brief Basis function for the Z axis part
         * 
         * @param z Z axis coordinate
         * @param n_z quantum number
         * @return arma::vec 
         */
        arma::vec zPart(arma::vec z, int n_z);
        /**
         * @brief Maximum value n for fixed m quantum number
         * 
         * @param m quantum number
         * @return int 
         */
        int nMax(int m);
        /**
         * @brief Maximum value n_z for fixed m and n quantum numbers
         * 
         * @param m quantum number
         * @param n quantum number
         * @return int 
         */
        int n_zMax(int m, int n);

        /**
         * @brief 
         * 
         * @param m quantum number
         * @param n quantum number
         * @param n_z quantum number
         * @param z Z axis coordinate
         * @param r radius coordinate
         * @return arma::mat 
         */
        arma::mat basisFunc(int m, int n, int n_z, arma::vec z, arma::vec r);

        /**
         * @brief basis deformation parameter br
         * 
         */
        double br;
        /**
         * @brief basis deformation parameter bz
         * 
         */
        double bz;
        /**
         * @brief basis truncation parameter N
         * 
         */
        int N; 
        /**
         * @brief basis truncation parameter Q
         * 
         */
        double Q;
};

#endif