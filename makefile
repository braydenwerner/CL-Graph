CXX = g++
CXXFLAGS = -O2
LIBS = -ltermbox

default: graph

graph: graph.cpp 
	$(CXX) $(CXXFLAGS) graph.cpp -o graph $(LIBS)

clean:
	$(RM) graph