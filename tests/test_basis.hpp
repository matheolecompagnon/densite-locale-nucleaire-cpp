/**
 * @file test_basis.hpp
 * @author Mathéo LECOMPAGNON
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

    void setUp();

    void tearDown();
    void test_rPart_en_etat_fondamental_a_l_origine();
    void test_rPart_en_etat_fondamental_partout()
    void test_rPart_en_partout_pour_n_et_m_different_de_0();
    void test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_br();
    void test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_bz();
    void test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_N();
    void test_le_constructeur_Basis_affecte_bien_sa_valeur_au_parametre_Q();
    void test_zPart_en_etat_fondamental_a_l_origine();
    void test_zPart_en_etat_fondamental_partout();
    void test_zPart_en_partout_pour_nz_different_de_0();



 };