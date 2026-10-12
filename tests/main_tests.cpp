/**
 * @file main.cpp
 * @author Mathéo LECOMAPGNON
 * @brief Ce fichier décrit la boucle principale du programme.
 * @version 0.1
 * @date 2026-09-09
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#include "basis.hpp"
#include "poly.hpp"
#include "test_mandatory.hpp"
//#include "test_basis.hpp"
#include "test_poly.hpp"
#include <armadillo>
#include <cmath>


#include <iostream>
#include <string>
#include <vector>
#include <functional>

int main(void) {
    // First test suite : Mandatory tests
    TestMandatory testMandatory;
    testMandatory.testMandatory0();
    testMandatory.testMandatory1();
    testMandatory.testMandatory2();
    testMandatory.testMandatory3();

    TestPoly test_poly;
    test_poly.testHermite();
    test_poly.testLaguerre();
    return 0;
}