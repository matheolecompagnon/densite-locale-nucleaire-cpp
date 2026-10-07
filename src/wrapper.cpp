#include <pythob.h>

#include "basis.hpp"
#include "poly.hpp"
#include "trucature.hpp"

static PyObject* phi(PyObject *self, PyObject *args){

    int m,n,nz;
    PyObject tupple_vect;
    if (!PyArg_ParseTuple(args, "iiiO", &m, &n, &nz, &tupple_vect))
        return NULL;

    // Transforme liste/tuple/... en séquence facile à parcourir
    PyObject* seq = PySequence_Fast(liste, "le 4e argument doit être une séquence de points");
    if (!seq) return nullptr;

    Py_ssize_t n = PySequence_Fast_GET_SIZE(seq);
    std::vector<Point> pts;
    pts.reserve(n);    Poly hermite;

    
}