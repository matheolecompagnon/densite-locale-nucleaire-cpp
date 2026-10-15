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
    int a = 0,b = 0;
    for (int m = 0; m < basis.mMax; m++)
    {
        for (int n = 0; n < basis.nMax(m); n++)
        {
            for (int n_z = 0; n_z < basis.n_zMax(m, n); n_z++)
            {

                b = 0;

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
                            
                            
                                result += funcA % funcB * rho(a, b);
                            

                            b++;


                        }
                    }
                }


                a++;

            }
        }
    }

    return result;
}

arma::mat rho_azimutal(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ){

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
    int a = 0,b = 0;
    for (int m = 0; m < basis.mMax; m++)
    {
        for (int n = 0; n < basis.nMax(m); n++)
        {
            for (int n_z = 0; n_z < basis.n_zMax(m, n); n_z++)
            {

                b = 0;

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
                            
                            if(m==mp) result += funcA % funcB * rho(a, b);
                            

                            b++;


                        }
                    }
                }


                a++;

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
    int a,b, offset;
    offset = 0;
    for (int m = 0; m < basis.mMax; m++)
    {

        int n_primMax = 0;
        if(0 < m) n_primMax = basis.nMax(m-1);
        for (int n_prim = 0; n_prim < n_primMax; ++n_prim) {
                // On somme les n_zMax pour chaque n (et non une multiplication)
                offset += basis.n_zMax(m, n_prim); 
        }
        

        a = offset;

        for (int n = 0; n < basis.nMax(m); n++)
        {
            for (int n_z = 0; n_z < basis.n_zMax(m, n); n_z++)
            {

                b = offset;

                    for (int np = 0; np < basis.nMax(m); np++)
                    {
                        for (int n_zp = 0; n_zp < basis.n_zMax(m, np); n_zp++)
                        {


                            arma::mat funcA = basis.basisFunc(m, n, n_z, zVals, rVals);
                            arma::mat funcB = basis.basisFunc(m, np, n_zp, zVals, rVals);

                            if (funcA.has_nan() || funcB.has_nan()) {
                                std::cout << "NaN détecté dans basisFunc pour m=" << m << ", n=" << n << ", n_z=" << n_z << std::endl;
                            }
                            
                            result += funcA % funcB * rho(a, b);

                            b++;
                            
                        }
                    }
                

                    a++;
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
    int a,b, offset;
    for (int m = 0; m < basis.mMax; m++)
    {

        offset = 0;
        for (int m_prim = 0; m_prim < m; ++m_prim) {
            int n_primMax = basis.nMax(m_prim);
            for (int n_prim = 0; n_prim < n_primMax; ++n_prim) {
                // On somme les n_zMax pour chaque n (et non une multiplication)
                offset += basis.n_zMax(m_prim, n_prim); 
            }
        }

        a = offset;

        for (int n = 0; n < basis.nMax(m); n++)
        {
            for (int n_z = 0; n_z < basis.n_zMax(m, n); n_z++)
            {

                b = offset;
                arma::mat funcA = basis.basisFunc(m, n, n_z, zVals, rVals);

                    for (int np = 0; np < basis.nMax(m); np++)
                    {
                        for (int n_zp = 0; n_zp < basis.n_zMax(m, np); n_zp++)
                        {



                            arma::mat funcB = basis.basisFunc(m, np, n_zp, zVals, rVals);

                            if (funcA.has_nan() || funcB.has_nan()) {
                                std::cout << "NaN détecté dans basisFunc pour m=" << m << ", n=" << n << ", n_z=" << n_z << std::endl;
                            }
                            
                            result += funcA % funcB * rho(a, b);

                            b++;
                            
                        }
                    }
                

                    a++;
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

arma::mat rho_symmetric(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ){

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
    if (rho.n_rows < (unsigned int)totalStates || rho.n_cols < (unsigned int)totalStates) {
        std::cerr << "Erreur : La matrice rho est trop petite !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    // 2. Créer le tableau de listes d'indices par bloc m
    std::vector<std::vector<int>> indices_par_m(basis.mMax);
    for (int idx = 0; idx < totalStates; ++idx) {
        indices_par_m[states[idx].m].push_back(idx);
    }

    arma::mat result = arma::zeros(nbR, nbZ);

    // 3. Boucle optimisée par blocs m et exploitant la symétrie i < j
    for (int m = 0; m < basis.mMax; ++m) {
        const auto& indices = indices_par_m[m];
        int size = indices.size();
        if (size == 0) continue;

        for (int idx_i = 0; idx_i < size; ++idx_i) {
            int i = indices[idx_i];
            
            // Calcul de funcA pour l'état i
            arma::mat funcA = basis.basisFunc(states[i].m, states[i].n, states[i].n_z, zVals, rVals);

            // A. Traitement du terme diagonal (i == j)
            double rho_ii = rho(i, i);
            if (rho_ii != 0.0) {
                result += (funcA % funcA) * rho_ii;
            }

            // B. Traitement des termes symétriques (j > i) -> On multiplie par 2
            for (int idx_j = idx_i + 1; idx_j < size; ++idx_j) {
                int j = indices[idx_j];
                double rho_ij = rho(i, j); // Égal à rho(j, i) par symétrie

                if (rho_ij != 0.0) {
                    arma::mat funcB = basis.basisFunc(states[j].m, states[j].n, states[j].n_z, zVals, rVals);
                    result += 2.0 * (funcA % funcB) * rho_ij;
                }
            }
        }
    }

    return result;
}

arma::mat rho_symmetric_optimized(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ){

    Basis basis(BR, BZ, N, Q);

    arma::mat rho;
    if (!rho.load("rho.arma", arma::arma_ascii)) {
        std::cerr << "Erreur : impossible de charger le fichier 'rho.arma' !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    std::vector<BasisState> states;
    for (int m = 0; m < basis.mMax; m++) {
        for (int n = 0; n < basis.nMax(m); n++) {
            for (int n_z = 0; n_z < basis.n_zMax(m, n); n_z++) {
                states.push_back({m, n, n_z});
            }
        }
    }

    int totalStates = states.size();
    if (rho.n_rows < (unsigned int)totalStates || rho.n_cols < (unsigned int)totalStates) {
        std::cerr << "Erreur : La matrice rho est trop petite !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    std::vector<std::vector<int>> indices_par_m(basis.mMax);
    for (int idx = 0; idx < totalStates; ++idx) {
        indices_par_m[states[idx].m].push_back(idx);
    }

    arma::mat result = arma::zeros(nbR, nbZ);
    
    // Variable temporaire pour éviter de réallouer des matrices de taille nbR x nbZ à chaque tour
    arma::mat term; 

    for (int m = 0; m < basis.mMax; ++m) {
        const auto& indices = indices_par_m[m];
        int size = indices.size();
        if (size == 0) continue;

        for (int idx_i = 0; idx_i < size; ++idx_i) {
            int i = indices[idx_i];
            
            // Calcul unique de funcA pour cet i
            arma::mat funcA = basis.basisFunc(states[i].m, states[i].n, states[i].n_z, zVals, rVals);

            double rho_ii = rho(i, i);
            if (rho_ii != 0.0) {
                result += (funcA % funcA) * rho_ii;
            }

            for (int idx_j = idx_i + 1; idx_j < size; ++idx_j) {
                int j = indices[idx_j];
                double rho_ij = rho(i, j);

                if (rho_ij != 0.0) {
                    arma::mat funcB = basis.basisFunc(states[j].m, states[j].n, states[j].n_z, zVals, rVals);
                    
                    // Utilisation d'expressions Armadillo optimisées
                    result += 2.0 * (funcA % funcB) * rho_ij;
                }
            }
        }
    }

    return result;
}




arma::mat rho_memory_max(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ){

    Basis basis(BR, BZ, N, Q);

    arma::mat rho;
    if (!rho.load("rho.arma", arma::arma_ascii)) {
        std::cerr << "Erreur : impossible de charger le fichier 'rho.arma' !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    std::vector<BasisState> states;
    std::vector<arma::mat> func;
    int i = 0;
    for (int m = 0; m < basis.mMax; m++) {
        for (int n = 0; n < basis.nMax(m); n++) {
            for (int n_z = 0; n_z < basis.n_zMax(m, n); n_z++) {
                states.push_back({m, n, n_z});
                func.push_back(basis.basisFunc(m, n, n_z, zVals, rVals));
                i++;
            }
        }
    }

    int totalStates = states.size();
    if (rho.n_rows < (unsigned int)totalStates || rho.n_cols < (unsigned int)totalStates) {
        std::cerr << "Erreur : La matrice rho est trop petite !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    std::vector<std::vector<int>> indices_par_m(basis.mMax);
    for (int idx = 0; idx < totalStates; ++idx) {
        indices_par_m[states[idx].m].push_back(idx);
    }


    arma::mat result = arma::zeros(nbR, nbZ);

    for (int m = 0; m < basis.mMax; ++m) {
        const auto& indices = indices_par_m[m];
        int size = indices.size();
        if (size == 0) continue;

        for (int idx_i = 0; idx_i < size; ++idx_i) {
            int i = indices[idx_i];
            

            double rho_ii = rho(i, i);
            result += (func[i] % func[i]) * rho_ii;
            

            for (int idx_j = idx_i + 1; idx_j < size; ++idx_j) {
                int j = indices[idx_j];
                double rho_ij = rho(i, j);

                result += 2.0 * (func[i]% func[j]) * rho_ij;
                
            }
        }
    }

    return result;
}

arma::mat rho_fabulous(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ){

    Basis basis(BR, BZ, N, Q);

    arma::mat rho;
    if (!rho.load("rho.arma", arma::arma_ascii)) {
        std::cerr << "Erreur : impossible de charger le fichier 'rho.arma' !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    std::vector<BasisState> states;
    for (int m = 0; m < basis.mMax; m++) {
        for (int n = 0; n < basis.nMax(m); n++) {
            for (int n_z = 0; n_z < basis.n_zMax(m, n); n_z++) {
                states.push_back({m, n, n_z});
            }
        }
    }

    

    int totalStates = states.size();
    if (rho.n_rows < (unsigned int)totalStates || rho.n_cols < (unsigned int)totalStates) {
        std::cerr << "Erreur : La matrice rho est trop petite !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    std::vector<std::vector<int>> indices_par_m(basis.mMax);
    for (int idx = 0; idx < totalStates; ++idx) {
        indices_par_m[states[idx].m].push_back(idx);
    }


    int max_K_m = 0;
    for (int m = 0; m < basis.mMax; m++) {
        if (indices_par_m[m].size() > (size_t)max_K_m) {
            max_K_m = indices_par_m[m].size();
        }
    }

    // Allouer une seule fois le buffer géant en dehors de la boucle
    arma::mat F_global(nbR * nbZ, max_K_m);

    arma::mat result = arma::zeros(nbR, nbZ);

for (int m = 0; m < basis.mMax; m++) {
    const auto& indices = indices_par_m[m];
    int K_m = indices.size();
    if (K_m == 0) continue;

    // On revient à l'allocation locale directe (qui était plus rapide en -O2)
    arma::mat F_m(nbR * nbZ, K_m);
    for (int k = 0; k < K_m; ++k) {
        int idx = indices[k];
        const auto& state = states[idx];
        F_m.col(k) = arma::vectorise(basis.basisFunc(state.m, state.n, state.n_z, zVals, rVals));
    }

    // 1. Extraire le sous-bloc rho_m
    arma::uvec idx_vec = arma::conv_to<arma::uvec>::from(indices);
    arma::mat rho_m = rho.submat(idx_vec, idx_vec);

    // 2. PRODUIT INTERMÉDIAIRE (Taille : (nbR*nbZ) x K_m)
    arma::mat A = F_m * rho_m;

    // 3. EXTRACTION DIRECTE DE LA DIAGONALE (Zéro matrice S x S en mémoire !)
    // Le symbole % fait un produit élément par élément (Hadamard), et arma::sum somme sur les colonnes
    arma::vec diag_R_m = arma::sum(A % F_m, 1);

    // 4. Redimensionner le vecteur diagonale en grille 2D et accumuler
    result += arma::reshape(diag_R_m, nbR, nbZ);
}
    return result;
}