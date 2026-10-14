/**
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
 */
#define PY_SSIZE_T_CLEAN
#include <Python.h>   // toujours en premier, avant les autres headers

#include <armadillo>
#include <exception>
#include <stdexcept>
#include <vector>

#include "basis.hpp"
#include "trucation.cpp"

#define N 10
#define Q 2

/* ======================================================================
 *  1. Code C++ « pur » : aucune dépendance à Python
 * ====================================================================== */

struct PointRZ { double r, z; };

// phi en tous les points : un vecteur de points en entrée, un vecteur de valeurs en sortie.
static std::vector<double> phi(int m, int n, int nz, double br, double bz,
                               const std::vector<PointRZ>& pts) {
    if (n < 0 || nz < 0) throw std::invalid_argument("n et n_z doivent être >= 0");
    if (br <= 0.0 || bz <= 0.0) throw std::invalid_argument("br et bz doivent être > 0");
    if (pts.empty()) return {};

    // On sépare les coordonnées : rPart et zPart travaillent sur des vecteurs.
    arma::vec r(pts.size()), z(pts.size());
    for (std::size_t i = 0; i < pts.size(); ++i) {
        r(i) = pts[i].r;
        z(i) = pts[i].z;
    }

    // N et Q ne servent qu'à la troncature de la base : inutiles pour calculer phi.
    Basis basis(br, bz, 0, 1.0);

    arma::vec R = basis.rPart(r, m, n);   // R(r_i, m, n)
    arma::vec Z = basis.zPart(z, nz);     // Z(z_i, n_z)

    std::vector<double> out(pts.size());
    for (std::size_t i = 0; i < pts.size(); ++i)
        out[i] = Z(i) * R(i);             // produit terme à terme : phi(r_i, z_i)
    return out;
}

static std::vector<double> rho_naif(double br, double bz, const std::vector<PointRZ>& pts){

    arma::mat rho;

    if (!rho.load("rho.arma")) {          // load renvoie false en cas d'échec
        std::cerr << "Impossible de lire rho.arma\n";
        return 1;
    }

    int m_max = m_max(N,Q); 
    
    arma::vec result_vec(pts.n_elem, arma::fill::zeros);

    /*somme sur touss les point a*/
    arma::mat result = arma::zeros(nbR, nbZ); // number of points on r- and z- axes
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
                            arma::mat funcA = basis.basisFunc( m,  n,  n_z, zVals, rVals);
                            arma::mat funcB = basis.basisFunc(mp, np, n_zp, zVals, rVals);
                            result += funcA % funcB * rho(m, n, n_z, mp, np, n_zp); // mat += mat % mat * double
                        }
                        }
                }
            }
        }
    }

}

/* ======================================================================
 *  2. Le wrapper (couche de traduction Python <-> C++)
 * ====================================================================== */

// Séquence Python de couples (r, z) -> std::vector<PointRZ>
static bool parse_points(PyObject* obj, std::vector<PointRZ>& out) {
    PyObject* seq = PySequence_Fast(obj, "les points doivent former une séquence de couples (r, z)");
    if (!seq) return false;                                      // nouvelle référence

    Py_ssize_t n = PySequence_Fast_GET_SIZE(seq);
    out.reserve(static_cast<std::size_t>(n));

    for (Py_ssize_t i = 0; i < n; ++i) {
        PyObject* item = PySequence_Fast_GET_ITEM(seq, i);       // référence empruntée

        PyObject* couple = PySequence_Fast(item, "chaque point doit être un couple (r, z)");
        if (!couple) { Py_DECREF(seq); return false; }
        if (PySequence_Fast_GET_SIZE(couple) != 2) {
            PyErr_SetString(PyExc_ValueError, "chaque point doit avoir exactement 2 coordonnées (r, z)");
            Py_DECREF(couple); Py_DECREF(seq);
            return false;
        }

        double r = PyFloat_AsDouble(PySequence_Fast_GET_ITEM(couple, 0));
        double z = PyFloat_AsDouble(PySequence_Fast_GET_ITEM(couple, 1));
        Py_DECREF(couple);
        if ((r == -1.0 || z == -1.0) && PyErr_Occurred()) { Py_DECREF(seq); return false; }

        out.push_back({r, z});
    }
    Py_DECREF(seq);
    return true;
}

// phi(m, n, n_z, points, br=1.0, bz=1.0) -> list[float]
static PyObject* py_phi(PyObject* /*self*/, PyObject* args) {
    int m, n, nz;
    PyObject* points_obj;
    double br = 1.0, bz = 1.0;                    // valeurs par défaut des arguments optionnels

    // "iiiO|dd" : 3 entiers, 1 objet, puis (après '|') 2 doubles optionnels
    if (!PyArg_ParseTuple(args, "iiiO|dd", &m, &n, &nz, &points_obj, &br, &bz))
        return nullptr;

    std::vector<PointRZ> pts;
    if (!parse_points(points_obj, pts)) return nullptr;

    std::vector<double> vals;
    try {
        vals = phi(m, n, nz, br, bz, pts);
    } catch (const std::invalid_argument& e) {
        PyErr_SetString(PyExc_ValueError, e.what());
        return nullptr;
    } catch (const std::exception& e) {           // y compris les erreurs d'Armadillo
        PyErr_SetString(PyExc_RuntimeError, e.what());
        return nullptr;
    } catch (...) {
        PyErr_SetString(PyExc_RuntimeError, "erreur C++ inconnue");
        return nullptr;
    }

    // std::vector<double> -> liste Python
    PyObject* out = PyList_New(static_cast<Py_ssize_t>(vals.size()));
    if (!out) return nullptr;
    for (std::size_t i = 0; i < vals.size(); ++i) {
        PyObject* v = PyFloat_FromDouble(vals[i]);
        if (!v) { Py_DECREF(out); return nullptr; }
        PyList_SET_ITEM(out, static_cast<Py_ssize_t>(i), v);     // vole la référence
    }
    return out;
}

static PyMethodDef Methods[] = {
    {"phi", py_phi, METH_VARARGS,
     "phi(m, n, n_z, points, br=1.0, bz=1.0) -> list\n"
     "Valeur de phi_{m,n,n_z} (sans la phase e^{i m theta}) en chaque point (r, z)."},
    {nullptr, nullptr, 0, nullptr}
};

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT, "ipsdev", "Wrapper Python du projet IPS-DEV",
    -1, Methods, nullptr, nullptr, nullptr, nullptr
};

PyMODINIT_FUNC PyInit_ipsdev(void) {
    return PyModule_Create(&moduledef);
}