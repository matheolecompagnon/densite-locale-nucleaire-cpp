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
#include "Basis.hpp"
#include "Poly.hpp"
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
    return 0;
}