#ifndef MYCLASS_H
#define MYCLASS_H

#include <armadillo>

arma::mat rho_ultimate(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);


class Basis {
    public:

        /**
         * @brief Constructeur pour la classe Basis
         * @param br
         * @param bz
         * @param N
         * @param Q
         */
        Basis(double br, double bz, int N, double Q); 

        /**
         * @brief Basis function for the radius part
         * @param r radius coordinate
         * @param m quantum number
         * @param n quantum number
         * @return arma::vec 
         */
        arma::vec rPart(arma::vec r, int m, int n);

        /**
         * @brief Basis function for the Z axis part
         * @param z Z axis coordinate
         * @param n_z quantum number
         * @return arma::vec 
         */
        arma::vec zPart(arma::vec z, int n_z);
};

#endif
