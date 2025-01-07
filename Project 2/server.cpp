// Author: Marwan Aridi


// This is server.cpp file 



#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/wait.h>
#include <strings.h>

// Function for error handling
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

// Signal handler to prevent zombie processes (reaps child processes)
void fireman(int)
{
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

// Comparator function to sort symbols by frequency and ASCII value
bool custom_comparator(const std::pair<char, int> &a, const std::pair<char, int> &b)
{
    // Sort by descending frequency, then by descending ASCII value if frequencies are equal
    return a.second > b.second || (a.second == b.second && a.first > b.first);
}

// Function to calculate Shannon codes for the symbols
void calculateShannonCodes(const std::vector<std::pair<char, int> > &symbols, int overall_frequency, std::map<char, std::string> &shannon_algorithm)
{
    std::vector<double> probabilities(symbols.size(), 0.0);
    double total_probability = 0.0;

    // Calculate the probability for each symbol
    for (size_t i = 0; i < symbols.size(); ++i)
    {
        probabilities[i] = static_cast<double>(symbols[i].second) / overall_frequency;
    }

    // Generate Shannon codes for each symbol
    for (size_t i = 0; i < symbols.size(); ++i)
    {
        char ch = symbols[i].first;
        double outcomeProbability = probabilities[i];
        std::string s_code = "";
        int code_length = (int)ceil(-log2(outcomeProbability));  // Calculate code length
        double cumulativeSum = total_probability;

        // Generate the code bits
        for (int j = 0; j < code_length; ++j)
        {
            cumulativeSum *= 2;
            if (cumulativeSum >= 1.0)
            {
                s_code += '1';
                cumulativeSum -= 1.0;
            }
            else
            {
                s_code += '0';
            }
        }

        // Assign the code to the symbol
        shannon_algorithm[ch] = s_code;
        total_probability += outcomeProbability;
    }
}

// Function to perform Shannon coding on the input message
void shannon_coding(const std::string &input, std::map<char, std::string> &shannon_algorithm, std::string &encoded_string, std::vector<std::pair<char, int> > &sorted_symbols)
{
    std::map<char, int> frequency;

    // Calculate the frequency of each character in the input
    for (size_t i = 0; i < input.size(); ++i)
    {
        char ch = input[i];
        frequency[ch]++;
    }

    // Create a vector of symbols and their frequencies
    sorted_symbols.assign(frequency.begin(), frequency.end());

    // Sort the symbols based on frequency and ASCII value
    std::sort(sorted_symbols.begin(), sorted_symbols.end(), custom_comparator);

    // Total frequency is the length of the input
    int overall_frequency = input.length();

    // Generate Shannon codes for the symbols
    calculateShannonCodes(sorted_symbols, overall_frequency, shannon_algorithm);

    // Encode the message using the Shannon codes
    std::stringstream encoded_stream;
    for (size_t i = 0; i < input.size(); ++i)
    {
        char ch = input[i];
        encoded_stream << shannon_algorithm[ch];
    }
    encoded_string = encoded_stream.str();
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    // Check if port number is provided
    if (argc != 2)
    {
        std::cerr << "Port not provided" << std::endl;
        exit(1);
    }

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Error opening socket");
    }
    
    // Set socket options to reuse the address
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
    {
        error("Error setting SO_REUSEADDR");
    }
        

    // Initialize server address structure
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);  // Get port number from arguments
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP
    serv_addr.sin_port = htons(portno);

    // Bind the socket to the address
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Error on binding");
    }

    // Listen for incoming connections
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    // Handle zombie child processes
    signal(SIGCHLD, fireman);

    while (1)
    {
        // Accept a new connection
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0)
        {
            error("Error on accept");
        }

        // Fork a child process to handle the client
        pid_t pid = fork();
        if (pid < 0)
        {
            error("Error on fork");
        }
        if (pid == 0)
        {
            // Child process
            close(sockfd);  // Close the listening socket in the child

            int n, msgSize = 0;

            // Read the size of the incoming message
            n = read(newsockfd, &msgSize, sizeof(int));
            if (n < 0)
            {
                error("Error reading from socket");
            }

            // Allocate buffer and read the actual message
            int totalBytesRead = 0;
            char *tempBuffer = new char[msgSize + 1];
            bzero(tempBuffer, msgSize + 1);
            while (totalBytesRead < msgSize)
            {
                n = read(newsockfd, tempBuffer + totalBytesRead, msgSize - totalBytesRead);
                if (n < 0)
                {
                    error("Error reading from socket");
                }
                totalBytesRead += n;
            }
            std::string input_message = tempBuffer;
            delete[] tempBuffer;

            // Perform Shannon coding on the input message
            std::map<char, std::string> shannon_algorithm;
            std::string encoded_string;
            std::vector<std::pair<char, int> > sorted_symbols;
            shannon_coding(input_message, shannon_algorithm, encoded_string, sorted_symbols);

            // Prepare the response to send back to the client
            std::stringstream response_stream;
            response_stream << "Message: " << input_message << std::endl
                            << std::endl;
            response_stream << "Alphabet:" << std::endl;

            // Add each symbol's details to the response
            for (size_t i = 0; i < sorted_symbols.size(); ++i)
            {
                char ch = sorted_symbols[i].first;
                int freq = sorted_symbols[i].second;
                response_stream << "Symbol: " << ch
                                << ", Frequency: " << freq
                                << ", Shannon code: " << shannon_algorithm[ch] << std::endl;
            }
            response_stream << std::endl;
            response_stream << "Encoded message: " << encoded_string << std::endl
                            << std::endl;

            std::string response = response_stream.str();
            int responseSize = response.size();

            // Send the size of the response first
            n = write(newsockfd, &responseSize, sizeof(int));
            if (n < 0)
            {
                error("Error writing to socket");
            }

            // Send the actual response
            int totalBytesWritten = 0;
            while (totalBytesWritten < responseSize)
            {
                n = write(newsockfd, response.c_str() + totalBytesWritten, responseSize - totalBytesWritten);
                if (n < 0)
                {
                    error("Error writing to socket");
                }
                totalBytesWritten += n;
            }

            // Close the connection and exit the child process
            close(newsockfd);
            exit(0);
        }
        else
        {
            // Parent process closes the connected socket and continues listening
            close(newsockfd);
        }
    }

    // Close the listening socket 
    close(sockfd);
    return 0;
}


