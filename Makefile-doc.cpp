#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>

// Structure to represent a Makefile target
struct Target {
    std::string name;           // Name of the target
    std::string dependencies;   // Dependencies for the target
    std::string comment;        // Comment describing the target
};

// Function to validate input arguments
void validateArguments(int argc, char* argv[]) {
    if (argc != 2) {
        throw std::invalid_argument("Usage: makefile_doc_tool <path_to_makefile>");
    }
}

// Function to validate the Makefile path
void validateMakefilePath(const std::string& filepath) {
    if (!std::filesystem::exists(filepath)) {
        throw std::runtime_error("Error: Makefile not found at " + filepath);
    }
    if (!std::filesystem::is_regular_file(filepath)) {
        throw std::runtime_error("Error: Path does not point to a valid Makefile.");
    }
}

// Function to parse the Makefile and extract targets
std::vector<Target> parseMakefile(const std::string& filepath) {
    std::vector<Target> targets;
    std::regex targetPattern(R"(([\w\-\.]+):\s*(.*?)\s+#\s*(.*)$)"); // Allow special characters in names
    std::ifstream file(filepath);

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open Makefile: " + filepath);
    }

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        std::smatch match;
        if (std::regex_match(line, match, targetPattern)) {
            targets.push_back({match[1], match[2], match[3]});
        } else if (!line.empty() && line[0] != '#') {
            std::cerr << "Warning: Could not parse line " << lineNumber << ": " << line << std::endl;
        }
    }

    if (targets.empty()) {
        throw std::runtime_error("No valid targets with comments found in the Makefile.");
    }

    return targets;
}

// Function to generate the "help" target content for the Makefile
std::string generateHelpTarget(const std::vector<Target>& targets) {
    std::ostringstream helpTargetStream;
    helpTargetStream << "help: # Print help on Makefile\n";
    helpTargetStream << "\t@echo \"Please use 'make <target>' where <target> is one of\"\n";
    helpTargetStream << "\t@echo \"\"\n";

    for (const auto& target : targets) {
        helpTargetStream << "\t@echo \"  \033[93m" << target.name << "\033[0m\t"
                         << target.comment << " [" << target.dependencies << "]\"\n";
    }

    helpTargetStream << "\t@echo \"\"\n";
    helpTargetStream << "\t@echo \"Check the Makefile to know exactly what each target is doing.\"\n";

    return helpTargetStream.str();
}

// Function to append the generated "help" target to the Makefile
void updateMakefile(const std::string& filepath, const std::string& helpTarget) {
    std::ofstream file(filepath, std::ios::app);

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open Makefile for writing: " + filepath);
    }

    file << "\n.DEFAULT_GOAL = help\n";
    file << helpTarget;
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

