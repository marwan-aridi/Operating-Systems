// Author: Marwan Aridi

// This is client.cpp file 


#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>

// Function for error handling
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

// Structure to hold data for each thread
struct ThreadData
{
    std::string input_message;     
    std::string response_message;  
    std::string hostname;          
    int portno;                    
};

// Thread function that communicates with the server
void *client_thread(void *arg)
{
    ThreadData *data = (ThreadData *)arg;

    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Error opening socket");
    }

    // Get the server's address by hostname
    server = gethostbyname(data->hostname.c_str());
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        pthread_exit(NULL);
    }

    // Set up the server address structure
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(data->portno);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Error connecting");
    }

    // Send the size of the message first
    int msgSize = data->input_message.size();
    n = write(sockfd, &msgSize, sizeof(int));
    if (n < 0)
    {
        error("Error writing to socket");
    }

    // Send the actual message to the server
    int totalBytesWritten = 0;
    while (totalBytesWritten < msgSize)
    {
        n = write(sockfd, data->input_message.c_str() + totalBytesWritten, msgSize - totalBytesWritten);
        if (n < 0)
        {
            error("Error writing to socket");
        }
        totalBytesWritten += n;
    }

    // Read the size of the response from the server
    int responseSize = 0;
    n = read(sockfd, &responseSize, sizeof(int));
    if (n < 0)
    {
        error("Error reading from socket");
    }

    // Allocate a buffer to read the response
    char *tempBuffer = new char[responseSize + 1];
    bzero(tempBuffer, responseSize + 1);

    // Read the actual response from the server
    int totalBytesRead = 0;
    while (totalBytesRead < responseSize)
    {
        n = read(sockfd, tempBuffer + totalBytesRead, responseSize - totalBytesRead);
        if (n < 0)
        {
            error("Error reading from socket");
        }
        totalBytesRead += n;
    }

    // Store the response in the thread data
    data->response_message = tempBuffer;
    delete[] tempBuffer;

    // Close the socket
    close(sockfd);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    // Check if the correct number of arguments is provided
    if (argc != 3)
    {
        std::cerr << "usage " << argv[0] << " hostname port" << std::endl;
        exit(0);
    }

    std::string hostname = argv[1];  // Get the hostname
    int portno = atoi(argv[2]);      // Get the port number

    // Read input messages from STDIN
    std::vector<ThreadData> threadDataList;
    std::string line;
    while (std::getline(std::cin, line))
    {
        if (!line.empty())
        {
            // Prepare thread data for each input message
            ThreadData data;
            data.input_message = line;
            data.hostname = hostname;
            data.portno = portno;
            threadDataList.push_back(data);
        }
    }

    // Create a thread for each input message
    std::vector<pthread_t> threads(threadDataList.size());
    for (size_t i = 0; i < threadDataList.size(); ++i)
    {
        int rc = pthread_create(&threads[i], NULL, client_thread, &threadDataList[i]);
        if (rc)
        {
            std::cerr << "Error: unable to create thread, " << rc << std::endl;
            exit(-1);
        }
    }

    // Wait for all threads to finish
    for (size_t i = 0; i < threads.size(); ++i)
    {
        pthread_join(threads[i], NULL);
    }

    // Output the responses received from the server
    for (size_t i = 0; i < threadDataList.size(); ++i)
    {
        std::cout << threadDataList[i].response_message;
    }

    pthread_exit(NULL);
    return 0;
}
