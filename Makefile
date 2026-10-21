# =============================================================================
#  Makefile
#
#  Ce fichier est généré par claude Sonnet 5.5, vérifié par Mathéo Lecompagnon
#
#  make / make debug     binaire debug (ASan)        -> build/model-debug
#  make release          binaire optimisé            -> build/model
#  make bench            binaire de benchmark        -> build/model-bench
#  make perf             profilage avec perf         -> build/model-perf
#  make run              compile et lance (debug)    (ARGS="...")
#  make run-release      compile et lance (release)  (ARGS="...")
#  make test             tests unitaires CxxTest     -> build/test
#  make python           module Python               -> build/simulation_cpp.so
#  make generate         lance python/jmmm/generation_wrapper_v0.py
#  make clean            supprime bin/ (build/ est conservé)
# =============================================================================

.DEFAULT_GOAL := all

CXX        ?= g++
CXXTESTGEN ?= cxxtestgen
PYTHON     ?= python3
PY_NUMPY_INCLUDE := $(shell $(PYTHON) -c "import numpy; print(numpy.get_include())" 2>/dev/null)

# ---------- Projet ----------
main_exec    := model
lib_files    := basis poly simulation bench
test_headers := $(wildcard tests/test_*.h)

# ---------- Options de compilation ----------
COMMON   := -std=c++17 -Wall -Wextra -Werror -Iinclude
DEPFLAGS := -MMD -MP
LDLIBS   := -larmadillo

FLAGS_debug   := $(COMMON) -g -O0 -fsanitize=address -fno-omit-frame-pointer
FLAGS_release := $(COMMON) -O0
FLAGS_bench   := $(FLAGS_release) -DBENCH
FLAGS_perf    := $(COMMON) -O0 -g -fno-omit-frame-pointer
FLAGS_python  := $(FLAGS_release) -fPIC -I$(PY_NUMPY_INCLUDE)

exe_debug   := build/$(main_exec)-debug
exe_release := build/$(main_exec)
exe_bench   := build/$(main_exec)-bench
exe_perf    := build/$(main_exec)-perf

# ---------- Règles génériques ----------
# Les objets sont dans bin/ et portent le nom du mode (bin/basis-debug.o,
# bin/basis-release.o...) pour ne jamais mélanger des options différentes.
define BUILD_MODE
objs_$(1) := $(lib_files:%=bin/%-$(1).o)

bin/%-$(1).o: src/%.cpp | bin
	$$(CXX) $$(FLAGS_$(1)) $$(DEPFLAGS) -c $$< -o $$@
endef

define LINK_EXE
$(2): bin/main-$(1).o $$(objs_$(1)) | build
	$$(CXX) $$(FLAGS_$(1)) $$^ -o $$@ $$(LDLIBS)
endef

$(foreach m,debug release bench perf python,$(eval $(call BUILD_MODE,$(m))))
$(eval $(call LINK_EXE,debug,$(exe_debug)))
$(eval $(call LINK_EXE,release,$(exe_release)))
$(eval $(call LINK_EXE,bench,$(exe_bench)))
$(eval $(call LINK_EXE,perf,$(exe_perf)))

bin:
	mkdir -p $@
build:
	mkdir -p $@

.PHONY: all debug release bench perf run run-release test python generate clean

# ---------- Programme principal ----------
all: debug

debug: $(exe_debug)
release: $(exe_release)
bench: $(exe_bench)

perf: $(exe_perf)
	perf record -F 999 -g ./$< $(ARGS)
	perf report --no-children --stdio 2>/dev/null | head -50

run: $(exe_debug)
	@printf "\n\n====== Debut Affichage Programme DEBUG ======\n\n"
	@./$< $(ARGS)
	@printf "\n\n========== Fin Affichage Programme ==========\n\n"

run-release: $(exe_release)
	@printf "\n\n========== Debut Affichage Programme ==========\n\n"
	@./$< $(ARGS)
	@printf "\n\n========== Fin Affichage Programme ==========\n\n"

# ---------- Tests unitaires (CxxTest) ----------
test_cpp  := bin/tests.cpp
test_obj  := bin/tests.o
test_exec := build/test

test: $(test_exec)
	@printf "\n\n========== Execution de la Suite de Tests ==========\n\n"
	@./$<
	@printf "\n\n====================================================\n\n"

$(test_exec): $(test_obj) $(objs_debug) | build
	$(CXX) $(FLAGS_debug) $^ -o $@ $(LDLIBS)

$(test_obj): $(test_cpp)
	$(CXX) $(FLAGS_debug) -c $< -o $@

$(test_cpp): $(test_headers) | bin
	$(CXXTESTGEN) --error-printer -o $@ $^

# ---------- Module Python (wrapper) & script de génération ----------
PY_MODULE   := build/simulation_cpp.so
PY_INCLUDES = $(shell $(PYTHON)-config --includes)

python: $(PY_MODULE)

$(PY_MODULE): src/wrapper.cpp $(objs_python) | build
	$(CXX) $(FLAGS_python) -shared $(PY_INCLUDES) $^ -o $@ $(LDLIBS)

generate: python
	@printf "\n\n==== Lancement de generation_wrapper_v0.py ====\n\n"
	PYTHONPATH=build $(PYTHON) python/jmmm/generation_wrapper_v0.py
	@printf "\n\n================================================\n\n"

# ---------- Dépendances automatiques (headers) ----------
-include $(wildcard bin/*.d)

# ---------- Nettoyage ----------
clean:
	rm -rf bin
