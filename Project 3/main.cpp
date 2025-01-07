// Author: Marwan Aridi 


#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <cmath>
#include <pthread.h>
#include <semaphore.h> 

using namespace std;


// Struct to store the result for each message
struct EncodedResult 
{
    string message;
    map<char, string> shannon_algorithm; // Shannon codes for each character
    string encoded_string;
    map<char, int> frequency; // Frequency of each character
    vector<pair<char, int>> sorted_symbols; // Symbols sorted by frequency and ASCII
};

// Struct to hold shared data and synchronization primitives
struct ThreadData {
    pthread_mutex_t bsem;           // Mutex for shared data access
    sem_t* print_sems;              // Array of semaphores for printing in order
    int id;                         // Thread ID
    string message;                 // Message to process
    sem_t copy_sem;                 // Semaphore to synchronize copying of data
    int total_threads;              // Total number of threads
};

// Comparator to sort symbols by frequency and ASCII value
bool custom_comparator(const pair<char, int>& a, const pair<char, int>& b) 
{
    return a.second > b.second || (a.second == b.second && a.first > b.first);
}

// Function to calculate Shannon codes for symbols
void calculateShannonCodes(const vector<pair<char, int>>& symbols, int overall_frequency, map<char, string>& shannon_algorithm)
{
    vector<double> probabilities(symbols.size(), 0.0);
    double total_probability = 0.0;

    // Calculate probabilities for each symbol
    for (size_t i = 0; i < symbols.size(); ++i) 
    {
        probabilities[i] = static_cast<double>(symbols[i].second) / overall_frequency;
    }

    // Generate Shannon codes based on cumulative probabilities
    for (size_t i = 0; i < symbols.size(); ++i)
    {
        char ch = symbols[i].first;
        double outcomeProbability = probabilities[i];
        string s_code = "";
        int code_length = ceil(-log2(outcomeProbability));
        double cumulativeSum = total_probability;

        for (int j = 0; j < code_length; ++j) 
        {
            cumulativeSum *= 2;
            if (cumulativeSum >= 1.0)
            {
                s_code += '1';
                cumulativeSum -= 1.0;
            } else
            {
                s_code += '0'; 
            }
        }

        shannon_algorithm[ch] = s_code;
        total_probability += outcomeProbability;
    }
}

// Function to perform Shannon coding on the input string
void shannon_coding(const string& input, EncodedResult& result) 
{
    map<char, int> frequency;

    // Count frequency of each character
    for (char ch : input) 
    {
        frequency[ch]++;
    }

    // Sort symbols by frequency and ASCII value
    vector<pair<char, int>> sorted_symbols(frequency.begin(), frequency.end());
    sort(sorted_symbols.begin(), sorted_symbols.end(), custom_comparator);
    result.sorted_symbols = sorted_symbols;

    // Generate Shannon codes
    int overall_frequency = input.length();
    calculateShannonCodes(sorted_symbols, overall_frequency, result.shannon_algorithm);

    // Encode the message
    stringstream encoded_stream;
    for (char ch : input) 
    {
        encoded_stream << result.shannon_algorithm[ch];
    }
    result.encoded_string = encoded_stream.str();
    result.message = input;
    result.frequency = frequency;
}

// Thread function to process each message
void* threadFunction(void* arg) 
{
    ThreadData* data = (ThreadData*) arg;

    // Lock to access shared data
    pthread_mutex_lock(&data->bsem);

    // Copy data to local variables
    int local_id = data->id;
    string local_message = data->message;
    int total_threads = data->total_threads;

    // Unlock after copying
    pthread_mutex_unlock(&data->bsem);

    // Signal to main thread that data has been copied
    sem_post(&data->copy_sem); // Let the parent know we've copied the data

    // Process the message
    EncodedResult result;
    shannon_coding(local_message, result);

    // Wait for our turn to print
    sem_wait(&data->print_sems[local_id]);

    // Print the output
    cout << "Message: " << result.message << endl;
    cout << "Alphabet:" << endl;
    for (const auto& ch_pair : result.sorted_symbols)
    {
        char ch = ch_pair.first;
        int freq = ch_pair.second;
        cout << "Symbol: " << ch 
             << ", Frequency: " << freq 
             << ", Shannon code: " << result.shannon_algorithm.at(ch) << endl; 
    }
    cout << "Encoded message: " << result.encoded_string << endl << endl;

    // Signal the next thread to print
    if (local_id + 1 < total_threads)
    {
        sem_post(&data->print_sems[local_id + 1]);
    }

    pthread_exit(nullptr);
}

int main() 
{
    vector<string> messages;
    string line;

    // Read input messages from standard input
    while (getline(cin, line)) 
    {
        if (!line.empty()) 
        { 
            messages.push_back(line);
        }
    }

    int total_threads = messages.size();

    // Initialize shared data and synchronization primitives
    ThreadData threadData;
    pthread_mutex_init(&threadData.bsem, nullptr);

    // Initialize the semaphore for copying data
    sem_init(&threadData.copy_sem, 0, 0);

    // Initialize the semaphores for printing in order
    threadData.total_threads = total_threads;
    threadData.print_sems = new sem_t[total_threads];
    for (int i = 0; i < total_threads; ++i)
    {
        sem_init(&threadData.print_sems[i], 0, 0);
    }
    // Allow the first thread to print
    sem_post(&threadData.print_sems[0]);

    vector<pthread_t> threads(total_threads);

    // Create and start threads
    for (int x = 0; x < total_threads; ++x) 
    {
        pthread_mutex_lock(&threadData.bsem); 

        threadData.id = x;                   
        threadData.message = messages[x];    

        // Create a new thread
        if (pthread_create(&threads[x], nullptr, threadFunction, &threadData)) 
        {
            cerr << "Error creating thread." << endl;
            return -1;
        }

        pthread_mutex_unlock(&threadData.bsem); // Unlock after creating the thread

        // Wait until the child thread has copied the data
        sem_wait(&threadData.copy_sem); // Wait for child to signal data copied
    }

    // Wait for all threads to finish
    for (int y = 0; y < total_threads; ++y) 
    {
        pthread_join(threads[y], nullptr); 
    }

    // Clean up and release synchronization constructs
    pthread_mutex_destroy(&threadData.bsem);
    sem_destroy(&threadData.copy_sem);

    for (int i = 0; i < total_threads; ++i)
    {
        sem_destroy(&threadData.print_sems[i]);
    }
    delete[] threadData.print_sems;

    return 0;
}
