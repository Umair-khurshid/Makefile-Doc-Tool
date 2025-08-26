#include "Parser.h"
#include <fstream>
#include <regex>
#include <iostream>

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
