# Compiler and tools
CXX = g++
CFLAGS = -Wall -std=c++11 -DPLATFORM_DESKTOP
LDFLAGS = -lraylib -lm -lpthread -ldl -lrt -lX11

# Source files
SRC = main.cpp

# Output executable
OUT = main

# Build rules
all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(OUT)

clean:
	rm -f $(OUT)

.PHONY: all clean
