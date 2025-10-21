# MiniDrive Architecture

**Draft you should delete this or edit it, just an idea how to structure the project**

## High-Level Components

- **Client (`client/`)**
  - Command-line interface with interactive shell and CLI parser.
  - Local filesystem manager for uploads/downloads/resume handling.
  - Synchronization engine for hashing, diffing, and incremental updates.
  - Transfer manager implementing chunked binary streaming over TCP.
- **Server (`server/`)**
  - Listener accepting TCP connections using Asio with a thread pool.
  - Session manager controlling public/private roots and single-session limits.
  - Command dispatcher with handlers for file/folder operations and sync APIs.
  - Persistence layer storing users, hashes, and resumable transfer metadata.
  - Filesystem executor guarded against path traversal using `std::filesystem`.
- **Shared (`shared/`)**
  - JSON protocol schema and serialization helpers using `nlohmann::json`.
  - Error code definitions and mapping utilities.
  - Cryptographic helpers leveraging libsodium for password hashing and file hashes.
  - Logging helpers wrapping `spdlog` (optional) and console fallbacks.

## Directory Layout

```
.
├── CMakeLists.txt            # Root build orchestrator
├── cmake/                    # Toolchain and dependency helpers
├── external/                 # Vendored single-header libraries (Asio, JSON)
├── client/
│   ├── include/
│   ├── src/
│   └── CMakeLists.txt
├── server/
│   ├── include/
│   ├── src/
│   └── CMakeLists.txt
├── shared/
│   ├── include/
│   ├── src/
│   └── CMakeLists.txt
├── tests/
│   ├── integration/
│   └── CMakeLists.txt
├── data/
│   └── server_root/          # Default runtime root for server
├── docs/                     # Documentation
│   ├── architecture.md
│   ├── protocol.md
│   └── requirements.md
└── README.md
```
