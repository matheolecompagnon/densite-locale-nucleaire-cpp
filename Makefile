CXX := g++

OPTIONS_DEBUG := -Wall -Wextra -Werror -g -O0 -ggdb -fsanitize=address -fno-omit-frame-pointer -Iinclude
OPTIONS_RELEASE := -Wall -Wextra -Werror -O2 -Iinclude

debug: OPTIONS := $(OPTIONS_DEBUG)
release: OPTIONS := $(OPTIONS_RELEASE)

debug: run
release: run

files := main Basis Poly
objects := $(files:%=./bin/%.o)



run : $(objects)
	$(CXX) -o exec $^ $(OPTIONS)
	@echo "\n\n========== Debut Affichage Programme ==========\n\n"
	@./exec
	@echo "\n\n========== Fin Affichage Programme ==========\n\n"

$(objects): ./bin/%.o: ./src/%.cpp
	$(CXX) -c $^ -o $@ $(OPTIONS)


clean :
	rm -f $(objects)
	rm -f exec


