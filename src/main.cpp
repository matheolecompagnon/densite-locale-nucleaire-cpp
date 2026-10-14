/**
 * @file main.cpp
 * @author Mathéo LECOMPAGNON
 * @brief Fichier de test et de benchmark des performances pour le calcul de rho.
 */

#include <iostream>
#include <armadillo>
#include "simulation.hpp" 
#include <cstdlib>


#ifdef BENCH

    #include "bench.hpp"

    extern Bench bench_naif, bench_kronecker, bench_factorized, bench_linearized, bench_linearized_again, 
    bench_optimized_m;

#endif

int main() {

    arma::wall_clock tps_exec;
    tps_exec.tic();

    #ifdef BENCH

        // On déterminise
        arma::arma_rng::set_seed(3);

        // Initialisation des positions pour 50*50 positions différentes : 2500
        int nbR = 50; 
        int nbZ = 50;
        arma::vec rVals = arma::linspace<arma::vec>(0.1, 10.0, nbR);
        arma::vec zVals = arma::linspace<arma::vec>(-5.0, 5.0, nbZ);

        // Résultats
        arma::mat res_naif, res_kron, res_fact, res_lin, res_lin_again, res_opt_m; 
        // Permet d'éviter l'évaluation paresseuse d'armadillo
        double anti_optimization_checksum = 0.0;

        // Nombre d'itérations
        int nb_warmup = 10;
        int nb_iter = 10;

        // Préchauffe (Warmup)
        for (int i=0; i<nb_warmup; i++){
            res_naif = rho_naif(rVals, zVals, nbR, nbZ);
            res_kron = rho_kronecker(rVals, zVals, nbR, nbZ);
            res_fact = rho_factorized(rVals, zVals, nbR, nbZ);
            res_lin = rho_linearized(rVals, zVals, nbR, nbZ);
            res_lin_again = rho_linearized_again(rVals, zVals, nbR, nbZ);
            res_opt_m = rho_optimized_m(rVals, zVals, nbR, nbZ);
            anti_optimization_checksum += arma::norm(res_naif, 1) 
                                + arma::norm(res_kron, 1) 
                                + arma::norm(res_fact, 1) 
                                + arma::norm(res_lin, 1) 
                                + arma::norm(res_lin_again, 1) 
                                + arma::norm(res_opt_m, 1);
        }

        // Compteurs à zéro
        BENCH_RESET(bench_naif);
        BENCH_RESET(bench_kronecker);
        BENCH_RESET(bench_factorized);
        BENCH_RESET(bench_linearized);
        BENCH_RESET(bench_linearized_again);
        BENCH_RESET(bench_optimized_m);

        // Mesures
        
        // Benchmarks en appel de fonction
        Bench bench_naif_call, bench_kronecker_call, bench_factorized_call, bench_linearized_call, 
        bench_linearized_again_call, bench_optimized_m_call;

        for (int i=0; i<nb_iter; i++){
            BENCH_START(bench_naif_call);
            res_naif = rho_naif(rVals, zVals, nbR, nbZ);
            BENCH_END(bench_naif_call);
            BENCH_START(bench_kronecker_call);
            res_kron = rho_kronecker(rVals, zVals, nbR, nbZ);
            BENCH_END(bench_kronecker_call);
            BENCH_START(bench_factorized_call);
            res_fact = rho_factorized(rVals, zVals, nbR, nbZ);
            BENCH_END(bench_factorized_call);
            BENCH_START(bench_linearized_call);
            res_lin = rho_linearized(rVals, zVals, nbR, nbZ);
            BENCH_END(bench_linearized_call);
            BENCH_START(bench_linearized_again_call);
            res_lin_again = rho_linearized_again(rVals, zVals, nbR, nbZ);
            BENCH_END(bench_linearized_again_call);
            BENCH_START(bench_optimized_m_call);
            res_opt_m = rho_optimized_m(rVals, zVals, nbR, nbZ);
            BENCH_END(bench_optimized_m_call);
            anti_optimization_checksum += arma::norm(res_naif, 1) 
                                + arma::norm(res_kron, 1) 
                                + arma::norm(res_fact, 1) 
                                + arma::norm(res_lin, 1) 
                                + arma::norm(res_lin_again, 1) 
                                + arma::norm(res_opt_m, 1);
        }

        std::cout << "============== BENCHMARK ===============" << std::endl;
        
        std::cout << "Nombre de préchauffes : " << nb_warmup << " itérations" << std::endl;
        std::cout << "Nombre de mesures : " << nb_iter << " itérations" << std::endl;

        std::cout << std::endl << "Algorithme seulement :" << std::endl << std::endl;
        std::cout << "| Algorithme    | temps (s/res.) |" << std::endl;
        std::cout << "|:--------------|:---------------|" << std::endl;
        BENCH_PRINT(bench_naif,
            "Naïf         ");
        BENCH_PRINT(bench_kronecker, 
            "Kroenecker   ");
        BENCH_PRINT(bench_factorized, 
            "Factorized   ");
        BENCH_PRINT(bench_linearized, 
            "Linearized   ");
        BENCH_PRINT(bench_linearized_again, 
            "Linearized 2.");
        BENCH_PRINT(bench_optimized_m, 
            "Optimized    ");

        std::cout << std::endl << "Appel fonction de l'algorithme :" << std::endl << std::endl;
        std::cout << "| Algorithme    | temps (s/res.) |" << std::endl;
        std::cout << "|:--------------|:---------------|" << std::endl;
        BENCH_PRINT(bench_naif_call,
            "Naïf         ");
        BENCH_PRINT(bench_kronecker_call, 
            "Kroenecker   ");
        BENCH_PRINT(bench_factorized_call, 
            "Factorized   ");
        BENCH_PRINT(bench_linearized_call, 
            "Linearized   ");
        BENCH_PRINT(bench_linearized_again_call, 
            "Linearized 2.");
        BENCH_PRINT(bench_optimized_m_call, 
            "Optimized    ");
        
        std::cout << "============ FIN BENCHMARK =============" << std::endl;

    #else

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

    #endif

    std::cout << "Temps d'execution : " << tps_exec.toc() << " s" << std::endl;

    return 0;
}