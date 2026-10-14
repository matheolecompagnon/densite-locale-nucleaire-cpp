#include "basis.hpp"
#include "simulation.hpp"

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