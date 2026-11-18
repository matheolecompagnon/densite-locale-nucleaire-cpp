import numpy
import os
from setuptools import setup, Extension

# ==============================================================================
# ==============================================================================
# ==============================================================================

def get_env(key):
  try:
    v = os.environ[key]
  except:
    v = ''
  return v

module1 = Extension('_density_simulation',
                    include_dirs = ['./include/armanpy/', '../include', numpy.get_include()],
                    libraries = ['m', 'z', 'armadillo'],
                    sources = ['density_simulation.i', '../src/simulation.cpp', '../src/basis.cpp', '../src/poly.cpp'],
                    swig_opts = ["-c++", "-Wall", "-I.", "-I./include/armanpy/", "-I../include"])

setup (name = 'density_simulation',
       py_modules = ['density_simulation'],
       version = '1.0',
       description = 'This module is a binding for some CPP functions from project IPS-DEV. ',
       ext_modules = [module1])

