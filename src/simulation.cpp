#include "basis.hpp"
#include "simulation.hpp"

#include <cstdint> // Pour uint64_t
#include <vector>

#define BZ 1.935801664793151
#define BR 2.829683956491218
#define N 14
#define Q 1.3



arma::mat rho_naif(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ){

    Basis basis(BR, BZ, N, Q);

    arma::mat rho;
    if (!rho.load("rho.arma", arma::arma_ascii)) {
        std::cerr << "Erreur : impossible de charger le fichier 'rho.arma' !" << std::endl;
        // Optionnel : créer une matrice par défaut pour tester
        // rho = arma::eye(basis.mMax, basis.mMax);
        return arma::zeros(nbR, nbZ);
    }

    std::cout << "Dimensions de rho chargées : " << rho.n_rows << " x " << rho.n_cols << std::endl;
    std::cout << "mMax attendu : " << basis.mMax << std::endl;

    if (rho.n_rows < (unsigned int)basis.mMax || rho.n_cols < (unsigned int)basis.mMax) {
        std::cerr << "Erreur : La matrice rho est trop petite par rapport à mMax !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    arma::mat result = arma::zeros(nbR, nbZ);

    for (int m = 0; m < basis.mMax; m++)
    {
        for (int n = 0; n < basis.nMax(m); n++)
        {
            for (int n_z = 0; n_z < basis.n_zMax(m, n); n_z++)
            {
                for (int mp = 0; mp < basis.mMax; mp++)
                {
                    for (int np = 0; np < basis.nMax(mp); np++)
                    {
                        for (int n_zp = 0; n_zp < basis.n_zMax(mp, np); n_zp++)
                        {
                            arma::mat funcA = basis.basisFunc(m, n, n_z, zVals, rVals);
                            arma::mat funcB = basis.basisFunc(mp, np, n_zp, zVals, rVals);

                            if (funcA.has_nan() || funcB.has_nan()) {
                                std::cout << "NaN détecté dans basisFunc pour m=" << m << ", n=" << n << ", n_z=" << n_z << std::endl;
                            }
                            
                            if(m == mp) {
                                result += funcA % funcB * rho(m, mp);
                            }
                        }
                    }
                }
            }
        }
    }

    return result;
}

arma::mat rho_kronecker(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ){

    Basis basis(BR, BZ, N, Q);

    arma::mat rho;
    if (!rho.load("rho.arma", arma::arma_ascii)) {
        std::cerr << "Erreur : impossible de charger le fichier 'rho.arma' !" << std::endl;
        // Optionnel : créer une matrice par défaut pour tester
        // rho = arma::eye(basis.mMax, basis.mMax);
        return arma::zeros(nbR, nbZ);
    }

    std::cout << "Dimensions de rho chargées : " << rho.n_rows << " x " << rho.n_cols << std::endl;
    std::cout << "mMax attendu : " << basis.mMax << std::endl;

    if (rho.n_rows < (unsigned int)basis.mMax || rho.n_cols < (unsigned int)basis.mMax) {
        std::cerr << "Erreur : La matrice rho est trop petite par rapport à mMax !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    arma::mat result = arma::zeros(nbR, nbZ);

    for (int m = 0; m < basis.mMax; m++)
    {
        for (int n = 0; n < basis.nMax(m); n++)
        {
            for (int n_z = 0; n_z < basis.n_zMax(m, n); n_z++)
            {

                    for (int np = 0; np < basis.nMax(m); np++)
                    {
                        for (int n_zp = 0; n_zp < basis.n_zMax(m, np); n_zp++)
                        {
                            arma::mat funcA = basis.basisFunc(m, n, n_z, zVals, rVals);
                            arma::mat funcB = basis.basisFunc(m, np, n_zp, zVals, rVals);

                            if (funcA.has_nan() || funcB.has_nan()) {
                                std::cout << "NaN détecté dans basisFunc pour m=" << m << ", n=" << n << ", n_z=" << n_z << std::endl;
                            }
                            
                            result += funcA % funcB * rho(m, m);
                            
                        }
                    }
                
            }
        }
    }

    return result;
}

arma::mat rho_factorized(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ){

    Basis basis(BR, BZ, N, Q);

    arma::mat rho;
    if (!rho.load("rho.arma", arma::arma_ascii)) {
        std::cerr << "Erreur : impossible de charger le fichier 'rho.arma' !" << std::endl;
        // Optionnel : créer une matrice par défaut pour tester
        // rho = arma::eye(basis.mMax, basis.mMax);
        return arma::zeros(nbR, nbZ);
    }

    std::cout << "Dimensions de rho chargées : " << rho.n_rows << " x " << rho.n_cols << std::endl;
    std::cout << "mMax attendu : " << basis.mMax << std::endl;

    if (rho.n_rows < (unsigned int)basis.mMax || rho.n_cols < (unsigned int)basis.mMax) {
        std::cerr << "Erreur : La matrice rho est trop petite par rapport à mMax !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    arma::mat result = arma::zeros(nbR, nbZ);

    for (int m = 0; m < basis.mMax; m++)
    {
        for (int n = 0; n < basis.nMax(m); n++)
        {
            for (int n_z = 0; n_z < basis.n_zMax(m, n); n_z++)
            {
                    arma::mat funcA = basis.basisFunc(m, n, n_z, zVals, rVals);


                    for (int np = 0; np < basis.nMax(m); np++)
                    {
                        for (int n_zp = 0; n_zp < basis.n_zMax(m, np); n_zp++)
                        {

                            arma::mat funcB = basis.basisFunc(m, np, n_zp, zVals, rVals);

                            if (funcA.has_nan() || funcB.has_nan()) {
                                std::cout << "NaN détecté dans basisFunc pour m=" << m << ", n=" << n << ", n_z=" << n_z << std::endl;
                            }
                            
                            result += funcA % funcB * rho(m, m);
                            
                        }
                    }
                
            }
        }
    }

    return result;
}



struct BasisState {
    int m;
    int n;
    int n_z;
};

arma::mat rho_linearized(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ){

    Basis basis(BR, BZ, N, Q);

    arma::mat rho;
    if (!rho.load("rho.arma", arma::arma_ascii)) {
        std::cerr << "Erreur : impossible de charger le fichier 'rho.arma' !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    // 1. Remplir le petit tableau des triplets quantiques (très léger en mémoire)
    std::vector<BasisState> states;
    for (int m = 0; m < basis.mMax; m++) {
        for (int n = 0; n < basis.nMax(m); n++) {
            for (int n_z = 0; n_z < basis.n_zMax(m, n); n_z++) {
                states.push_back({m, n, n_z});
            }
        }
    }

    int totalStates = states.size();
    std::cout << "Nombre total d'états : " << totalStates << std::endl;

    if (rho.n_rows < (unsigned int)totalStates || rho.n_cols < (unsigned int)totalStates) {
        std::cerr << "Erreur : La matrice rho est trop petite !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    arma::mat result = arma::zeros(nbR, nbZ);

    // 2. Les deux boucles i et j
    for (int i = 0; i < totalStates; ++i) {
        int m = states[i].m;
        int n = states[i].n;
        int n_z = states[i].n_z;

        // Calcul de funcA à la volée pour l'état i
        arma::mat funcA = basis.basisFunc(m, n, n_z, zVals, rVals);

        for (int j = 0; j < totalStates; ++j) {
            int mp = states[j].m;

            // Condition : rho vaut 0 quand m != mp
            if (m == mp) {
                double rho_val = rho(i, j);
                
                
                int np = states[j].n;
                int n_zp = states[j].n_z;

                // Calcul de funcB à la volée pour l'état j
                arma::mat funcB = basis.basisFunc(mp, np, n_zp, zVals, rVals);
                    
                result += funcA % funcB * rho_val;
                
            }
        }
    }

    return result;
}


arma::mat rho_linearized_again(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ){

    Basis basis(BR, BZ, N, Q);

    arma::mat rho;
    if (!rho.load("rho.arma", arma::arma_ascii)) {
        std::cerr << "Erreur : impossible de charger le fichier 'rho.arma' !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    // 1. Remplir le petit tableau des triplets quantiques
    std::vector<BasisState> states;
    for (int m = 0; m < basis.mMax; m++) {
        for (int n = 0; n < basis.nMax(m); n++) {
            for (int n_z = 0; n_z < basis.n_zMax(m, n); n_z++) {
                states.push_back({m, n, n_z});
            }
        }
    }

    int totalStates = states.size();
    std::cout << "Nombre total d'états : " << totalStates << std::endl;

    if (rho.n_rows < (unsigned int)totalStates || rho.n_cols < (unsigned int)totalStates) {
        std::cerr << "Erreur : La matrice rho est trop petite !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    arma::mat result = arma::zeros(nbR, nbZ);

    // 2. Boucle linéarisée avec uint64_t et déclaration propre de i et j
    uint64_t totalIterations = (uint64_t)totalStates * (uint64_t)totalStates;
    
    for (uint64_t a = 0; a < totalIterations; ++a) {
        int i = a / totalStates;
        int j = a % totalStates; 

        int m = states[i].m;
        int mp = states[j].m;

        // Condition : rho vaut 0 quand m != mp (on filtre tout de suite pour gagner du temps)
        if (m == mp) {
            double rho_val = rho(i, j);
            
            if (rho_val != 0.0) {
                int n = states[i].n;
                int n_z = states[i].n_z;
                int np = states[j].n;
                int n_zp = states[j].n_z;

                // Calculs uniquement si la condition est remplie
                arma::mat funcA = basis.basisFunc(m, n, n_z, zVals, rVals);
                arma::mat funcB = basis.basisFunc(mp, np, n_zp, zVals, rVals);
                
                result += funcA % funcB * rho_val;
            }
        }
    }

    return result;
}


#include <vector>

arma::mat rho_optimized_m(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ){

    Basis basis(BR, BZ, N, Q);

    arma::mat rho;
    if (!rho.load("rho.arma", arma::arma_ascii)) {
        std::cerr << "Erreur : impossible de charger le fichier 'rho.arma' !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    // 1. Remplir le tableau des triplets quantiques
    std::vector<BasisState> states;
    for (int m = 0; m < basis.mMax; m++) {
        for (int n = 0; n < basis.nMax(m); n++) {
            for (int n_z = 0; n_z < basis.n_zMax(m, n); n_z++) {
                states.push_back({m, n, n_z});
            }
        }
    }

    int totalStates = states.size();
    std::cout << "Nombre total d'états : " << totalStates << std::endl;

    if (rho.n_rows < (unsigned int)totalStates || rho.n_cols < (unsigned int)totalStates) {
        std::cerr << "Erreur : La matrice rho est trop petite !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    // 2. Créer le tableau de tableaux : pour chaque m, on stocke la liste des indices j correspondants
    std::vector<std::vector<int>> indices_par_m(basis.mMax);
    for (int j = 0; j < totalStates; ++j) {
        int mp = states[j].m;
        indices_par_m[mp].push_back(j);
    }

    arma::mat result = arma::zeros(nbR, nbZ);

    // 3. Boucle principale sur i
    for (int i = 0; i < totalStates; ++i) {
        int m = states[i].m;
        int n = states[i].n;
        int n_z = states[i].n_z;

        // Calcul de funcA à la volée pour l'état i
        arma::mat funcA = basis.basisFunc(m, n, n_z, zVals, rVals);

        // 4. Boucle restreinte : on ne parcourt QUE les j qui ont exactement le même m
        for (int j : indices_par_m[m]) {
            double rho_val = rho(i, j);
            
            if (rho_val != 0.0) {
                int np = states[j].n;
                int n_zp = states[j].n_z;

                // Calcul de funcB pour l'état j
                arma::mat funcB = basis.basisFunc(m, np, n_zp, zVals, rVals);
                
                result += funcA % funcB * rho_val;
            }
        }
    }

    return result;
}