#pragma once

#include <cstdint>

// Starts a blocking IPv4 TCP server on given port.
// Accepts one connection at a time, prints all received bytes to stdout.
// Returns when the first client disconnects.
void start_simple_server(std::uint16_t port);
