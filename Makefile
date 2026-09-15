#variable stockant le compilateur C++ utilisé
CXX := g++

#variables stockant les otions de compilation de base ainsi qu'un mode debug
OPTIONS := -Wall -Wextra -Werror -Iinclude
DEBUG_FLAGS := -g3 -Og -fsanitize=undefined,address -fno-omit-frame-pointer

#variable stockant le nom des fichiers à compiler
files := main Basis Poly
#variable construit pat une régles qui prend le nom des fichier et les cherchers dans bin
objects := $(files:%=./bin/%.o)
objects_debug := $(files:%=./bin/%.o)


#régle de base qu'executera le MakeFile
run : $(objects)
	$(CXX) -o exec $^ $(OPTIONS)
	@echo "\n\n========== Debut Affichage Programme ==========\n\n"
	@./exec
	@echo "\n\n========== Fin Affichage Programme ==========\n\n"

#régle de compilation des fichiers objets
$(objects): ./bin/%.o: ./src/%.cpp #on change les cible .o en cible .c dans le bon fichier
	$(CXX) -c $^ -o $@ $(OPTIONS) 

#régle de debug pour le MakeFile
debug : $(objects_debug)
	$(CXX) -o exec $^ $(OPTIONS) $(DEBUG_FLAG)
	@echo "\n\n========== Debut Affichage Programme ==========\n\n"
	@./exec
	@echo "\n\n========== Fin Affichage Programme ==========\n\n"

#régle de compilation des fichiers objets
$(objects_debug): ./bin/%.o: ./src/%.cpp #on change les cible .o en cible .c dans le bon fichier
	$(CXX) -c $^ -o $@ $(OPTIONS) $(DEBUG_FLAG)

#régle pour netoyer le depot avant de push sur le git
clean :
	rm -f $(objects)
	rm -f exec


