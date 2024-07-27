#include "search.h"

/**
 * Performs an exhaustive search to find the best partition.
 * 
 * @param[in, out] model        Struct containing the characteristic of the model. 
 *                              -'best_mcm' will contain all partitions with the largest evidence found by the algorithm.
 *                              -'evidence' will be the evidence of the partition(s) found by the algorithm.
 * 
 * @return void                 Nothing is returned by this function.
 */
void exhaustive_search(mcm& model){
    // Reset best mcm in case another search has been ran previously
    model.best_mcm.clear();
    
    // Indicate that this is not an exhaustive search
    // Necessary because different data structure is used to store the evidence of ICCs
    model.exhaustive = true;
    // Reserve memory for storage of evidence of icc (2^n - 1 iccs) -> store in a vector because will encounter all of them in an exhaustive search
    __uint128_t n_iccs = 1 << model.n;
    model.evidence_storage_es.assign(n_iccs-1, 0);

    // Variable to store the log evidence
    double log_evidence;
    // Variable to keep track of the best partition
    std::vector<__uint128_t> best_mcm;
    // Representation of the partition that can used by the function to calculate the evidence
    std::vector<__uint128_t> partition(model.n, 0);
    // Initialize arrays to keep track of the next partition to generate
    int a[model.n];
    int b[model.n];
    for (int i = 0; i < model.n; i++){
        a[i] = 0;
        b[i] = 1;
    }
    // Index of first value that is different (from right to left) between a and b
    int j = model.n - 1;

    while (j != 0){
        // Partition is written as a restricted growth string -> convert it (updates 'partition')
        convert_partition(a, partition, model.n);
        log_evidence = calc_evidence(partition, model);
        // Check if this is equal to the best log evidence found so far
        if (abs(log_evidence - model.best_evidence) < 1E-6){
            // Found MCM with the same evidence
            // Make a hard copy of current partition to store
            best_mcm.assign(partition.begin(), partition.end());
            model.best_mcm.push_back(best_mcm);
        }
        // Check if this is the new best log evidence
        else if (log_evidence > model.best_evidence){
            // Update the current best
            model.best_evidence = log_evidence;
            // Remove all current best MCMs
            model.best_mcm.clear();
            // Make a hard copy of current partition to store
            best_mcm.assign(partition.begin(), partition.end());
            model.best_mcm.push_back(best_mcm);
        }

        // Store evidence of all partitions
        if (model.store_all_ev){
            model.all_evidence.push_back(log_evidence);
        }

        j = generate_next_partition(a, b, model.n);
        if (j == 0){
            // All possible partitions are generated
            break;
        }
        // Reset the converted partition
        std::fill(partition.begin(), partition.end(), 0);
    }
}

/**
 * Helper function for the exhaustive search that updates the partition.
 * 
 * @param[in, out] a            Array of size n that represents the partition as a restricted growth string.
 * @param[in, out] b            Array of size n that keeps track of how many partitions each variable can move to.
 * @param n                     Number of variables.
 * 
 * @return 1 if next partition is generated, 0 if all partitions are generated.
 */
int generate_next_partition(int* a, int* b, int n){
    // Compare the last bit
    if (a[n-1] != b[n-1]){
        // Increase the last bit of 'a' by 1 to generate new partition
        a[n-1] += 1;
        return 1;
    }
    // Find the first bit that is different (starting from the right)
    int j = find_j(a, b, n);
    if (j == 0){
        // All bits are the same -> all possible partitions are generated
        return 0;
    }
    // Increase the first bit from the right in 'a' that is different from 'b' by 1
    a[j] += 1;
    if (a[j] == b[j]){
        // Change the number of partitions the next member is allowed to be in
        b[j+1] = b[j] + 1;
    }
    // Set values for all member to the right
    int i = j+1;
    while (i < n){
        // Move variable back to the first component
        a[i] = 0;
        // Change the number of components this member is allowed to be in
        b[i] = b[j+1];
        ++i;
    }
    return 1;
}
/**
 * Helper function for generating partition that updates the partition.
 * 
 * @param[in] a                 Array of size n.
 * @param[in] b                 Array of size n.
 * @param n                     size of the arrays.
 * 
 * @return Index of the first bit from right to left that is different between a and b.
 */
int find_j(int* a, int* b, int n){
    int j = n-2;
    while(a[j] == b[j]){--j;}
    return j;
}