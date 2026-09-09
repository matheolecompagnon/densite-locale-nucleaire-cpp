CXX := g++

OPTIONS := -Wall -Wextra -Werror

src := ./src/Basis.cpp ./src/Poly.cpp
objects := ./bin/Basis.o ./bin/Poly.o



all: run clean

run : $(objects)
	$(CXX) -o exec ./include/$^ $(OPTIONS)
	@echo "\n\n========== Debut Affichage Programme ==========\n\n"
	@./exec
	@echo "\n\n========== Fin Affichage Programme ==========\n\n"

$(objects): %.o: %.c
	$(CXX) -c ./bin/$^ -o ./src/$@ $(OPTIONS)


clean :
	rm -f $(objects)
	rm -f exec


