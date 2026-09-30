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

void setUp(){
    };

void tearDown(){
};

//fondamental pour signifier, n = 0, m = 0
//a l'origine pour dire un vecteur r ou z nul

void test_rPart_en_etat_fondamental_a_l_origine(){

     /**
      * Remarque : absence d'un constructeur pour la classe basis
      * donc impossible pour le moment de fixer un contexte précis
      * dans ce test
      * Ce test suppose que la valeur br est fixée à 2
      * Donc , si ce n'est pas celle utilisée plus tard 
      * lors de l'implémentation de la fonction rpart
      * il va falloir recalculer le résultat par rapport 
      * auquel on fait le test. A discuter en groupe
      * 
      */
    Basis basis(2,0,0,0);

    arma::vec r = arma::zeros<arma::vec>(8);

    arma::vec res = arma::ones<arma::vec>(8);
    res = 0.2821 * res;

    TS_ASSERT_DELTA(arma::norm(basis.rpart(r , 0 ,0 ) - res ),0.0,  1e-15) ;
}

void test_rPart_en_etat_fondamental_partout(){
     /**
      * Remarque : absence d'un constructeur pour la classe basis
      * donc impossible pour le moment de fixer un contexte précis
      * dans ce test
      * Ce test suppose que la valeur br est fixée à 2
      * Donc , si ce n'est pas celle utilisée plus tard 
      * lors de l'implémentation de la fonction rpart
      * il va falloir recalculer le résultat par rapport 
      * auquel on fait le test. A discuter en groupe
      * 
      */
    Basis basis(2,0,0,0);
    arma::vec r = {3.1, 2.3, 1.0, 0.0, 0.1, 4.3, 9.2, 13.7};

    arma::vec res = {8.4859e-02,
                    1.4562e-01,
                    2.4895e-01,
                    2.8209e-01,
                    2.8174e-01,
                    2.7966e-02,
                    7.1707e-06,
                    1.8252e-11
                };

    TS_ASSERT_DELTA(arma::norm(basis.rpart(r , 0 ,0 ) - res ),0.0,  1e-15) ;
}

void test_rPart_en_partout_pour_n_et_m_different_de_0(){
     /**
      * Remarque : absence d'un constructeur pour la classe basis
      * donc impossible pour le moment de fixer un contexte précis
      * dans ce test
      * Ce test suppose que la valeur br est fixée à 2
      * Donc , si ce n'est pas celle utilisée plus tard 
      * lors de l'implémentation de la fonction rpart
      * il va falloir recalculer le résultat par rapport 
      * auquel on fait le test. A discuter en groupe
      * 
      */
    Basis basis(2,0,0,0);
    arma::vec r = {9.1, 6.3, 1.0, 4.0, 0.1, 0.3, 9.2, 13.7};

    arma::vec res = {-0.0683
                    0.0097,
                    0.1276,
                    0.0738,
                    0.0023,
                    0.0196,
                    -0.0684,
                    -0.0001
                };

    TS_ASSERT_DELTA(arma::norm(basis.rpart(r , 2 ,5 ) - res ),0.0,  1e-15) ;
}

void test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_br(){
    //Meme remarque par rapport au constructeur
    Basis basis(2,0,0,0);

    TS_ASSERT_EQUALS(basis.br , 2);
}

void test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_bz(){
    //Meme remarque par rapport au constructeur
    Basis basis(2,5,0,0);

    TS_ASSERT_EQUALS(basis.bz , 5);
}

void test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_N(){
    //Meme remarque par rapport au constructeur
    Basis basis(2,5,3,0);

    TS_ASSERT_EQUALS(basis.N , 3);
}

void test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_Q(){
    //Meme remarque par rapport au constructeur
    Basis basis(2,5,3,4);

    TS_ASSERT_EQUALS(basis.Q , 4);
}

void test_zPart_en_etat_fondamental_a_l_origine(){

     /**
      * Remarque : absence d'un constructeur pour la classe basis
      * donc impossible pour le moment de fixer un contexte précis
      * dans ce test
      * Ce test suppose que la valeur bz est fixée à 2.5
      * Donc , si ce n'est pas celle utilisée plus tard 
      * lors de l'implémentation de la fonction rpart
      * il va falloir recalculer le résultat par rapport 
      * auquel on fait le test. A discuter en groupe
      * 
      */
    Basis basis(0,2.5,0,0);

    arma::vec z = arma::zeros<arma::vec>(8);

    arma::vec res = arma::ones<arma::vec>(8);
    res = 0.4751 * res;

    TS_ASSERT_DELTA(arma::norm(basis.zpart(z , 0 ) - res ), 0.0,  1e-15) ;
}

void test_zPart_en_etat_fondamental_partout(){
     /**
      * Remarque : absence d'un constructeur pour la classe basis
      * donc impossible pour le moment de fixer un contexte précis
      * dans ce test
      * Ce test suppose que la valeur bz est fixée à 2.5
      * Donc , si ce n'est pas celle utilisée plus tard 
      * lors de l'implémentation de la fonction rpart
      * il va falloir recalculer le résultat par rapport 
      * auquel on fait le test. A discuter en groupe
      * 
      */
    Basis basis(2,2.5 ,0,0);
    arma::vec z = {-4.5, -2.1, 0.0, 1.5, 3.8, 10.2, 2 ,1};

    arma::vec res = { 0.0940
                    0.3338,
                    0.4751,
                    0.3968,
                    0.1496,
                    0.0001,
                    0.3450,
                    0.4385
                };

    TS_ASSERT_DELTA(arma::norm(basis.zpart(z , 0 ) - res ),0.0,  1e-15) ;
}

void test_zPart_en_partout_pour_nz_different_de_0(){
     /**
      * Remarque : absence d'un constructeur pour la classe basis
      * donc impossible pour le moment de fixer un contexte précis
      * dans ce test
      * Ce test suppose que la valeur bz est fixée à 2.5
      * Donc , si ce n'est pas celle utilisée plus tard 
      * lors de l'implémentation de la fonction rpart
      * il va falloir recalculer le résultat par rapport 
      * auquel on fait le test. A discuter en groupe
      * 
      */
    Basis basis(2,2.5,0,0);
    arma::vec z = {-4.5, -2.1, 0.0, 1.5, 3.8, 10.2, 2 ,1};

    arma::vec res = {0.1706
                    -0.1042,
                    0,
                    0.2557,
                    -0.2894,
                    0.0480,
                    0.1368,
                    0.2695
                };

    TS_ASSERT_DELTA(arma::norm(basis.zpart(z , 5 ) - res ),0.0,  1e-15) ;
}