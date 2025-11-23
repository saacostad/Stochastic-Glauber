# Compiler
CXX = g++ 
CXXFLAGS = -lgsl -std=c++23 -O3 -fopenmp -fmodules 

# RUNPARAMS_READ = -m read -i test -E 200.0 -t 197 -p 197 -o data_test.csv
# RUNPARAMS_WRITE = -m write -i test -E 200.0 -T 100 -P 100 -t 197 -p 197


# For uranium
RUNPARAMS_READ = -m read -i test_uranium -E 200.0 -t 238 -p 238 -o rotated_uranium.csv
RUNPARAMS_WRITE = -m write -i test_uranium -E 200.0 -T 100 -P 100 -t 238 -p 238


# Targets and files 
TARGET = StocasticGlauber
SRC = main.cpp
DATAFILE = systems/test data_test.csv

GRAPH = outils/graph.py

# Default  rule
all: build clean run_all
nograph: build clean run_no_graph
graph: graph
build: build
test: build clean run_test run_sigma
test_nb: clean run_test run_sigma
test_run: run_test run_sigma


# Compile the C++ program 
build: 
	$(CXX) $(SRC) $(CXXFLAGS) -o $(TARGET)


# Remove the datafile in case of needed
clean: $(SRC)
	rm -r $(DATAFILE)

run_test: $(SRC)
	./$(TARGET) $(RUNPARAMS_WRITE) 
	./$(TARGET) $(RUNPARAMS_READ) 

run_sigma:
	python outils/calculateCrossSection.py

# Run the program depending on what one wants
run_all: $(SRC) $(GRAPH)
	./$(TARGET) >> data.csv
	# python $(GRAPH)
	python outils/calculateCrossSection.py


run_no_graph: $(SRC)
	./$(TARGET)

graph: $(GRAPH)
	python $(GRAPH)
