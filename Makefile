CXX       := g++
CXX_FLAGS := -std=c++17 -O3 -Wall

LIB		:= lib
SRC     := src
HEADERS := include
PRIVATE_HEADERS := src/headers
BUILD   := build
EXAMPLE := example
BIN     := bin

LIB_NAME    := libdnc.a

SRC_FILES := $(wildcard $(SRC)/*.cpp)
OBJ_FILES := $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(SRC_FILES))
EXAMPLES  := $(wildcard $(EXAMPLE)/*.cpp)
BINARIES  := $(patsubst $(EXAMPLE)/%.cpp, $(BIN)/%, $(EXAMPLES))

.PHONY: all clean examples test

all: $(LIB)/$(LIB_NAME) $(EXAMPLES)

examples: all $(BINARIES)
	
test: examples bin/offline

clean:
	$(RM) -r $(BUILD)
	$(RM) -r $(LIB)
	$(RM) -r $(BIN)

$(LIB)/$(LIB_NAME): $(OBJ_FILES)
	@mkdir -p $(LIB)
	ar rcs $@ $^
	
$(BUILD)/%.o: $(SRC)/%.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(CXX_FLAGS) -I$(HEADERS) -I$(PRIVATE_HEADERS) -c $< -o $@

$(BIN)/offline: test/offline.cpp
	$(CXX) $(CXX_FLAGS) test/offline.cpp -o $@

$(BIN)/%: $(EXAMPLE)/%.cpp $(LIB)/$(LIB_NAME)
	@mkdir -p $(BIN)
	$(CXX) $(CXX_FLAGS) -I$(HEADERS) $< -o $@ -L$(LIB) -ldnc
