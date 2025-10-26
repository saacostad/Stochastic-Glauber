# Compiler
CXX = g++ 
CXXFLAGS = -lgsl -std=c++23 -O3 -fopenmp -fmodules 

# Targets and files 
TARGET = exec
SRC = main.cpp
DATAFILE = systems/Au79-Au79/projectiles.csv systems/Au79-Au79/targets.csv data.csv
# DATAFILE = 
GRAPH = outils/graph.py

# Default  rule
all: build clean run_all
nograph: build clean run_no_graph
graph: graph
build: build

# Compile the C++ program 
build: $(SRC)
	$(CXX) $(SRC) $(CXXFLAGS) -o $(TARGET)


# Remove the datafile in case of needed
clean: $(SRC)
	rm -f $(DATAFILE)


# Run the program depending on what one wants
run_all: $(SRC) $(GRAPH)
	./$(TARGET) >> data.csv
	# python $(GRAPH)
	python outils/calculateCrossSection.py


run_no_graph: $(SRC)
	./$(TARGET)

graph: $(GRAPH)
	python $(GRAPH)
