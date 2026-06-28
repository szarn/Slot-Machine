CXX      := c++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
BIN      := slot

SRC := main.cpp symbol.cpp rng.cpp machine.cpp paylines.cpp wallet.cpp display.cpp game.cpp
OBJ := $(SRC:.cpp=.o)
HDR := $(wildcard *.h)

.PHONY: all run clean

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

# Recompile any object when its source or any header changes (simple, safe for
# a project this size).
%.o: %.cpp $(HDR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(OBJ) $(BIN)
