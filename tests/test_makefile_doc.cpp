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

void test_complex_targets() {
    std::ofstream test_makefile("test_makefile_complex");
    test_makefile << "src/%.o: src/%.cpp # Compile objects" << std::endl;
    test_makefile << "\t@echo \"Compiling...\"" << std::endl;
    test_makefile << "$(EXEC): $(OBJS) # Link executable" << std::endl;
    test_makefile << "\t@echo \"Linking...\"" << std::endl;
    test_makefile.close();

    exec("./Makefile-doc test_makefile_complex");

    std::ifstream updated_makefile("test_makefile_complex");
    std::string content((std::istreambuf_iterator<char>(updated_makefile)),
                        std::istreambuf_iterator<char>());
    updated_makefile.close();

    // Check if targets were found and documented
    // We expect the help target to contain "src/%.o" and "$(EXEC)"
    // Note: The coloring codes \033[93m and \033[0m might make direct string search tricky if we don't account for them.
    // The Generator adds: \t@echo "  \033[93m" << target.name << "\033[0m\t"
    
    // std::cout << "Content: " << content << std::endl; 

    assert(content.find("src/%.o") != std::string::npos);
    assert(content.find("Link executable") != std::string::npos); // description should be there

    remove("test_makefile_complex");
}

void test_safe_update() {
    std::ofstream test_makefile("test_makefile_safe");
    test_makefile << "build: # Build" << std::endl;
    test_makefile << "\t@echo \"Build\"" << std::endl;
    // Simulate user adding content *after* the help target if it wasn't there, 
    // or arguably just content at the bottom.
    // But the main issue is that the tool deletes everything after ".DEFAULT_GOAL = help".
    // So if we run the tool, it adds help. Then if we add something AFTER help, and run it again, it should persist (or be safely replaced).
    test_makefile.close();

    // First run
    exec("./Makefile-doc test_makefile_safe");

    // Append user content at the end
    std::ofstream append_makefile("test_makefile_safe", std::ios::app);
    append_makefile << "\n# User added content\ncustom_target:\n\t@echo \"Custom\"" << std::endl;
    append_makefile.close();

    // Second run
    exec("./Makefile-doc test_makefile_safe");

    std::ifstream updated_makefile("test_makefile_safe");
    std::string content((std::istreambuf_iterator<char>(updated_makefile)),
                        std::istreambuf_iterator<char>());
    updated_makefile.close();

    // Verify user content persisted
    // Currently this will FAIL because the tool truncates at .DEFAULT_GOAL = help
    assert(content.find("# User added content") != std::string::npos);
    assert(content.find("custom_target:") != std::string::npos);

    remove("test_makefile_safe");
}

int main() {
    run_test("test_generates_help_target", test_generates_help_target);
    run_test("test_replaces_existing_help_target", test_replaces_existing_help_target);
    run_test("test_complex_targets", test_complex_targets);
    run_test("test_safe_update", test_safe_update);
    return 0;
}
