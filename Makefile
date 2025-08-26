# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
LDFLAGS =

# Source and executable names
SRCS = Makefile-doc.cpp Parser.cpp Generator.cpp FileUtils.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = Makefile-doc
TEST_EXEC = test_runner

.PHONY: all build clean install help test

all: build

build: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: build
	$(CXX) $(CXXFLAGS) -o $(TEST_EXEC) tests/test_makefile_doc.cpp
	./$(TEST_EXEC)

clean:
	rm -f $(EXEC) $(TEST_EXEC) *.o

install: build
	sudo cp $(EXEC) /usr/local/bin/

.DEFAULT_GOAL = help
help:
	@echo "Usage: make [target]"
	@echo ""
	@echo "Targets:"
	@echo "  build    Compile the source code and create the executable."
	@echo "  clean    Remove the executable and object files."
	@echo "  install  Install the executable to /usr/local/bin."
	@echo "  help     Show this help message."
