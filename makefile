
  all: world.cpp CellSim.cpp cell.cpp functions.cpp DNA.cpp
	g++ -c world.cpp CellSim.cpp cell.cpp functions.cpp DNA.cpp
	g++ world.o CellSim.o cell.o functions.o DNA.o -o CellSim -lsfml-graphics -lsfml-window -lsfml-system

  clean: 
	  $(RM) test.o
