CXX = g++
CXXFLAGS = -O2
LIBS = -ltermbox

default: graph 

typings: graph.cpp 
	$(CXX) $(CXXFLAGS) graph.cpp -o graph $(LIBS)

clean:
	$(RM) graph