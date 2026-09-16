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

// Codes ANSI pour les couleurs
#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[32m"
#define COLOR_RED "\033[31m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_BOLD "\033[1m"

class TestFramework {
private:
    struct TestCase {
        std::string name;
        std::function<void()> test;
    };

    std::vector<TestCase> tests;
    int passed = 0;
    int failed = 0;

public:
    // Ajouter un test
    void addTest(const std::string& name, std::function<void()> test) {
        tests.push_back({name, test});
    }

    // Exécuter tous les tests
    void runAll() {
        std::cout << COLOR_BOLD COLOR_BLUE << "\n╔════════════════════════════════════════╗\n"
                  << "║         TEST FRAMEWORK RESULTS         ║\n"
                  << "╚════════════════════════════════════════╝" << COLOR_RESET << "\n\n";

        for (const auto& testCase : tests) {
            try {
                testCase.test();
                passed++;
                std::cout << COLOR_GREEN << "✓ PASS" << COLOR_RESET << " - " << testCase.name << "\n";
            } catch (const std::exception& e) {
                failed++;
                std::cout << COLOR_RED << "✗ FAIL" << COLOR_RESET << " - " << testCase.name << "\n";
                std::cout << "        " << COLOR_RED << "Error: " << e.what() << COLOR_RESET << "\n";
            } catch (...) {
                failed++;
                std::cout << COLOR_RED << "✗ FAIL" << COLOR_RESET << " - " << testCase.name << "\n";
                std::cout << "        " << COLOR_RED << "Unknown error" << COLOR_RESET << "\n";
            }
        }

        // Résumé
        std::cout << "\n" << COLOR_BOLD << "Summary:" << COLOR_RESET << "\n";
        std::cout << COLOR_GREEN << "  Passed: " << passed << COLOR_RESET << "\n";
        std::cout << COLOR_RED << "  Failed: " << failed << COLOR_RESET << "\n";
        std::cout << COLOR_YELLOW << "  Total:  " << (passed + failed) << COLOR_RESET << "\n\n";
    }

    int getPassedCount() const { return passed; }
    int getFailedCount() const { return failed; }
};

// Macro pour les assertions
#define ASSERT_TRUE(condition, message) \
    if (!(condition)) throw std::runtime_error(message);

#define ASSERT_FALSE(condition, message) \
    if ((condition)) throw std::runtime_error(message);

#define ASSERT_EQUAL(a, b, message) \
    if ((a) != (b)) throw std::runtime_error(message);

#define ASSERT_NOT_EQUAL(a, b, message) \
    if ((a) == (b)) throw std::runtime_error(message);




// Helper function pour comparer deux vecteurs avec tolérance relative
void assertVectorDelta(const arma::vec& actual, const arma::vec& expected,
                       double tolerance, const std::string& message) {
    double error = arma::norm(actual / expected - 1.0);
    if (error > tolerance) {
        throw std::runtime_error(message + " (error: " + 
                                std::to_string(error) + ")");
    }
}

// Helper function pour comparer deux vecteurs/matrices avec tolérance absolue
void assertMatrixDelta(const arma::mat& actual, const arma::mat& expected,
                       double tolerance, const std::string& message) {
    double error = arma::norm(actual - expected);
    if (error > tolerance) {
        throw std::runtime_error(message + " (error: " + 
                                std::to_string(error) + ")");
    }
}

// Helper function pour comparer deux ivec
void assertIvecEqual(const arma::ivec& actual, const arma::ivec& expected,
                     const std::string& message) {
    if (!arma::all(actual == expected)) {
        throw std::runtime_error(message);
    }
}

// Helper function pour comparer deux imat
void assertImatEqual(const arma::imat& actual, const arma::imat& expected,
                     const std::string& message) {
    if (!arma::all(arma::all(actual == expected))) {
        throw std::runtime_error(message);
    }
}

int main() {
    TestFramework test;

    // ============================================================================
    // TEST #00 - Hermite and Laguerre polynomials
    // ============================================================================
    
    test.addTest("Poly #00 - Hermite(n=4)", []() {
        Poly poly;
        arma::vec zVals = {-3.1, -2.3, -1.0, -0.3, 0.1, 4.3, 9.2, 13.7};
        poly.calcHermite(6, zVals);
        
        arma::vec calcVals = poly.hermite(4);
        arma::vec targetVals = {1.02835360e+03, 2.05825600e+02, -2.00000000e+01, 
                                7.80960000e+00, 1.15216000e+01, 4.59456160e+03, 
                                1.10572154e+05, 5.54643458e+05};
        
        assertVectorDelta(calcVals, targetVals, 1e-08, "Hermite n=4");
    });

    test.addTest("Poly #00 - Hermite(n=5)", []() {
        Poly poly;
        arma::vec zVals = {-3.1, -2.3, -1.0, -0.3, 0.1, 4.3, 9.2, 13.7};
        poly.calcHermite(6, zVals);
        
        arma::vec calcVals = poly.hermite(5);
        arma::vec targetVals = {-4.76676832e+03, -3.88909760e+02, 8.00000000e+00, 
                                -3.17577600e+01, 1.18403200e+01, 3.48375818e+04, 
                                1.98557479e+06, 1.50339793e+07};
        
        assertVectorDelta(calcVals, targetVals, 1e-08, "Hermite n=5");
    });

    test.addTest("Poly #00 - Laguerre(m=4, n=2)", []() {
        Poly poly;
        arma::vec zVals = {0.1, 0.3, 1.2, 1.8, 2.0, 2.5, 7.1, 11.1};
        poly.calcLaguerre(6, 4, zVals);
        
        arma::vec calcVals = poly.laguerre(4, 2);
        arma::vec targetVals = {14.405, 13.245, 8.52, 5.82, 5.0, 3.125, -2.395, 10.005};
        
        assertVectorDelta(calcVals, targetVals, 1e-08, "Laguerre m=4, n=2");
    });

    test.addTest("Poly #00 - Laguerre(m=5, n=3)", []() {
        Poly poly;
        arma::vec zVals = {0.1, 0.3, 1.2, 1.8, 2.0, 2.5, 7.1, 11.1};
        poly.calcLaguerre(6, 4, zVals);
        
        arma::vec calcVals = poly.laguerre(5, 3);
        arma::vec targetVals = {53.23983333, 47.95550000, 27.87200000, 17.5880,
                                14.66666667, 8.39583333, -0.81183333, 10.1015};
        
        assertVectorDelta(calcVals, targetVals, 1e-08, "Laguerre m=5, n=3");
    });

    // ============================================================================
    // TEST #01 - Basis truncation
    // ============================================================================
    
    test.addTest("Basis #01 - mMax = 14", []() {
        Basis basis(1.935801664793151, 2.829683956491218, 14, 1.3);
        ASSERT_EQUAL(basis.mMax, 14, "mMax should be 14");
    });

    test.addTest("Basis #01 - nMax vector", []() {
        Basis basis(1.935801664793151, 2.829683956491218, 14, 1.3);
        arma::ivec nMax = {7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1};
        assertIvecEqual(basis.nMax, nMax, "nMax vector mismatch");
    });

    test.addTest("Basis #01 - n_zMax matrix", []() {
        Basis basis(1.935801664793151, 2.829683956491218, 14, 1.3);
        arma::imat n_zMax = {{18, 15, 13, 10, 7, 5, 2}, 
                             {16, 14, 11,  9, 6, 3, 1}, 
                             {15, 13, 10,  7, 5, 2, 0}, 
                             {14, 11,  9,  6, 3, 1, 0}, 
                             {13, 10,  7,  5, 2, 0, 0}, 
                             {11,  9,  6,  3, 1, 0, 0}, 
                             {10,  7,  5,  2, 0, 0, 0}, 
                             { 9,  6,  3,  1, 0, 0, 0}, 
                             { 7,  5,  2,  0, 0, 0, 0}, 
                             { 6,  3,  1,  0, 0, 0, 0}, 
                             { 5,  2,  0,  0, 0, 0, 0}, 
                             { 3,  1,  0,  0, 0, 0, 0}, 
                             { 2,  0,  0,  0, 0, 0, 0}, 
                             { 1,  0,  0,  0, 0, 0, 0}};
        
        assertImatEqual(basis.n_zMax, n_zMax, "n_zMax matrix mismatch");
    });

    // ============================================================================
    // TEST #02 - Basis r-functions
    // ============================================================================
    
    test.addTest("Basis #02 - rPart(r, 0, 0)", []() {
        Basis basis(1.935801664793151, 2.829683956491218, 14, 1.3);
        arma::vec r = {3.1, 2.3, 1.0, 0.0, 0.1, 4.3, 9.2, 13.7};
        arma::vec res00 = {8.08521235111303e-02, 1.43887615825118e-01, 2.55045100912706e-01,
                           2.91450097294984e-01, 2.91061479407116e-01, 2.47240792330589e-02,
                           3.63004153921473e-06, 3.87659726026123e-12};
        
        arma::vec result = basis.rPart(r, 0, 0);
        assertMatrixDelta(result, res00, 1e-15, "rPart(r, 0, 0)");
    });

    test.addTest("Basis #02 - rPart(r, 8, 2)", []() {
        Basis basis(1.935801664793151, 2.829683956491218, 14, 1.3);
        arma::vec r = {3.1, 2.3, 1.0, 0.0, 0.1, 4.3, 9.2, 13.7};
        arma::vec res82 = {5.87858442372438e-02, 1.35240488413384e-02, 4.06810074575519e-05,
                           0.00000000000000e+00, 4.92817669085478e-13, 8.52011998934850e-02,
                           5.20525909328609e-02, 1.44615166152252e-05};
        
        arma::vec result = basis.rPart(r, 8, 2);
        assertMatrixDelta(result, res82, 1e-15, "rPart(r, 8, 2)");
    });

    // ============================================================================
    // TEST #03 - Basis z-functions
    // ============================================================================
    
    test.addTest("Basis #03 - zPart(z, 0)", []() {
        Basis basis(1.935801664793151, 2.829683956491218, 14, 1.3);
        arma::vec z = {-10.1, -8.4, -1.0, 0.0, 0.1, 4.3, 9.2, 13.7};
        arma::vec res00 = {7.64546544834383e-04, 5.44886272162148e-03, 4.19492564268520e-01,
                           4.46522724110539e-01, 4.46243982300708e-01, 1.40736821086932e-01,
                           2.26186220733178e-03, 3.62929640195959e-06};
        
        arma::vec result = basis.zPart(z, 0);
        assertMatrixDelta(result, res00, 1e-15, "zPart(z, 0)");
    });

    test.addTest("Basis #03 - zPart(z, 15)", []() {
        Basis basis(1.935801664793151, 2.829683956491218, 14, 1.3);
        arma::vec z = {-10.1, -8.4, -1.0, 0.0, 0.1, 4.3, 9.2, 13.7};
        arma::vec res15 = {-9.48674551049192e-02, -1.40338701953237e-03, 1.85620628040096e-01,
                           -0.00000000000000e+00, -3.93028470685214e-02, -1.79526868763440e-01,
                            2.15604096600475e-01, 2.44977220882127e-01};
        
        arma::vec result = basis.zPart(z, 15);
        assertMatrixDelta(result, res15, 1e-15, "zPart(z, 15)");
    });

    // Exécuter tous les tests
    test.runAll();

    // Retourner le statut d'erreur
    return test.getFailedCount() > 0 ? 1 : 0;
}