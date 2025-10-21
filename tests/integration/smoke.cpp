#include "minidrive/version.hpp"

#include <cassert>
#include <iostream>
#include <string>

// Test library headers
#include <asio.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <sodium.h>

int main() {
    // Test 1: Version check
    const auto v = minidrive::version();
    assert(!v.empty());
    std::cout << "Version library linked: " << v << std::endl;

    // Test 2: Asio
    asio::io_context io_context;
    assert(io_context.stopped() == false);
    std::cout << "Asio library linked" << std::endl;

    // Test 3: nlohmann/json
    nlohmann::json j = {{"test", "value"}, {"number", 42}};
    assert(j["test"] == "value");
    assert(j["number"] == 42);
    std::cout << "nlohmann/json library linked" << std::endl;

    // Test 4: spdlog
    spdlog::set_level(spdlog::level::off); // Suppress output
    spdlog::info("Test message");
    std::cout << "spdlog library linked" << std::endl;

    // Test 5: libsodium
    if (sodium_init() < 0) {
        std::cerr << "libsodium initialization failed" << std::endl;
        return 1;
    }
    unsigned char hash[crypto_generichash_BYTES];
    const char* message = "test";
    crypto_generichash(hash, sizeof(hash),
                      reinterpret_cast<const unsigned char*>(message), 4,
                      nullptr, 0);
    std::cout << "libsodium library linked" << std::endl;

    std::cout << "\nAll libraries successfully linked!" << std::endl;
    return 0;
}
