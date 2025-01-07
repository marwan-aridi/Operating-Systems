# Operating Systems Projects

This repository contains three projects that demonstrate fundamental concepts of operating systems, including multithreading, client-server communication, and semaphore-based synchronization. Each project is implemented in C++ and showcases real-world applications of these concepts.

---

## Project 1: Shannon Coding with Multithreading
### Overview
This project implements the Shannon-Fano coding algorithm for data compression, leveraging multithreading to process multiple input messages concurrently. The implementation calculates symbol frequencies, generates Shannon codes, and encodes input messages into binary form.

### Key Features
- Multithreading for concurrent message processing.
- Frequency-based Shannon coding.
- Outputs encoded messages with detailed symbol statistics.

### Usage
Compile and run the `main.cpp` file using `g++` with pthread support.

```bash
g++ -pthread -o shannon main.cpp
./shannon
```

---

## Project 2: Client-Server Communication
### Overview
This project establishes a client-server model using TCP sockets. The server processes messages from the client, performs Shannon coding on the input, and sends the encoded data and statistics back to the client. The implementation supports multiple clients using forking and multithreading.

### Key Features
- Bidirectional communication between client and server.
- Forking on the server to handle multiple clients.
- Multithreaded client for concurrent message handling.

### Usage
Compile the `client.cpp` and `server.cpp` files, then run them.

Server:
```bash
g++ -o server server.cpp
./server <port>
```

Client:
```bash
g++ -pthread -o client client.cpp
./client <hostname> <port>
```

---

## Project 3: Semaphore-Based Message Processing
### Overview
This project uses semaphores to ensure ordered output when processing multiple messages with Shannon coding. Each message is handled by a separate thread, and semaphores are used to synchronize thread output.

### Key Features
- Semaphore-based synchronization for ordered output.
- Shannon coding for data compression.
- Multithreading for efficient message processing.

### Usage
Compile and run the `main.cpp` file using `g++` with pthread and semaphore libraries.

```bash
g++ -pthread -o semaphore_processing main.cpp
./semaphore_processing
```

---

## Applications
- **Data Compression**: Efficient encoding and compression of text data.
- **Concurrent Processing**: Demonstrates multithreading and synchronization techniques.
- **Client-Server Communication**: Explores real-time message exchange and processing.

---

