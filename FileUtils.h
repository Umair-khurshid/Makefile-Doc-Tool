#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <string>

// Function to validate the Makefile path
void validateMakefilePath(const std::string& filepath);

// Function to update the Makefile with the generated "help" target
void updateMakefile(const std::string& filepath, const std::string& helpTarget);

#endif // FILEUTILS_H
