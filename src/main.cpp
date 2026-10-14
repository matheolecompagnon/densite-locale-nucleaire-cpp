/**
 * @file main.cpp
 * @author Mathéo LECOMPAGNON
 * @brief Ce fichier décrit la boucle principale du programme.
 * @version 0.1
 * @date 2026-09-09
 * @copyright Copyright (c) 2026
 */

#include <iostream>
#include <armadillo>
#include "simulation.hpp" 

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
        int nbR = 10;
        int nbZ = 10;

        // Création de 10 valeurs pour r (ex: de 0.0 à 5.0) et z (ex: de -5.0 à 5.0)
        arma::vec rVals = arma::linspace(0.0, 5.0, nbR);
        arma::vec zVals = arma::linspace(-5.0, 5.0, nbZ);

        std::cout << "=== Test de rho_naif ===" << std::endl;
        std::cout << "Génération de " << nbR << " points pour r et " << nbZ << " points pour z." << std::endl;

        // Appel de la fonction rho_naif avec les paramètres requis
        arma::mat rho = rho_naif(rVals, zVals, nbR, nbZ);

        std::cout << "rho_naif exécuté avec succès !" << std::endl;
        std::cout << "Dimensions de la matrice résultat : " << rho.n_rows << " x " << rho.n_cols << std::endl;

        // Vérification et affichage selon l'état de la matrice
        if (rho.is_zero()) {
            std::cout << "Information : La matrice résultat est entièrement nulle." << std::endl;
        } else {
            std::cout << "Affichage de la matrice résultat :" << std::endl;
            rho.print();
        }

    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 1;
    }

    #endif

    std::cout << "Temps d'execution : " << tps_exec.toc() << " s" << std::endl;

    return 0;
}