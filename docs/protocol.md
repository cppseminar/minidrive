# MiniDrive Protocol

**Draft you should delete this or edit it** 

This document will capture the JSON command/response schema and binary transfer framing once the implementation stabilises.

## Control Channel

- All control messages are JSON documents encoded as UTF-8.
- Each message is framed using a 32-bit unsigned length prefix (network byte order).
- Example request:
  ```json
  { "cmd": "LIST", "args": { "path": "." } }
  ```
- Example response:
  ```json
  { "status": "OK", "code": 0, "message": "", "data": { "entries": [] } }
  ```

## Data Channel

File uploads/downloads reuse the TCP connection and stream binary chunks with per-chunk metadata (size, hash). Details TBD.
