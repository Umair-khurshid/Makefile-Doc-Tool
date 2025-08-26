#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

// Function to run a command and get the output
std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

void run_test(const std::string& test_name, void (*test_func)()) {
    std::cout << "Running test: " << test_name << "..." << std::endl;
    test_func();
    std::cout << "Test passed: " << test_name << std::endl;
}

void test_generates_help_target() {
    // Create a dummy Makefile
    std::ofstream test_makefile("test_makefile");
    test_makefile << "build: # Build the project" << std::endl;
    test_makefile << "\t@echo \"Building...\"" << std::endl;
    test_makefile.close();

    // Run the tool
    exec("./Makefile-doc test_makefile");

    // Read the modified Makefile
    std::ifstream updated_makefile("test_makefile");
    std::string content((std::istreambuf_iterator<char>(updated_makefile)),
                        std::istreambuf_iterator<char>());
    updated_makefile.close();

    // Check if the help target was added
    assert(content.find(".DEFAULT_GOAL = help") != std::string::npos);
    assert(content.find("help: # Print help on Makefile") != std::string::npos);

    // Clean up
    remove("test_makefile");
}

void test_replaces_existing_help_target() {
    // Create a dummy Makefile with an existing help target
    std::ofstream test_makefile("test_makefile");
    test_makefile << "build: # Build the project" << std::endl;
    test_makefile << "\t@echo \"Building...\"" << std::endl;
    test_makefile << ".DEFAULT_GOAL = help" << std::endl;
    test_makefile << "help: # Old help target" << std::endl;
    test_makefile.close();

    std::ifstream initial_makefile("test_makefile");
    std::string initial_content((std::istreambuf_iterator<char>(initial_makefile)),
                        std::istreambuf_iterator<char>());
    initial_makefile.close();
    std::cout << "Before:\n" << initial_content << std::endl;

    // Run the tool
    exec("./Makefile-doc test_makefile");

    // Read the modified Makefile
    std::ifstream updated_makefile("test_makefile");
    std::string updated_content((std::istreambuf_iterator<char>(updated_makefile)),
                        std::istreambuf_iterator<char>());
    updated_makefile.close();

    std::cout << "After:\n" << updated_content << std::endl;

    // Check that the help target was replaced, not duplicated
    size_t pos = updated_content.find("help:");
    assert(pos != std::string::npos);
    pos = updated_content.find("help:", pos + 1);
    assert(pos == std::string::npos);


    // Clean up
    remove("test_makefile");
}

int main() {
    run_test("test_generates_help_target", test_generates_help_target);
    run_test("test_replaces_existing_help_target", test_replaces_existing_help_target);
    return 0;
}
