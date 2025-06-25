# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

import os
import sys
sys.path.insert(0, os.path.abspath('..'))

project = 'SM4rel4in'
copyright = '2025, Sequent Microsystems'
author = 'Sequent Microsystems'
release = '1.0.6'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
        'sphinx_markdown_builder',
        'sphinx.ext.autodoc',
        'sphinx.ext.napoleon',
        'sphinx.ext.todo',
]

exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']
