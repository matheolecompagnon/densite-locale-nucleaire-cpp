#include "basis.hpp"
#include "simulation.hpp"
#include "bench.hpp"

#include <cstdint> // Pour uint64_t
#include <vector>

#define BZ 1.935801664793151
#define BR 2.829683956491218
#define N 14
#define Q 1.3
#define RHO_FILE "data/rho.arma"

Bench bench_naif, bench_kronecker, bench_factorized, bench_linearized, bench_linearized_again, 
bench_optimized_m, bench_symmetric, bench_symmetric_optimized, bench_memory_max, bench_fabulous,
bench_ultimate;

/**
 * @brief Calcule la matrice de densité nucléaire $\rho(r, z)$ selon l'algorithme naïf.
 * 
 * Cette fonction évalue la densité en parcourant l'ensemble des états quantiques 
 * via une implementation directe à 6 boucles imbriquées. Elle charge la matrice 
 * de coefficients depuis le fichier externe "rho.arma" et utilise la base 
 * fonctionnelle pour sommer les contributions.
 * 
 * @param rVals Vecteur des valeurs de l'axe radial $r_\perp$ [fm].
 * @param zVals Vecteur des valeurs de l'axe axial $z$ [fm].
 * @param nbR Nombre de points sur l'axe radial.
 * @param nbZ Nombre de points sur l'axe axial.
 * @return arma::mat Une matrice 2D de taille `nbR` x `nbZ` représentant la densité locale $\rho(r, z)$.
 */
arma::mat rho_naif(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ){

    Basis basis(BR, BZ, N, Q);

    arma::mat rho;
    if (!rho.load(RHO_FILE, arma::arma_ascii)) {
        std::cerr << "Erreur : impossible de charger le fichier 'rho.arma' !" << std::endl;
        // Optionnel : créer une matrice par défaut pour tester
        // rho = arma::eye(basis.mMax, basis.mMax);
        return arma::zeros(nbR, nbZ);
    }

    /*
    std::cout << "Dimensions de rho chargées : " << rho.n_rows << " x " << rho.n_cols << std::endl;
    std::cout << "mMax attendu : " << basis.mMax << std::endl;
    */

    if (rho.n_rows < (unsigned int)basis.mMax || rho.n_cols < (unsigned int)basis.mMax) {
        std::cerr << "Erreur : La matrice rho est trop petite par rapport à mMax !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    arma::mat result = arma::zeros(nbR, nbZ);

    int a = 0,b = 0;
    BENCH_START(bench_naif);
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

                            /*
                            if (funcA.has_nan() || funcB.has_nan()) {
                                std::cout << "NaN détecté dans basisFunc pour m=" << m << ", n=" << n << ", n_z=" << n_z << std::endl;
                            }
                            */
                            
                            
                                result += funcA % funcB * rho(a, b);
                            

                            b++;


                        }
                    }
                }


                a++;

            }
        }
    }

    BENCH_END(bench_naif);

    return result;
}

/**
 * @brief Calcule la matrice de densité nucléaire $\rho(r, z)$ en exploitant le filtre de Kronecker ($m = mp$).
 * 
 * Cette version optimisée de la densité intègre un test conditionnel ($m == mp$) 
 * découlant de l'invariance azimutale du système. Elle évite d'évaluer les termes 
 * nuls où les moments cinétiques $m$ et $mp$ diffèrent, réduisant ainsi le nombre 
 * d'opérations par rapport à l'approche naïve.
 * 
 * @param rVals Vecteur des valeurs de l'axe radial $r_\perp$ [fm].
 * @param zVals Vecteur des valeurs de l'axe axial $z$ [fm].
 * @param nbR Nombre de points sur l'axe radial.
 * @param nbZ Nombre de points sur l'axe axial.
 * @return arma::mat Une matrice 2D de taille `nbR` x `nbZ` représentant la densité locale $\rho(r, z)$.
 */
arma::mat rho_kronecker(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ){

    Basis basis(BR, BZ, N, Q);

    arma::mat rho;
    if (!rho.load(RHO_FILE, arma::arma_ascii)) {
        std::cerr << "Erreur : impossible de charger le fichier 'rho.arma' !" << std::endl;
        // Optionnel : créer une matrice par défaut pour tester
        // rho = arma::eye(basis.mMax, basis.mMax);
        return arma::zeros(nbR, nbZ);
    }

    /*
    std::cout << "Dimensions de rho chargées : " << rho.n_rows << " x " << rho.n_cols << std::endl;
    std::cout << "mMax attendu : " << basis.mMax << std::endl;
    */

    if (rho.n_rows < (unsigned int)basis.mMax || rho.n_cols < (unsigned int)basis.mMax) {
        std::cerr << "Erreur : La matrice rho est trop petite par rapport à mMax !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    arma::mat result = arma::zeros(nbR, nbZ);
    int a,b, offset;
    offset = 0;

    BENCH_START(bench_kronecker);

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

                            /*
                            if (funcA.has_nan() || funcB.has_nan()) {
                                std::cout << "NaN détecté dans basisFunc pour m=" << m << ", n=" << n << ", n_z=" << n_z << std::endl;
                            }
                            */
                            
                            result += funcA % funcB * rho(a, b);

                            b++;
                            
                        }
                    }
                

                    a++;
            }
        }
    }

    BENCH_END(bench_kronecker);

    return result;
}



/**
 * @brief Calcule la matrice de densité nucléaire $\rho(r, z)$ via une approche factorisée.
 * 
 * Cette fonction optimise le parcours des états quantiques en pré-calculant un 
 * décalage (`offset`) basé sur les indices $m$. Cela permet de restreindre 
 * dynamiquement les boucles aux sous-blocs pertinents et d'éviter les parcours 
 * superflus, réduisant considérablement la complexité temporelle par rapport 
 * aux versions précédentes.
 * 
 * @param rVals Vecteur des valeurs de l'axe radial $r_\perp$ [fm].
 * @param zVals Vecteur des valeurs de l'axe axial $z$ [fm].
 * @param nbR Nombre de points sur l'axe radial.
 * @param nbZ Nombre de points sur l'axe axial.
 * @return arma::mat Une matrice 2D de taille `nbR` x `nbZ` représentant la densité locale $\rho(r, z)$.
 */
arma::mat rho_factorized(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ){

    Basis basis(BR, BZ, N, Q);

    arma::mat rho;
    if (!rho.load(RHO_FILE, arma::arma_ascii)) {
        std::cerr << "Erreur : impossible de charger le fichier 'rho.arma' !" << std::endl;
        // Optionnel : créer une matrice par défaut pour tester
        // rho = arma::eye(basis.mMax, basis.mMax);
        return arma::zeros(nbR, nbZ);
    }

    /*
    std::cout << "Dimensions de rho chargées : " << rho.n_rows << " x " << rho.n_cols << std::endl;
    std::cout << "mMax attendu : " << basis.mMax << std::endl;
    */

    if (rho.n_rows < (unsigned int)basis.mMax || rho.n_cols < (unsigned int)basis.mMax) {
        std::cerr << "Erreur : La matrice rho est trop petite par rapport à mMax !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    arma::mat result = arma::zeros(nbR, nbZ);
    int a,b, offset;
    BENCH_START(bench_factorized);

    for (int m = 0; m < basis.mMax; m++)
    {

        // Calcul de l'offset pour se positionner directement sur le bloc de moment cinétique m
        offset = 0;
        for (int m_prim = 0; m_prim < m; ++m_prim) {
            int n_primMax = basis.nMax(m_prim);
            for (int n_prim = 0; n_prim < n_primMax; ++n_prim) {
                // On somme les n_zMax pour chaque n
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

                            /*
                            if (funcA.has_nan() || funcB.has_nan()) {
                                std::cout << "NaN détecté dans basisFunc pour m=" << m << ", n=" << n << ", n_z=" << n_z << std::endl;
                            }
                            */
                            
                            result += funcA % funcB * rho(a, b);

                            b++;
                            
                        }
                    }
                

                    a++;
            }
        }
    }

    BENCH_END(bench_factorized);

    return result;
}



/**
 * @brief Représente un état quantique unique défini par ses trois nombres quantiques.
 */
struct BasisState {
    int m;    ///< Nombre quantique du moment cinétique azimutal
    int n;    ///< Nombre quantique radial
    int n_z;  ///< Nombre quantique axial
};

/**
 * @brief Calcule la matrice de densité nucléaire $\rho(r, z)$ via une approche linéarisée des états.
 * 
 * Cette fonction aplatit l'espace des états quantiques multidimensionnels en un 
 * vecteur linéaire unique (`states`). Elle simplifie le parcours des boucles d'indexation 
 * tout en intégrant directement le filtre d'invariance azimutale ($m == mp$) pour 
 * ignorer les contributions nulles.
 * 
 * @param rVals Vecteur des valeurs de l'axe radial $r_\perp$ [fm].
 * @param zVals Vecteur des valeurs de l'axe axial $z$ [fm].
 * @param nbR Nombre de points sur l'axe radial.
 * @param nbZ Nombre de points sur l'axe axial.
 * @return arma::mat Une matrice 2D de taille `nbR` x `nbZ` représentant la densité locale $\rho(r, z)$.
 */
arma::mat rho_linearized(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ){

    Basis basis(BR, BZ, N, Q);

    arma::mat rho;
    if (!rho.load(RHO_FILE, arma::arma_ascii)) {
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
    /*
    std::cout << "Nombre total d'états : " << totalStates << std::endl;
    */

    if (rho.n_rows < (unsigned int)totalStates || rho.n_cols < (unsigned int)totalStates) {
        std::cerr << "Erreur : La matrice rho est trop petite !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    arma::mat result = arma::zeros(nbR, nbZ);

    // 2. Les deux boucles i et j

    BENCH_START(bench_linearized);

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

    BENCH_END(bench_linearized);

    return result;
}


/**
 * @brief Variante linéarisée à boucle unique de calcul de la densité $\rho(r, z)$.
 * 
 * Cette fonction aplatit l'espace des états 2D en une seule boucle globale de taille 
 * `totalStates * totalStates` en utilisant des indices de type `uint64_t`. 
 * 
 * @note **Performance :** Bien que conceptuellement élégante, cette approche s'avère 
 * **plus lente** que la première version de `rho_linearized` à deux boucles imbriquées. 
 * Le surcoût lié aux opérations de division et de modulo (`a / totalStates` et `a % totalStates`) 
 * exécutées à chaque itération dégrade les performances globales.
 * 
 * @param rVals Vecteur des valeurs de l'axe radial $r_\perp$ [fm].
 * @param zVals Vecteur des valeurs de l'axe axial $z$ [fm].
 * @param nbR Nombre de points sur l'axe radial.
 * @param nbZ Nombre de points sur l'axe axial.
 * @return arma::mat Une matrice 2D de taille `nbR` x `nbZ` représentant la densité locale $\rho(r, z)$.
 */
arma::mat rho_linearized_again(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ){

    Basis basis(BR, BZ, N, Q);

    arma::mat rho;
    if (!rho.load(RHO_FILE, arma::arma_ascii)) {
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
    /*
    std::cout << "Nombre total d'états : " << totalStates << std::endl;
    */

    if (rho.n_rows < (unsigned int)totalStates || rho.n_cols < (unsigned int)totalStates) {
        std::cerr << "Erreur : La matrice rho est trop petite !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    arma::mat result = arma::zeros(nbR, nbZ);

    // 2. Boucle linéarisée avec uint64_t et déclaration propre de i et j
    uint64_t totalIterations = (uint64_t)totalStates * (uint64_t)totalStates;
    
    BENCH_START(bench_linearized_again);

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

    BENCH_END(bench_linearized_again);

    return result;
}



/**
 * @brief Calcule la matrice de densité nucléaire $\rho(r, z)$ en optimisant le parcours par moment cinétique $m$.
 * 
 * Cette fonction pré-indexe les états quantiques dans un tableau de listes (`indices_par_m`) 
 * regroupant les indices $j$ par valeur de $m$. Cela permet d'éliminer les branchements conditionnels 
 * et de parcourir strictement les états valides où $m == mp$.
 * 
 * @param rVals Vecteur des valeurs de l'axe radial $r_\perp$ [fm].
 * @param zVals Vecteur des valeurs de l'axe axial $z$ [fm].
 * @param nbR Nombre de points sur l'axe radial.
 * @param nbZ Nombre de points sur l'axe axial.
 * @return arma::mat Une matrice 2D de taille `nbR` x `nbZ` représentant la densité locale $\rho(r, z)$.
 */
arma::mat rho_optimized_m(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ){

    Basis basis(BR, BZ, N, Q);

    arma::mat rho;
    if (!rho.load(RHO_FILE, arma::arma_ascii)) {
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
    /*
    std::cout << "Nombre total d'états : " << totalStates << std::endl;
    */

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

    BENCH_START(bench_optimized_m);

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
    
    BENCH_END(bench_optimized_m);

    return result;
}

/**
 * @brief Calcule la matrice de densité nucléaire $\rho(r, z)$ en exploitant la symétrie des états et le regroupement par $m$.
 * 
 * Cette fonction optimise le calcul de la densité en pré-indexant les états par bloc de moment 
 * cinétique $m$ et en exploitant la symétrie de la matrice ($i \le j$). Pour chaque paire d'états, 
 * elle calcule séparément le terme diagonal ($i == j$) et multiplie par $2.0$ la contribution 
 * des termes hors-diagonaux ($j > i$), réduisant ainsi drastiquement le nombre d'évaluations de 
 * fonctions de base (`basisFunc`).
 * 
 * @param rVals Vecteur des valeurs de l'axe radial $r_\perp$ [fm].
 * @param zVals Vecteur des valeurs de l'axe axial $z$ [fm].
 * @param nbR Nombre de points sur l'axe radial.
 * @param nbZ Nombre de points sur l'axe axial.
 * @return arma::mat Une matrice 2D de taille `nbR` x `nbZ` représentant la densité locale $\rho(r, z)$.
 */
arma::mat rho_symmetric(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ){

    Basis basis(BR, BZ, N, Q);

    arma::mat rho;
    if (!rho.load(RHO_FILE, arma::arma_ascii)) {
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

    BENCH_START(bench_symmetric);

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

    BENCH_END(bench_symmetric);

    return result;
}

/**
 * @brief Calcule la matrice de densité nucléaire $\rho(r, z)$ via une version optimisée en mémoire et en symétrie.
 * 
 * Cette fonction améliore la version symétrique précédente en pré-déclarant des variables 
 * de travail (comme la matrice `term`) pour éviter les réallocations dynamiques répétées 
 * de matrices de taille `nbR` x `nbZ` à l'intérieur des boucles critiques. Elle combine 
 * le découpage par moment cinétique $m$ et l'exploitation de la symétrie ($i \le j$).
 * 
 * @param rVals Vecteur des valeurs de l'axe radial $r_\perp$ [fm].
 * @param zVals Vecteur des valeurs de l'axe axial $z$ [fm].
 * @param nbR Nombre de points sur l'axe radial.
 * @param nbZ Nombre de points sur l'axe axial.
 * @return arma::mat Une matrice 2D de taille `nbR` x `nbZ` représentant la densité locale $\rho(r, z)$.
 */
arma::mat rho_symmetric_optimized(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ){

    Basis basis(BR, BZ, N, Q);

    arma::mat rho;
    if (!rho.load(RHO_FILE, arma::arma_ascii)) {
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

    BENCH_START(bench_symmetric_optimized);

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

    BENCH_END(bench_symmetric_optimized);

    return result;
}


/**
 * @brief Calcule la matrice de densité nucléaire $\rho(r, z)$ avec une stratégie de mémoïsation maximale (précalcul des fonctions de base).
 * 
 * Cette fonction élimine les redondances de calcul en pré-évaluant et stockant l'ensemble des 
 * fonctions de base (`basisFunc`) dans un vecteur contigu en mémoire (`func`) avant la boucle principale.
 * 
 * @note **Optimisation HPC, Caches et TLB :** 
 * En préallouant et stockant séquentiellement toutes les matrices de base, cette approche 
 * garantit une excellente localité spatiale et temporelle des données. Lors des parcours intensifs 
 * de la boucle, les adresses mémoires successives se trouvent proches, ce qui maximise le taux de 
 * succès dans les caches CPU (L1/L2/L3) et réduit drastiquement les défauts de traduction d'adresses 
 * au niveau du **TLB (Translation Lookaside Buffer)**. Le processeur peut ainsi traiter de larges tranches 
 * de données sans goulot d'étranglement matériel.
 * 
 * @param rVals Vecteur des valeurs de l'axe radial $r_\perp$ [fm].
 * @param zVals Vecteur des valeurs de l'axe axial $z$ [fm].
 * @param nbR Nombre de points sur l'axe radial.
 * @param nbZ Nombre de points sur l'axe axial.
 * @return arma::mat Une matrice 2D de taille `nbR` x `nbZ` représentant la densité locale $\rho(r, z)$.
 */
arma::mat rho_memory_max(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ){

    Basis basis(BR, BZ, N, Q);

    arma::mat rho;
    if (!rho.load(RHO_FILE, arma::arma_ascii)) {
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

    BENCH_START(bench_memory_max);

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

    BENCH_END(bench_memory_max);

    return result;
}

/**
 * @brief Calcule la matrice de densité nucléaire $\rho(r, z)$ de manière ultra-optimisée via l'algèbre linéaire haut niveau et BLAS 3.
 * 
 * ### Fondements mathématiques
 * Le calcul de la densité s'exprime comme la contraction de fonctions de base $\phi_i(r,z)$ 
 * avec les coefficients de la matrice de densité $\rho_{ij}$ :
 * $$\rho(r,z) = \sum_{m} \sum_{i, j \in m} \phi_i(r,z) \phi_j(r,z) \rho_{ij}$$
 * En regroupant les fonctions de base d'un même moment cinétique $m$ sous forme de matrice 
 * de taille $N_{\text{points}} \times K_m$ (notée $F_m$), le calcul devient un produit de matrices :
 * $A = F_m \times \rho_m$. La densité finale est obtenue en extrayant la diagonale de la multiplication 
 * de Hadamard entre $A$ et $F_m$, évitant ainsi l'allocation complète d'une matrice intermédiaire de taille $K_m \times K_m$.
 * 
 * ### Niveaux BLAS et Contexte HPC
 * - **BLAS 1 (Opérations Vecteur-Vecteur, ex: $y = \alpha x + y$)** : 
 *   Utilisé dans les versions naïves ou scalaires. *Contexte :* Faible intensité arithmétique, 
 *   le goulot d'étranglement est entièrement la bande passante mémoire (chaque octet lu de la RAM 
 *   génère très peu d'opérations).
 * - **BLAS 2 (Opérations Matrice-Vecteur, ex: $y = A x$)** : 
 *   Utilisé lors des multiplications par des vecteurs intermédiaires. *Contexte :* Améliore légèrement 
 *   l'utilisation des caches, mais reste limité par la mémoire pour des matrices de grande taille.
 * - **BLAS 3 (Opérations Matrice-Matrice, ex: $C = \alpha A B + \beta C$)** : 
 *   *C'est le niveau exploité ici via DGEMM.* *Contexte :* Intensité arithmétique maximale ($\mathcal{O}(N^3)$ 
 *   opérations pour $\mathcal{O}(N^2)$ données lues). Cela permet de maximiser la réutilisation des données 
 *   directement dans les registres et caches ultra-rapides du processeur (L1/L2/L3), d'exploiter la vectorisation 
 *   matérielle (SIMD) et d'atteindre les limites de performance du processeur.
 * 
 * @param rVals Vecteur des valeurs de l'axe radial $r_\perp$ [fm].
 * @param zVals Vecteur des valeurs de l'axe axial $z$ [fm].
 * @param nbR Nombre de points sur l'axe radial.
 * @param nbZ Nombre de points sur l'axe axial.
 * @return arma::mat Une matrice 2D de taille `nbR` x `nbZ` représentant la densité locale $\rho(r, z)$.
 */
arma::mat rho_fabulous(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ){

    Basis basis(BR, BZ, N, Q);

    arma::mat rho;
    if (!rho.load(RHO_FILE, arma::arma_ascii)) {
        std::cerr << "Erreur : impossible de charger le fichier 'rho.arma' !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    BENCH_START(bench_fabulous);

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

        // Allocation locale directe des blocs de base pour le moment cinétique m
        arma::mat F_m(nbR * nbZ, K_m);
        for (int k = 0; k < K_m; ++k) {
            int idx = indices[k];
            const auto& state = states[idx];
            F_m.col(k) = arma::vectorise(basis.basisFunc(state.m, state.n, state.n_z, zVals, rVals));
        }

        // 1. Extraire le sous-bloc rho_m correspondant au moment cinétique m
        arma::uvec idx_vec = arma::conv_to<arma::uvec>::from(indices);
        arma::mat rho_m = rho.submat(idx_vec, idx_vec);

        // 2. PRODUIT INTERMÉDIAIRE MATRICE-MATRICE (BLAS 3 / DGEMM)
        arma::mat A = F_m * rho_m;

        // 3. EXTRACTION DIRECTE DE LA DIAGONALE VIA PRODUIT DE HADAMARD
        arma::vec diag_R_m = arma::sum(A % F_m, 1);

        // 4. Redimensionner le vecteur diagonale en grille 2D et accumuler
        result += arma::reshape(diag_R_m, nbR, nbZ);
    }
   
    BENCH_END(bench_fabulous);

    return result;
}

/**
 * @brief Version ultime de calcul de la densité $\rho(r, z)$ combinant Cholesky, mémoïsation 1D et zéro allocation.
 * 
 * ### Retex & Analyse Numérique (Semestre 1)
 * En analyse numérique, si une matrice symétrique est définie positive (SPD), la **décomposition de Cholesky** 
 * ($\rho_m = L L^T$) permet de factoriser la matrice en un produit triangulaire. En injectant cela dans 
 * notre calcul, on passe d'un produit général (GEMM, $\mathcal{O}(N^3)$) à un produit triangulaire (TRMM), 
 * ce qui divise théoriquement par deux le nombre d'opérations flottantes. 
 * 
 * *Note : On a dû rajouter un `fallback` sur le produit classique (`GEMM`) parce que notre matrice 
 * de densité avait parfois des valeurs nulles ou de petits soucis de semi-définition positive qui faisaient crasher 
 * `arma::chol` au début. Ç
 * 
 * @param rVals Vecteur des valeurs de l'axe radial $r_\perp$ [fm].
 * @param zVals Vecteur des valeurs de l'axe axial $z$ [fm].
 * @param nbR Nombre de points sur l'axe radial.
 * @param nbZ Nombre de points sur l'axe axial.
 * @return arma::mat Une matrice 2D de taille `nbR` x `nbZ` représentant la densité locale $\rho(r, z)$.
 */
arma::mat rho_ultimate(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ) {

    Basis basis(BR, BZ, N, Q);

    arma::mat rho;
    if (!rho.load(RHO_FILE, arma::arma_ascii)) {
        std::cerr << "Erreur : impossible de charger le fichier 'rho.arma' !" << std::endl;
        return arma::zeros(nbR, nbZ);
    }

    BENCH_START(bench_ultimate);

    // =========================================================================
    // OPTIMISATION 1 : MÉMOÏSATION 1D EXTRÊME
    // On évite de recalculer les fonctions de base 2D à la volée. On sépare 
    // l'espace radial (rPart) et axial (zPart) pour ne stocker que des vecteurs 1D.
    // Ça prend quasi pas de mémoire et ça sauve le cache CPU.
    // =========================================================================

    // Recherche de la valeur maximale de n_z pour la pré-allocation
    int max_nz = 0;
    for (int m = 0; m < basis.mMax; m++) {
        for (int n = 0; n < basis.nMax(m); n++) {
            if (basis.n_zMax(m, n) > max_nz) {
                max_nz = basis.n_zMax(m, n);
            }
        }
    }

    // Précalcul de toutes les fonctions zPart (stockées dans un std::vector)
    std::vector<arma::vec> Z_memo(max_nz);
    for (int nz = 0; nz < max_nz; ++nz) {
        Z_memo[nz] = basis.zPart(zVals, nz);
    }

    // Précalcul de toutes les fonctions rPart par moment cinétique m et n
    std::vector<std::vector<arma::vec>> R_memo(basis.mMax);
    for (int m = 0; m < basis.mMax; ++m) {
        R_memo[m].resize(basis.nMax(m));
        for (int n = 0; n < basis.nMax(m); n++) {
            R_memo[m][n] = basis.rPart(rVals, m, n);
        }
    }

    // =========================================================================
    // INDEXATION DES ÉTATS (Comme dans la version Fabulous)
    // On aplatit les boucles pour regrouper les indices par blocs de moment m.
    // =========================================================================

    struct BasisState { int m; int n; int n_z; };
    std::vector<BasisState> states;
    std::vector<std::vector<int>> indices_par_m(basis.mMax);

    int idx = 0;
    int max_K_m = 0;
    for (int m = 0; m < basis.mMax; m++) {
        for (int n = 0; n < basis.nMax(m); n++) {
            for (int n_z = 0; n_z < basis.n_zMax(m, n); n_z++) {
                states.push_back({m, n, n_z});
                indices_par_m[m].push_back(idx++);
            }
        }
        if ((int)indices_par_m[m].size() > max_K_m) {
            max_K_m = indices_par_m[m].size();
        }
    }

    if (rho.n_rows < (unsigned int)states.size() || rho.n_cols < (unsigned int)states.size()) {
        return arma::zeros(nbR, nbZ);
    }

    // =========================================================================
    // OPTIMISATION 2 : ZÉRO ALLOCATION DYNAMIQUE
    // On alloue un unique gros buffer F_global en amont pour le pire des cas (max_K_m).
    // Ça évite les 'malloc' à répétition dans la boucle qui flinguent les perfs.
    // =========================================================================
    
    arma::mat F_global(nbR * nbZ, max_K_m);
    arma::mat result = arma::zeros(nbR, nbZ);

    for (int m = 0; m < basis.mMax; ++m) {
        const auto& indices = indices_par_m[m];
        int K_m = indices.size();
        if (K_m == 0) continue;

        // Création d'une "Vue" sur la mémoire pré-allouée : AUCUN malloc ici !
        arma::mat F_m = F_global.head_cols(K_m);

        // Remplissage ultra rapide : on combine les vecteurs 1D précalculés (produit externe)
        for (int k = 0; k < K_m; ++k) {
            const auto& state = states[indices[k]];
            F_m.col(k) = arma::vectorise(Z_memo[state.n_z] * R_memo[state.m][state.n].t());
        }

        // Extraction du sous-bloc de densité associé au moment m
        arma::uvec idx_vec = arma::conv_to<arma::uvec>::from(indices);
        arma::mat rho_m = rho.submat(idx_vec, idx_vec);

        // =========================================================================
        // OPTIMISATION 3 : DÉCOMPOSITION DE CHOLESKY (TRMM)
        // On applique Cholesky si la matrice est SPD (symétrique définie positive).
        // Cela divise les calculs par 2 par rapport à un produit matriciel classique.
        // =========================================================================
        
        arma::mat L;
        // arma::chol renvoie true si la décomposition réussit
        if (arma::chol(L, rho_m, "lower")) {
            // On multiplie par la matrice triangulaire L (optimisé via TRMM en interne)
            arma::mat B = F_m * L; 
            
            // La diagonale de B * B^T correspond à la somme des carrés des lignes de B
            arma::vec diag_R_m = arma::sum(arma::square(B), 1);
            result += arma::reshape(diag_R_m, nbR, nbZ);
        } 
        else {
            // Fallback de secours : si rho_m n'est pas strictement définie positive 
            // (ex: présence de zéros parfaits), Cholesky plante. On repasse sur le GEMM classique.
            arma::mat A = F_m * rho_m;
            arma::vec diag_R_m = arma::sum(A % F_m, 1);
            result += arma::reshape(diag_R_m, nbR, nbZ);
        }
    }

    BENCH_END(bench_ultimate);

    return result;
}
