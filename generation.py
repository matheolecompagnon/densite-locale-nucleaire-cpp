import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import RegularGridInterpolator

# Paramètres physiques du système
BZ = 1.935801664793151
BR = 2.829683956491218
N = 14
Q = 1.3

def factoriel(n):
    f = 1.0
    for k in range(2, n + 1):
        f *= k
    return f

def calc_laguerre(m_max, n_max, vect):
    coefficients = np.zeros((len(vect), m_max * n_max))
    for i in range(n_max):
        for j in range(m_max):
            col_idx = m_max * i + j
            if i == 0:
                coefficients[:, col_idx] = 1.0
            elif i == 1:
                coefficients[:, col_idx] = 1.0 + j - vect
            else:
                term1 = (2 * i - 1 + j - vect) * coefficients[:, m_max * (i - 1) + j]
                term2 = (i + j - 1) * coefficients[:, m_max * (i - 2) + j]
                coefficients[:, col_idx] = (term1 - term2) / float(i)
    return coefficients

def calc_hermite(n_max, vect):
    coefficients = np.zeros((len(vect), n_max))
    for i in range(n_max):
        if i == 0:
            coefficients[:, i] = 1.0
        elif i == 1:
            coefficients[:, i] = 2.0 * vect
        else:
            coefficients[:, i] = 2.0 * vect * coefficients[:, i - 1] - 2.0 * (i - 1) * coefficients[:, i - 2]
    return coefficients

def r_part(r, m, n, br):
    abs_m = abs(m)
    fact = (1.0 / (br * np.sqrt(np.pi))) * np.sqrt(factoriel(n) / factoriel(n + abs_m))
    puissance = (r ** 2) / (br ** 2)
    exponentielle = np.exp(-puissance / 2.0)
    fact_vec = (r / br) ** abs_m
    lag_coeffs = calc_laguerre(n + 1, abs_m + 1, puissance)
    laguerre = lag_coeffs[:, abs_m * (n + 1) + n]
    return fact * fact_vec * laguerre * exponentielle

def z_part(z, n_z, bz):
    fact = 1.0 / (np.sqrt(bz) * np.sqrt((2.0 ** n_z) * factoriel(n_z) * np.sqrt(np.pi)))
    base = z / bz
    exponentielle = np.exp(-(base ** 2) / 2.0)
    herm_coeffs = calc_hermite(n_z + 1, base)
    hermite = herm_coeffs[:, n_z]
    return fact * hermite * exponentielle

def basis_func(m, n, n_z, z, r, br, bz):
    rp = r_part(r, m, n, br)
    zp = z_part(z, n_z, bz)
    return np.outer(zp, rp)

# --- Chargement de rho ---
try:
    rho = np.loadtxt("rho.arma", skiprows=1)
    print("Fichier rho.arma chargé avec succès.")
except Exception as e:
    print("Utilisation d'une matrice identité de test.")
    rho = np.eye(50)

# --- Grille de calcul 2D ---
nbR = 100
nbZ = 100
rVals = np.linspace(0.0, 5.0, nbR)
zVals = np.linspace(-5.0, 5.0, nbZ)

q_pow = Q ** (2.0 / 3.0)
nu = (N + 2) * q_pow + 0.5
mMax = 0
while nu >= 1.0:
    mMax += 1
    nu -= Q

states = []
for m in range(mMax):
    nMax_m = (mMax - m - 1) // 2 + 1
    for n in range(nMax_m):
        i = m + 2 * n + 1
        nu_i = (N + 2) * q_pow + 0.5 - i * Q
        n_zMax = int(nu_i)
        for n_z in range(max(0, n_zMax)):
            states.append({'m': m, 'n': n, 'n_z': n_z})

result = np.zeros((nbZ, nbR))
indices_par_m = {}
for idx, st in enumerate(states):
    indices_par_m.setdefault(st['m'], []).append(idx)

for m in range(mMax):
    indices = indices_par_m.get(m, [])
    K_m = len(indices)
    if K_m == 0:
        continue
    
    F_m = np.zeros((nbZ * nbR, K_m))
    for k, idx in enumerate(indices):
        st = states[idx]
        func = basis_func(st['m'], st['n'], st['n_z'], zVals, rVals, BR, BZ)
        F_m[:, k] = func.flatten()
        
    idx_arr = np.array(indices)
    rho_m = rho[np.ix_(idx_arr, idx_arr)] if rho.shape[0] >= max(indices)+1 else np.eye(K_m)
    
    A = F_m @ rho_m
    diag_R_m = np.sum(A * F_m, axis=1)
    result += diag_R_m.reshape(nbZ, nbR)

# ==========================================
# 1. TRACÉ 2D
# ==========================================
plt.figure(figsize=(8, 6))
plt.contourf(rVals, zVals, result, levels=100, cmap='inferno')
plt.colorbar(label=r'Densité $\rho(r, z)$')
plt.title(r"Profil de densité 2D $\rho(r, z)$")
plt.xlabel("Rayon $r$")
plt.ylabel("Axe $z$")
plt.tight_layout()

# ==========================================
# 2. TRACÉ 3D VOLUMÉTRIQUE (Nébuleuse / Fond Noir)
# ==========================================
interpolator = RegularGridInterpolator((zVals, rVals), result, bounds_error=False, fill_value=0)

n_pts = 45  
x_lin = np.linspace(-np.max(rVals), np.max(rVals), n_pts)
y_lin = np.linspace(-np.max(rVals), np.max(rVals), n_pts)
z_lin = np.linspace(np.min(zVals), np.max(zVals), n_pts)

X, Y, Z = np.meshgrid(x_lin, y_lin, z_lin, indexing='ij')

R_3d = np.sqrt(X**2 + Y**2)
mask = R_3d <= np.max(rVals)

points = np.column_stack((Z[mask].ravel(), R_3d[mask].ravel()))
density_3d = np.zeros(X.shape)
density_3d[mask] = interpolator(points)

fig3d = plt.figure(figsize=(10, 8), facecolor='black')
ax3d = fig3d.add_subplot(111, projection='3d', facecolor='black')

threshold = 0.15 * np.max(density_3d)
visible_mask = density_3d > threshold

x_plot = X[visible_mask]
y_plot = Y[visible_mask]
z_plot = Z[visible_mask]
c_plot = density_3d[visible_mask]

sc = ax3d.scatter(x_plot, y_plot, z_plot, c=c_plot, cmap='inferno', marker='o', s=6, alpha=0.12)

# Esthétique des axes pour le fond noir
ax3d.xaxis.set_pane_color((0.0, 0.0, 0.0, 1.0))
ax3d.yaxis.set_pane_color((0.0, 0.0, 0.0, 1.0))
ax3d.zaxis.set_pane_color((0.0, 0.0, 0.0, 1.0))

ax3d.tick_params(colors='white')
ax3d.xaxis.label.set_color('white')
ax3d.yaxis.label.set_color('white')
ax3d.zaxis.label.set_color('white')
ax3d.title.set_color('white')

cbar = plt.colorbar(sc, pad=0.1)
cbar.set_label(r'Densité $\rho(x, y, z)$', color='white')
cbar.ax.yaxis.set_tick_params(color='white')
plt.setp(plt.getp(cbar.ax.axes, 'yticklabels'), color='white')

ax3d.set_title(r"Densité quantique 3D ($\theta$-invariance)")
ax3d.set_xlabel("X")
ax3d.set_ylabel("Y")
ax3d.set_zlabel("Z")

# Affichage des deux fenêtres graphiques en même temps
plt.show()