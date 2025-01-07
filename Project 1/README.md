
# Project 1: Shannon Coding with Multithreading

## Overview
This project implements the **Shannon-Fano coding algorithm** for data compression, leveraging multithreading for parallel processing of input messages. It reads multiple strings, computes Shannon codes for each symbol based on their frequency, and encodes the input using the generated codes. The implementation uses **POSIX threads (pthreads)** for concurrent processing, ensuring efficient execution for large input datasets.

---

## Features
- **Frequency-Based Symbol Encoding**:
  - Calculates the frequency of each character in the input message.
  - Generates Shannon codes based on symbol probabilities.

- **Multithreading**:
  - Each input string is processed in parallel using pthreads.
  - Improves performance by distributing the workload across multiple threads.

- **Custom Symbol Sorting**:
  - Symbols are sorted by frequency (descending) and ASCII value (descending).

- **Thread-Safe Processing**:
  - Ensures proper synchronization and safe data handling for multithreaded execution.

---

## Input and Output
### Input:
- A list of text strings provided through standard input.

### Output:
- For each input string:
  1. **Original Message**: The input string.
  2. **Alphabet**: Symbols, their frequencies, and their Shannon codes.
  3. **Encoded Message**: The binary-encoded representation of the input string.

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

## How It Works
1. **Frequency Analysis**:
   - The program computes the frequency of each symbol in the input string.

2. **Shannon Code Generation**:
   - Shannon codes are generated based on the probability of each symbol.
   - Code length is proportional to `-log2(probability)`.

3. **Multithreaded Processing**:
   - Each string is processed in a separate thread.
   - Threads calculate frequencies, generate codes, and encode the input string.

4. **Output Generation**:
   - The results, including the alphabet, Shannon codes, and encoded message, are displayed for each input.

---

## Implementation Details
### Key Structures:
1. **EncodedResult**:
   - Stores the results for each input message, including:
     - Original message.
     - Symbol frequencies.
     - Shannon codes.
     - Encoded binary string.

2. **Threads**:
   - Each thread processes one input string independently.

### Algorithm:
1. **Custom Symbol Sorting**:
   - Symbols are sorted by frequency (descending) and ASCII value (descending).
2. **Shannon Code Calculation**:
   - Codes are generated using cumulative probabilities and binary representation.
3. **Message Encoding**:
   - Each character is replaced with its corresponding Shannon code.

---

## Code Highlights
- **Frequency Calculation**:
```cpp
for (char ch : input) {
    frequency[ch]++;
}
```

- **Custom Sorting**:
```cpp
sort(sorted_symbols.begin(), sorted_symbols.end(), custom_comparator);
```

- **Shannon Code Generation**:
```cpp
int code_length = ceil(-log2(probabilities[i]));
for (int j = 0; j < code_length; ++j) {
    cumulativeSum *= 2;
    s_code += (cumulativeSum >= 1.0) ? '1' : '0';
    cumulativeSum -= (cumulativeSum >= 1.0) ? 1.0 : 0;
}
```

- **Multithreading**:
```cpp
pthread_create(&threads[i], nullptr, process_string, &results[i]);
pthread_join(threads[i], nullptr);
```

---

## Compilation and Execution
### Compilation:
Compile the program using `g++`:
```bash
g++ -pthread -o shannon main.cpp
```

### Execution:
Run the program and provide input through standard input:
```bash
./shannon
```

---

## Applications
- **Data Compression**: Efficiently encodes data for transmission or storage.
- **Parallel Processing**: Demonstrates the use of multithreading to optimize performance.
- **Algorithm Design**: Explores Shannon-Fano coding, a foundational concept in data compression.

