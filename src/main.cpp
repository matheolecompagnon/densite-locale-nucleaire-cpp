/**
 * @file main.cpp
 * @author Mathéo LECOMPAGNON
 * @brief Fichier de test et de benchmark des performances pour le calcul de rho.
 */

#include <iostream>
#include <armadillo>
#include "simulation.hpp" 
#include <cstdlib>


int main() {
    try {
      // Force OpenBLAS à utiliser tous les cœurs disponibles
      setenv("OPENBLAS_NUM_THREADS", "8", 1);
      setenv("MKL_NUM_THREADS", "8", 1);
      setenv("OMP_NUM_THREADS", "8", 1);
        int nbR = 300;
        int nbZ = 300;

        // Création des grilles de points pour r et z
        arma::vec rVals = arma::linspace(0.0, 5.0, nbR);
        arma::vec zVals = arma::linspace(-5.0, 5.0, nbZ);

        std::cout << "=== Lancement du Benchmark ===" << std::endl;
        std::cout << "Grille : " << nbR << " x " << nbZ << " points." << std::endl;
        std::cout << "----------------------------------------" << std::endl;

        // Chronomètre Armadillo
        arma::wall_clock timer;

        // 1. Benchmark de rho_naif
        timer.tic();
        arma::mat rho_naive = rho_naif(rVals, zVals, nbR, nbZ);
        double time_naif = timer.toc();
        std::cout << "[1/2] rho_naif exécuté en : " << time_naif << " secondes." << std::endl;

        // 2. Benchmark de rho_memory_max
        timer.tic();
        arma::mat rho_optimized = rho_fabulous(rVals, zVals, nbR, nbZ);
        double time_opt = timer.toc();
        std::cout << "[2/2] rho_memory_max exécuté en : " << time_opt << " secondes." << std::endl;

        std::cout << "----------------------------------------" << std::endl;

        // 3. Calcul du facteur d'accélération (Speedup)
        if (time_opt > 0.0) {
            double speedup = time_naif / time_opt;
            std::cout << ">>> RÉSULTAT DU BENCHMARK <<<" << std::endl;
            std::cout << "rho_memory_max est " << speedup << " fois plus rapide que rho_naif !" << std::endl;
            std::cout << "Gain de temps : " << ((time_naif - time_opt) / time_naif) * 100.0 << "%" << std::endl;
        } else {
            std::cout << "Calculs trop rapides pour mesurer un speedup significatif." << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}