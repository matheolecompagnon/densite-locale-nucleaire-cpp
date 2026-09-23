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

  #include "../include/Basis.hpp"
  #include <cxxtest/TestSuite.h>

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

   void test_rPart_renvoie_la_valeur_attendue_pour_la_basis_function_R();
   void test_ZPart_renvoie_la_valeur_attendue_pour_la_basis_function_Z();
   void test_n_Max();
   void test_n_zMax();



 };