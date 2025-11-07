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

## Environment Variables

The dev container sets these via `containerEnv` (see `.devcontainer/devcontainer.json`). You can modify the devcontainer for persistence of your custom environment variables.

| Variable | Purpose | Default |
|----------|---------|---------|
| `MINIDRIVE_HOST` | Host/IP the client connects to; server binds 0.0.0.0 | `127.0.0.1` |
| `MINIDRIVE_PORT` | TCP port for server listen + client connect | `9000` |
| `MINIDRIVE_USERNAME` | Reserved for future auth | (empty) |

Launch configs reference these with `${env:MINIDRIVE_PORT}`; tasks use shell expansion `${MINIDRIVE_PORT}`.

## VS Code Tasks

Defined in `.vscode/tasks.json`:

- `project-configure` – CMake configure (exports compile commands)
- `project-build` – Build targets
- `run-server` – Run server (w/o attached debugger) with port/root
- `run-client` – Run client (w/o attached debugger) connecting host:port
- `terminate-server` – SIGTERM active server process

Use the Command Palette > Run Task to invoke any of them.

## Debugging

Launch configurations (`.vscode/launch.json`):

- `Debug Server` – Builds then starts server under gdb
- `Debug Client` – Starts the client

To debug both you can run two separate debug sessions, then it is possible to switch between them using the Debug Console dropdown.

### Test Implementation

Current implementation in server and client has nothing to do with the specification in the assignment. It is only a minimal prototype demonstrating network communication between client and server using Berkeley sockets. You may use it to see if tasks and debug configurations are working properly.

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
- `tests/` – integration smoke tests (this is just for you if you want to make some tests)

See `docs/architecture.md` for more information.
