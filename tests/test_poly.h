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
   * Une propriété mathématique forte est également testée : ici H_n(x) = (-1)^n H_n(-x)
   */
    void testHermite(){

      // 1. Obtenir une graine aléatoire depuis le système
      std::random_device rd;

      // 2. Initialiser le générateur avec la graine (Mersenne Twister)
      std::mt19937 gen(rd());

      // 3. Définir la distribution (loi normale de moyenne 0 et d'écart-type 3)
      std::normal_distribution<double> dis(0.0, 3.0);

      // 4. Générer un nombre aléatoire
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
      
      // Test des valeurs calculées à la main pour H1
      arma::vec result_h1 = {6.0,10.0,14.0};
      arma::vec computed_h1 = poly.hermite(1);
      TS_ASSERT_DELTA(arma::norm(result_h1 - computed_h1), 0.0, 1e-08);

      // Test d'antisymétrie H1
      arma::vec computed_h1_rnd = poly_rnd.hermite(1);
      arma::vec computed_h1_neg_rnd = poly_neg_rnd.hermite(1);
      TS_ASSERT_DELTA(arma::norm(computed_h1_rnd + computed_h1_neg_rnd), 0.0, 1e-08); //ici plus car H1 doit être antisymétrique

      // Test des valeurs calculées à la main pour H2
      arma::vec result_h2 = {34.0,98.0,194.0};
      arma::vec computed_h2 = poly.hermite(2);
      TS_ASSERT_DELTA(arma::norm(result_h2 - computed_h2 ), 0.0, 1e-08);

      // Test de symétrie H2
      arma::vec computed_h2_rnd = poly_rnd.hermite(2);
      arma::vec computed_h2_neg_rnd = poly_neg_rnd.hermite(2);
      TS_ASSERT_DELTA(arma::norm(computed_h2_rnd - computed_h2_neg_rnd ), 0.0, 1e-08); //ici moins car H2 doit être symétrique

      // Test des valeurs calculées à la main pour H3
      arma::vec result_h3 = {180.0,940.0,2660.0};
      arma::vec computed_h3 = poly.hermite(3);
      TS_ASSERT_DELTA(arma::norm(result_h3 - computed_h3), 0.0, 1e-08);

      // Test d'antisymétrie H3
      arma::vec computed_h3_rnd = poly_rnd.hermite(3);
      arma::vec computed_h3_neg_rnd = poly_neg_rnd.hermite(3);
      TS_ASSERT_DELTA(arma::norm(computed_h3_rnd + computed_h3_neg_rnd ), 0.0, 1e-08); //ici plus car H3 doit être antisymétrique

      // Test des valeurs calculées à la main pour H4
      arma::vec result_h4 = {876.0,8812.0,36076.0};
      arma::vec computed_h4 = poly.hermite(4);
      TS_ASSERT_DELTA(arma::norm(result_h4 - computed_h4), 0.0, 1e-08);

      // Test de symétrie H4
      arma::vec computed_h4_rnd = poly_rnd.hermite(4);
      arma::vec computed_h4_neg_rnd = poly_neg_rnd.hermite(4);
      TS_ASSERT_DELTA(arma::norm(computed_h4_rnd - computed_h4_neg_rnd), 0.0, 1e-08); //ici moins car H4 doit être symétrique

      // Test des valeurs calculées à la main pour H5
      arma::vec result_h5 = {3816.0,80600.0,483784.0};
      arma::vec computed_h5 = poly.hermite(5);
      TS_ASSERT_DELTA(arma::norm(result_h5 - computed_h5), 0.0, 1e-08);

      // Test d'antisymétrie H5
      arma::vec computed_h5_rnd = poly_rnd.hermite(5);
      arma::vec computed_h5_neg_rnd = poly_neg_rnd.hermite(5);
      TS_ASSERT_DELTA(arma::norm(computed_h5_rnd + computed_h5_neg_rnd), 0.0, 1e-08); //ici plus car H5 doit être antisymétrique
    }

   /**
   * @brief  Test le calcul et l'évaluation des polynomes de Laguerre généralisés
   * sur 3 valeurs, pour m de 0 à 5 avec la troncature nMax[m] = (mMax - m - 1)/2 + 1.
   * Une propriété mathématique forte est également testée :
   * ici x*L'_n^m(x) - n*L_n^m(x) + (n+m)*L_{n-1}^m(x) = 0 (pour n >= 1)
   */
    void testLaguerre(){

      // Troncature de la base : m varie de 0 à 5 (mMax = 6) et n < nMax[m]
      const int mMax = 6;
      const int nMaxGlobal = (mMax - 1) / 2 + 1; // nMax[0], le plus grand des nMax[m]

      // 1. Obtenir une graine aléatoire depuis le système
      std::random_device rd;

      // 2. Initialiser le générateur avec la graine (Mersenne Twister)
      std::mt19937 gen(rd());

      // 3. Définir la distribution (loi normale de moyenne 0 et d'écart-type 3)
      std::normal_distribution<double> dis(0.0, 3.0);

      // 4. Générer trois nombres aléatoires
      arma::vec rnd_points = {dis(gen), dis(gen), dis(gen)};

      Poly poly;
      Poly poly_rnd;
      Poly derivative_rnd;

      arma::vec evaluatingPoints;
      evaluatingPoints = {3.0, 5.0, 7.0};
      poly.calcLaguerre(mMax, nMaxGlobal, evaluatingPoints);
      poly_rnd.calcLaguerre(mMax, nMaxGlobal, rnd_points);
      derivative_rnd.calcDerivativeLaguerre(mMax, nMaxGlobal, rnd_points);

      // Les tests à la main utilisent une différence absolue (et non un rapport)
      // car L_1^2(3) = 0 empêcherait la division

      // Test des valeurs calculées à la main pour m = 0, n = 0
      arma::vec result_l_m0_n0 = {1.0,1.0,1.0};
      arma::vec computed_l_m0_n0 = poly.laguerre(0, 0);
      TS_ASSERT_DELTA(arma::norm(result_l_m0_n0 - computed_l_m0_n0), 0.0, 1e-08);

      // Test des valeurs calculées à la main pour m = 0, n = 1
      arma::vec result_l_m0_n1 = {-2.0,-4.0,-6.0};
      arma::vec computed_l_m0_n1 = poly.laguerre(0, 1);
      TS_ASSERT_DELTA(arma::norm(result_l_m0_n1 - computed_l_m0_n1), 0.0, 1e-08);

      // Test des valeurs calculées à la main pour m = 0, n = 2
      arma::vec result_l_m0_n2 = {-0.5,3.5,11.5};
      arma::vec computed_l_m0_n2 = poly.laguerre(0, 2);
      TS_ASSERT_DELTA(arma::norm(result_l_m0_n2 - computed_l_m0_n2), 0.0, 1e-08);

      // Test des valeurs calculées à la main pour m = 1, n = 0
      arma::vec result_l_m1_n0 = {1.0,1.0,1.0};
      arma::vec computed_l_m1_n0 = poly.laguerre(1, 0);
      TS_ASSERT_DELTA(arma::norm(result_l_m1_n0 - computed_l_m1_n0), 0.0, 1e-08);

      // Test des valeurs calculées à la main pour m = 1, n = 1
      arma::vec result_l_m1_n1 = {-1.0,-3.0,-5.0};
      arma::vec computed_l_m1_n1 = poly.laguerre(1, 1);
      TS_ASSERT_DELTA(arma::norm(result_l_m1_n1 - computed_l_m1_n1), 0.0, 1e-08);

      // Test des valeurs calculées à la main pour m = 1, n = 2
      arma::vec result_l_m1_n2 = {-1.5,0.5,6.5};
      arma::vec computed_l_m1_n2 = poly.laguerre(1, 2);
      TS_ASSERT_DELTA(arma::norm(result_l_m1_n2 - computed_l_m1_n2), 0.0, 1e-08);

      // Test des valeurs calculées à la main pour m = 2, n = 0
      arma::vec result_l_m2_n0 = {1.0,1.0,1.0};
      arma::vec computed_l_m2_n0 = poly.laguerre(2, 0);
      TS_ASSERT_DELTA(arma::norm(result_l_m2_n0 - computed_l_m2_n0), 0.0, 1e-08);

      // Test des valeurs calculées à la main pour m = 2, n = 1
      arma::vec result_l_m2_n1 = {0.0,-2.0,-4.0};
      arma::vec computed_l_m2_n1 = poly.laguerre(2, 1);
      TS_ASSERT_DELTA(arma::norm(result_l_m2_n1 - computed_l_m2_n1), 0.0, 1e-08);

      // Test des valeurs calculées à la main pour m = 3, n = 0
      arma::vec result_l_m3_n0 = {1.0,1.0,1.0};
      arma::vec computed_l_m3_n0 = poly.laguerre(3, 0);
      TS_ASSERT_DELTA(arma::norm(result_l_m3_n0 - computed_l_m3_n0), 0.0, 1e-08);

      // Test des valeurs calculées à la main pour m = 3, n = 1
      arma::vec result_l_m3_n1 = {1.0,-1.0,-3.0};
      arma::vec computed_l_m3_n1 = poly.laguerre(3, 1);
      TS_ASSERT_DELTA(arma::norm(result_l_m3_n1 - computed_l_m3_n1), 0.0, 1e-08);

      // Test des valeurs calculées à la main pour m = 4, n = 0 (seul n autorisé)
      arma::vec result_l_m4_n0 = {1.0,1.0,1.0};
      arma::vec computed_l_m4_n0 = poly.laguerre(4, 0);
      TS_ASSERT_DELTA(arma::norm(result_l_m4_n0 - computed_l_m4_n0), 0.0, 1e-08);

      // Test des valeurs calculées à la main pour m = 5, n = 0 (seul n autorisé)
      arma::vec result_l_m5_n0 = {1.0,1.0,1.0};
      arma::vec computed_l_m5_n0 = poly.laguerre(5, 0);
      TS_ASSERT_DELTA(arma::norm(result_l_m5_n0 - computed_l_m5_n0), 0.0, 1e-08);

      // Test de la propriété x*L'_n^m - n*L_n^m + (n+m)*L_{n-1}^m = 0 sur les points aléatoires
      // (n >= 1 car la relation fait intervenir L_{n-1}, donc seuls m = 0, 1, 2, 3 sont concernés)
      for(int m = 0; m < mMax; m++){
         int nMax = (mMax - m - 1) / 2 + 1; // troncature (division entière)
         for(int n = 1; n < nMax; n++){
            arma::vec computed_ln = poly_rnd.laguerre(m, n);
            arma::vec computed_lnm1 = poly_rnd.laguerre(m, n - 1);
            arma::vec computed_dln = derivative_rnd.derivativeLaguerre(m, n);
            // % est le produit terme à terme d'Armadillo
            TS_ASSERT_DELTA(arma::norm(rnd_points % computed_dln - n * computed_ln + (n + m) * computed_lnm1), 0.0, 1e-08);
         }
      }
    }

 };