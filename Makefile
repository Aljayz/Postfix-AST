# Compiler and flags
CXX = g++
CXXFLAGS = -g -Wall -std=c++11

# Source files
SRCS = main.cpp InfixToPostfix.cpp PostfixToAST.cpp AST_NODE.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = project

# Default rule: build the executable
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile source files into object files
%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Phony rule to prevent conflict with a file named 'clean'
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)

# Phony rule for running the executable
.PHONY: run
run: all
	./$(TARGET)
