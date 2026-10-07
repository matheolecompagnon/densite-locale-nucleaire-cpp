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
#include <armadillo>
#include <cmath>
#include "truncation.hpp"


int m_max(int N, double Q){
    int m_max = 0;
    double q_pow = pow(q, 2.0/3.0);
    double nu = ((double)(N + 2))*(q_pow) + 1.0/2.0
    while(nu < 1) {
        m_max++;
        nu - Q;
    }

    return m_max;
}

double n_max(int m_max, int m) {return 1.0/2.0*(double)(m_max - m + 1)}

double nz_max(int m, int n, int N, double Q) {
    int i = m +2*n + 1;
    double q_pow = pow(Q,2.0/3.0);
    return q_pow - ((double)i) * Q;
}