# Compiler
CXX = g++ 
CXXFLAGS = -lgsl

# Targets and files 
TARGET = exec
SRC = main.cpp
DATAFILE = data.csv 
GRAPH = graph.py

# Default  rule
all: build clean run_all
nograph: build clean run_no_graph

# Compile the C++ program 
build: $(SRC)
	$(CXX) $(SRC) $(CXXFLAGS) -o $(TARGET)


# Remove the datafile in case of needed
clean: $(SRC)
	rm -f $(DATAFILE)


# Run the program depending on what one wants
run_all: $(SRC) $(GRAPH)
	./$(TARGET) >> $(DATAFILE)
	python $(GRAPH)


run_no_graph: $(SRC)
	./$(TARGET)
