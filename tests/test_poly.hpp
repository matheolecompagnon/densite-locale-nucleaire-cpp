/**
 * @file test_poly.hpp
 * @author Mathéo LECOMPAGNON
 * @brief Ce fichier décrit les tests de la classe poly.
 * @version 0.1
 * @date 2026-09-09
 * 
 * @copyright Copyright (c) 2026
 * 
 */
 #include <cxxtest/TestSuite.h>
 #include <random> //necessaire pour une partie des test randomisé
 #include "poly.hpp"

 /**
    \brief Cette classe décrit les tests de la classe poly
 */

 class TestPoly : public CxxTest::TestSuite
 {
    public:
    
   /**
   * @brief  Test le calcul et l'évaluation de polynome d'Hermite sur 3 valeurs.
   * Une propriété mathématique fort est également testé : ici H_n(x) = (-1)^n H_n(-x)
   */
    void testHermite(){


      // 1. Obtenir une graine aléatoire depuis le système
      std::random_device rd;

      // 2. Initialiser le générateur avec la graine (Mersenne Twister)
      std::mt19937 gen(rd());

      // 3. Définir la distribution (par exemple, entre 1.0 et 10.0)
      std::uniform_real_distribution<double> dis(1.0, 10.0);

      // 4. Générer et afficher un nombre aléatoire
      double random_value = dis(gen);
      arma::vec vec_of_double = {random_value};
      arma::vec vec_of_neg_double = {-random_value};

      Poly poly;
      Poly poly_rnd;
      Poly poly_neg_rnd;

      arma::vec evaluatingPoints;
      evaluatingPoints = {3.0, 5.0, 7.0};
      poly.calcHermite(6, evaluatingPoints);
      poly_rnd.calcHermite(6, vec_of_double);
      poly_neg_rnd.calcHermite(6, vec_of_neg_double);
      
      // Test des valeur calculé à la main pour H1
      arma::vec result_h1 = {6.0,10.0,14.0};
      arma::vec computed_h1 = poly.hermite(1);
      TS_ASSERT_DELTA(arma::norm(result_h1 / computed_h1  - 1.0), 0.0, 1e-08);

      // Test d'antisymetrie H1
      arma::vec computed_h1_rnd = poly_rnd.hermite(1);
      arma::vec computed_h1_neg_rnd = poly_neg_rnd.hermite(1);
      TS_ASSERT_DELTA(arma::norm(computed_h1_rnd / computed_h1_neg_rnd  + 1.0), 0.0, 1e-08); //ici plus car H1 doit être antisymétrique

      // Test des valeur calculé à la main pour H2
      arma::vec result_h2 = {34.0,98.0,194.0};
      arma::vec computed_h2 = poly.hermite(2);
      TS_ASSERT_DELTA(arma::norm(result_h2 / computed_h2  - 1.0), 0.0, 1e-08);

      // Test d'antisymetrie H2
      arma::vec computed_h2_rnd = poly_rnd.hermite(2);
      arma::vec computed_h2_neg_rnd = poly_neg_rnd.hermite(2);
      TS_ASSERT_DELTA(arma::norm(computed_h2_rnd / computed_h2_neg_rnd  - 1.0), 0.0, 1e-08); //ici moins car H2 doit être symétrique

      // Test des valeur calculé à la main pour H3
      arma::vec result_h3 = {180.0,940.0,2660.0};
      arma::vec computed_h3 = poly.hermite(3);
      TS_ASSERT_DELTA(arma::norm(result_h3 / computed_h3  - 1.0), 0.0, 1e-08);

      // Test d'antisymetrie H3
      arma::vec computed_h3_rnd = poly_rnd.hermite(3);
      arma::vec computed_h3_neg_rnd = poly_neg_rnd.hermite(3);
      TS_ASSERT_DELTA(arma::norm(computed_h3_rnd / computed_h3_neg_rnd  + 1.0), 0.0, 1e-08); //ici plus car H3 doit être antisymétrique

      // Test des valeur calculé à la main pour H5
      arma::vec result_h5 = {3816.0,80600.0,483784.0};
      arma::vec computed_h5 = poly.hermite(3);
      TS_ASSERT_DELTA(arma::norm(result_h5 / computed_h5  - 1.0), 0.0, 1e-08);

      // Test d'antisymetrie H4
      arma::vec computed_h4_rnd = poly_rnd.hermite(4);
      arma::vec computed_h4_neg_rnd = poly_neg_rnd.hermite(4);
      TS_ASSERT_DELTA(arma::norm(computed_h4_rnd / computed_h4_neg_rnd  - 1.0), 0.0, 1e-08); //ici moins car H4 doit être symétrique

      // Test d'antisymetrie H5
      arma::vec computed_h5_rnd = poly_rnd.hermite(5);
      arma::vec computed_h5_neg_rnd = poly_neg_rnd.hermite(5);
      TS_ASSERT_DELTA(arma::norm(computed_h5_rnd / computed_h5_neg_rnd  + 1.0), 0.0, 1e-08); //ici plus car H5 doit être antisymétrique
    }

   /**
   * @brief  Test le calcul et l'évaluation de polynome de Laguerre sur 3 valeurs.
   * Une propriété mathématique fort est également testé : 
   * ici x*L'_n(x) - nL_n(x) + nL_{n-1}(x) = 0
   */
   /*void testLaguerre(){

      Poly poly;
      Poly derivative;

      arma::vec evaluatingPoints;
      evaluatingPoints = {3.0, 5.0, 7.0};

      poly.calcLaguerre(6,5,evaluatingPoints);
      derivative.calcDerivatveLaguerre(6,5, evaluatingPoints);
    }*/

 };