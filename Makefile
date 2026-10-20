CXX        := g++
CXXTESTGEN := cxxtestgen

OPTIONS_COMMON  := -Wall -Wextra -Werror -Iinclude
OPTIONS_DEBUG   := $(OPTIONS_COMMON) -g -O0 -ggdb -fsanitize=address -fno-omit-frame-pointer
OPTIONS_RELEASE := $(OPTIONS_COMMON) -O2
DEPFLAGS        := -MMD -MP
LDLIBS          := -larmadillo

# Options par défaut (surchargées par les cibles debug, release et test)
OPTIONS := $(OPTIONS_DEBUG)

files   := basis poly simulation bench
objects := $(files:%=bin/%.o)

main_exec    := exec
test_headers := tests/test_basis.h tests/test_mandatory.h tests/test_poly.h
test_cpp     := bin/tests.cpp
test_obj     := bin/tests.o
test_exec    := bin/test

.PHONY: all debug release run run-release test generate bench perf clean

all: debug

debug: OPTIONS := $(OPTIONS_DEBUG)
debug: $(main_exec)

release: OPTIONS := $(OPTIONS_RELEASE)
release: $(main_exec)

# ---------- Benchmark ----------

bench: OPTIONS := $(OPTIONS_RELEASE) -DBENCH
bench: $(main_exec)

# ------------- Perf ------------

perf: OPTIONS := $(OPTIONS_COMMON) -O2 -g -fno-omit-frame-pointer
perf: $(main_exec)
	perf record -F 999 -g ./$(main_exec) $(ARGS)
	perf report --no-children --stdio 2>/dev/null | head -50

# ---------- Programme principal ----------
run: debug
	@printf "\n\n====== Debut Affichage Programme DEBUG ======\n\n"
	@./$(main_exec) $(ARGS)
	@printf "\n\n========== Fin Affichage Programme ==========\n\n"

run-release: release
	@printf "\n\n========== Debut Affichage Programme ==========\n\n"
	@./$(main_exec) $(ARGS)
	@printf "\n\n========== Fin Affichage Programme ==========\n\n"

$(main_exec): src/main.cpp $(objects)
	$(CXX) $(OPTIONS) $^ -o $@ $(LDLIBS)

# ---------- Tests unitaires (CxxTest) ----------
test: OPTIONS := $(OPTIONS_DEBUG)
test: $(test_exec)
	@printf "\n\n========== Execution de la Suite de Tests ==========\n\n"
	@./$(test_exec)
	@printf "\n\n====================================================\n\n"

$(test_exec): $(test_obj) $(objects)
	$(CXX) $(OPTIONS) $^ -o $@ $(LDLIBS)

$(test_obj): $(test_cpp)
	$(CXX) $(OPTIONS) -c $< -o $@

$(test_cpp): $(test_headers) | bin
	$(CXXTESTGEN) --error-printer -o $@ $(test_headers)


# ---------- Module Python & Script de génération ----------
#generate: $(PY_TARGET)
#	@printf "\n\n========== Lancement de generation.py ==========\n\n"
#	python3 generation.py
#	@printf "\n\n===============================================\n\n"
#
#$(PY_TARGET): $(PY_OBJS) $(objects)
#	$(CXX) $(OPTIONS) -shared $^ -o $@ $(LDLIBS)
#
#bin/wrapper.o: src/wrapper.cpp | bin
#	$(CXX) $(OPTIONS) $(PYTHON_INCLUDES) $(NUMPY_INCLUDES) $(DEPFLAGS) -c $< -o $@


# ---------- Compilation des sources ----------
bin/%.o: src/%.cpp | bin
	$(CXX) $(OPTIONS) $(DEPFLAGS) -c $< -o $@

bin:
	mkdir -p $@

# Recompile automatiquement si un header change (fichiers .d générés par -MMD)
-include $(objects:.o=.d)

clean:
	rm -rf bin $(main_exec)
