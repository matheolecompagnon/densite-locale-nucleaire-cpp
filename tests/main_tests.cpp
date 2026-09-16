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

int main(void){
    return 0;
}