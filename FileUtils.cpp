#include "FileUtils.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

// Function to validate the Makefile path
void validateMakefilePath(const std::string& filepath) {
    if (!std::filesystem::exists(filepath)) {
        throw std::runtime_error("Error: Makefile not found at " + filepath);
    }
    if (!std::filesystem::is_regular_file(filepath)) {
        throw std::runtime_error("Error: Path does not point to a valid Makefile.");
    }
}

// Function to update the Makefile with the generated "help" target
void updateMakefile(const std::string& filepath, const std::string& helpTarget) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open Makefile for reading: " + filepath);
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    std::string delimiterStart = "# --- MAKEFILE-DOC-TOOL-START ---";
    std::string delimiterEnd = "# --- MAKEFILE-DOC-TOOL-END ---";

    size_t startPos = content.find(delimiterStart);
    size_t endPos = content.find(delimiterEnd);

    std::string preContent;
    std::string postContent;

    if (startPos != std::string::npos) {
        preContent = content.substr(0, startPos);
        if (endPos != std::string::npos) {
            postContent = content.substr(endPos + delimiterEnd.length());
            // Consume one separate newline if it exists to be clean, but purely optional
            if (!postContent.empty() && postContent[0] == '\n') {
                postContent.erase(0, 1);
            }
        }
    } else {
        // Backward compatibility
        size_t legacyPos = content.find(".DEFAULT_GOAL = help");
        if (legacyPos != std::string::npos) {
            preContent = content.substr(0, legacyPos);
        } else {
            preContent = content;
            if (!preContent.empty() && preContent.back() != '\n') {
                preContent += "\n";
            }
        }
    }

    std::ofstream outfile(filepath);
    if (!outfile.is_open()) {
        throw std::runtime_error("Unable to open Makefile for writing: " + filepath);
    }

    outfile << preContent << helpTarget << postContent;
}
