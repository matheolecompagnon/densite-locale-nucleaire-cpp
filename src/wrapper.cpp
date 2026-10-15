/**
 * @file wrapper.cpp
 * @brief Module Python « ipsdev » (Python.h) : expose la fonction phi du projet IPS-DEV.
 *
 * Python :
 *     import ipsdev
 *     vals = ipsdev.phi(m, n, n_z, [(r0), (r1), ...], [(r0), (r1), ...])
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
#include <Python.h>
#include <armadillo>

// Déclarations des fonctions existantes (provenant de simulation.cpp)
arma::mat rho_naif(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);
arma::mat rho_fabulous(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);

// Fonction utilitaire pour convertir un tableau NumPy 1D en arma::vec
bool pyarray_to_armavec(PyObject* obj, arma::vec& out_vec) {
    PyArrayObject* array = (PyArrayObject*)PyArray_FROM_OTF(obj, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY);
    if (!array) {
        PyErr_SetString(PyExc_TypeError, "Le tableau doit être un numpy.ndarray de type float64 (1D).");
        return false;
    }

    if (PyArray_NDIM(array) != 1) {
        PyErr_SetString(PyExc_ValueError, "Le tableau doit être de dimension 1.");
        Py_DECREF(array);
        return false;
    }

    npy_intp* dims = PyArray_DIMS(array);
    double* data = (double*)PyArray_DATA(array);

    out_vec = arma::vec(data, dims[0], /*copy_aux_mem=*/true);
    Py_DECREF(array);
    return true;
}

// Fonction utilitaire pour convertir un arma::mat en numpy.ndarray 2D
PyObject* armamat_to_pyarray(const arma::mat& mat) {
    npy_intp dims[2] = { (npy_intp)mat.n_rows, (npy_intp)mat.n_cols };
    // Création d'un tableau numpy vide
    PyObject* py_array = PyArray_SimpleNew(2, dims, NPY_DOUBLE);
    if (!py_array) {
        return nullptr;
    }

    double* dest_data = (double*)PyArray_DATA((PyArrayObject*)py_array);
    // Copie des données d'Armadillo vers NumPy
    std::memcpy(dest_data, mat.memptr(), mat.n_elem * sizeof(double));

    return py_array;
}

// --- Wrapper pour rho_naif ---
static PyObject* wrap_rho_naif(PyObject* self, PyObject* args) {
    PyObject *py_rVals, *py_zVals;
    int nbR, nbZ;

    if (!PyArg_ParseTuple(args, "OOii", &py_rVals, &py_zVals, &nbR, &nbZ)) {
        return nullptr;
    }

    arma::vec rVals, zVals;
    if (!pyarray_to_armavec(py_rVals, rVals)) return nullptr;
    if (!pyarray_to_armavec(py_zVals, zVals)) return nullptr;

    arma::mat res = rho_naif(rVals, zVals, nbR, nbZ);

    return armamat_to_pyarray(res);
}

// --- Wrapper pour rho_fabulous ---
static PyObject* wrap_rho_fabulous(PyObject* self, PyObject* args) {
    PyObject *py_rVals, *py_zVals;
    int nbR, nbZ;

    if (!PyArg_ParseTuple(args, "OOii", &py_rVals, &py_zVals, &nbR, &nbZ)) {
        return nullptr;
    }

    arma::vec rVals, zVals;
    if (!pyarray_to_armavec(py_rVals, rVals)) return nullptr;
    if (!pyarray_to_armavec(py_zVals, zVals)) return nullptr;

    arma::mat res = rho_fabulous(rVals, zVals, nbR, nbZ);

    return armamat_to_pyarray(res);
}

// Table des méthodes du module
static PyMethodDef SimulationMethods[] = {
    {"rho_naif", wrap_rho_naif, METH_VARARGS, "Calcul rho_naif"},
    {"rho_fabulous", wrap_rho_fabulous, METH_VARARGS, "Calcul rho_fabulous"},
    {nullptr, nullptr, 0, nullptr}
};

// Structure du module
static struct PyModuleDef simulationmodule = {
    PyModuleDef_HEAD_INIT,
    "simulation_cpp",
    "Module C++ pour les calculs de rho avec Armadillo",
    -1,
    SimulationMethods
};

// Initialisation du module
PyMODINIT_FUNC PyInit_simulation_cpp(void) {
    import_array(); // Indispensable pour initialiser NumPy C-API
    return PyModule_Create(&simulationmodule);
}