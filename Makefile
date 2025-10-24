# Compiler
CXX = g++ 
CXXFLAGS = -lgsl -std=c++23

# Targets and files 
TARGET = exec
SRC = main.cpp
DATAFILE = systems/nucleusTest.csv systems/nucleus1.csv systems/nucleus2.csv 
GRAPH = outils/graph.py

# Default  rule
all: build clean run_all
nograph: build clean run_no_graph
graph: graph 

# Compile the C++ program 
build: $(SRC)
	$(CXX) $(SRC) $(CXXFLAGS) -o $(TARGET)


# Remove the datafile in case of needed
clean: $(SRC)
	rm -f $(DATAFILE)


# Run the program depending on what one wants
run_all: $(SRC) $(GRAPH)
	./$(TARGET)
	python $(GRAPH)


run_no_graph: $(SRC)
	./$(TARGET)

graph: $(GRAPH)
	python $(GRAPH)
