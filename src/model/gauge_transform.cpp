#include "model.h"

/**
 * Calculates the greatest common divisor of two integers.
 * 
 * @param a                     Integer 1
 * @param b                     Integer 2
 * 
 * @return The greatest common divisor ob integer 1 and integer 2.
 */
int gcd(int a, int b){
    if (a == 0){
        return b;
    }
    return gcd(b % a, a);
}

/**
 * Perform a gauge transformation of a single state.
 * 
 * @param[in, out] state        A state represented as n_ints 128bit integers.
 * @param[in] gt                Vector of operators represented as n_ints 128bit integers that form the new basis variables.
 * @param q                     Number of values a single variable can take.
 * @param n                     Number of variables in the system.
 * @param n_ints                Number of 128bit integers
 * 
 * @return void                 Nothing is returned by this function 
 */
void gt_state(std::vector<__uint128_t>& state, std::vector<std::vector<__uint128_t>>& gt, int q, int n, int n_ints){
    // Variable for transformed state
    std::vector<int> new_state(n);
    for (int i = 0; i < n; ++i){
        // Determine the spin value for each new variable
        new_state[i] = spin_value(state, gt[i], q, n_ints);
    }
    state = convert_representation(new_state, n, n_ints);
}

/**
 * Perform a gauge transformation of the entire dataset.
 * 
 * @param[in, out] data         Dataset containing all observations represented as n_ints 128bit integers.
 * @param[in] gt                Vector of operators represented as n_ints 128bit integers that form the new basis variables.
 * @param q                     Number of values a single variable can take.
 * @param n                     Number of variables in the system.
 * @param n_ints                Number of 128bit integers
 * 
 * @return void                 Nothing is returned by this function 
 */
void transform_data(std::vector<std::vector<__uint128_t>>& data, std::vector<std::vector<__uint128_t>>& gt, int q, int n, int n_ints){
    // Loop over all observations and perform a gauge transformation on each observation
    for (std::vector<__uint128_t>& obs : data){
        gt_state(obs, gt, q, n, n_ints);
    }
}

/**
 * Sorts spin operators from low to high entropy.
 * 
 * @param[in] model             Struct containing the characteristic of the model.
 * @param[in, out] sorted_ops   Empty vector to store all operators as vectors with n values between 0 and q-1.
 * @param max_order             Maximum interaction order of the operators that should be considered, default value (0) considers all q^n-1 operators.
 * 
 * @return void                 Nothing is returned by this function.
 */
void sort_operators(mcm& model, std::vector<std::vector<int>>& sorted_ops, unsigned int max_order=0){
    // Variable to indicate if all operators are generated
    bool all_ops_generated = false;
    // Variable to indicate if operator is valid (doesn't reduce state space, ex. s^2 for q=4)
    bool valid;
    // Default max_order is n (all operators)
    if (max_order == 0){
        max_order = model.n;
    }
    // Calculate and store the entropy of all operators
    std::vector<std::pair<std::vector<int>, double>> entropy_of_ops;
    std::pair<std::vector<int>, double> entropy;

    // Array to keep track of the bitstring representation of the operator
    std::vector<int> a(model.n, 0);

    int a_k;
    int order = 0;
    int leading_bit = 0;
    std::vector<__uint128_t> op;
    while (!all_ops_generated){
        // Increase first bit
        a[0] += 1;

        // Check if max value for a bit is reached
        for (int j = 0; j < model.n; ++j){
            if  (a[j] == model.q){
                // Max value for current bit is reached
                if (j == model.n -1){
                    all_ops_generated = true;
                }
                else{
                    // Increase next bit by 1, set current bit to zero and decrease interaction order
                    ++a[j + 1];
                    a[j] = 0;
                    --order;
                }
            }
            else{
                // Max value not reached
                // Increase order if current bit went from zero to one and stop checking next bits
                if (a[j] == 1){
                    ++order;
                }
                break;
            }
        }
        // Exclude spin operators with interaction order above max_order
        if (order <= max_order){
            valid = false;
            for (int k = 0; k < model.n; ++k){
                a_k = a[k];
                if (a_k){
                    // Determine if value is coprime with q (gcd = 1)
                    int val = gcd(a_k, model.q);
                    if (gcd(a_k, model.q) == 1){
                        valid = true;
                        break;
                    }
                }
            }

            // Check the leading bit to exclude the second operator in a conjugate pair
            if (valid and a_k <= - a_k + model.q){
                // Store the representation with n values between 0 and q-1 because these will be the columns of the matrix
                entropy.first = a;
                // Calculate the entropy
                op = convert_representation(a, model.n, model.n_ints);
                entropy.second = entropy_of_op(model.data, op, model.q, model.n_ints);
                entropy_of_ops.push_back(entropy);
            }
        }
    }

    // Sort the operators based on entropy from low to high
    sort(entropy_of_ops.begin(), entropy_of_ops.end(), comp_entropy);
    for (std::pair<std::vector<int>, double>& pair : entropy_of_ops){
        sorted_ops.push_back(pair.first);
    }
}

/**
 * Comparing function based on the entropy used to sort the operators.
 * 
 * @param[in] op1                   Pair containing operator 1 and its entropy.
 * @param[in] op2                   Pair containing operator 2 and its entropy.
 * 
 * @return True if entropy of the first operator is lower, false otherwise.
 */
bool comp_entropy(std::pair<std::vector<int>, double>& op1, std::pair<std::vector<int>, double>& op2){
    return op1.second < op2.second;
}

/**
 * Creates the matrix with the operators sorted from low to high entropy as its columns.
 * 
 * @param[in, out] matrix       Matrix to fill in the values of the operators.
 * @param[in] ops               Vector with all the operators as vectors with n values between 0 and q-1.
 * @param n_ops                 Number of operators that we consider (for finding the best basis).
 * @param n                     Number of variables in the system (number of rows in the matrix).
 * 
 * @return void                 Nothing is returned by this function.
 */
void construct_matrix(std::vector<std::vector<unsigned int>>& matrix, std::vector<std::vector<int>>& ops, __uint128_t n_ops, int q, int n){
    std::vector<int> op_j;
    int i;
    for (__uint128_t j = 0; j < n_ops; ++j){
        op_j = ops[j];
        for (int i = 0; i < n; ++i){
            matrix[i][j] = op_j[i];
        }
    }   
}

/**
 * Finds the operators that form the best basis.
 * 
 * @param[in, out] model        Struct containing the characteristic of the model.
 *                              -'best_basis' will contain the n operators that form the best basis.
 * @param max_order             Maximum interaction order of the operators that should be considered, default value (0) considers all q^n-1 operators.
 * 
 * @return void                 Nothing is returned by this function.
 */
void find_best_basis(mcm& model, unsigned int max_order){
    model.best_basis.clear();
    // Sort all operators from low to high entropy
    std::vector<std::vector<int>> sorted_ops;
    sort_operators(model, sorted_ops, max_order);
    __uint128_t n_ops = sorted_ops.size();

    // Construct the matrix with all operators as columns
    std::vector<std::vector<unsigned int>> matrix(model.n, std::vector<unsigned int>(n_ops));
    construct_matrix(matrix, sorted_ops, n_ops, model.q, model.n);

    // Find the first n most biased independent operators using Gaussian elimination (add mod q) of the matrix
    int pivot_value, value, factor;
    int row, col = 0;
    // Loop over the rows
    for (int i = 0; i < model.n; ++i){
        row = i;
        // Find the first non-zero element
        while (true){
            if (matrix[row][col] == 0){
                row += 1;
                if (row == model.n){
                    // No non zero element in this column (linearly dependent on previous columns)
                    // Move to the next column and start at the ith row again
                    row = i;
                    col += 1;
                }

                if (col == n_ops){
                    // All columns have been checked -> search finished
                    return;
                }
            }
            else{
                // If q is nonprime, column can be linearly dependent even if value is nonzero
                pivot_value = matrix[row][col];
                value = gcd(pivot_value, model.q);
                if (value == 1){
                    // Power is coprime with q -> valid pivot value
                    break;
                }
                // Multiple of the power is 0 mod q -> put a zero on this place and check the column again
                value = model.q / value;
                for (int j = 0; j < model.n; ++j){
                    matrix[j][col] = (matrix[j][col] * value) % model.q;
                }
            }
        }
        // If non-zero element was not the first element, swap rows
        if (row != i){
            std::vector<unsigned int> tmp = matrix[row];
            matrix[row] = matrix[i];
            matrix[i] = tmp;
            row = i;
        }
        // Put all elements in rows below to zero by adding multiple of current row
        for (int j = row+1; j < model.n; ++j){
            // Check how many times we have to add pivot row to current row
            value = matrix[j][col];
            factor = 0;
            while (value % model.q){
                value += pivot_value;
                ++factor;
            }
            if (factor){
                // (factor * pivot row  + current row) mod q
                for (int k = col; k < n_ops; ++k){
                    matrix[j][k] = (matrix[j][k] + factor * matrix[row][k]) % model.q;
                }
            }
        }
        // Add operator in current column to the basis
        model.best_basis.push_back(convert_representation(sorted_ops[col], model.n, model.n_ints));
        ++col;
    }
}
