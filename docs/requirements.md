# 📂 MiniDrive 

*Client/Server File Synchronization System (C++)*

| Section | Content |
| :--- | :--- |
| **Objective** | Design and implement a client–server application in **C++** (max C++20) that synchronizes files between a local folder and a remote server, similar to OneDrive or Dropbox. |
| **Focus** | C++, Networking, Concurrency, File Systems, Data Synchronization. |
| **Platform** | Both server and client must run on a POSIX-compliant operating system (Linux, macOS). If you use Windows, you can use WSL (Windows Subsystem for Linux) to create a compatible environment. *We may allow Windows native support in the future, if someone requests it.* |
| **Building** | Use **CMake** as the build system. Some basic CMake configuration files will be provided to help you get started. |
| **Specification** | Requirements are detailed below. Read them carefully. They specify what should done with some tips on implementation, there may be ambiguities that you need to resolve. If not sure, please reach out for clarification. |
| **Deadline** | *December 31, 2025* |

---

## 💻 System Overview

Your system consists of two programs:

1. **`server`**: Runs persistently, listens for client connections, and manages the file repository.
2. **`client`**: Connects to the server, optionally authenticates, and allows the user to manage and synchronize files.

---

## ⚙️ Server Requirements

| Command | `./server --port <PORT> --root <ROOT_PATH>` |
| :--- | :--- |
| **Binding** | Binds to `0.0.0.0` on the given port. |
| **Concurrency** | Accepts **multiple clients concurrently**. |
| **Public Mode (Default)** | No authentication. All unauthenticated clients share a common **"public" directory**. |
| **Authenticated Mode** | Client provides username/password. Server authenticates or registers the user and assigns a **private directory** for that user. All operations apply only inside their private directory. |
| **Root Directory** | The server's root directory (specified by *`--root <ROOT_PATH>`*) should contain all public and user directories, and all your configuration files should be here. |
| **Persistence** | User credentials and file structure must persist across server restarts. |
| **Graceful Handling** | Gracefully handle invalid commands, missing files, permission issues. |
| **Shutdown** | On termination (receiving `SIGTERM`), cleanly close all client connections and save any necessary state. |
| **Logging (Optional)** | Log significant events (connections, commands, errors) to stdout for debugging. |

---

## ⌨️ Client Requirements

| Command | `./client [username@]<server_ip>:<port> [--log <log_file>]` |
| :--- | :--- |
| **Connection** | Connects to the specified IP and port. |
| **Public Mode** | If no username is provided. Must print: `[warning] operating in public mode - files are visible to everyone` |
| **Authenticated Mode** | Attempts authentication if username is provided and found on server. Must ask for password from stdin. If auth is successful must print: `Logged as <username>`. |
| **Registration** | If a username is provided, but does not exist on server, must prompt: `User <username> not found. Register? (y/n):` and if the answer is `y`, then prompt (stdin) for a password and register the user. User is created on server. Application then informs about result and exits. |
| **Prompt** | Upon successful connection, displays a prompt: `>` for interactive command entry. |
| **Logging (Optional)** | If `--log <log_file>` is provided, log commands, server responses and other useful information to the specified file. |
| **Ctrl+C Handling** | Gracefully handle `Ctrl+C` (SIGINT) by closing the connection and exiting. If you implement file resuming, this is the time when communicate with server and save state. |

* Parameters in `<angle brackets>` are required, while those in `[square brackets]` are optional. 

---

## 🚀 Client Interactive Commands

The client must support the following commands at the `>` prompt:

* Parameters in `<angle brackets>` are required, while those in `[square brackets]` are optional. 
* Paths are relative to the current directory on server unless they start with `/` (absolute path). Paths on local machine behave as normal filesystem paths, no special handling needed.
* All paths are relative to user’s root directory (public or private). You must **never allow directory traversal outside this root** (e.g., `../`).
* Commands and paths are **case-sensitive**.
* If a command is finished, new prompt should be printed.

### Local Commands

| Command | Description |
| :--- | :--- |
| **`HELP`** | Prints a list of available commands. |
| **`EXIT`** | Closes the connection and terminates the client. |

* Those commands are handled locally by the client and do not require server interaction, although `EXIT` may inform the server about the disconnection.

### Remote Commands

* Every command must be sent to the server, and the client must wait for and display the server's response and any relevant output.
* When the command successfully completes, the client should print `OK` on first first line any relevant output (e.g., file lists, success messages) on next lines. If an error occurs, it should print `ERROR: <error_code>` and the error message with optional additional details on next lines.
* If the error is dropped connection (either the server is dead or network issue), the client should exit after printing error message.
* Error code should be defined by you. There is no strong requirement to follow the exact codes for exact situations, but use them sensibly. The error code should be at least somewhat indicative of the error type. 

### Main Commands

| Command | Description |
| :--- | :--- |
| **`LIST [path]`** | Lists files and folders in the given path. If no path is given, lists the current directory. |
| **`UPLOAD <local_path> [remote_path]`** | Uploads a file from the client’s local file system to the server. If `remote_path` is omitted, the same name is used. |
| **`DOWNLOAD <remote_path> [local_path]`**| Downloads a file from the server to the client. If `local_path` is omitted, current directory with filename from remote is used. |
| **`DELETE <path>`** | Deletes a file on the server. |

* Upload or download should fail if it would overwrite an existing file. User should either delete it, or pick a different name/path.

### Folder Commands

| Command | Description |
| :--- | :--- |
| **`CD <path>`** | Changes the current directory to the specified path. |
| **`MKDIR <path>`** | Creates a new folder on the server. |
| **`RMDIR <path>`** | Removes a folder on the server (recursive). |
| **`MOVE <src> <dst>`** | Moves or renames a file or folder on the server. |
| **`COPY <src> <dst>`** | Copies a file or folder on the server. |

* Move, copy or mkdir should fail if the target path already exists. Same as above for files.

### ⌚  Synchronization Command

| Command | Description |
| :--- | :--- |
| **`SYNC <src> <dst>`** | Synchronizes the local directory with the server (Local to Remote, one-way). |

* Uploads only files that have **changed** locally (based on hash).
* Deletes files on the server that were **deleted locally**.
* Does **not re-upload** unchanged files.
* Prints a summary of actions taken (files uploaded, deleted, skipped).
* `SYNC` should first get a list of files and their hashes from the server, then compare with local files to determine which files to upload or delete.
* Should handle nested directories recursively.

---

## 📁 File handling

* Upload and download files in **binary mode** to support all file types.
* Upload and download should be done in **chunks** (e.g., 64KB) to support large files without excessive memory usage.
* How you do the chunking is up to you, but the server must be able to handle files of at least **4GB** in size. Above that size, you can have a threshold where you refuse to upload files.
* Support for resuming interrupted uploads/downloads is **required**, but not with the base implementation.
* Handle file I/O errors gracefully (e.g., file not found, permission denied).

### 🐌 Resuming Transfers

* **Resuming transfers should only work for private mode, so no need to implement it when user asks for public mode**
* Server can crash or connection can be lost at any time during file transfer. You should be able to resume in this two instances (others scenations can result in non resumable upload/download)
   * SIGTERM was sent to server
   * Connection from client is lost
* Client must be able to resume interrupted uploads and downloads.
* For **resuming uploads**, the client should first check with the server how many bytes (chunks) of the file have already been uploaded, the flow should be like this:
   * After handshake and auth with the server client requests file server is currently able to resume
   * Server responds with the file (it is up to you whether you will send local path, hash, or you have this saved by client, or you add some request id as part of protocol and used that)
   * Client should then request resume and start uploading
* For **resuming downloads**, the client have to store some info about last operation, upon successfull connect it should request download, but only specific part
* You can implement resuming by storing temporary files with a special extension (e.g., `.part`) and renaming them once the transfer is complete.
* Hashes should be used to verify file integrity after download/upload is complete.
* Incomplete uploads on the server should be cleaned up if the client does not resume within a certain timeout (e.g., 1 hour). You can implement this with a timer or thread.
* Upon client startup, when there are incomplete transfers (either file, or sync), the client should inform the user and offer to resume them or discard them. 

```plain
Logged as <username>
Incomplete upload/downloads detected, resume? (y/n):
> y
UPLOAD <file1>
```

* After user agrees to resume, the client should write the command that is about to be resumed, so the user can see what is happening.


### 🤹 Multiple sessions

* In base implementation, you may allow only one user session at a time.
* By session we mean a client connected to the server with a specific username (or in public mode).
* If a second client tries to connect with the same username (or in public mode), the server should reject the connection with an appropriate error message.
* More points will be awarded if you allow multiple sessions per user. 
    * This means that multiple clients can connect simultaneously with the same username (or in public mode) and perform file operations concurrently.
    * No file should be corrupted, if two clients upload the same file at the same time, one upload may overwrite the other, but the server must not crash and other clients must not be affected.
    * Resuming downloads should be easy to implement, but resuming uploads need some care, since multiple session can upload to one user's directory

---

## 📡 Communication Protocol

| Aspect | Requirement |
| :--- | :--- |
| **Protocol** | **JSON** (recommended via `nlohmann/json`) or a custom text-based protocol. **Must use raw TCP sockets or Asio sockets.** |
| **Client Message** | Include at least a command name and arguments. **Example (JSON):** `{"cmd": "LIST", "path": "/"}` |
| **Server Response** | Should contain `status`, `message text`. |

---

### Error codes

Response format (text protocol in client):

```plain
OK
<data lines...>

ERROR: <code>
<primary message>
<optional detail lines...>
```

Please define your own error codes and messages and document them in your documentation.:


## 🔐 Authentication and Passwords

| Aspect | Requirement |
| :--- | :--- |
| **Authentication** | Optional; default is **public mode**. |
| **Credentials** | When provided, the server must **authenticate or register** the user. |
| **Security** | Passwords must **never** be stored as **plain text**. |
| **Hashing** | Use appropriate password hashing with a crypto library. Must use **salt**. |
| **Storage** | Store user credentials and metadata in a simple **file-based database**. |

---

## 🛠️ Allowed and Technical Requirements

### Allowed Libraries

| Category | Recommended Libraries |
| :--- | :--- |
| **Networking** | **Asio** (standalone version w/o Boost) |
| **Serialization** | **nlohmann/json** |
| **Crypto** | **libsodium** |
| **Logging (Optional)** | **spdlog** |
| **Build System** | **CMake** |
| **Restriction** | Do **not** use any other libraries besides those listed above and the C++ standard library. |

### Technical Requirements

| Requirement | Details |
| :--- | :--- |
| **Language** | C++20 or later |
| **Networking** | TCP (Asio or BSD sockets) |
| **Threads/Concurrency** | `std::thread`, `std::mutex`, or **Asio async model** |
| **Filesystem** | `std::filesystem` for file handling |
| **Error Handling** | Gracefully handle failed transfers, invalid commands, missing files. |
| **Security** | **Never allow directory traversal outside user’s root folder.** |
| **Documentation** | Provide a documentations (do not need to be exhaustive), design decisions, and protocol description. Include examples of requests and responses. |

---

## ✅ Grading Breakdown (Total: 30 points)

| Criteria | Description | Points |
| :--- | :--- | :--- |
| **Core File Commands** | Working with one directory LIST, UPLOAD, DOWNLOAD, DELETE, server handles at least one client simultaneously | 5 |
| **Folder Commands** | Working with directories CD, MKDIR, RMDIR, MOVE, COPY | 5 |
| **Synchronization** | SYNC uploads changes, handles deletions (Local to Remote) | 4 |
| **Authentication** | Public/private directories, hashed passwords (salted) | 5 |
| **Server Concurrency** | Handles multiple clients simultaneously | 5 |
| **Resume Transfers** | Supports resuming interrupted uploads/downloads | 3 |
| **Robustness** | Graceful error handling, no crashes on invalid input | 3 |
| **Subtotal** | | **30** |
| **Logging** | Optional nice structured logging with library **spdlog** | 1 |
| **Multiple Sessions** | Optional, but appreciated for extra challenge | 2 |

| **TOTAL** | | **33** |
