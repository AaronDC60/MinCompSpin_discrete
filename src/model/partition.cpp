#include "model.h"

/**
 * Converts a component from the integer representation to string representation.
 * 
 * @param component             Integer representation of the bitstring representing a component.
 * @param n                     Number of variables in the system.
 * 
 * @return The string representation of the component
 */
std::string component_as_string(__uint128_t component, int n){
    // String representation of component
    std::string comp(n, '0');
    for (int j = 0 ; j < n; ++j){
        if (component & 1){
            comp[j] = '1';
        }
        component >>= 1;
    }
    return comp;
}

/**
 * Calculates the size of a given component.
 * 
 * @param component             Integer representation of the bitstring representing a component.
 *
 *@return The size of the component. 
 */
int component_size(__uint128_t component){
    int size = 0;
    while(component){
        // Check if last bit is equal to 1
        if (component & 1){
            size += 1;
        }
        // Bitshift to the right
        component >>= 1;
    }
    return size;
}

/**
 * Converts the partition from a restricted growth string to a vector of n integers representing the components.
 * 
 * @param[in] a                 Array of size n that represents the partition as a restricted growth string.
 * @param[in, out] partition    Partition as a vector of n integers representing the components.
 * @param n                     Number of variables in the system.
 * 
 * @return void                 Nothing is returned by this function.
 */
void convert_partition(int* a, std::vector<__uint128_t>& partition, int n){
    __uint128_t element = 1;
    // Loop over all variables
    for (int i = 0; i < n; ++i){
            // The element a[i] is the index of the component to which variable i belongs
            partition[a[i]] += element;
            // Bitshift to the left to get the integer value of the next variable
            element <<= 1;
        }
}

/**
 * Prints the partition as bitstrings of the non-empty components to a file.
 * 
 * @param[in] file              Path to the file where the partition should be written.
 * @param[in] partition         Partition as a vector of n integers representing the components.
 * 
 * @return void                 Nothing is returned by this function.
 */
void print_partition_to_file(std::ofstream& file, std::vector<__uint128_t>& partition){
    // Number of variables
    int n = partition.size();
    // Counter for the component number
    int i = 0;
    for (__uint128_t component : partition){
        // Ignore empty component
        if (! component){continue;}
        file << "Component " << i << " : " << component_as_string(component, n) << '\n';
        ++i;
    }
}

/**
 * Prints the partition as bitstrings of the non-empty components to the terminal.
 * 
 * @param[in] partition         Partition as a vector of n integers representing the components.
 * 
 * @return void                 Nothing is returned by this function.
 */
void print_partition_to_terminal(std::vector<__uint128_t>& partition){
    // Number of variables
    int n = partition.size();
    // Counter for the component number
    int i = 0;
    for (__uint128_t component : partition){
        // Ignore empty component
        if (! component){continue;}
        std::cout << "Component " << i << " : " << component_as_string(component, n) << '\n';
        ++i;
    }
    std::cout << '\n';
}
