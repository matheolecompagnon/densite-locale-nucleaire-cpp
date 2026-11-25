#!/usr/bin/env python3
"""
Tests unitaires des bindings SWIG (module density_simulation).

Lancement :   python3 tests/python/test_bindings.py
"""
import math
import os
import sys
import unittest

import numpy as np
import numpy.testing as npt

RACINE = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
sys.path.append(os.path.join(RACINE, "bindings"))

import density_simulation as d

BR, BZ, N, Q = 2.829683956491218, 1.935801664793151, 14, 1.3


class TestBindings(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        os.chdir(RACINE)  # rho_* charge data/rho.arma en chemin relatif
        cls.r = np.linspace(0, 10, 32)
        cls.z = np.linspace(-20, 20, 64)
        cls.basis = d.Basis(BR, BZ, N, Q)

    def test_rPart(self):
        v = self.basis.rPart(self.r, 0, 0)
        self.assertEqual(v.shape, (32,))
        self.assertAlmostEqual(v[0], 1 / (BR * math.sqrt(math.pi)))

    def test_zPart(self):
        v = self.basis.zPart(np.array([0.0]), 0)
        self.assertAlmostEqual(v[0], 1 / (math.sqrt(BZ) * math.pi ** 0.25))

    def test_basis_repete(self):
        # Régression : segfault quand Basis était redéclarée dans le header des bindings
        for _ in range(1000):
            d.Basis(BR, BZ, N, Q).rPart(self.r, 1, 1)

    def test_rho_ultimate(self):
        rho = d.rho_ultimate(self.r, self.z, 32, 64)
        self.assertEqual(rho.shape, (32, 64))
        self.assertGreater(np.abs(rho).max(), 0)

    def test_arma_load_mat(self):
        m = np.zeros((1, 1), order="F")
        d.arma_load_mat(m, "data/rho.arma")
        self.assertEqual(m.shape, (374, 374))
        npt.assert_array_equal(m, m.T)
        print(f"trace rho : {np.trace(m)}")

    def test_mauvais_type(self):
        with self.assertRaises(TypeError):
            self.basis.rPart([0.0, 1.0], 0, 0)


if __name__ == "__main__":
    unittest.main()
