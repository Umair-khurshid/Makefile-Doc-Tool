build: main.o utils.o # Build the project
	g++ -o build main.o utils.o

clean: # Clean build artifacts
	rm -f *.o build

install: # Install the application
	sudo cp build /usr/local/bin


.DEFAULT_GOAL = help
help: # Print help on Makefile
	@echo "Please use 'make <target>' where <target> is one of"
	@echo ""
	@echo "  [93mbuild[0m	Build the project [main.o utils.o]"
	@echo "  [93mclean[0m	Clean build artifacts []"
	@echo "  [93minstall[0m	Install the application []"
	@echo ""
	@echo "Check the Makefile to know exactly what each target is doing."
