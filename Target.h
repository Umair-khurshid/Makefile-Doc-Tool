#ifndef TARGET_H
#define TARGET_H

#include <string>

// Structure to represent a Makefile target
struct Target {
    std::string name;           // Name of the target
    std::string dependencies;   // Dependencies for the target
    std::string comment;        // Comment describing the target
};

#endif // TARGET_H
