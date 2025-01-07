
# Project 3: Semaphore-Based Message Processing

## Overview
This project implements a **semaphore-based system** to process and encode messages using **Shannon coding**. The system uses multithreading to handle multiple messages simultaneously, ensuring that threads print their output in order by using semaphores for synchronization. It provides a detailed analysis of each message, including symbol frequencies, Shannon codes, and the encoded message.

---

## Features
- **Shannon Coding**:
  - Encodes messages based on symbol probabilities, generating variable-length binary codes for efficient representation.
  - Outputs symbol frequencies, Shannon codes, and the final encoded message.

- **Multithreading**:
  - Creates a separate thread for each input message.
  - Ensures concurrent processing for faster execution.

- **Ordered Output with Semaphores**:
  - Uses semaphores to synchronize threads, ensuring output is displayed in the order messages were received.

- **Custom Sorting**:
  - Sorts symbols by frequency (descending) and ASCII value (descending).

---

## How It Works
### Workflow:
1. **Input Handling**:
   - Reads multiple lines of input from the user or standard input.
   - Each line represents a message to be processed.

2. **Thread Creation**:
   - A separate thread is created for each message.
   - Threads use semaphores to ensure output is printed in order.

3. **Shannon Coding**:
   - Calculates symbol frequencies for the message.
   - Generates Shannon codes based on symbol probabilities.
   - Encodes the message into a binary string using the Shannon codes.

4. **Ordered Output**:
   - Threads wait for their turn to print the result, maintaining the order of input messages.

---

## Input and Output
### Input:
- A list of text messages provided via standard input.

### Output:
- For each message:
  1. Original message.
  2. Alphabet with symbol frequencies and Shannon codes.
  3. Encoded binary message.

### Example:
**Input**:
```
hello
world
```

**Output**:
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

## Implementation Details
### Key Structures:
1. **EncodedResult**:
   - Stores the processed data for each message, including:
     - Original message.
     - Symbol frequencies.
     - Shannon codes.
     - Encoded message.

2. **ThreadData**:
   - Manages shared data and synchronization primitives, including:
     - Mutex for thread-safe access.
     - Semaphores to control output order.

### Algorithm:
1. **Frequency Calculation**:
   - Counts the occurrences of each symbol in the message.

2. **Shannon Code Generation**:
   - Calculates probabilities for each symbol and generates binary codes based on cumulative probabilities.

3. **Ordered Printing**:
   - Threads use semaphores to print their results in the correct order.

---

## Code Highlights
- **Frequency Calculation**:
```cpp
for (char ch : input) {
    frequency[ch]++;
}
```

- **Shannon Code Generation**:
```cpp
int code_length = ceil(-log2(probabilities[i]));
for (int j = 0; j < code_length; ++j) {
    cumulativeSum *= 2;
    if (cumulativeSum >= 1.0) {
        s_code += '1';
        cumulativeSum -= 1.0;
    } else {
        s_code += '0';
    }
}
```

- **Thread Synchronization with Semaphores**:
```cpp
sem_wait(&data->print_sems[local_id]);
cout << "Message: " << result.message << endl;
sem_post(&data->print_sems[local_id + 1]);
```

---

## Compilation and Execution
### Compilation:
Compile the program using `g++` with pthread and semaphore libraries:
```bash
g++ -pthread -o semaphore_processing main.cpp
```

### Execution:
Run the program and provide input via standard input:
```bash
./semaphore_processing
```

---

## Applications
- **Data Compression**: Provides efficient encoding of text data for storage or transmission.
- **Parallel Processing**: Demonstrates the use of multithreading for concurrent execution.
- **Synchronization**: Explores the use of semaphores for managing thread execution order.

