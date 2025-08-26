#ifndef GENERATOR_H
#define GENERATOR_H

#include <string>
#include <vector>
#include "Target.h"

// Function to generate the "help" target content for the Makefile
std::string generateHelpTarget(const std::vector<Target>& targets);

#endif // GENERATOR_H
