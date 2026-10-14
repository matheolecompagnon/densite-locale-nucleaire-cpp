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

int main() {
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

    return 0;
}