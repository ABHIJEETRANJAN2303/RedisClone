# RedisClone

A Redis-inspired in-memory key-value database server built from scratch in modern C++ using non-blocking sockets, event-driven architecture, and the RESP protocol.

---

# Features

- Event-driven TCP server
- Non-blocking socket architecture
- kqueue-based IO multiplexing (macOS)
- RESP protocol parser
- RESP response encoder
- Incremental TCP stream parsing
- Per-client read/write buffers
- Partial write handling
- In-memory key-value storage
- Redis-compatible `PING`
- `SET` / `GET` commands
- TTL expiration support (`SET key value EX seconds`)
- Compatible with `redis-cli`

---

# Architecture

The project is divided into modular backend layers:

```text
Client (redis-cli / nc)
        |
        v
+-------------------+
|   TCP Server      |
+-------------------+
        |
        v
+-------------------+
|   Event Loop      |
|    (kqueue)       |
+-------------------+
        |
        v
+-------------------+
|   Connection      |
| Read/Write Buffers|
+-------------------+
        |
        v
+-------------------+
|   RESP Parser     |
|  RESP Encoder     |
+-------------------+
        |
        v
+-------------------+
| Command Dispatcher|
+-------------------+
        |
        v
+-------------------+
| Key Value Store   |
+-------------------+
````

---

# Tech Stack

* C++17
* POSIX sockets
* kqueue
* CMake
* macOS

---

# Supported Commands

## PING

```bash
PING
```

Response:

```bash
PONG
```

---

## SET

```bash
SET key value
```

Example:

```bash
SET language cpp
```

---

## GET

```bash
GET language
```

Example response:

```bash
"cpp"
```

---

## TTL Expiration

```bash
SET temp hello EX 5
```

The key expires automatically after 5 seconds.

---

# Build Instructions

## Clone Repository

```bash
git clone <your-repository-url>
cd RedisClone
```

---

## Build

```bash
mkdir build
cd build
cmake ..
make
```

---

## Run Server

```bash
./redis_clone
```

---

# Testing

Use `redis-cli`:

```bash
redis-cli
```

Example:

```bash
PING

SET name abhijeet

GET name

SET temp hello EX 5
```

---

# Key Backend Concepts Implemented

* Event-driven architecture
* Non-blocking IO
* TCP stream handling
* Incremental protocol parsing
* Per-client connection state
* Read/write buffering
* Partial read handling
* Partial write handling
* Backpressure-aware design
* Protocol encoding/decoding
* Runtime key expiration

---

# Future Improvements

* epoll portability (Linux)
* Persistent storage
* Thread pool / multi-reactor scaling
* Transactions
* Pub/Sub
* LRU eviction
* Benchmark suite
* Config system

---

# Learning Goals

This project was built to deeply understand:

* Backend systems programming
* Event-driven server design
* TCP networking internals
* Async IO patterns
* Protocol engineering
* Scalable server architecture

---

# Author

Abhijeet Ranjan