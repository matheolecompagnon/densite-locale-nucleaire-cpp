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


// Évalue par Horner chaque ligne de polynomial_coefficients (colonne p = coefficient de X^p)
// en tous les points de z. Résultat : une colonne par polynôme.
/*
        polynomial_coefficients est de la forme :
                     X^0    X^1    X^2   ...  X^(d)
        polynôme 0 [ a_0,0  a_0,1  a_0,2 ...  a_0,d ]
        polynôme 1 [ a_1,0  a_1,1  a_1,2 ...  a_1,d ]
        ...        [ ...                            ]
        polynôme q [ a_q,0  a_q,1  a_q,2 ...  a_q,d ]

        z est de la forme :
        [ z_0    z_1   ... ]

        Le résultat evaluated_points est de la forme :
                        polynôme 0   polynôme 1  ...  polynôme q
        point z_0     [ P_0(z_0)     P_1(z_0)    ...  P_q(z_0) ]
        point z_1     [ P_0(z_1)     P_1(z_1)    ...  P_q(z_1) ]
        ...

*/

arma::mat hornerEvaluation(const arma::mat& polynomial_coefficients, const arma::vec& z)
{
    int q = polynomial_coefficients.n_rows; // Nombre de polynômes
    int d = polynomial_coefficients.n_cols - 1; // Degré maximal des polynômes

    // Initialisation de la matrice des evaluations
    arma::mat evaluated_points(
        z.n_elem, // points évalués -> lignes
        q, // nombre de polynômes -> colonnes
        arma::fill::zeros
    );

    // Parcours des coefficients, de X^d vers X^0 (Méthode d'Horner), pour tous les polynômes à la fois
    for (int j = d; j >= 0; j--) {
        // P(x)=(...(( a(d)*x + a(d−1) )*x + a(d−2) )*x + ... + a1)x+a0
        evaluated_points.each_col() %= z; // multiplication terme à terme par z (inutile pour a(d))
        // Ajout du coefficient de degré j pour chaque polynôme
        evaluated_points.each_row() += polynomial_coefficients.col(j).t();
    }
    return evaluated_points;
}


void Poly::calcHermite(int n_, const arma::vec& z)
{
    n = n_;
    m = 0;

    
    coefficients.zeros(n_, n_);
    // coefficients(i,j) = coefficient du polynôme d'Hermite de degré i pour le terme X^j

    // H(0) = 1
    coefficients(0, 0) = 1.0;


    for (int i = 1; i < n_; i++) {
        
        // H(i) = 2 * X * H(i-1) - 2 * (i-1) * H(i-2)

        if (i > 0) {
            // coefficients.row(i-1).subvec(0, i-1) est la partie de H(i-1) correspondant aux termes X^0 à X^(i-1)
            // décalage de H(i-1) par 1 pour le multiplier par 2*X
            coefficients.row(i).subvec(1, i) += 2.0 * coefficients.row(i - 1).subvec(0, i - 1);
        }
        if (i > 1) {
            // H(i-2) est de degré i-2, il suffit donc d'opérer de 0 à i-2 pour les coefficients
            coefficients.row(i).subvec(0, i - 2) -= 2.0 * (i - 1) * coefficients.row(i - 2).subvec(0, i - 2);
        }
    }
    evaluated_value = hornerEvaluation(coefficients, z);
}

void Poly::calcLaguerre(int m_, int n_, const arma::vec& z)
{
    n = n_;
    m = m_;
    // Les polynômes L_(j)^(i) : n_ polynômes par paramètre i, m_ paramètres
    // ligne i*n_ + j  = L_j^(i)
    coefficients.zeros(m_ * n_, n_ + 1);

    // L_0^(i) = 1
    for (int i = 0; i < m_; i++) {
        coefficients(i * n_, 0) = 1.0;
    }

    // L_1^(i)(X) = 1 + i - X
    if (n_ > 1) {
        for (int i = 0; i < m_; i++) {
            coefficients(i * n_ + 1, 0) = 1.0 + i;
            coefficients(i * n_ + 1, 1) = -1.0;
        }
    }

    // j L_j^(i) = (2j - 1 + i - X) L_{j-1}^(i) - (j - 1 + i) L_{j-2}^(i)
    for (int i = 0; i < m_; i++) {
        for (int j = 2; j < n_; j++) {

            // Coefficients de degré 0 (pas de terme -X * L_{j-1})
            coefficients(i * n_ + j, 0) =
                ((2.0 * j - 1.0 + i) * coefficients(i * n_ + j - 1, 0)
                 - (j - 1.0 + i) * coefficients(i * n_ + j - 2, 0)) / j;

            // Coefficients de degré 1 à j
            for (int k = 1; k <= j; k++) {
                coefficients(i * n_ + j, k) =
                    ((2.0 * j - 1.0 + i) * coefficients(i * n_ + j - 1, k)
                     - (j - 1.0 + i) * coefficients(i * n_ + j - 2, k)
                     - coefficients(i * n_ + j - 1, k - 1)) / j;   // -X * L_{j-1}
            }
        }
    }

    evaluated_value = hornerEvaluation(coefficients, z);
}

arma::vec Poly::laguerre(int m_, int n_)
{
    return evaluated_value.col(m_ * n + n_);
}
arma::vec Poly::hermite(int n_)
{
    return evaluated_value.col(n_);
}