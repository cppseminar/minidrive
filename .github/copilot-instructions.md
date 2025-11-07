# Copilot Instructions for MiniDrive

Concise, project-specific guidance for AI coding agents contributing to this C++20 client/server file sync prototype.

## Architecture Snapshot
- Three primary targets: `server/`, `client/`, `shared/` (defined in root `CMakeLists.txt` in that order). Tests (`tests/`) link only against `minidrive_shared` and warnings target.
- `shared/` owns cross-cutting facilities: version API (`minidrive/version.hpp`), (future) protocol serialization, error + logging helpers, crypto wrappers. Prefer adding common code here instead of duplicating in client/server.
- `server/` and `client/` `main.cpp` are minimal stubs; they are the extension points for CLI parsing, event loop (Asio), and command dispatch. Build incremental features behind small translation units (avoid monolithic `main.cpp`).
- Planned JSON protocol (see `docs/protocol.md`): length‑prefixed JSON control messages over a single TCP connection + chunked binary streaming for file data.

## Build & Test Workflow
- Standard build: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug` then `cmake --build build -- -j$(nproc)` (already provided as VS Code tasks `project-configure`, `project-build`).
- Third-party deps (Asio, nlohmann/json, spdlog, libsodium) are fetched automatically via `cmake/Dependencies.cmake` (FetchContent). Do NOT vendor or add manual find_package calls unless extending dependency management.
- Enable/disable tests via `-DMINIDRIVE_BUILD_TESTS=ON|OFF`. Integration smoke test target name: `integration_smoke` (set in `tests/CMakeLists.txt`). Build directly: `cmake --build build --target integration_smoke`. Run all: `ctest --test-dir build`.

## Coding Conventions
- Namespace root is `minidrive`; put public headers in `shared/include/minidrive/` with small focused headers (e.g., `version.hpp`).
- Public API functions return lightweight types (`std::string_view` used for `version()`); prefer non-owning views for constants.
- Keep `inline constexpr` for version/string constants; expose a runtime helper when ABI-stable pointer needed (`resolved_version()` pattern).
- Headers: `#pragma once` + minimal includes; source files include corresponding header then standard/library headers.
- Future networking code: use Asio async model with an `io_context` owned at top-level; avoid global singletons.
- Logging: Wrap spdlog usage (create a helper in shared) rather than calling spdlog directly from many TUs.
- Crypto: Initialize libsodium once early (server start) and guard with error check similar to integration test pattern.

## External Dependencies Usage Patterns
- Asio: Construct `asio::io_context` early; pass references to session/connection objects. Avoid calling `run()` in multiple threads until design established.
- nlohmann/json: Define explicit schema objects for requests/responses; validate presence of expected keys before acting. Use structured binding for clarity.
- spdlog: Set level early; prefer `spdlog::debug/ info` wrappers; suppress in tests with level off.
- libsodium: Use `crypto_generichash` for chunk hashes; centralize hash function in shared.

## Extension Points & Guidelines
- Implement CLI option parsing in separate TU (e.g., `server/src/options.cpp`) returning a struct of config; keep `main.cpp` minimal.
- Protocol framing: add a `FrameReader`/`FrameWriter` in `shared/` to handle length-prefix logic described in `docs/protocol.md`.
- Add error codes enum + mapping to JSON once first commands (LIST/GET/PUT) are implemented.
- When adding new shared utilities, update tests or add a new small integration test target (follow pattern in `tests/integration/smoke.cpp`).

## Typical Workflow Examples
- Add new shared component: create header in `shared/include/minidrive/` + source in `shared/src/`, list source in `shared/CMakeLists.txt` if not using globbing (inspect file for pattern before modifying).
- Add a server feature: new handler file `server/src/list_handler.cpp` using shared protocol structures; update `server/CMakeLists.txt` to include it.
- Introduce a JSON command: define request/response schema in a new header `shared/include/minidrive/protocol/list.hpp`; add parsing + validation helpers.

## Non-Goals / Avoid
- Don’t introduce build system changes that bypass `FetchContent` unless absolutely required.
- Don’t add global mutable state for configuration; prefer passing const config structs.
- Avoid premature threading; start single-threaded until correctness tests exist.

## Quick Reference
- Version API: `minidrive::version()` (constexpr), `minidrive::resolved_version()` (pointer).
- Test target name: `integration_smoke`.
- Tasks: `project-configure`, `project-build`.

Feedback: Please clarify any missing conventions (error handling, logging wrapper specifics) or request more examples; this document should stay terse (≈40 lines) and reflect actual code.
