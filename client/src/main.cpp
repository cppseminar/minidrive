#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "minidrive/version.hpp"

struct HostPort {
    std::string host;
    uint16_t port{};
};

static bool parse_host_port(const std::string& input, HostPort& out) {
    auto colon = input.rfind(':');
    if (colon == std::string::npos) return false;
    std::string host = input.substr(0, colon);
    std::string port_str = input.substr(colon + 1);
    if (host.empty() || port_str.empty()) return false;
    char* end = nullptr;
    long p = std::strtol(port_str.c_str(), &end, 10);
    if (*end != '\0' || p < 0 || p > 65535) return false;
    out.host = std::move(host);
    out.port = static_cast<uint16_t>(p);
    return true;
}

int main(int argc, char* argv[]) {
    // Echo full command line once for diagnostics
    std::cout << "[cmd]";
    for (int i = 0; i < argc; ++i) {
        std::cout << " \"" << argv[i] << '"';
    }
    std::cout << std::endl;

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <host>:<port>" << std::endl;
        return 1;
    }

    HostPort hp;
    if (!parse_host_port(argv[1], hp)) {
        std::cerr << "Invalid endpoint format: " << argv[1] << std::endl;
        return 1;
    }

    std::cout << "MiniDrive client (version " << minidrive::version() << ")" << std::endl;
    std::cout << "Connecting to " << hp.host << ':' << hp.port << std::endl;

    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        std::perror("socket");
        return 2;
    }
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(hp.port);
    if (::inet_pton(AF_INET, hp.host.c_str(), &addr.sin_addr) != 1) {
        std::cerr << "Invalid IPv4 address: " << hp.host << std::endl;
        ::close(fd);
        return 2;
    }
    if (::connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        std::perror("connect");
        ::close(fd);
        return 2;
    }
    const char* msg = "Hello from client";
    ssize_t sent = ::send(fd, msg, std::strlen(msg), 0);
    if (sent < 0) {
        std::perror("send");
        ::close(fd);
        return 3;
    }
    std::cout << "Sent greeting (" << sent << " bytes)" << std::endl;
    ::close(fd);
    return 0;
}
