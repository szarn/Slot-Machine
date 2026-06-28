CXX      := c++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
BIN      := slot
SRCDIR   := src

SRC := $(wildcard $(SRCDIR)/*.cpp)
OBJ := $(SRC:.cpp=.o)
HDR := $(wildcard $(SRCDIR)/*.h)

.PHONY: all run clean

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

# Recompile any object when its source or any header changes (simple, safe for
# a project this size).
$(SRCDIR)/%.o: $(SRCDIR)/%.cpp $(HDR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(OBJ) $(BIN)
