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

    size_t pos = content.find(".DEFAULT_GOAL = help");
    if (pos != std::string::npos) {
        content = content.substr(0, pos);
    }

    std::ofstream outfile(filepath);
    if (!outfile.is_open()) {
        throw std::runtime_error("Unable to open Makefile for writing: " + filepath);
    }

    outfile << content;
    outfile << "\n.DEFAULT_GOAL = help\n";
    outfile << helpTarget;
}
