#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include "Target.h"

// Function to parse the Makefile and extract targets
std::vector<Target> parseMakefile(const std::string& filepath);

#endif // PARSER_H
