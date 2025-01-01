# Makefile Doc Tool

# Makefile Doc Tool

## Description
**Makefile Doc Tool** is a command-line utility designed to simplify and enhance the usability of your Makefiles. It parses your Makefile to extract targets, dependencies, and comments, and then appends a self-documenting `help` target to make your Makefile user-friendly and easy to understand.

## Features
- **Automatic Target Parsing**: Extracts Makefile targets along with their dependencies and descriptions.
- **Self-Documenting `help` Target**: Adds a standardized `help` target for better Makefile documentation.
- **Cross-Platform Compatibility**: Works on Linux, macOS, and Windows.
- **Robust Error Handling**: Ensures smooth operation with meaningful error messages.

## Usage

1. Compile the C++ source code:
   ```bash
   g++ -std=c++17 -o Makefile-doc Makefile_doc.cpp
   ```
2. Run the compiled binary:
   ```bash
   ./Makefile_doc <path_to_makefile>
   ```

## Example
### Input Makefile
```makefile
build: # Build the project
	@echo "Building the project..."

test: build # Run tests
	@echo "Running tests..."

clean: # Clean the project
	rm -rf build/
	@echo "Project cleaned."
```

### Output Makefile
After running the tool:
```makefile
build: # Build the project
	@echo "Building the project..."

test: build # Run tests
	@echo "Running tests..."

clean: # Clean the project
	rm -rf build/
	@echo "Project cleaned."

.DEFAULT_GOAL = help

help: # Print help on Makefile
	@echo "Please use 'make <target>' where <target> is one of"
	@echo ""
	@echo "   33[93mbuild 33[0m	Build the project []"
	@echo "   33[93mtest 33[0m	Run tests [build]"
	@echo "   33[93mclean 33[0m	Clean the project []"
	@echo ""
	@echo "Check the Makefile to know exactly what each target is doing."
```

### Running `make help`
```bash
$ make help
Please use 'make <target>' where <target> is one of

  build	Build the project []
  test	Run tests [build]
  clean	Clean the project []

Check the Makefile to know exactly what each target is doing.
```


