#ifndef __BENCH_HPP__
#define __BENCH_HPP__

/**
 * @file bench.cpp
 * @author Mathéo LECOMPAGNON
 * @author Jean-Marie ASSOGBA
 * @brief Ce fichier décrit l'entête de la classe Bench.
 * @version 0.1
 * @date 2026-09-09
 * @copyright Copyright (c) 2026
 */

#include <armadillo>
#include <iostream>
#include <iomanip>

class Bench {
    public:
        /**
         * @brief Timer pour compter le temps écoulé pendant une mesure
         * 
         */
        arma::wall_clock timer;
        /**
         * @brief Compteur de temps écoulé pour toutes les mesures
         * 
         */
        double total = 0.0;
        /**
         * @brief Compteur du nombre de mesures effectuées
         * 
         */
        unsigned long calls = 0;

        /**
         * @brief Commence le compteur
         * 
         */
        void start(void){ timer.tic(); }

        /**
         * @brief Termine le compteur en relevant la mesure
         * 
         */
        void end(void){ total+=timer.toc(); calls++; }

         /**
          * @brief Remet les compteurs à zéro
          * 
          */
        void reset(void){total=0.0; calls=0; }

         /**
          * @brief affiche la ligne du benchmark de la fonctionnalité name
          * @param name nom de la fonctionnalité à tester
          */
        void print(const char* name) {
            std::cout << "| " << name << " |    " << std::fixed << std::setprecision(6) << total / (double)calls << "    |" << std::endl;
        }

};

#ifdef BENCH
    #define BENCH_START(b) (b).start()
    #define BENCH_END(b) (b).end()
    #define BENCH_RESET(b) (b).reset()
    #define BENCH_PRINT(b, name) (b).print(name)
#else
    #define BENCH_START(b) ((void)0)
    #define BENCH_END(b) ((void)0)
    #define BENCH_RESET(b) ((void)0)
    #define BENCH_PRINT(b, name) ((void)0)
#endif

#endif

