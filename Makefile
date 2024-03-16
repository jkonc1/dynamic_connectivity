CXX       := g++
CXX_FLAGS := -std=c++17 -g -fsanitize=address

BIN     := bin
SRC     := src
INCLUDE := include

EXECUTABLE  := dynamic_connectivity


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

assembly: $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -S -fverbose-asm $^ -o $(BIN)/$(EXECUTABLE).s

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@

clean:
	-rm $(BIN)/*
