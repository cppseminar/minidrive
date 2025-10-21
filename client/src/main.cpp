#include <iostream>

#include "minidrive/version.hpp"

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    std::cout << "MiniDrive client stub (version " << minidrive::version() << ")" << std::endl;
    std::cout << "Interactive shell and networking are not yet implemented." << std::endl;
    return 0;
}
