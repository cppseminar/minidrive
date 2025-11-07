#include <iostream>
#include <string>
#include <cstdint>

#include "minidrive/version.hpp"
#include "simple_server.hpp"

int main(int argc, char* argv[]) {
    // Echo full command line once for diagnostics
    std::cout << "[cmd]";
    for (int i = 0; i < argc; ++i) {
        std::cout << " \"" << argv[i] << '"';
    }
    std::cout << std::endl;
    std::uint16_t port = 9000; // default
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--port" && i + 1 < argc) {
            port = static_cast<std::uint16_t>(std::stoi(argv[++i]));
        }
    }

    std::cout << "Starting simple server (version " << minidrive::version() << ") on port " << port << std::endl;
    start_simple_server(port);
    std::cout << "Server exited." << std::endl;
    return 0;
}
