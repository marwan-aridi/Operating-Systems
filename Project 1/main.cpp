// Author: Marwan Aridi
// Shannon Algorithm with multithreading



#include <iostream>
#include <map>
#include <vector>        // handles resizable array containers
#include <algorithm>
#include <string>
#include <sstream>
#include <cmath>        // for math functions
#include <pthread.h>

using namespace std;

// Struct to store the result for each encoded message
struct EncodedResult 
{
   string message;
   map<char, string> shannon_algorithm; // Shannon codes for each character
   string encoded_string;
   map<char, int> frequency; // The frequency of each character
   vector<pair<char, int>> sorted_symbols; // Always keep symbols in a sorted order
};

// Custom comparator function to sort symbols first by frequency (descending)
// and by ASCII value
bool custom_comparator(const pair<char, int>& a, const pair<char, int>& b) 
{
    return a.second > b.second || (a.second == b.second && a.first > b.first);
}

// Function to calculate the Shannon code for each symbol based on its probability
void calculateShannonCodes(const vector<pair<char, int>>& symbols, int overall_frequency, map<char, string>& shannon_algorithm)
{
    vector<double> probabilities(symbols.size(), 0.0); // Vector to store probabilities of each symbol
    double total_probability = 0.0; // Keeps track of cumulative probability

    // Calculate probabilities for each symbol based on its frequency
    for (size_t i = 0; i < symbols.size(); ++i) 
    {
        probabilities[i] = static_cast<double>(symbols[i].second) / overall_frequency; // Probability = frequency / total frequency
    }

    // Generate Shannon codes based on cumulative probabilities
    for (size_t i = 0; i < symbols.size(); ++i)
    {
        char ch = symbols[i].first;
        double outcomeProbability = probabilities[i];
        string s_code = ""; // The Shannon code to be generated for the character
        
        // Calculate the code length based on the negative logarithm of the probability
        int code_length = ceil(-log2(outcomeProbability)); // Number of bits for the Shannon code
        double cumulativeSum = total_probability; // Start with the current cumulative probability

        // Generate the binary code for the character
        for (int j = 0; j < code_length; ++j) 
        {
            cumulativeSum *= 2; // Multiply cumulative probability by 2
            if (cumulativeSum >= 1.0)
            {
                s_code += '1'; // If cumulative is greater than or equal to 1, append '1'
                cumulativeSum -= 1.0; // Subtract 1 from cumulative
            } else
            {
                s_code += '0'; 
            }
        }

        shannon_algorithm[ch] = s_code; // Assign the generated Shannon code to the character
        total_probability += outcomeProbability; // Update the cumulative probability
    }
}

// Main function that performs Shannon coding for a given input string
void shannon_coding(const string& input, EncodedResult& result) 
{
    map<char, int> frequency; // Map to store frequency of each character

    // Calculate the frequency of each character in the input string
    for (char ch : input) 
    {
        frequency[ch]++; // Increment frequency count for the character
    }

    // Sort the characters based on frequency (descending) and ASCII value (descending)
    vector<pair<char, int>> sorted_symbols(frequency.begin(), frequency.end()); // Create a vector from the frequency map
    sort(sorted_symbols.begin(), sorted_symbols.end(), custom_comparator); // Sort the symbols using the custom comparator
    result.sorted_symbols = sorted_symbols; // Will store the sorted symbols

    // Generate Shannon codes for the sorted symbols
    int overall_frequency = input.length(); // Total number of characters in the input
    calculateShannonCodes(sorted_symbols, overall_frequency, result.shannon_algorithm); // Generate codes and store in the result

    // Encode the input message using the generated Shannon codes
    stringstream encoded_stream; // Use stringstream to build the encoded message
    for (char ch : input) 
    {
        encoded_stream << result.shannon_algorithm[ch]; // Append the Shannon code for each character
    }
    result.encoded_string = encoded_stream.str(); // Store the final encoded message
    result.message = input; // Store the original input message
    result.frequency = frequency; // Store the frequency data for output purposes
}

// Thread function that processes each input string
void* process_string(void* arg) 
{
    EncodedResult* result = static_cast<EncodedResult*>(arg); // Cast the input argument to EncodedResult
    shannon_coding(result->message, *result); // Perform Shannon coding on the input string
    pthread_exit(nullptr); 
}

int main() 
{
    vector<string> inputs; // Vector to store multiple input strings
    string line; // Temporary variable to hold each input line
    vector<EncodedResult> results; // Vector to store results for each thread

    // Reading input strings from standard input 
    while (getline(cin, line)) 
    { // Read each line of input
        if (!line.empty()) { 
            EncodedResult result;
            result.message = line; // Store the input message
            results.push_back(result); // Store the result
        }
    }

    // Create and launch threads using POSIX threads (pthreads)
    vector<pthread_t> threads(results.size()); // Create a vector to store thread identifiers
    for (size_t i = 0; i < results.size(); ++i) 
    {
        pthread_create(&threads[i], nullptr, process_string, &results[i]); // Create a thread for each input string
    }

    // Join the threads 
    for (size_t i = 0; i < results.size(); ++i) 
    {
        pthread_join(threads[i], nullptr); 
    }

    // Output results 
    for (const auto& result : results) 
    {
        cout << "Message: " << result.message << endl << endl;
        cout << "Alphabet: " << endl;
        for (const auto& ch_pair : result.sorted_symbols)
        {
            char ch = ch_pair.first;
            int freq = ch_pair.second;
            cout << "Symbol: " << ch 
                 << ", Frequency: " << freq 
                 << ", Shannon code: " << result.shannon_algorithm.at(ch) << endl; 
        }
        cout << "\nEncoded message: " << result.encoded_string << endl << endl; 
    }

    return 0;
}
