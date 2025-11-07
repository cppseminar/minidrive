#include "simple_server.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>

namespace {

int create_listen_socket(std::uint16_t port) {
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        std::perror("socket");
        return -1;
    }

    int enable = 1;
    if (::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
        std::perror("setsockopt");
        ::close(fd);
        return -1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (::bind(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        std::perror("bind");
        ::close(fd);
        return -1;
    }

    // Allow a small backlog so multiple clients can queue while one is being processed.
    if (::listen(fd, 8) < 0) {
        std::perror("listen");
        ::close(fd);
        return -1;
    }
    return fd;
}

}

void start_simple_server(std::uint16_t port) {
    int listen_fd = create_listen_socket(port);
    if (listen_fd < 0) {
        std::cerr << "Failed to set up listen socket on port " << port << std::endl;
        return;
    }
    std::cout << "Simple server listening on port " << port << std::endl;

    // Accept loop: handle clients sequentially. For each client, accumulate full message then print once.
    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = ::accept(listen_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_len);
        if (client_fd < 0) {
            if (errno == EINTR) continue; // retry accept
            std::perror("accept");
            break; // fatal accept error -> exit server
        }

        char ipbuf[INET_ADDRSTRLEN];
        const char* ipstr = ::inet_ntop(AF_INET, &client_addr.sin_addr, ipbuf, sizeof(ipbuf));
        if (ipstr) {
            std::cout << "Client connected from " << ipstr << ":" << ntohs(client_addr.sin_port) << std::endl;
        }

        constexpr std::size_t BUF_SIZE = 4096;
        char buffer[BUF_SIZE];
        std::string message;
        while (true) {
            ssize_t n = ::recv(client_fd, buffer, BUF_SIZE, 0);
            if (n < 0) {
                if (errno == EINTR) continue;
                std::perror("recv");
                break;
            }
            if (n == 0) { // EOF
                std::cout << "Full message from client (" << message.size() << " bytes):\n";
                if (!message.empty()) {
                    std::cout << message << std::endl;
                }
                std::cout << "Client disconnected" << std::endl;
                break;
            }
            message.append(buffer, static_cast<std::size_t>(n));
        }
        ::close(client_fd);
    }

    ::close(listen_fd);
}
