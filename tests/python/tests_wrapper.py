import ipsdev
"""**
 * @file wrapper.cpp
 * @brief Module Python « ipsdev » (Python.h) : expose la fonction phi du projet IPS-DEV.
 *
 * Python :
 *     import ipsdev
 *     vals = ipsdev.phi(m, n, n_z, [(r0, z0), (r1, z1), ...], br=1.0, bz=1.0)
 *     # -> [phi(r0, z0), phi(r1, z1), ...]   une valeur par point
 *
 * phi_{m,n,n_z}(r, z) = Z(z, n_z) * R(r, m, n)
 * (la phase e^{i m theta} est omise : on ne garde que la partie indépendante de theta, réelle)
 *
 * Le calcul est fait par la classe Basis :
 *   - Basis::zPart(z, n_z)  -> Z(z, n_z)
 *   - Basis::rPart(r, m, n) -> R(r, m, n)   (attention : le 3e paramètre de rPart est le n de la
 *                                             diapo, même si l'en-tête l'appelle n_z)
 *"""
#!/usr/bin/env python3
"""
Teste que le module ipsdev (wrapper de phi) fonctionne.

Lancement :   python3 test_wrapper.py        (depuis le dossier du projet)
Code de sortie : 0 si tout passe, 1 sinon.
"""
import math
import os
import sys

# Permet de lancer le script depuis la racine du projet ou depuis tests/
ici = os.path.dirname(os.path.abspath(__file__))
for d in (os.path.join(ici, ".."), ici):
    sys.path.insert(0, d)

try:
    import ipsdev
except ImportError as e:
    print("[FAIL] import ipsdev :", e)
    print("       Python utilise :", sys.version.split()[0])
    print("       Le .so doit etre dans le dossier du projet et compile pour CETTE version")
    print("       (le nom du .so contient cpython-3XX, ex. cpython-313 pour Python 3.13).")
    sys.exit(1)

BR, BZ = 1.2, 0.8


# ----------------------------------------------------------------------
# Reference independante, ecrite en Python pur, directement d'apres la diapo
# ----------------------------------------------------------------------
def hermite(n, x):
    if n == 0:
        return 1.0
    a, b = 1.0, 2.0 * x
    for k in range(1, n):
        a, b = b, 2.0 * x * b - 2.0 * k * a
    return b


def laguerre(n, alpha, x):
    if n == 0:
        return 1.0
    a, b = 1.0, 1.0 + alpha - x
    for k in range(1, n):
        a, b = b, ((2 * k + 1 + alpha - x) * b - (k + alpha) * a) / (k + 1)
    return b


def phi_ref(m, n, nz, r, z, br=BR, bz=BZ):
    am = abs(m)
    u = z / bz
    Z = (1 / math.sqrt(bz)) / math.sqrt(2.0 ** nz * math.sqrt(math.pi) * math.factorial(nz)) \
        * math.exp(-u * u / 2) * hermite(nz, u)
    x = r / br
    R = (1 / (br * math.sqrt(math.pi))) * math.sqrt(math.factorial(n) / math.factorial(n + am)) \
        * math.exp(-x * x / 2) * x ** am * laguerre(n, am, x * x)
    return Z * R


def grille(L=10.0, nr=200, nz=400):
    dr, dz = L / nr, 2 * L / nz
    pts = [(i * dr, -L + j * dz) for i in range(nr + 1) for j in range(nz + 1)]
    return pts, dr, dz


def integrale(f, g, pts, dr, dz):
    """Integrale de 2*pi*r * f * g dr dz sur la grille."""
    return sum(2 * math.pi * p[0] * a * b for p, a, b in zip(pts, f, g)) * dr * dz


# ----------------------------------------------------------------------
# Les tests
# ----------------------------------------------------------------------
def test_valeur_analytique():
    """phi_000(0,0) = bz^(-1/2) pi^(-1/4) / (br sqrt(pi))."""
    attendu = BZ ** -0.5 * math.pi ** -0.25 / (BR * math.sqrt(math.pi))
    obtenu = ipsdev.phi(0, 0, 0, [(0.0, 0.0)], BR, BZ)[0]
    return abs(obtenu - attendu) < 1e-12, f"obtenu={obtenu:.10f} attendu={attendu:.10f}"


def test_reference():
    """Comparaison avec la reference Python sur plusieurs (m, n, n_z) et plusieurs points."""
    pts = [(0.0, 0.0), (0.3, -0.5), (0.7, 0.4), (1.5, 1.1), (2.2, -1.7)]
    pire = 0.0
    mauvais = None
    for (m, n, nz) in [(0, 0, 0), (0, 1, 0), (0, 0, 1), (1, 0, 0),
                       (1, 2, 3), (-2, 3, 1), (0, 3, 3)]:
        vals = ipsdev.phi(m, n, nz, pts, BR, BZ)
        for (r, z), v in zip(pts, vals):
            ecart = abs(v - phi_ref(m, n, nz, r, z))
            if ecart > pire:
                pire = ecart
            if ecart > 1e-9 and mauvais is None:
                mauvais = (m, n, nz, r, z, v, phi_ref(m, n, nz, r, z))
    if mauvais:
        m, n, nz, r, z, v, ref = mauvais
        return False, f"premier ecart en (m,n,nz)=({m},{n},{nz}) r={r} z={z} : obtenu={v:.6g} attendu={ref:.6g}"
    return True, f"ecart max = {pire:.2e}"


def test_symetries():
    """phi(m) = phi(-m) ; phi(r,-z) = (-1)^n_z phi(r,z)."""
    pts = [(0.4, 0.9), (1.1, 0.3), (2.0, 1.4)]
    ok = True
    for (m, n, nz) in [(1, 2, 3), (2, 1, 2), (0, 2, 1)]:
        a = ipsdev.phi(m, n, nz, pts, BR, BZ)
        b = ipsdev.phi(-m, n, nz, pts, BR, BZ)
        ok &= all(abs(x - y) < 1e-12 for x, y in zip(a, b))
        opp = ipsdev.phi(m, n, nz, [(r, -z) for r, z in pts], BR, BZ)
        ok &= all(abs(o - (-1) ** nz * x) < 1e-12 for o, x in zip(opp, a))
    return ok, ""


def test_normalisation():
    """Integrale de 2*pi*r*phi^2 = 1."""
    pts, dr, dz = grille()
    pire = 0.0
    for (m, n, nz) in [(0, 0, 0), (1, 2, 3), (2, 3, 1), (-1, 1, 2)]:
        v = ipsdev.phi(m, n, nz, pts, BR, BZ)
        pire = max(pire, abs(integrale(v, v, pts, dr, dz) - 1.0))
    return pire < 1e-2, f"ecart max a 1 = {pire:.2e}"


def test_orthogonalite():
    """Deux fonctions de base differentes sont orthogonales."""
    pts, dr, dz = grille()
    paires = [((1, 1, 0), (1, 2, 0)),      # n different
              ((0, 1, 1), (0, 1, 2)),      # n_z different
              ((0, 2, 1), (0, 2, 3))]
    pire = 0.0
    for a, b in paires:
        fa = ipsdev.phi(*a, pts, BR, BZ)
        fb = ipsdev.phi(*b, pts, BR, BZ)
        pire = max(pire, abs(integrale(fa, fb, pts, dr, dz)))
    return pire < 1e-4, f"produit scalaire max = {pire:.2e}"


def test_format_retour():
    pts = [(0.1, 0.2), (0.3, 0.4), (0.5, 0.6)]
    v = ipsdev.phi(0, 1, 1, pts)
    ok = isinstance(v, list) and len(v) == 3 and all(isinstance(x, float) for x in v)
    ok &= ipsdev.phi(0, 0, 0, []) == []
    return ok, ""


def test_valeurs_par_defaut():
    """br et bz valent 1.0 par defaut."""
    pts = [(0.3, 0.2), (1.0, -0.4)]
    a = ipsdev.phi(1, 1, 1, pts)
    b = ipsdev.phi(1, 1, 1, pts, 1.0, 1.0)
    return a == b, ""


def test_erreurs():
    """Les mauvais arguments levent une exception Python (et ne plantent pas)."""
    cas = [
        ((0, 0, 0, [(1, 2, 3)]), ValueError),     # point a 3 coordonnees
        ((0, 0, 0, [(1,)]), ValueError),          # point a 1 coordonnee
        ((0, -1, 0, [(1, 2)]), ValueError),       # n < 0
        ((0, 0, -1, [(1, 2)]), ValueError),       # n_z < 0
        ((0, 0, 0, [(1, 2)], -1.0, 1.0), ValueError),  # br <= 0
        ((0, 0, 0, 5), TypeError),                # points n'est pas une sequence
        ((0, 0, 0, [("a", 2)]), TypeError),       # coordonnee non numerique
        ((0, 0), TypeError),                      # arguments manquants
    ]
    manques = []
    for args, attendu in cas:
        try:
            ipsdev.phi(*args)
            manques.append(f"{args} : aucune exception")
        except attendu:
            pass
        except Exception as e:
            manques.append(f"{args} : {type(e).__name__} au lieu de {attendu.__name__}")
    return not manques, "; ".join(manques)


def test_numpy():
    """Entree sous forme de tableau NumPy (ignore si NumPy n'est pas installe)."""
    try:
        import numpy as np
    except ImportError:
        return None, "NumPy absent"
    pts = np.array([[0.3, 0.2], [1.0, -0.4], [0.7, 0.1]])
    a = ipsdev.phi(1, 2, 1, pts, BR, BZ)
    b = ipsdev.phi(1, 2, 1, [tuple(p) for p in pts.tolist()], BR, BZ)
    return a == b, ""


# ----------------------------------------------------------------------
# Execution
# ----------------------------------------------------------------------
def main():
    tests = [test_valeur_analytique, test_reference, test_symetries, test_normalisation,
             test_orthogonalite, test_format_retour, test_valeurs_par_defaut,
             test_erreurs, test_numpy]
    echecs = 0
    print(f"Python {sys.version.split()[0]} - module : {getattr(ipsdev, '__file__', '?')}\n")
    for t in tests:
        nom = t.__name__.replace("test_", "")
        try:
            ok, detail = t()
        except Exception as e:                       # plantage inattendu = echec
            ok, detail = False, f"exception {type(e).__name__} : {e}"
        if ok is None:
            etat = "[SKIP]"
        elif ok:
            etat = "[ OK ]"
        else:
            etat = "[FAIL]"
            echecs += 1
        print(f"{etat} {nom:<22} {detail}")
    print()
    if echecs:
        print(f"{echecs} test(s) en echec.")
        return 1
    print("Tous les tests passent : le wrapper fonctionne.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
 

vals = ipsdev.phi(10,3,3, [(0,0), (1,1)], br=1.0, bz=1.0)

print(vals)#!/usr/bin/env python3
"""
Teste que le module ipsdev (wrapper de phi) fonctionne.

Lancement :   python3 test_wrapper.py        (depuis le dossier du projet)
Code de sortie : 0 si tout passe, 1 sinon.
"""
import math
import os
import sys

# Permet de lancer le script depuis la racine du projet ou depuis tests/
ici = os.path.dirname(os.path.abspath(__file__))
for d in (os.path.join(ici, ".."), ici):
    sys.path.insert(0, d)

try:
    import ipsdev
except ImportError as e:
    print("[FAIL] import ipsdev :", e)
    print("       Python utilise :", sys.version.split()[0])
    print("       Le .so doit etre dans le dossier du projet et compile pour CETTE version")
    print("       (le nom du .so contient cpython-3XX, ex. cpython-313 pour Python 3.13).")
    sys.exit(1)

BR, BZ = 1.2, 0.8


# ----------------------------------------------------------------------
# Reference independante, ecrite en Python pur, directement d'apres la diapo
# ----------------------------------------------------------------------
def hermite(n, x):
    if n == 0:
        return 1.0
    a, b = 1.0, 2.0 * x
    for k in range(1, n):
        a, b = b, 2.0 * x * b - 2.0 * k * a
    return b


def laguerre(n, alpha, x):
    if n == 0:
        return 1.0
    a, b = 1.0, 1.0 + alpha - x
    for k in range(1, n):
        a, b = b, ((2 * k + 1 + alpha - x) * b - (k + alpha) * a) / (k + 1)
    return b


def phi_ref(m, n, nz, r, z, br=BR, bz=BZ):
    am = abs(m)
    u = z / bz
    Z = (1 / math.sqrt(bz)) / math.sqrt(2.0 ** nz * math.sqrt(math.pi) * math.factorial(nz)) \
        * math.exp(-u * u / 2) * hermite(nz, u)
    x = r / br
    R = (1 / (br * math.sqrt(math.pi))) * math.sqrt(math.factorial(n) / math.factorial(n + am)) \
        * math.exp(-x * x / 2) * x ** am * laguerre(n, am, x * x)
    return Z * R


def grille(L=10.0, nr=200, nz=400):
    dr, dz = L / nr, 2 * L / nz
    pts = [(i * dr, -L + j * dz) for i in range(nr + 1) for j in range(nz + 1)]
    return pts, dr, dz


def integrale(f, g, pts, dr, dz):
    """Integrale de 2*pi*r * f * g dr dz sur la grille."""
    return sum(2 * math.pi * p[0] * a * b for p, a, b in zip(pts, f, g)) * dr * dz


# ----------------------------------------------------------------------
# Les tests
# ----------------------------------------------------------------------
def test_valeur_analytique():
    """phi_000(0,0) = bz^(-1/2) pi^(-1/4) / (br sqrt(pi))."""
    attendu = BZ ** -0.5 * math.pi ** -0.25 / (BR * math.sqrt(math.pi))
    obtenu = ipsdev.phi(0, 0, 0, [(0.0, 0.0)], BR, BZ)[0]
    return abs(obtenu - attendu) < 1e-12, f"obtenu={obtenu:.10f} attendu={attendu:.10f}"


def test_reference():
    """Comparaison avec la reference Python sur plusieurs (m, n, n_z) et plusieurs points."""
    pts = [(0.0, 0.0), (0.3, -0.5), (0.7, 0.4), (1.5, 1.1), (2.2, -1.7)]
    pire = 0.0
    mauvais = None
    for (m, n, nz) in [(0, 0, 0), (0, 1, 0), (0, 0, 1), (1, 0, 0),
                       (1, 2, 3), (-2, 3, 1), (0, 3, 3)]:
        vals = ipsdev.phi(m, n, nz, pts, BR, BZ)
        for (r, z), v in zip(pts, vals):
            ecart = abs(v - phi_ref(m, n, nz, r, z))
            if ecart > pire:
                pire = ecart
            if ecart > 1e-9 and mauvais is None:
                mauvais = (m, n, nz, r, z, v, phi_ref(m, n, nz, r, z))
    if mauvais:
        m, n, nz, r, z, v, ref = mauvais
        return False, f"premier ecart en (m,n,nz)=({m},{n},{nz}) r={r} z={z} : obtenu={v:.6g} attendu={ref:.6g}"
    return True, f"ecart max = {pire:.2e}"


def test_symetries():
    """phi(m) = phi(-m) ; phi(r,-z) = (-1)^n_z phi(r,z)."""
    pts = [(0.4, 0.9), (1.1, 0.3), (2.0, 1.4)]
    ok = True
    for (m, n, nz) in [(1, 2, 3), (2, 1, 2), (0, 2, 1)]:
        a = ipsdev.phi(m, n, nz, pts, BR, BZ)
        b = ipsdev.phi(-m, n, nz, pts, BR, BZ)
        ok &= all(abs(x - y) < 1e-12 for x, y in zip(a, b))
        opp = ipsdev.phi(m, n, nz, [(r, -z) for r, z in pts], BR, BZ)
        ok &= all(abs(o - (-1) ** nz * x) < 1e-12 for o, x in zip(opp, a))
    return ok, ""


def test_normalisation():
    """Integrale de 2*pi*r*phi^2 = 1."""
    pts, dr, dz = grille()
    pire = 0.0
    for (m, n, nz) in [(0, 0, 0), (1, 2, 3), (2, 3, 1), (-1, 1, 2)]:
        v = ipsdev.phi(m, n, nz, pts, BR, BZ)
        pire = max(pire, abs(integrale(v, v, pts, dr, dz) - 1.0))
    return pire < 1e-2, f"ecart max a 1 = {pire:.2e}"


def test_orthogonalite():
    """Deux fonctions de base differentes sont orthogonales."""
    pts, dr, dz = grille()
    paires = [((1, 1, 0), (1, 2, 0)),      # n different
              ((0, 1, 1), (0, 1, 2)),      # n_z different
              ((0, 2, 1), (0, 2, 3))]
    pire = 0.0
    for a, b in paires:
        fa = ipsdev.phi(*a, pts, BR, BZ)
        fb = ipsdev.phi(*b, pts, BR, BZ)
        pire = max(pire, abs(integrale(fa, fb, pts, dr, dz)))
    return pire < 1e-4, f"produit scalaire max = {pire:.2e}"


def test_format_retour():
    pts = [(0.1, 0.2), (0.3, 0.4), (0.5, 0.6)]
    v = ipsdev.phi(0, 1, 1, pts)
    ok = isinstance(v, list) and len(v) == 3 and all(isinstance(x, float) for x in v)
    ok &= ipsdev.phi(0, 0, 0, []) == []
    return ok, ""


def test_valeurs_par_defaut():
    """br et bz valent 1.0 par defaut."""
    pts = [(0.3, 0.2), (1.0, -0.4)]
    a = ipsdev.phi(1, 1, 1, pts)
    b = ipsdev.phi(1, 1, 1, pts, 1.0, 1.0)
    return a == b, ""


def test_erreurs():
    """Les mauvais arguments levent une exception Python (et ne plantent pas)."""
    cas = [
        ((0, 0, 0, [(1, 2, 3)]), ValueError),     # point a 3 coordonnees
        ((0, 0, 0, [(1,)]), ValueError),          # point a 1 coordonnee
        ((0, -1, 0, [(1, 2)]), ValueError),       # n < 0
        ((0, 0, -1, [(1, 2)]), ValueError),       # n_z < 0
        ((0, 0, 0, [(1, 2)], -1.0, 1.0), ValueError),  # br <= 0
        ((0, 0, 0, 5), TypeError),                # points n'est pas une sequence
        ((0, 0, 0, [("a", 2)]), TypeError),       # coordonnee non numerique
        ((0, 0), TypeError),                      # arguments manquants
    ]
    manques = []
    for args, attendu in cas:
        try:
            ipsdev.phi(*args)
            manques.append(f"{args} : aucune exception")
        except attendu:
            pass
        except Exception as e:
            manques.append(f"{args} : {type(e).__name__} au lieu de {attendu.__name__}")
    return not manques, "; ".join(manques)


def test_numpy():
    """Entree sous forme de tableau NumPy (ignore si NumPy n'est pas installe)."""
    try:
        import numpy as np
    except ImportError:
        return None, "NumPy absent"
    pts = np.array([[0.3, 0.2], [1.0, -0.4], [0.7, 0.1]])
    a = ipsdev.phi(1, 2, 1, pts, BR, BZ)
    b = ipsdev.phi(1, 2, 1, [tuple(p) for p in pts.tolist()], BR, BZ)
    return a == b, ""


# ----------------------------------------------------------------------
# Execution
# ----------------------------------------------------------------------
def main():
    tests = [test_valeur_analytique, test_reference, test_symetries, test_normalisation,
             test_orthogonalite, test_format_retour, test_valeurs_par_defaut,
             test_erreurs, test_numpy]
    echecs = 0
    print(f"Python {sys.version.split()[0]} - module : {getattr(ipsdev, '__file__', '?')}\n")
    for t in tests:
        nom = t.__name__.replace("test_", "")
        try:
            ok, detail = t()
        except Exception as e:                       # plantage inattendu = echec
            ok, detail = False, f"exception {type(e).__name__} : {e}"
        if ok is None:
            etat = "[SKIP]"
        elif ok:
            etat = "[ OK ]"
        else:
            etat = "[FAIL]"
            echecs += 1
        print(f"{etat} {nom:<22} {detail}")
    print()
    if echecs:
        print(f"{echecs} test(s) en echec.")
        return 1
    print("Tous les tests passent : le wrapper fonctionne.")
    return 0


if __name__ == "__main__":
    sys.exit(main())