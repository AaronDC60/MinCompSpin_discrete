#include "model.h"

/**
 * Reads in and processes the dataset.
 * 
 * @param file                  Path to the file.
 * @param n                     Number of variables in the system
 * @param n_ints                Number of 128bit integers necessary to represent the data
 * 
 * @return The processed dataset, which is an empty vector if the file is not found.
 */
std::vector<std::vector<__uint128_t>> data_processing(std::string file, int n, int n_ints){
    // Open file
    std::ifstream myfile(file);

    // Store dataset as vector of vectors containing n_ints 128bit integers
    std::vector<std::vector<__uint128_t>> data;

    // Check if file exists
    if (myfile.fail()){
        std::cout << "Not able to open the file." << std::endl;
        return data;
    }

    std::string line;
    std::vector<__uint128_t> observation(n_ints);
    while (getline(myfile, line)) {
        // Exctract the first n variables from the observation
        line = line.substr(0, n);
        // Convert the observation and add it to the dataset
        convert_observation(observation, line, n);
        data.push_back(observation);
    }
    return data;
}

/**
 * Converts the data from a string to log2(q) 128bit integers.
 * 
 * @param[in, out] obs          Vector of log2(q) 128bit integers that will contain the converted observation.
 * @param[in] raw_obs           The observation as a string of length n.
 * @param n                     Number of variables in the system.
 * 
 * @return void                 Nothing is returned by this function.
 */
void convert_observation(std::vector<__uint128_t>& obs, std::string& raw_obs, int n){
    // Set all elements equal to zero
    std::fill(obs.begin(), obs.end(), 0);
    // Variable for the integer value of the ith bit
    __uint128_t element = 1;
    // Loop over the variables
    for (int i = 0; i < n; ++i){
        // Convert value of variable i from string to integer
        int value = raw_obs[i] - '0';
        int bit = 0;
        while (value){
            // Check if last bit in the binary representation is nonzero
            if (value & 1){
                obs[bit] += element;
            }
            // Bitshift to the right
            ++bit;
            value >>= 1;
        }
        // Bitshift to the left to get the decimal value of the next variable
        element <<= 1;
    }
}