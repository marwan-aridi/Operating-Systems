
# Project 2: Client-Server Communication

## Overview
This project implements a **client-server model** using sockets in C++. The server processes text messages received from clients and performs **Shannon coding** on the input, returning encoded data and symbol statistics to the client. Multithreading and forking are employed to manage multiple connections simultaneously, ensuring efficient communication.

---

## Features
- **Bidirectional Communication**:
  - The client sends text messages to the server, and the server returns encoded results.
  - Includes details about symbols, frequencies, Shannon codes, and the encoded message.

- **Multithreading on Client**:
  - Each input message is processed in a separate thread, allowing concurrent message handling.

- **Forking on Server**:
  - The server uses the `fork()` system call to create child processes for each client connection.

- **Shannon Coding**:
  - Implements Shannon-Fano coding to encode messages based on symbol probabilities.

- **Socket Programming**:
  - Uses TCP sockets for reliable communication between client and server.

---

## Components
1. **Client** (`client.cpp`):
   - Reads input messages from the user or standard input.
   - Sends messages to the server over a socket connection.
   - Receives and displays responses from the server.
   - Supports multithreading to handle multiple messages concurrently.

2. **Server** (`server.cpp`):
   - Listens for incoming client connections on a specified port.
   - For each client, performs Shannon coding on the received message.
   - Returns the encoded message along with symbol statistics to the client.
   - Handles multiple client connections using `fork()`.

---

## How It Works
### Client Workflow:
1. Reads messages from the user or standard input.
2. Creates a thread for each message.
3. Connects to the server and sends the message.
4. Receives and displays the server's response.

### Server Workflow:
1. Accepts incoming connections from clients.
2. Forks a child process for each connection.
3. Reads the client's message and performs Shannon coding.
4. Sends the encoded result back to the client.

---

## Input and Output
### Input:
- Client: Text messages provided via standard input or command-line arguments.
- Server: Listens for connections and processes messages sent by the client.

### Output:
- Client: Displays the server's response, including encoded data and symbol statistics.
- Server: Sends encoded data and symbol statistics to the client.

### Example:
**Client Input**:
```
hello
world
```

**Server Response**:
```
Message: hello

Alphabet:
Symbol: l, Frequency: 2, Shannon code: 0
Symbol: h, Frequency: 1, Shannon code: 10
Symbol: e, Frequency: 1, Shannon code: 110
Symbol: o, Frequency: 1, Shannon code: 111

Encoded message: 1011011110


Message: world

Alphabet:
Symbol: l, Frequency: 1, Shannon code: 00
Symbol: d, Frequency: 1, Shannon code: 01
Symbol: o, Frequency: 1, Shannon code: 10
Symbol: r, Frequency: 1, Shannon code: 110
Symbol: w, Frequency: 1, Shannon code: 111

Encoded message: 111101100
```

---

## Compilation and Execution
### Compilation:
1. Compile the client:
   ```bash
   g++ -pthread -o client client.cpp
   ```
2. Compile the server:
   ```bash
   g++ -o server server.cpp
   ```

### Execution:
1. Start the server:
   ```bash
   ./server <port>
   ```
   Replace `<port>` with the desired port number.

2. Start the client:
   ```bash
   ./client <hostname> <port>
   ```
   Replace `<hostname>` with the server's address (e.g., `localhost`) and `<port>` with the server's port number.

3. Provide input messages to the client via standard input.

---

## Applications
- **Real-Time Communication**: Facilitates encoding and decoding of messages in real time.
- **Socket Programming**: Demonstrates TCP communication between client and server.
- **Parallel Processing**: Explores multithreading on the client and forking on the server for concurrent processing.
