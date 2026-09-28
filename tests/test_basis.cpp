/**
 * @file Basis_test.cpp
 * @author Jean-Marie ASSOGBA
 * @brief 
 * @version 0.1
 * @date 2026-09-23
 * 
 * @copyright Copyright (c) 2026
 * 
 */

//#include <armadillo>
//#include "../include/Basis.hpp"
#include "test_basis.hpp"

/**
     * @brief etat fondamental , on a r = 0, n = 0 , et  m = 0)
     * dans ces conditions $H_0(x) = 1$ et $L_0^{(0)}(x) = 1$
     * 
     * 
     */

void test_rPart_en_etat_fondamental_a_l_origine(){
    
    //
    // 

    /**
     * Ce test suppose que la valeur br est fixée à 2
     * Donc , si ce n'est pas celle utilisée plus tard 
     * lors de l'implémentation de la focntion rpart
     * il va falloir recalculer le résultat par rapport 
     * auquel on fait le test.
     * Ce test est implémenté comme cela pour le moment 
     * du à l'absence de constructeur pour attribuer à 
     * br une valeur (à voir en groupe)
     */

    arma::vec r = arma::zeros<arma::vec>(8);

    arma::vec res = arma::ones<arma::vec>(8);
    res = 0.2821 * res;

    TS_ASSERT_DELTA(Basis(rpart(r , 0 ,0 ) - res ), )
}

//void test_rPart_renvoie_la_valeur_attendue_pour_la_basis_function_R();
  // void test_ZPart_renvoie_la_valeur_attendue_pour_la_basis_function_Z();
   //void test_n_Max();
   //void test_n_zMax();