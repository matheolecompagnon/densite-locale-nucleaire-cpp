# ipsdev

Projet C++ de modélisation de la **densité nucléaire locale** $\rho(\mathbf{r})$ à partir d'une base d'oscillateur harmonique déformé en coordonnées cylindriques. Il s'appuie sur **Armadillo** pour l'algèbre linéaire, est validé par des tests unitaires **CxxTest**, dispose d'un mode benchmark et d'un wrapper Python optionnel.

## Objectif

La densité nucléaire locale est définie par

$$
\rho(\mathbf{r}) = \sum_{a,b} \rho_{ab}\,\psi_a(\mathbf{r})\,\psi_b^*(\mathbf{r})
$$

où les $\psi_{m,n,n_z}$ sont les fonctions de base, repérées par les nombres quantiques $(m, n, n_z)$ et les paramètres de déformation $b_\perp$ et $b_z$. Comme $\rho_{ab}$ est nul dès que $m_a \neq m_b$, la phase $e^{im\theta}$ s'élimine et l'on travaille avec la fonction réelle :

$$
\psi_{m,n,n_z}(r_\perp, z) = Z(z, n_z)\,R(r_\perp, m, n)
$$

- **Partie axiale** $Z(z, n_z)$ : fonction de Hermite, de paramètre $b_z$.
- **Partie radiale** $R(r_\perp, m, n)$ : fonction de Laguerre généralisée, de paramètre $b_\perp$.

Le but du projet est de **simuler ce calcul et de proposer des algorithmes pour évaluer $\phi$**, ce qui est réalisé ici. Cela passe par :

1. **Les polynômes** de Hermite $H_n$ et de Laguerre généralisés $L_n^m$, évalués par relation de récurrence (`poly`).
2. **Les fonctions de base** $Z$, $R$ et $\psi$ (`basis`).
3. **La troncation de la base** : les bornes sur $m$, $n$ et $n_z$ sont déterminées à partir des paramètres $N$ et $Q$ (`mMax`, `nMax[m]`, `n_zMax[m, n]`), afin de ne conserver que les états utiles.
4. **La simulation et le benchmark** (`simulation`, `bench`) pour calculer $\rho(\mathbf{r})$ et comparer les performances.

Les fonctions de base sont aussi accessibles depuis Python grâce au wrapper (`ipsdev.phi`).

## Structure du projet

```
.
├── Makefile
├── rho.arma        # Matrice rho du projet
├── test_wrapper.py # Test du wrapper
├── include/        # Headers (.h)
├── doc/            # Documentation Doxygen
├── src/            # Sources : main.cpp, basis.cpp, poly.cpp, simulation.cpp, bench.cpp, wrapper.cpp
├── tests/          # Tests CxxTest : test_basis.h, test_mandatory.h, test_poly.h
├── bin/            # Objets, dépendances (.d) et exécutable de test (généré)
└── exec            # Exécutable principal (généré)
```

## Dépendances

| Dépendance | Rôle | Obligatoire |
|---|---|---|
| `g++` | Compilateur C++ | Oui |
| `make` | Outil de build | Oui |
| **Armadillo** | Algèbre linéaire (`-larmadillo`) | Oui |
| **CxxTest** | Tests unitaires (`cxxtestgen`) | Pour `make test` |
| Python 3 + headers de dev | Compilation du wrapper (`python3-config`) | Pour le wrapper |

Perf est aussi requis pour utiliser `make perf ARGS=...`

### Installation

**Debian / Ubuntu**
```sh
sudo apt install g++ make libarmadillo-dev cxxtest python3-dev
```

**macOS (Homebrew)**
```sh
brew install armadillo cxxtest
```

## Utilisation

```sh
make            # Équivalent de `make debug`
make debug      # Compile en debug puis lance le programme
make release    # Compile en release puis lance le programme
make run        # Compile (si nécessaire) et lance ./exec avec les options courantes
make test       # Génère, compile et exécute les tests CxxTest (en debug)
make bench      # Compile en release avec -DBENCH puis lance le programme
make clean      # Supprime bin/ et l'exécutable exec
make perf ARGS=fabulous # Lance l'analyse de Perf sur l'algorithme fabulous
```

## Documentation

Pour compiler la documentation :

```sh
cd doc
doxygen Doxyfile
```

## Tests

Les tests sont écrits avec CxxTest dans `tests/`. `make test`.

Pour ajouter un fichier de test, créer `tests/test_xxx.h` et l'ajouter à la variable `test_headers` du Makefile.

## Wrapper Python

Le wrapper génère un module Python (`ipsdev.cpython-3xx-....so`) :

```sh
g++ -O2 -Wall -std=c++17 -fPIC -shared $(python3-config --includes) \
    src/wrapper.cpp src/basis.cpp src/poly.cpp \
    -o ipsdev$(python3-config --extension-suffix) -larmadillo -Iinclude
```

Utilisation depuis Python (dans le dossier contenant le `.so`) :

```python
import ipsdev

vals = ipsdev.phi(m, n, n_z, [(r0, z0), (r1, z1), ...], br=1.0, bz=1.0) # -> [phi(r0, z0), phi(r1, z1), ...]

```

## Benchmark

Le benchmark compare plusieurs algorithmes de calcul de la densité $\rho(\mathbf{r})$. Il est lancé avec :

```sh
make bench
```

Cette commande compile avec `-DBENCH` et exécute le programme principal. Chaque algorithme est mesuré après **10 itérations de préchauffe**, puis moyenné sur **100 itérations**.

### Résultats

* **Nombre de préchauffes (Warmup) :** 10 itérations
* **Nombre de mesures :** 60 itérations
* **nbR** : 100
* **nbZ** : 100

| Algorithme    | Temps (s/res.) | Gain vs Naïf |
|:--------------|---------------:|-------------:|
| Naïf          |       2.114799 |         x1.0 |
| Kroenecker    |       0.250359 |         x8.4 |
| Factorized    |       0.154771 |        x13.7 |
| Linearized    |       0.154739 |        x13.7 |
| Linearized 2. |       0.249960 |         x8.5 |
| Optimized     |       0.154670 |        x13.7 |
| Symmetric     |       0.097446 |        x21.7 |
| Sym. Opti.    |       0.097073 |        x21.8 |
| Memory max    |       0.047831 |        x44.2 |
| Fabulous      |       0.034142 |        x61.9 |
| **Ultimate**  |   **0.013065** |   **x161.9** |


Durée totale du benchmark : environ 739 s.

### Lecture des résultats

- L'algorithme **Fabulous** est le plus rapide : environ **68 fois plus rapide** que l'algorithme naïf, et environ 7 fois plus rapide que le meilleur des autres (*Memory max*).
- Les algorithmes **Factorized**, **Linearized** et **Optimized** donnent des temps quasiment identiques (environ 0.54 s), de même que **Symmetric** et **Sym. Opti.** (environ 0.34 s).
- **Kroenecker** et **Linearized 2.** se situent autour de 0.67 s.
- Exploiter la symétrie du problème fait gagner environ 36 % par rapport à *Factorized*, et *Memory max* pousse le gain un peu plus loin.

### Remarques

- Les mesures ci-dessus ont été obtenues avec les options `OPTIONS_RELEASE`.
- Les temps dépendent de la machine. À compléter : processeur, mémoire, version de `g++` et d'Armadillo.

## Module C++ et Wrapper Python Avancé (`simulation_cpp`)

En plus des exécutables natifs, le projet propose un module Python natif nommé `simulation_cpp` permettant d'appeler directement les cœurs de calcul optimisés depuis des scripts d'analyse ou de visualisation[cite: 3].

### Fonctions exposées
Le wrapper expose les principales versions du solveur de densité (mais pas toutes) :
- `simulation_cpp.rho_naif(rVals, zVals, nbR, nbZ)`[cite: 3]
- `simulation_cpp.rho_linearized(rVals, zVals, nbR, nbZ)`[cite: 3]
- `simulation_cpp.rho_memory_max(rVals, zVals, nbR, nbZ)`[cite: 3]
- `simulation_cpp.rho_fabulous(rVals, zVals, nbR, nbZ)`[cite: 3]

### Exemple d'utilisation en Python

```python
import numpy as np
import simulation_cpp

# Définition de la grille radiale et axiale
rVals = np.linspace(0.01, 10.0, 150)
zVals = np.linspace(-10.0, 10.0, 150)
nbR, nbZ = len(rVals), len(zVals)

# Appel de la version ultra-optimisée (Fabulous / BLAS 3)
rho_matrix = simulation_cpp.rho_fabulous(rVals, zVals, nbR, nbZ)

print("Dimensions de la matrice de densité :", rho_matrix.shape)

# État de la Documentation Doxygen - Projet IPSDEV

Ce document dresse le bilan de la documentation Doxygen mise en place pour l'ensemble du projet de modélisation de la densité nucléaire en C++ et son interface Python.


## 1. Couverture du Cœur de Calcul C++

L'intégralité des fonctions de résolution et d'optimisation de la densité nucléaire est documentée selon les standards Doxygen (`@brief`, `@param`, `@return`, `@note`) :

* **Approches de base et filtrage :**
  * `rho_naif` : Implémentation de référence à 6 boucles imbriquées.
  * `rho_kronecker` : Exploitation de l'invariance azimutale via le filtre $m == mp$.

* **Stratégies de réorganisation :**
  * `rho_factorized` : Utilisation d'un décalage (*offset*) pour cibler directement les blocs $m$.
  * `rho_linearized` : Aplatissement de l'espace des états en un vecteur linéaire unique.
  * `rho_linearized_again` : Variante à boucle unique (documentée avec la note de performance sur le surcoût des divisions/modulos).
  * `rho_optimized_m` : Indexation préalable des états par blocs de moment cinétique $m$.

* **Optimisations mémoire et symétrie :**
  * `rho_symmetric` & `rho_symmetric_optimized` : Exploitation de la symétrie des termes ($i \le j$) pour diviser les calculs par deux.
  * `rho_memory_max` : Mémoïsation maximale des fonctions de base, avec une attention particulière portée sur l'optimisation des caches CPU et du **TLB (Translation Lookaside Buffer)** en HPC.

* **Niveaux supérieurs et mathématiques avancées :**
  * `rho_fabulous` : Documentation détaillée sur l'utilisation de l'algèbre linéaire haut niveau et des routines **BLAS 3 (DGEMM)**, incluant une distinction claire avec les niveaux BLAS 1 et 2.
  * `rho_ultimate` : Intégration de la décomposition de **Cholesky** (rappel d'analyse numérique du semestre 1 sur les matrices définies positives) et gestion des fallbacks.



## 2. Structures et Classes

* **`BasisState`** : Structure représentant un état quantique unique par ses trois nombres quantiques $(m, n, n_z)$.
* **`Basis`** : Classe de gestion de la base fonctionnelle (parties axiales $Z$ et radiales $R$).


## 3. Documentation du Wrapper Python (`wrapper.cpp`)

Le fichier d'interface C++/Python (`wrapper.cpp`) est entièrement documenté :
* Description des fonctions utilitaires de conversion (zéro-copie de `numpy.ndarray` vers `arma::vec` et conversion inverse `arma::mat` vers NumPy).
* Documentation des enveloppes de liaison (`wrap_rho_naif`, `wrap_rho_fabulous`, etc.) exposant le module natif `simulation_cpp` à l'interpréteur Python.

## 4. Génération de la Documentation

Si un fichier de configuration Doxygen (`Doxyfile`) est présent à la racine du projet, la documentation technique HTML/LaTeX peut être générée instantanément via la commande :
