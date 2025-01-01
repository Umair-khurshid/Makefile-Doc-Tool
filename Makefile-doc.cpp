#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <filesystem>

// Structure to represent a Makefile target
struct Target {
    std::string name;            // Name of the target
    std::string dependencies;   // Dependencies for the target
    std::string comment;        // Comment describing the target
};

// Function to parse the Makefile and extract targets, dependencies, and comments
std::vector<Target> parseMakefile(const std::string& filepath) {
    std::vector<Target> targets;
    std::regex targetPattern(R"((\w+):\s*(.*)\s+#\s*(.*)$)"); // Regular expression to match targets
    std::ifstream file(filepath);

    // Check if the Makefile can be opened
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open Makefile: " + filepath);
    }

    std::string line;
    std::cout << "Parsing Makefile: " << filepath << std::endl;
    // Read the Makefile line by line
    while (std::getline(file, line)) {
        std::smatch match;
        // Match lines against the target pattern
        if (std::regex_match(line, match, targetPattern)) {
            std::cout << "Found target: " << match[1] << " with dependencies: " << match[2] << " and comment: " << match[3] << std::endl;
            targets.push_back({match[1], match[2], match[3]}); // Extract name, dependencies, and comment
        } else {
            std::cout << "No match for line: " << line << std::endl;
        }
    }

    file.close();
    std::cout << "Finished parsing Makefile." << std::endl;
    return targets; // Return the list of targets
}

// Function to generate the "help" target content for the Makefile
std::string generateHelpTarget(const std::vector<Target>& targets) {
    std::string helpTarget;
    helpTarget += "help: # Print help on Makefile\n";
    helpTarget += "\t@echo \"Please use 'make <target>' where <target> is one of\"\n";
    helpTarget += "\t@echo \"\"\n";

    // Append each target's information to the help target
    for (const auto& target : targets) {
        helpTarget += "\t@echo \"  \033[93m" + target.name + "\033[0m\t" + target.comment + " [" + target.dependencies + "]\"\n";
    }

    helpTarget += "\t@echo \"\"\n";
    helpTarget += "\t@echo \"Check the Makefile to know exactly what each target is doing.\"\n";

    std::cout << "Generated help target content." << std::endl;
    return helpTarget; // Return the complete help target as a string
}

// Function to append the generated help target to the Makefile
void updateMakefile(const std::string& filepath, const std::string& helpTarget) {
    std::ofstream file(filepath, std::ios::app); // Open the file in append mode

    // Check if the file can be opened for writing
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open Makefile for writing: " + filepath);
    }

    std::cout << "Appending help target to Makefile." << std::endl;
    file << "\n.DEFAULT_GOAL = help\n"; // Add default goal to the Makefile
    file << helpTarget;                   // Append the help target content

    file.close();
    std::cout << "Successfully updated Makefile with help target." << std::endl;
}

// Entry point of the program
int main(int argc, char* argv[]) {
    // Check if the correct number of arguments is provided
    if (argc != 2) {
        std::cerr << "Usage: makefile_doc_tool <path_to_makefile>" << std::endl;
        return 1;
    }

    std::string makefilePath = argv[1]; // Get the path to the Makefile

    std::cout << "Starting Makefile Doc Tool..." << std::endl;
    std::cout << "Makefile path: " << makefilePath << std::endl;

    // Check if the specified Makefile exists
    if (!std::filesystem::exists(makefilePath)) {
        std::cerr << "Error: Makefile not found at " << makefilePath << std::endl;
        return 1;
    }

    try {
        // Parse the Makefile to extract targets
        auto targets = parseMakefile(makefilePath);

        // If no targets are found, display an error message
        if (targets.empty()) {
            std::cerr << "No valid targets found in the Makefile." << std::endl;
            return 1;
        }

        std::cout << "Number of targets found: " << targets.size() << std::endl;

        // Generate the help target content
        std::string helpTarget = generateHelpTarget(targets);

        // Update the Makefile with the help target
        updateMakefile(makefilePath, helpTarget);

        std::cout << "Help target added to " << makefilePath << std::endl;
    } catch (const std::exception& e) {
        // Handle exceptions and display error messages
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Makefile Doc Tool completed successfully." << std::endl;
    return 0; // Exit the program successfully
}

