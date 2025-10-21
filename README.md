# MiniDrive

Experimental client/server file synchronization system written in modern C++ as part of the Application Development in C++ course at FIIT STU.

## Assignment

See [docs/requirements.md](docs/requirements.md) for the full assignment description.

## Build

This is sample project layout for C++ applications using CMake. You can use it as a starting point for your own projects. It is in fact recommended to fork this repository and build upon it. But of course we only need your project to build with CMake and create client/server executables.

MiniDrive uses CMake (3.22+) and automatically downloads its third-party dependencies (Asio, nlohmann/json, spdlog, libsodium) via `FetchContent`.

```
cmake -S . -B build
cmake --build build
```

On Windows you may need to generate build files for `Ninja` or `Visual Studio` (or better use Docker for development). Linux and macOS users should ensure a working toolchain with a C++20-capable compiler.

## Run

```
./build/server --port 9000 --root ./data/server_root
./build/client 127.0.0.1:9000
```

(Commands above are just an example.)

## Testing

```
cmake --build build --target integration_smoke
ctest --test-dir build
```

## Repository Layout

- `client/`, `server/`, `shared/` – application targets
- `cmake/Dependencies.cmake` – dependency management
- `docs/` – architecture and protocol documentation
- `data/` – sample server runtime root
- `tests/` – integration smoke tests

See `docs/architecture.md` for more information.
