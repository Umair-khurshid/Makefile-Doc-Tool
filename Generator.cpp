#include "Generator.h"
#include <sstream>

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
