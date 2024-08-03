CXX       := g++
CXX_FLAGS := -std=c++17 -O3 -Wall

BIN     := bin
SRC     := src
INCLUDE := include

EXECUTABLE  := dynamic_connectivity


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)



$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@

clean:
	-rm $(BIN)/*
