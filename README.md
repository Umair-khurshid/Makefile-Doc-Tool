# Makefile Doc Tool

A command-line utility that simplifies and enhances the usability of your Makefiles. It parses your Makefile to extract targets, dependencies, and comments, and then generates a self-documenting `help` target to make your Makefile user-friendly and easy to understand.

## Features

- **Automatic Target Parsing**: Extracts Makefile targets along with their dependencies and descriptions.
- **Self-Documenting `help` Target**: Adds a standardized `help` target for better Makefile documentation.
- **Cross-Platform Compatibility**: Works on Linux, macOS, and Windows.
- **Robust Error Handling**: Ensures smooth operation with meaningful error messages.

## How it Works

The tool parses your Makefile for lines that match the following pattern:

```makefile
target: [dependencies] # comment
```

It then uses this information to generate a `help` target that displays a list of all the targets, their dependencies, and their comments.

## Usage

1.  **Build the project:**

    ```bash
    make build
    ```

2.  **Run the tool:**

    ```bash
    ./Makefile-doc <path_to_makefile>
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

### Running the Tool

```bash
./Makefile-doc Makefile
```

### Output Makefile

After running the tool, the following `help` target will be added to your `Makefile`:

```makefile
.DEFAULT_GOAL = help

help: # Print help on Makefile
	@echo "Please use 'make <target>' where <target> is one of"
	@echo ""
	@echo "  \033[93mbuild\033[0m\tBuild the project []"
	@echo "  \033[93mtest\033[0m\tRun tests [build]"
	@echo "  \033[93mclean\033[0m\tClean the project []"
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