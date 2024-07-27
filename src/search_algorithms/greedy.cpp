#include "search.h"

/**
 * Performs a greedy search to find an estimation of the best partition.
 * 
 * @param[in, out] model        Struct containing the characteristic of the model. 
 *                              -'best_mcm[0]' will contain the partition with the largest evidence found by the algorithm.
 *                              -'evidence' will be the evidence of the partition found by the algorithm.
 * 
 * @return void                 Nothing is returned by this function.
 */
void greedy_search(mcm& model){
    // Reset best mcm in case another search has been ran previously
    model.best_mcm.clear();
    
    // Indicate that this is not an exhaustive search
    // Necessary because different data structure is used to store the evidence of ICCs
    model.exhaustive = false;

    // Start from the independent model (n components of size 1)
    std::vector<__uint128_t> partition(model.n, 0);
    __uint128_t element = 1;
    for (int i = 0; i < model.n; i++){
        partition[i] += element;
        element <<= 1;
    }

    // Write to file
    if(model.log_file){
        *model.greedy_file << "Start greedy merging procedure \n" << std::endl;
        print_partition_to_file(*model.greedy_file, partition);
    }

    // Variables to store the calculated evidences
    double evidence_i;
    double evidence_j;
    double evidence_diff;

    // Variables to store the best values
    int best_i;
    int best_j;
    double best_evidence_diff;

    while (true){
        best_evidence_diff = 0;
        for (int i = 0; i < model.n; i++){
            // Skip empty components
            if (partition[i] == 0){continue;}
            evidence_i = get_evidence_icc(partition[i], model);
            for (int j = i+1; j < model.n; j++){
                // Skip empty components
                if (partition[j] == 0){continue;}
                evidence_j = get_evidence_icc(partition[j], model);
                // Calculate difference in evidence between merged and separate partitions
                evidence_diff = get_evidence_icc(partition[i] + partition[j], model) - evidence_i - evidence_j;
                // Check if the difference is the best merge so far
                if (evidence_diff > best_evidence_diff){
                    // Update the largest increase in evidence
                    best_evidence_diff = evidence_diff;
                    // Keep track of the components that resulted in the largest increase
                    best_i = i;
                    best_j = j;
                }
            }
        }
        // Stop the algorithm if the evidence did not increase
        if (best_evidence_diff == 0){
            break;
        }
        else{
            // Merge the two components that results in the biggest increase in evidence
            partition[best_i] += partition[best_j];
            partition[best_j] = 0;

            // Write to file
            if(model.log_file){
                *model.greedy_file << "\nMerging componets " << best_i << " and " << best_j << " Evidence difference: "<<  best_evidence_diff << std::endl;
                print_partition_to_file(*model.greedy_file, partition);
            }
        }
    }
    // Store the best MCM and corresponding log evidence found using the greedy merging scheme
    model.best_mcm.push_back(partition);
    model.best_evidence = calc_evidence(partition, model);
}