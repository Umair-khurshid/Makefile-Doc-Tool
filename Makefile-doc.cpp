#include <iostream>
#include <vector>
#include "Target.h"
#include "Parser.h"
#include "Generator.h"
#include "FileUtils.h"

// Function to validate input arguments
void validateArguments(int argc, [[maybe_unused]] char* argv[]) {
    if (argc != 2) {
        throw std::invalid_argument("Usage: makefile_doc_tool <path_to_makefile>");
    }
}

// Main function
int main(int argc, char* argv[]) {
    try {
        // Validate arguments
        validateArguments(argc, argv);
        std::string makefilePath = argv[1];

        // Validate Makefile path
        validateMakefilePath(makefilePath);

        std::cout << "Starting Makefile Doc Tool..." << std::endl;

        // Parse the Makefile to extract targets
        auto targets = parseMakefile(makefilePath);
        std::cout << "Number of targets found: " << targets.size() << std::endl;

        // Generate the help target content
        std::string helpTarget = generateHelpTarget(targets);

        // Update the Makefile with the help target
        updateMakefile(makefilePath, helpTarget);

        std::cout << "Help target successfully added to " << makefilePath << std::endl;
        return 0; // Exit successfully

    } catch (const std::invalid_argument& e) {
        std::cerr << "Usage Error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
