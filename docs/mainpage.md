# ipsdev

Modélisation en C++ de la **densité nucléaire locale** \(\rho(\mathbf{r})\) à partir d'une base d'oscillateur harmonique déformé en coordonnées cylindriques. Le projet utilise **Armadillo** pour l'algèbre linéaire, est validé par des tests unitaires **CxxTest**, dispose d'un mode benchmark et d'un module Python (**pybind11**).

## Principe & Objectif

La densité nucléaire locale s'écrit :

\[\rho(\mathbf{r}) = \sum_{a,b} \rho_{ab}\,\psi_a(\mathbf{r})\,\psi_b^*(\mathbf{r})\]

Les fonctions de base \(\psi_{m,n,n_z}\) dépendent des nombres quantiques \((m, n, n_z)\) et des paramètres de déformation \(b_\perp\) et \(b_z\). Comme \(\rho_{ab}\) est nul dès que \(m_a \neq m_b\), la phase \(e^{im\theta}\) disparaît et on utilise la fonction réelle :

\[\psi_{m,n,n_z}(r_\perp, z) = Z(z, n_z)\,R(r_\perp, m, n)\]

* **Partie axiale** \(Z(z, n_z)\) : fonction de Hermite, de paramètre \(b_z\).
* **Partie radiale** \(R(r_\perp, m, n)\) : fonction de Laguerre généralisée, de paramètre \(b_\perp\).

La base est tronquée à partir des paramètres \(N\) et \(Q\), qui fixent les bornes `mMax`, `nMax[m]` et `n_zMax[m, n]` afin de ne conserver que les états utiles.

Le but du projet est de modéliser ce calcul et de proposer plusieurs algorithmes d'évaluation de \(\rho\), de la version naïve à la version `ultimate`, et de les comparer par benchmark. Cela passe par :
1. **Les polynômes** de Hermite \(H_n\) et de Laguerre généralisés \(L_n^m\), évalués par relation de récurrence (`poly`).
2. **Les fonctions de base** \(Z\), \(R\) et \(\psi\) (`basis`).
3. **La troncation de la base** menée par la classe de gestion.
4. **La simulation et le benchmark** (`simulation`, `bench`) pour calculer \(\rho(\mathbf{r})\) et comparer les performances.

## Dépendances

| Dépendance | Usage / Rôle | Obligatoire |
|---|---|---|
| `g++` (C++17) | Compilateur C++ | Oui |
| `make` | Outil de build | Oui |
| **Armadillo** | Algèbre linéaire (`-larmadillo`) | Oui |
| **CxxTest** | Tests unitaires (`cxxtestgen`) | Non (uniquement pour `make test`) |
| Python 3 + headers de dev | Compilation du wrapper (`python3-config`) | Non (uniquement pour le wrapper) |
| pybind11, NumPy | Support du module Python | Non |
| `perf` | Profilage avec `make perf` | Non |
| Doxygen | Génération de la documentation | Non |

### Installation

**Debian / Ubuntu :**
```sh
sudo apt install g++ make libarmadillo-dev cxxtest python3-dev
pip install pybind11 numpy
```

**macOS (Homebrew) :**
```sh
brew install armadillo cxxtest
pip install pybind11 numpy
```

## Utilisation

Les arguments du programme se passent avec la variable `ARGS="..."`.

| Commande | Effet |
|---|---|
| `make` ou `make debug` | Compile en debug (ASan) vers `build/model-debug` |
| `make release` | Compile en optimisé vers `build/model` |
| `make bench` | Compile avec la variable `-DBENCH` vers `build/model-bench` |
| `make run` | Compile et lance la version debug |
| `make run-release` | Compile et lance la version release |
| `make test` | Génère, compile et lance les tests CxxTest |
| `make perf ARGS=fabulous` | Profile l'algorithme `fabulous` avec l'outil perf |
| `make python` | Compile le module Python |
| `make generate` | Lance le script de génération |
| `make py-test` | Lance les tests Python (pytest) |
| `make clean` | Supprime `bin/` et le module Python compilé |

*Note : `make clean` conserve les exécutables de `build/`. Les objets sont compilés par mode (`bin/basis-debug.o`, `bin/basis-release.o`, etc.), donc passer d'un mode à l'autre ne nécessite pas de nettoyage.*

## Structure du projet

.
├── Makefile
├── rho.arma        # Matrice rho du projet
├── test_wrapper.py # Test du wrapper
├── include/        # Headers C++ (.hpp / .h)
├── src/            # Sources C++ (main, basis, poly, simulation, bench, wrapper)
├── bindings/       # Code des bindings Python
├── python/         # Package Python (contient le module compilé dans python/ipsdev/)
├── tests/          # Tests CxxTest (test_*.h) et tests Python dans tests/python
├── data/           # Contient rho.arma
├── docs/           # Configuration et sortie Doxygen
├── bin/            # Objets, dépendances (.d) et exécutable de test (généré)
└── build/          # Exécutables finaux (généré)

## Tests

Les tests sont écrits avec CxxTest dans `tests/`. Pour ajouter une suite de tests, il suffit de créer un fichier nommé `tests/test_xxx.h` ; il sera automatiquement détecté et doit être ajouté à la variable `test_headers` du Makefile. Exécuter avec :
```sh
make test
```

## Module Python & Wrapper

### Compilation manuelle du Wrapper
Le wrapper génère un module Python (`ipsdev.cpython-3xx-....so`) :
```sh
g++ -O2 -Wall -std=c++17 -fPIC -shared $(python3-config --includes) \
    src/wrapper.cpp src/basis.cpp src/poly.cpp \
    -o ipsdev$(python3-config --extension-suffix) -larmadillo -Iinclude
```

Il est également possible de compiler via le Makefile :
```sh
make python
```
Le module compilé est automatiquement placé dans `python/ipsdev/`.

### Fonctions exposées (Module `ipsdev` et avancé `simulation_cpp`)
En plus des exécutables natifs, le projet propose un module Python natif nommé `simulation_cpp` permettant d'appeler directement les cœurs de calcul optimisés depuis des scripts d'analyse ou de visualisation. Les enveloppes de liaison gèrent la conversion zéro-copie de `numpy.ndarray` vers `arma::vec` et inversement.

Toutes les versions ne sont pas exposées, mais le module expose principalement :
- `simulation_cpp.rho_naif(rVals, zVals, nbR, nbZ)`
- `simulation_cpp.rho_linearized(rVals, zVals, nbR, nbZ)`
- `simulation_cpp.rho_memory_max(rVals, zVals, nbR, nbZ)`
- `simulation_cpp.rho_fabulous(rVals, zVals, nbR, nbZ)`
- `phi(m, n, n_z, points, br=1.0, bz=1.0)` : valeurs de la fonction de base en une liste de points `(r, z)`.

### Exemple d'utilisation

```python
import numpy as np
import ipsdev

rVals = np.linspace(0.01, 10.0, 150)
zVals = np.linspace(-10.0, 10.0, 150)

rho = ipsdev.rho_fabulous(rVals, zVals, len(rVals), len(zVals))
print(rho.shape)
```

## Benchmark

Le benchmark compare plusieurs algorithmes de calcul de la densité \(\rho(\mathbf{r})\). Il est lancé avec :
```sh
make bench
./build/model-bench
```

### Méthodologie et Paramètres
Chaque algorithme est mesuré après **10 itérations de préchauffe (Warmup)**, puis moyenné sur **60 itérations**, avec `nbR = 100` et `nbZ = 100`. Durée totale : environ 739 s.

### Résultats de référence

*Note : Les mesures ci-dessous ont été obtenues avec les options `OPTIONS_RELEASE`, qui ne sont pas optimisées à la compilation (`-O0`).*

| Algorithme | Temps (s) | Gain vs naïf |
|:---|---:|---:|
| Naïf | 2.114799 | x1.0 |
| Kronecker / Kroenecker | 0.250359 | x8.4 |
| Linearized 2 | 0.249960 | x8.5 |
| Linearized | 0.154739 | x13.7 |
| Factorized | 0.154771 | x13.7 |
| Optimized | 0.154670 | x13.7 |
| Symmetric | 0.097446 | x21.7 |
| Symmetric optimized / Sym. Opti. | 0.097073 | x21.8 |
| Memory max | 0.047831 | x44.2 |
| Fabulous | 0.034142 | x61.9 |
| Ultimate | 0.013065 | x161.9 |

### Spécifications de la machine de test
* **Processeur :** Qualcomm Oryon (Snapdragon X Series, ARM64)
* **Mémoire :** 16 Go LPDDR5x (dont 8 Go alloués à l'environnement Linux WSL utilisé pour le benchmark)
* **Système d'exploitation :** Debian GNU/Linux (via WSL2)
* **Version g++ :** g++ 14.2.0 (Debian)
* **Version Armadillo :** Armadillo v14.2.3

## Documentation Technique (Doxygen)

L'intégralité des fonctions de résolution, d'optimisation et les structures du cœur de calcul C++ (`BasisState`, `Basis`) est documentée selon les standards Doxygen (`@brief`, `@param`, `@return`, `@note`).

### Implémentations couvertes :
* **Approches de base et filtrage :** `rho_naif` (référence à 6 boucles imbriquées) et `rho_kronecker` (filtre \(m == mp\) via invariance azimutale).
* **Stratégies de réorganisation :** `rho_factorized` (décalage/offset par blocs \(m\)), `rho_linearized` (en vecteur linéaire unique) et sa variante `rho_linearized_again` (boucle unique avec surcoût de divisions/modulos), ainsi que `rho_optimized_m` (indexation par blocs de moment cinétique \(m\)).
* **Optimisations mémoire et symétrie :** `rho_symmetric` & `rho_symmetric_optimized` (symétrie des termes \(i \le j\)) et `rho_memory_max` (mémoïsation maximale, optimisations des caches CPU et du **TLB (Translation Lookaside Buffer)** en HPC).
* **Niveaux supérieurs :** `rho_fabulous` (utilisation des routines **BLAS 3 - DGEMM**) et `rho_ultimate` (décomposition de **Cholesky** sur matrices définies positives).

### Génération de la documentation
Le fichier de configuration (`Doxyfile`) étant dans le dossier `docs/`, la documentation technique technique HTML/LaTeX peut être générée par les commandes :
```sh
cd docs
doxygen Doxyfile
```

## Annexes

### Contribuer
Voir [CONTRIBUTING.md](CONTRIBUTING.md). L'historique des versions est dans [CHANGELOG.md](CHANGELOG.md).

### Licence
Voir [LICENSE](LICENSE).