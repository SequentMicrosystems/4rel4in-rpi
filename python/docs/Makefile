SPHINX_BUILD  ?= sphinx-build
PYTHON	      = python
SETUP_PY      = setup.py
SETUP_DISTS   = sdist bdist_wheel
DOC_SRCDIR    = docs
DOC_BLDDIR    = docs

.PHONY: all Makefile

all: dist markdown

dist:
	$(PYTHON) $(SETUP_PY) sdist bdist_wheel

markdown:
	@$(SPHINX_BUILD) -M $@ "$(DOC_SRC)" "$(DOC_BLDDIR)"
	@cp docs/markdown/index.md README.md
