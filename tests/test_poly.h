/**
 * @file test_poly.hpp
 * @author Mathéo LECOMPAGNON
 * @brief Ce fichier décrit les tests de la classe poly.
 * @version 0.2
 * @date 2026-09-09
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#include <cxxtest/TestSuite.h>
#include "poly.hpp"

/**
 * \brief Cette classe décrit les tests de la classe poly
 *
 * Les tests sont des tests unitaires : les points d'évaluation et les
 * résultats attendus sont écrits en dur. Les 5 points ont été tirés une
 * fois pour toutes suivant une loi normale de moyenne 0 et de variance 3,
 * puis arrondis à 4 décimales (les valeurs de référence sont calculées
 * exactement sur ces points arrondis).
 */
class TestPoly : public CxxTest::TestSuite
{
   public:

   /**
   * @brief Test le calcul et l'évaluation des polynômes d'Hermite H_0 à H_5
   * sur 5 points aléatoires (loi normale N(0, 3)) fixés en dur.
   */
   void testHermite(){

      arma::vec evaluatingPoints = {-1.3737, 0.4167, -3.2845, 2.4175, 1.1056};

      Poly poly;
      poly.calcHermite(6, evaluatingPoints);

      // H0
      arma::vec result_h0 = {1.0, 1.0, 1.0, 1.0, 1.0};
      arma::vec computed_h0 = poly.hermite(0);
      TS_ASSERT_DELTA(arma::norm(result_h0 - computed_h0), 0.0, 1e-08);

      // H1
      arma::vec result_h1 = {-2.7474, 0.8334, -6.569, 4.835, 2.2112};
      arma::vec computed_h1 = poly.hermite(1);
      TS_ASSERT_DELTA(arma::norm(result_h1 - computed_h1), 0.0, 1e-08);

      // H2
      arma::vec result_h2 = {5.548206759999999, -1.30544444, 41.151761, 21.377225, 2.889405439999999};
      arma::vec computed_h2 = poly.hermite(2);
      TS_ASSERT_DELTA(arma::norm(result_h2 - computed_h2), 0.0, 1e-08);

      // H3
      arma::vec result_h3 = {-4.253543252423999, -4.421557396296, -244.049918009, 84.01888287499999, -2.455746691072002};
      arma::vec computed_h3 = poly.hermite(3);
      TS_ASSERT_DELTA(arma::norm(result_h3 - computed_h3), 0.0, 1e-08);

      // H4
      arma::vec result_h4 = {-21.6030558282903, 4.147740705926914, 1356.2533454011211, 277.96794870062496, -22.766579723298406};
      arma::vec computed_h4 = poly.hermite(4);
      TS_ASSERT_DELTA(arma::norm(result_h4 - computed_h4), 0.0, 1e-08);

      // H5
      arma::vec result_h5 = {93.38058160203676, 38.829186274687494, -6956.828881867965, 671.8239689675219, -30.695487555581416};
      arma::vec computed_h5 = poly.hermite(5);
      TS_ASSERT_DELTA(arma::norm(result_h5 - computed_h5), 0.0, 1e-08);
   }

   /**
   * @brief Test le calcul et l'évaluation des polynômes de Laguerre généralisés
   * sur 5 points aléatoires (loi normale N(0, 3)) fixés en dur, pour m de 0 à 5
   * avec la troncature nMax[m] = (mMax - m - 1)/2 + 1.
   */
   void testLaguerre(){

      // Troncature de la base : m varie de 0 à 5 (mMax = 6) et n < nMax[m]
      const int mMax = 6;
      const int nMaxGlobal = (mMax - 1) / 2 + 1; // nMax[0], le plus grand des nMax[m]

      arma::vec evaluatingPoints = {-1.3737, 0.4167, -3.2845, 2.4175, 1.1056};

      Poly poly;
      poly.calcLaguerre(mMax, nMaxGlobal, evaluatingPoints);

      // m = 0, n = 0
      arma::vec result_l_m0_n0 = {1.0, 1.0, 1.0, 1.0, 1.0};
      arma::vec computed_l_m0_n0 = poly.laguerre(0, 0);
      TS_ASSERT_DELTA(arma::norm(result_l_m0_n0 - computed_l_m0_n0), 0.0, 1e-08);

      // m = 0, n = 1
      arma::vec result_l_m0_n1 = {2.3737, 0.5832999999999999, 4.2844999999999995, -1.4175, -0.10559999999999992};
      arma::vec computed_l_m0_n1 = poly.laguerre(0, 1);
      TS_ASSERT_DELTA(arma::norm(result_l_m0_n1 - computed_l_m0_n1), 0.0, 1e-08);

      // m = 0, n = 2
      arma::vec result_l_m0_n2 = {4.690925845, 0.25341944499999997, 12.962970125, -0.9128468750000001, -0.6000243199999999};
      arma::vec computed_l_m0_n2 = poly.laguerre(0, 2);
      TS_ASSERT_DELTA(arma::norm(result_l_m0_n2 - computed_l_m0_n2), 0.0, 1e-08);

      // m = 1, n = 0
      arma::vec result_l_m1_n0 = {1.0, 1.0, 1.0, 1.0, 1.0};
      arma::vec computed_l_m1_n0 = poly.laguerre(1, 0);
      TS_ASSERT_DELTA(arma::norm(result_l_m1_n0 - computed_l_m1_n0), 0.0, 1e-08);

      // m = 1, n = 1
      arma::vec result_l_m1_n1 = {3.3737, 1.5833, 5.2844999999999995, -0.4175, 0.8944000000000001};
      arma::vec computed_l_m1_n1 = poly.laguerre(1, 1);
      TS_ASSERT_DELTA(arma::norm(result_l_m1_n1 - computed_l_m1_n1), 0.0, 1e-08);

      // m = 1, n = 2
      arma::vec result_l_m1_n2 = {8.064625845, 1.836719445, 18.247470125, -1.3303468749999996, 0.29437568000000014};
      arma::vec computed_l_m1_n2 = poly.laguerre(1, 2);
      TS_ASSERT_DELTA(arma::norm(result_l_m1_n2 - computed_l_m1_n2), 0.0, 1e-08);

      // m = 2, n = 0
      arma::vec result_l_m2_n0 = {1.0, 1.0, 1.0, 1.0, 1.0};
      arma::vec computed_l_m2_n0 = poly.laguerre(2, 0);
      TS_ASSERT_DELTA(arma::norm(result_l_m2_n0 - computed_l_m2_n0), 0.0, 1e-08);

      // m = 2, n = 1
      arma::vec result_l_m2_n1 = {4.3736999999999995, 2.5833, 6.2844999999999995, 0.5825, 1.8944};
      arma::vec computed_l_m2_n1 = poly.laguerre(2, 1);
      TS_ASSERT_DELTA(arma::norm(result_l_m2_n1 - computed_l_m2_n1), 0.0, 1e-08);

      // m = 3, n = 0
      arma::vec result_l_m3_n0 = {1.0, 1.0, 1.0, 1.0, 1.0};
      arma::vec computed_l_m3_n0 = poly.laguerre(3, 0);
      TS_ASSERT_DELTA(arma::norm(result_l_m3_n0 - computed_l_m3_n0), 0.0, 1e-08);

      // m = 3, n = 1
      arma::vec result_l_m3_n1 = {5.3736999999999995, 3.5833, 7.2844999999999995, 1.5825, 2.8944};
      arma::vec computed_l_m3_n1 = poly.laguerre(3, 1);
      TS_ASSERT_DELTA(arma::norm(result_l_m3_n1 - computed_l_m3_n1), 0.0, 1e-08);

      // m = 4, n = 0
      arma::vec result_l_m4_n0 = {1.0, 1.0, 1.0, 1.0, 1.0};
      arma::vec computed_l_m4_n0 = poly.laguerre(4, 0);
      TS_ASSERT_DELTA(arma::norm(result_l_m4_n0 - computed_l_m4_n0), 0.0, 1e-08);

      // m = 5, n = 0
      arma::vec result_l_m5_n0 = {1.0, 1.0, 1.0, 1.0, 1.0};
      arma::vec computed_l_m5_n0 = poly.laguerre(5, 0);
      TS_ASSERT_DELTA(arma::norm(result_l_m5_n0 - computed_l_m5_n0), 0.0, 1e-08);
   }

};