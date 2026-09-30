/**
 * @file test_basis.hpp
 * @author Mathéo LECOMPAGNON
 * @author Jean-Marie ASSOGBA
 * @brief Ce fichier décrit les tests de la classe Basis.
 * @version 0.1
 * @date 2026-09-09
 * 
 * @copyright Copyright (c) 2026
 * 
 */

  #include "basis.hpp"
  #include <cxxtest/TestSuite.h>
  #include <numbers>
  #include <armadillo>
  #include <cmath>

 /**
    \brief Cette classe décrit les tests de la classe basis
 */

 class TestBasis : public CxxTest::TestSuite
 {
    public:
    // Exemple de test :
    // void testMyFunc(void)
    // {
    // MyClass myClass(2);
    // TS_ASSERT_EQUALS(myClass.myFunc(3), 5);
    // }

   void setUp(){
   };

   void tearDown(){
   };

   /**
    * @brief Un test en état  fondamental signifie que les valeurs
    * de m et de n sont égales à 0 pour les polynômes d'Hermite ou 
    * de Laguerre. Un test à l'origine signifie que les points qu'on évalue
    * ont tous pour valeur 0 (soit r , soit z en fonction de ce qu'on teste) 
    * Cela veut donc dire que le vecteur r (ou z) est un vecteu nul
    * Un test 'partout' veut dire qu'on prend n'importe quel valeur 
    * de r,  n et m  pour le test
    */


   /**
    * @brief ON teste le vecteur de retour fournit par rPart en condition
    * r = vecteur nul, n = 0 , et m = 0
    * 
    */
   void test_rPart_en_etat_fondamental_a_l_origine(){

      /**
       * @brief On instancie ici une base avec br = 2
       * bz = 0 , N = 0 et Q = 0      * 
       */
      Basis basis(2,0,0,0);
      
      //r un vecteur nul
      arma::vec r = arma::zeros<arma::vec>(8);
      //res me resultat qu'on attend
      arma::vec res = arma::ones<arma::vec>(8);
      res = 0.2821 * res;
      //on teste que la norme de la difference entre les deux vectreurs est égale 
      //à 0, avec une erreur de 1e-15
      TS_ASSERT_DELTA(arma::norm(basis.rPart(r , 0 ,0 ) - res ),0.0,  1e-15) ;
   }
   
   /**
    * @brief On teste le vecteur de retour de rPart 
    * pour r un vecteur choisi et n et m  à 0
    * 
    */
   void test_rPart_en_etat_fondamental_partout(){
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

      TS_ASSERT_DELTA(arma::norm(basis.rPart(r , 0 ,0 ) - res ),0.0,  1e-15) ;
   }

   /**
    * @brief On teste le vecteur de retour de rPart 
    * pour r un vecteur choisi et n et m quelconques
    * 
    */

   void test_rPart_en_partout_pour_n_et_m_different_de_0(){

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

      TS_ASSERT_DELTA(arma::norm(basis.rPart(r , 2 ,5 ) - res ),0.0,  1e-15) ;
   }

   /**
    * @brief Le constructeur de la classe basis permet d'affecter 
    * les variables ,br , bz, N et Q
    * ON verifie ici si le constructeur a bine affecté à br 
    * la valeur passée en paramètres
    * 
    */
   void test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_br(){
      Basis basis(2,0,0,0);

      TS_ASSERT_EQUALS(basis.br , 2);
   }

   /**
    * @brief ON verifie ici si le constructeur a bine affecté à bz 
    * la valeur passée en paramètres
    * 
    */

   void test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_bz(){
      Basis basis(2,5,0,0);

      TS_ASSERT_EQUALS(basis.bz , 5);
   }

   /**
    * @brief ON verifie ici si le constructeur a bine affecté à N 
    * la valeur passée en paramètres
    * 
    */

   void test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_N(){
      Basis basis(2,5,3,0);

      TS_ASSERT_EQUALS(basis.N , 3);
   }

   /**
    * @brief ON verifie ici si le constructeur a bine affecté à Q
    * la valeur passée en paramètres
    * 
    */

   void test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_Q(){
      Basis basis(2,5,3,4);

      TS_ASSERT_EQUALS(basis.Q , 4);
   }

   /**
    * @brief On teste le vecteur de retour de rPart 
    * pour r un vecteur nul et n et m à 0
    * 
    */

   void test_zPart_en_etat_fondamental_a_l_origine(){

      Basis basis(0,2.5,0,0);

      arma::vec z = arma::zeros<arma::vec>(8);

      arma::vec res = arma::ones<arma::vec>(8);
      res = 0.4751 * res;

      TS_ASSERT_DELTA(arma::norm(basis.zPart(z , 0 ) - res ), 0.0,  1e-15) ;
   }

   /**
    * @brief On teste le vecteur de retour de rPart 
    * pour r un vecteur choisi et n et m à 0
    * 
    */

   void test_zPart_en_etat_fondamental_partout(){
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

      TS_ASSERT_DELTA(arma::norm(basis.zPart(z , 0 ) - res ),0.0,  1e-15) ;
   }
   
   /**
    * @brief On teste le vecteur de retour de zPart 
    * pour r un vecteur nul et n et m quelconques
    * 
    */
   void test_zPart_en_partout_pour_nz_different_de_0(){
      
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

      TS_ASSERT_DELTA(arma::norm(basis.zPart(z , 5 ) - res ),0.0,  1e-15) ;
   }

   /**
    * @brief mMax, nMax et m_zMax sont des parametres calculés 
    * à partir  de N et Q
    * On vérifie donc ici que le calcul de mMax( lors de l'appel
    * de constructeur) donne le résultat attendu
    */
   void test_sur_la_valeur_calculee_de_mMax(){
      //m_max est calculee a partir des valeurs de N et Q
      //On fixe ici N = 12 et Q = 1.2
      Basis basis(1 , 1,  12 , 1.2) ; 

      int mMax_res = 12;

      TS_ASSERT_EQUALS(basis.mMax , mMax_res) ;
   }

   /**
    * @brief On vérifie ici que le calcul de nMax( lors de l'appel
    * de constructeur) donne le résultat attendu
    * 
    */
   void test_sur_le_vecteur_nMax_calculee(){
      //Pas finis
      Basis basis(1 , 1,  11 , 1.2) ;
      arma::ivec nMax_res = {6 ,5 ,5 ,4, 4, 3, 3, 2, 2, 1, 1};

      TS_ASSERT((basis.nMax - nMax_res).iszero());      
   }

   /**
    * @brief On vérifie ici que le calcul de n_zMax( lors de l'appel
    * de constructeur) donne le résultat attendu
    * 
    */
   void test_sur_le_matrice_n_zMax_calculee(){
      Basis basis(1 , 1,  10 , 1.2) ;
      arma::imat n_zMax_res = {
         {12   ,     10 ,        8    ,     5,         3},
        {11     ,    9   ,      6      ,   4  ,       2},
        {10      ,   8    ,     5 ,        3   ,      0},
        { 9       ,  6     ,    4  ,       2    ,     0},
         {8        , 5      ,   3   ,      0     ,    0},
         {6  ,       4       ,  2    ,     0      ,   0},
         {5   ,      3  ,       0     ,    0       ,  0},
        { 4    ,     2   ,      0      ,   0 ,        0},
       {  3     ,    0    ,     0       ,  0  ,       0},
        { 2      ,   0     ,    0 ,        0   ,      0}
      };

      TS_ASSERT((basis.n_zMax - n_zMax_res).iszero());      
   }

 };