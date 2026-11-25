#ifndef MYCLASS_H
#define MYCLASS_H

#include <armadillo>
#include "basis.hpp"
#include "simulation.hpp"

void arma_load_mat(arma::mat &mat, char* filename) {
    mat.load(filename, arma::arma_ascii);
}

#endif
