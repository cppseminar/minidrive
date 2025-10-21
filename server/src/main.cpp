#include <iostream>

#include "minidrive/version.hpp"

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    std::cout << "MiniDrive server stub (version " << minidrive::version() << ")" << std::endl;
    std::cout << "Command-line parsing and server startup are not yet implemented." << std::endl;
    return 0;
}
