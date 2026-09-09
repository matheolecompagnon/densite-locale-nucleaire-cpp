# Simulateur de Particules - Groupe 32

## Introduction
Bienvenue dans la documentation technique du **Simulateur d'oscillations harmoniques quantique**

## Architecture des Modules
Le code source est découpé en modules indépendants pour garantir une bonne séparation des responsabilités :
- **Distributions** (`distribution.h` / `distribution.c`) : Génération de nombres aléatoires.
- **Particules** (`particule.h` / `particule.c`) : Gestion de l'état individuel et calculs géométriques.
- **Environnement** (<code><a class="el" href="environment_8h.html">environment.h</a></code> / <code><a class="el" href="environment_8c.html">environment.c</a></code>) : Gestion globale de l'arène et orchestration globale.
- **Interface** (`interface.h` / `interface.c`) : Moteur de rendu graphique PBM et gestion sécurisée des I/O.
- **Programme Principal** (`main.c`) : Point d'entrée du simulateur.

## Compilation et Tests
Le projet utilise `make` pour l'automatisation de la compilation :

```bash
# Compiler le simulateur principal (génère bin/simulator)
make

# Compiler et exécuter la suite de tests unitaires (CUnit)
make test

# Nettoyer les fichiers compilés
make clean
```
