# Define the compiler and the flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic-errors -Weffc++ -Wno-unused-parameter -g

# Define the target executable
TARGET = solve

# Define the source files
SRCS = DPLL.cpp driver.cpp

# Default rule to compile and link in one step
all: $(TARGET)

# Rule to compile and link the sources directly
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Clean rule to remove the executable
clean:
	rm -f $(TARGET)
