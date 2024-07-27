#include "model.h"

/**
 * Function to create a struct representing the characteristics of the model.
 * 
 * @param q                     Number of states.
 * @param n                     Number of variables in the system.
 * @param log_file              Boolean to indicate if the search steps should be written to a file
 * 
 * @return The struct representing the characteristics of the model.
 */
mcm create_model(int q, int n, bool log_file){
    // Create the struct
    mcm model;
    // Number of variables
    model.n = n;
    // Number of states
    model.q = q;
    // Precompute the powers of q
    model.pow_q.assign(n+1, 0);
    __uint128_t element = 1;
    for(int i = 0; i < n+1; i++){
        model.pow_q[i] = element;
        element *= q;
    }
    // Determine the number of integers necessary to represent the data
    model.n_ints = ceil(log2(q));
    // Indicate if search steps must be written to log file
    model.log_file = log_file;
    
    return model;
}