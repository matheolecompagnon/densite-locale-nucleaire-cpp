/**
 * @file wrapper.cpp
 * @brief Module Python « simulation_cpp » : expose les fonctions de calcul de densité C++ à Python.
 * 
 * Ce fichier implémente l'interface de liaison entre les scripts Python et le cœur de 
 * calcul C++ en utilisant l'API C de Python et les objets de la bibliothèque Armadillo et NumPy.
 */

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <numpy/arrayobject.h>
#include <armadillo>

// Déclarations des fonctions de simulation existantes
arma::mat rho_naif(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);
arma::mat rho_linearized(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);
arma::mat rho_memory_max(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);
arma::mat rho_fabulous(const arma::vec rVals, const arma::vec zVals, int nbR, int nbZ);

/**
 * @brief Convertit un tableau NumPy 1D en vecteur Armadillo (`arma::vec`).
 * 
 * Cette fonction extrait le pointeur mémoire brut d'un `numpy.ndarray` pour l'injecter 
 * directement dans un vecteur Armadillo sans duplication inutile (principe de zéro-copie).
 * 
 * @param obj Pointeur vers l'objet Python représentant le tableau.
 * @param out_vec Référence vers le vecteur Armadillo de sortie.
 * @return true si la conversion réussit, false en cas d'erreur de type ou de dimension.
 */
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

    out_vec = arma::vec(data, dims[0], true);
    Py_DECREF(array);
    return true;
}

/**
 * @brief Convertit une matrice Armadillo (`arma::mat`) en un tableau NumPy 2D.
 * 
 * Alloue un nouveau tableau NumPy de dimensions 2D correspondantes et y copie 
 * les données calculées par le solveur C++.
 * 
 * @param mat La matrice Armadillo à convertir.
 * @return PyObject* Un pointeur vers le nouveau `numpy.ndarray` créé, ou nullptr en cas d'échec.
 */
PyObject* armamat_to_pyarray(const arma::mat& mat) {
    npy_intp dims[2] = { static_cast<npy_intp>(mat.n_rows), static_cast<npy_intp>(mat.n_cols) };
    PyObject* py_array = PyArray_SimpleNew(2, dims, NPY_DOUBLE);
    if (!py_array) {
        return nullptr;
    }

    double* dest_data = (double*)PyArray_DATA((PyArrayObject*)py_array);
    std::memcpy(dest_data, mat.memptr(), mat.n_elem * sizeof(double));

    return py_array;
}

/** @brief Enveloppe Python pour l'algorithme naïf (`rho_naif`). */
static PyObject* wrap_rho_naif(PyObject* self, PyObject* args) {
    (void)self;
    PyObject *py_rVals, *py_zVals;
    int nbR, nbZ;
    if (!PyArg_ParseTuple(args, "OOii", &py_rVals, &py_zVals, &nbR, &nbZ)) return nullptr;
    arma::vec rVals, zVals;
    if (!pyarray_to_armavec(py_rVals, rVals)) return nullptr;
    if (!pyarray_to_armavec(py_zVals, zVals)) return nullptr;
    arma::mat res = rho_naif(rVals, zVals, nbR, nbZ);
    return armamat_to_pyarray(res);
}

/** @brief Enveloppe Python pour l'algorithme linéarisé (`rho_linearized`). */
static PyObject* wrap_rho_linearized(PyObject* self, PyObject* args) {
    (void)self;
    PyObject *py_rVals, *py_zVals;
    int nbR, nbZ;
    if (!PyArg_ParseTuple(args, "OOii", &py_rVals, &py_zVals, &nbR, &nbZ)) return nullptr;
    arma::vec rVals, zVals;
    if (!pyarray_to_armavec(py_rVals, rVals)) return nullptr;
    if (!pyarray_to_armavec(py_zVals, zVals)) return nullptr;
    arma::mat res = rho_linearized(rVals, zVals, nbR, nbZ);
    return armamat_to_pyarray(res);
}

/** @brief Enveloppe Python pour l'algorithme avec mémoïsation maximale (`rho_memory_max`). */
static PyObject* wrap_rho_memory_max(PyObject* self, PyObject* args) {
    (void)self;
    PyObject *py_rVals, *py_zVals;
    int nbR, nbZ;
    if (!PyArg_ParseTuple(args, "OOii", &py_rVals, &py_zVals, &nbR, &nbZ)) return nullptr;
    arma::vec rVals, zVals;
    if (!pyarray_to_armavec(py_rVals, rVals)) return nullptr;
    if (!pyarray_to_armavec(py_zVals, zVals)) return nullptr;
    arma::mat res = rho_memory_max(rVals, zVals, nbR, nbZ);
    return armamat_to_pyarray(res);
}

/** @brief Enveloppe Python pour l'algorithme haute performance BLAS/DGEMM (`rho_fabulous`). */
static PyObject* wrap_rho_fabulous(PyObject* self, PyObject* args) {
    (void)self;
    PyObject *py_rVals, *py_zVals;
    int nbR, nbZ;
    if (!PyArg_ParseTuple(args, "OOii", &py_rVals, &py_zVals, &nbR, &nbZ)) return nullptr;
    arma::vec rVals, zVals;
    if (!pyarray_to_armavec(py_rVals, rVals)) return nullptr;
    if (!pyarray_to_armavec(py_zVals, zVals)) return nullptr;
    arma::mat res = rho_fabulous(rVals, zVals, nbR, nbZ);
    return armamat_to_pyarray(res);
}

/// Table des méthodes du module exposées à l'interpréteur Python
static PyMethodDef SimulationMethods[] = {
    {"rho_naif", wrap_rho_naif, METH_VARARGS, "Calcul de la densité nucléaire via l'algorithme naïf."},
    {"rho_linearized", wrap_rho_linearized, METH_VARARGS, "Calcul de la densité via l'algorithme linéarisé."},
    {"rho_memory_max", wrap_rho_memory_max, METH_VARARGS, "Calcul de la densité avec mémoïsation maximale."},
    {"rho_fabulous", wrap_rho_fabulous, METH_VARARGS, "Calcul de la densité optimisée BLAS / DGEMM."},
    {nullptr, nullptr, 0, nullptr}
};

/// Définition structurelle du module Python
static struct PyModuleDef simulationmodule = {
    PyModuleDef_HEAD_INIT,
    "simulation_cpp",
    "Module C++ pour les calculs de densité nucléaire avec Armadillo",
    -1,
    SimulationMethods,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

/**
 * @brief Point d'entrée d'initialisation du module lors de l'instruction `import simulation_cpp` en Python.
 */
PyMODINIT_FUNC PyInit_simulation_cpp(void) {
    import_array(); // Initialisation indispensable de l'API C de NumPy
    return PyModule_Create(&simulationmodule);
}
