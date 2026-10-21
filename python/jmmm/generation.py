import os
import numpy as np
import matplotlib.pyplot as plt

try:
    import simulation_cpp
except ImportError:
    print("Erreur : le module 'simulation_cpp' n'est pas compilé ou introuvable.")
    exit(1)

def run_generation():
    # 1. Paramètres de la grille
    nbR, nbZ = 150, 150
    rVals = np.linspace(0.01, 10.0, nbR)
    zVals = np.linspace(-10.0, 10.0, nbZ)

    # Création de l'axe cartésien symétrique pour la vue physique 2D
    xVals = np.concatenate((-rVals[::-1], rVals))
    X, Z = np.meshgrid(xVals, zVals)

    os.makedirs('images', exist_ok=True)

    # 2. Liste des méthodes exposées par le wrapper C++
    methods = ['naif', 'linearized', 'memory_max', 'fabulous']
    results = {}

    # 3. Calcul de toutes les densités
    for method in methods:
        print(f"Calcul de la densité avec la méthode : rho_{method}...")
        func = getattr(simulation_cpp, f"rho_{method}")
        rho_matrix = func(rVals, zVals, nbR, nbZ)
        
        # Traitement pour obtenir la coupe 2D physique symétrique
        rho_2d = rho_matrix.T
        rho_full = np.hstack((rho_2d[:, ::-1], rho_2d))
        results[method] = rho_full

        # --- A. Sauvegarde du plot 2D de la fonction ---
        plt.figure(figsize=(9, 6))
        contour = plt.contourf(X, Z, rho_full, levels=100, cmap='magma')
        plt.colorbar(contour, label=r'Densité $\rho(x, z)$ [fm$^{-3}$]')
        plt.xlabel(r'Axe cartésien $X$ [fm]', fontsize=12)
        plt.ylabel(r'Axe axial $Z$ [fm]', fontsize=12)
        plt.title(f'Densité nucléaire - Méthode : rho_{method}', fontsize=14, fontweight='bold')
        
        filename_density = f"images/density_{method}_2D.png"
        plt.tight_layout()
        plt.savefig(filename_density, dpi=300)
        plt.close()
        print(f" -> Sauvegardé : {filename_density}")

    # 4. Calcul et sauvegarde des différences par rapport à la méthode naïve
    rho_ref = results['naif']
    for method in ['linearized', 'memory_max', 'fabulous']:
        diff = results[method] - rho_ref

        plt.figure(figsize=(9, 6))
        # Utilisation d'une palette divergente (bwr ou seismic) centrée sur 0 pour les erreurs
        max_val = np.max(np.abs(diff)) if np.max(np.abs(diff)) > 0 else 1e-15
        contour = plt.contourf(X, Z, diff, levels=100, cmap='seismic', vmin=-max_val, vmax=max_val)
        
        cbar = plt.colorbar(contour)
        cbar.set_label(r'Différence $\Delta\rho$ (Optimisé - Naïf) [fm$^{-3}$]', fontsize=10)
        
        plt.xlabel(r'Axe cartésien $X$ [fm]', fontsize=12)
        plt.ylabel(r'Axe axial $Z$ [fm]', fontsize=12)
        plt.title(f'Écart numérique : rho_{method} vs rho_naif', fontsize=14, fontweight='bold')
        
        filename_diff = f"images/diff_{method}_vs_naif.png"
        plt.tight_layout()
        plt.savefig(filename_diff, dpi=300)
        plt.close()
        print(f" -> Sauvegardé : {filename_diff}")

    print("\nToutes les générations de graphiques 2D et de différences sont terminées avec succès !")

if __name__ == "__main__":
    run_generation()