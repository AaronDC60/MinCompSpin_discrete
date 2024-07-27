#include "model.h"

/**
 * Counts the number of bits set to 1 in a given bitstring.
 * 
 * @param value                 Integer representation of a bitstring.
 * 
 * @return The number of bits set to 1.
 */
int count_set_bits(__uint128_t value){
    int count = 0;
    while (value){
        // Bitwise AND between n and n-1
        value &= (value - 1);
        ++count;
    }
    return count;
}

/**
 * Converts the representation of a vector of size n with value ranging 0 to q-1 to a vector of n_ints 128bit integers.
 * 
 * @param[in] a                 Vector of size n with value ranging 0 to q-1.
 * @param n                     Size of the array a.
 * @param n_ints                Number of 128bit integers necessary in the new representation.
 * 
 * @return Vector with n_int 128bit integers representing the state or operator a.
 */
std::vector<__uint128_t> convert_representation(std::vector<int>& a, int n, int n_ints){
    std::vector<__uint128_t> new_representation(n_ints,0);
    __uint128_t element = 1;
    // Loop over the components
    for (int i = 0; i < n; ++i){
        int value = a[i];
        int bit = 0;
        while (value){
            // Check if last bit in the binary representation is nonzero
            if (value & 1){
                new_representation[bit] += element;
            }
            // Bitshift to the right
            ++bit;
            value >>= 1;
        }
        // Bitshift to the left to get the integer value of the next component
        element <<= 1;
    }
    return new_representation;
}

/**
 * Determines the spin value of a given operator for a given state.
 * 
 * @param[in] state             A state represented as n_ints 128bit integers.
 * @param[in] op                An operator represented as n_ints 128bit integers.
 * @param q                     Number of values a single variable can take.
 * @param n_ints                Number of 128bit integers
 * 
 * @return The spin value of the operator for the given state.
 */
int spin_value(std::vector<__uint128_t>& state, std::vector<__uint128_t>& op, int q, int n_ints){
    // s = sum(alpha_j * mu_j) mod q
    int spin = 0;
    int element_j = 1;
    // Read state from left to right (s1 is the first value in the vector)
    for (int j = 0; j < n_ints; ++j){
        int element_i = 1;
        // Read the operator from left to right
        for (int i = 0; i < n_ints; ++i){
            spin += element_j * element_i * count_set_bits(op[j] & state[i]);
            // Bitshift to the left to get the integer value of the variable (in the operator)
            element_i <<= 1;
        }
        // Bitshift to the left to get the integer value of the variable (in the state)
        element_j <<= 1;
    }
    return spin % q;
}

/**
 * Calculates the entropy of a probability distribution.
 * 
 * @param[in] prob_distr    Vector with values between 0 and 1 representing the probability distribution .
 * 
 * @return Entropy of the distribution.
 */
double entropy(std::vector<double>& prob_distr){
    // H(x) = - sum [p(x) log(px)]
    double entropy = 0;
    for (double p : prob_distr){
        // Ignore p == 0 because assume 0 log 0 = 0
        if (p){
            entropy -= (p * log2(p));
        }
    }
    return entropy;
}

/**
 * Calculates the entropy of a spin operator for a given dataset.
 * 
 * @param[in] data              Dataset containing observations as n_ints 128bit integers.
 * @param[in] op                An operator represented as n_ints 128bit integers.
 * @param q                     Number of values a single variable can take.
 * @param n_ints                Number of 128bit integers
 * 
 * @return The entropy of the operator.
 */
double entropy_of_op(std::vector<std::vector<__uint128_t>>& data, std::vector<__uint128_t>& op, int q, int n_ints){
    // Variable for probability distribution (# entries = # spin values/states)
    std::vector<double> prob_distr(q, 0);

    int s;
    for (std::vector<__uint128_t>& obs : data){
        // Determine the value of the spin operator for a given observation
        s = spin_value(obs, op, q, n_ints);
        // Increase the number of occurences of that spin value by 1
        prob_distr[s] += 1; 
    }

    // Normalize the distribution
    int N = data.size();
    for (int i = 0; i < q; ++i){
        prob_distr[i] /= N;
    }

    // Calculate the entropy
    return entropy(prob_distr);
}
