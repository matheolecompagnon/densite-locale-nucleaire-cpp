CXX := g++

OPTIONS_DEBUG := -Wall -Wextra -Werror -g -O0 -ggdb -fsanitize=address -fno-omit-frame-pointer -Iinclude
OPTIONS_RELEASE := -Wall -Wextra -Werror -O2 -Iinclude

OPTIONS := $(OPTIONS_DEBUG)

debug: OPTIONS := $(OPTIONS_DEBUG)
release: OPTIONS := $(OPTIONS_RELEASE)

.PHONY: all debug release run clean test

all: debug
debug: run
release: run


unit_debug: OPTIONS := $(OPTIONS_DEBUG)
unit_test: OPTIONS := $(OPTIONS_RELEASE)

unit_test: tests
unit_debug: tests

files := Basis Poly
objects := $(files:%=./bin/%.o)

test_dependencies := bin/Basis.o bin/Poly.o
test_headers := tests/test_basis.hpp tests/test_mandatory.hpp tests/test_poly.hpp
test_generated_cpp := bin/t.cpp
test_generated_obj := bin/t.o
test_exec := tests

run : $(objects)
	$(CXX) -o exec ./src/main.cpp $^ $(OPTIONS)
	@echo "\n\n========== Debut Affichage Programme ==========\n\n"
	@./exec
	@echo "\n\n========== Fin Affichage Programme ==========\n\n"

tests : $(objects)
	$(CXX) -o exec ./tests/main_tests.cpp $^ $(OPTIONS)
	@echo "\n\n========== Debut Affichage Programme ==========\n\n"
	@./exec
	@echo "\n\n========== Fin Affichage Programme ==========\n\n"

test: $(test_generated_obj) $(test_dependencies)
	$(CXX) $(OPTIONS) $^ -o $(test_exec)
	@echo "\n\n========== Execution de la Suite de Tests ==========\n\n"
	@./$(test_exec)
	@echo "\n\n====================================================\n\n"

$(objects): ./bin/%.o: ./src/%.cpp
	@mkdir -p bin
	$(CXX) $(OPTIONS) -c $< -o $@


$(test_generated_cpp): $(test_headers)
	@mkdir -p bin
	cxxtestgen --error-printer -o $@ $(test_headers)


$(test_generated_obj): $(test_generated_cpp)
	$(CXX) $(OPTIONS) -c $< -o $@


clean :
	rm -f $(objects) exec $(test_exec)
	rm -rf bin


