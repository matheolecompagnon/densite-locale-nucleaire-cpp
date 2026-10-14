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
├── bench.txt       # Résultat du benchmark sur 100 itérations
├── rho.arma        # Matrice rho du projet
├── test_wrapper.py # Test du wrapper
├── include/        # Headers (.h)
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

| Algorithme    | Temps (s/res.) | Gain vs Naïf |
|:--------------|---------------:|-------------:|
| Naïf          | 2.643643       | x1.0         |
| Kroenecker    | 0.668905       | x4.0         |
| Factorized    | 0.539542       | x4.9         |
| Linearized    | 0.538897       | x4.9         |
| Linearized 2. | 0.669147       | x4.0         |
| Optimized     | 0.539421       | x4.9         |
| Symmetric     | 0.343141       | x7.7         |
| Sym. Opti.    | 0.342538       | x7.7         |
| Memory max    | 0.271028       | x9.8         |
| **Fabulous**  | **0.038582**   | **x68.5**    |

Durée totale du benchmark : environ 739 s.

### Lecture des résultats

- L'algorithme **Fabulous** est le plus rapide : environ **68 fois plus rapide** que l'algorithme naïf, et environ 7 fois plus rapide que le meilleur des autres (*Memory max*).
- Les algorithmes **Factorized**, **Linearized** et **Optimized** donnent des temps quasiment identiques (environ 0.54 s), de même que **Symmetric** et **Sym. Opti.** (environ 0.34 s).
- **Kroenecker** et **Linearized 2.** se situent autour de 0.67 s.
- Exploiter la symétrie du problème fait gagner environ 36 % par rapport à *Factorized*, et *Memory max* pousse le gain un peu plus loin.

### Remarques

- Les mesures ci-dessus ont été obtenues avec les options `OPTIONS_RELEASE`.
- Les temps dépendent de la machine. À compléter : processeur, mémoire, version de `g++` et d'Armadillo.