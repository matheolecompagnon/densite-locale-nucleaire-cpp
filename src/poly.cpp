/**
 * @file poly.cpp
 * @author Mathéo LECOMPAGNON
 * @brief Ce fichier décrit l'implémentation de la classe Poly.
 * @version 0.1
 * @date 2026-09-09
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#include "poly.hpp"

/*
    Le résultat coefficients est de la forme :
                        polynôme 0   polynôme 1  ...  polynôme q
        point z_0     [ P_0(z_0)     P_1(z_0)    ...  P_q(z_0) ]
        point z_1     [ P_0(z_1)     P_1(z_1)    ...  P_q(z_1) ]
        ...
*/

void Poly::calcHermite(int n_, const arma::vec& vect)
{
    n = n_;
    m = 0;

    // Obligatoire pour notre algorithme
    coefficients.zeros(vect.n_elem, n_);
    
    // H(0) = 1
    // H(1) = 2X
    // H(i) = 2 * X * H(i-1) - 2 * (i-1) * H(i-2)
    
    for (int i=0; i<n_; i++)
    {
        if (i==0){
            coefficients.col(0).ones();
       }
       else if (i==1)
       {
            coefficients.col(1) = 2 * vect;
       }
       else {
            coefficients.col(i) = (2 * vect) % coefficients.col(i-1) - (2 * (i-1)) * coefficients.col(i-2);
       }
    }
}

/*
    Le résultat coefficients est de la forme :
                        P^0_0        P^1_0       ...  P^m_0        P^0_1       P^1_1       ...  P^m_1      ...
        point z_0     [ P^0_0(z_0)   P^1_0(z_0)  ...  P^m_0(z_0)   P^1_0(z_0)  P^1_1(z_0)  ...  P^m_1(z_0) ...]
        point z_1     [ P^0_0(z_1)   P^1_0(z_1)  ...  P^m_0(z_1)   P^1_0(z_1)  P^1_1(z_1)  ...  P^m_1(z_1) ...]
        ...
*/
void Poly::calcLaguerre(int m_, int n_, const arma::vec& vect)
{
    n = n_;
    m = m_;
    
    // Obligatoire pour notre algorithme
    coefficients.zeros(vect.n_elem, m_ * n_);

    // L_0^(j) = 1
    // L_1^(j)(X) = 1 + i - X
    // j L_i^(j) = (2i - 1 + j - X) L_{i-1}^(j) - (i - 1 + j) L_{i-2}^(j)

    for (int i=0; i<n_; i++)
    {
        for (int j=0; j<m_; j++)
        {
            if (i==0)
            {
                coefficients.col(m_ * i + j).ones();
                
            }
            else if (i==1)
            {
                coefficients.col(m_ * i + j) = 1 + j - vect;
            }
            else 
            {
                coefficients.col(m_ * i + j) += (2*i - 1 + j - vect) 
                % coefficients.col(m_ * (i-1) + j);

                coefficients.col(m_ * i + j) -= (i + j - 1) 
                * coefficients.col(m_ * (i-2) + j);

                coefficients.col(m_ * i + j) /= (double)(i);
            }
        }
    }

    
}

arma::vec Poly::laguerre(int m_, int n_)
{
    return evaluated_value.col(m_ * n + n_);
}
arma::vec Poly::hermite(int n_)
{
    return evaluated_value.col(n_);
}