#include "search.h"

/**
 * Performs a divide and conquer procedure to find an estimation of the best partition.
 * 
 * @param[in, out] model        Struct containing the characteristic of the model. 
 *                              -'best_mcm[0]' will contain the partition with the largest evidence found by the algorithm.
 *                              -'evidence' will be the evidence of the partition found by the algorithm.
 * 
 * @return void                 Nothing is returned by this function.
 */
void divide_and_conquer(mcm&model){
    // Reset best mcm adn storage in case another search has been ran previously
    model.best_mcm.clear();
    model.evidence_storage.clear();

    // Indicate that this is not an exhaustive search
    // Necessary because different data structure is used to store the evidence of ICCs
    model.exhaustive = false;

    // Start from complete model (1 component of size n)
    std::vector<__uint128_t> partition(model.n, 0);
    __uint128_t element = 1;
    for (int i = 0; i < model.n; i++){
        // Put every variable in first component
        partition[0] += element;
        element <<= 1;
    }
    // Store the starting partition because it will be updated during the recursive process
    model.best_mcm.push_back(partition);

    // Write to file
    if(model.log_file){
        *model.divide_and_conquer_file << "Start divide and conquer procedure" << std::endl;
    }

    // Start recursive algorithm by moving variables from component 0 to component 1
    division(0, 1, model);

    // Calculate the evidence of the best MCM found by the search algorithm
    model.best_evidence = calc_evidence(model.best_mcm[0], model);
}

/**
 * Recursive division procedure used during the divide and conquer algorithm.
 * 
 * @param move_from             Index of the component from which variables are moved
 * @param move_to               Index of the component to which variables are moved
 * @param[in, out] model        Struct containing the characteristic of the model.
 *                              -'best_mcm[0]' will be updated if a split occurs that increases the evidence.
 * 
 * @return Index of the next empty component
 */

int division(int move_from, int move_to, mcm& model){
    // Number of member in the component that we want to split
    int n_members_1 = component_size(model.best_mcm[0][move_from]);
    // If the component contains 1 variable, no further splits are possible
    if (n_members_1 == 1){return move_to;}

    // Hard copy of the starting partition
    std::vector<__uint128_t> partition = model.best_mcm[0];

    // Variables for the difference in evidence before and after split
    double best_evidence_diff = 0;
    double best_evidence_diff_tmp;
    double evidence_diff;

    // Variables to represent the unsplit and split components
    __uint128_t component_1;
    __uint128_t component_2;

    // Variable to indicate which member is moving
    int member;

    // Calculate the evidence of the component before splitting (reference point for the difference in evidence)
    double evidence_unsplit_component = get_evidence_icc(partition[move_from], model);

    // If the component has more than 2 members, we can skip the last step because it is the same as the first step
    if (n_members_1 > 2){n_members_1 -= 1;}
    
    while (n_members_1 > 1){
        // Initial values
        best_evidence_diff_tmp = -DBL_MAX; 
        component_1 = partition[move_from];
        component_2 = partition[move_to];

        // Write to file
        if(model.log_file){
            *model.divide_and_conquer_file << "\nStart moving variables from component " << move_from << " to component " << move_to << std::endl;
            print_partition_to_file(*model.divide_and_conquer_file, partition);
        }

        // Move each variable sequentially to component 'move_to'
        for (int i = 0; i <= n_members_1; i++){
            // Integer representation of the bitstring with only a 1 in the position of the (i+1)th bit set to 1 in component
            member = find_member_i(component_1, i+1);
            component_1 -= member;
            component_2 += member;

            // Calculate difference in evidence from splitting
            evidence_diff = get_evidence_icc(component_1, model) + get_evidence_icc(component_2, model) - evidence_unsplit_component;

            // Check if this difference is the best one so far (even if negative)
            if (evidence_diff > best_evidence_diff_tmp){
                // Update the temporary best difference
                best_evidence_diff_tmp = evidence_diff;
                // Update partition
                partition[move_from] = component_1;
                partition[move_to] = component_2;

                // Write to file
                if(model.log_file){
                    *model.divide_and_conquer_file << "\nBest intermediate split: moving variable " << index_of_member(member) << " from component " << move_from << " to component " << move_to << " Evidence difference: " << best_evidence_diff_tmp << std::endl;
                    print_partition_to_file(*model.divide_and_conquer_file, partition);
                }
            }

            // Move variable back to component 'move_from'
            component_1 += member;
            component_2 -= member;
        }
        // Check if the split results in an overall improvement of the evidence
        if (best_evidence_diff_tmp > best_evidence_diff){
            // Update the best difference
            best_evidence_diff = best_evidence_diff_tmp;
            // Update the best MCM
            model.best_mcm[0][move_from] = partition[move_from];
            model.best_mcm[0][move_to] = partition[move_to];

            // Write to file
            if(model.log_file){
                *model.divide_and_conquer_file << "\nNew best split" << std::endl;
                print_partition_to_file(*model.divide_and_conquer_file, partition);
            }
        }
        // Update number of members
        n_members_1 -= 1;
    }
    // Stop if no split increased the evidence -> component 'move_to' will be empty in that case
    if (model.best_mcm[0][move_to] == 0){
        return move_to;
    }
    // If there was a succesful split, component 'move_to' is no longer empty -> increase the index of the first empty component
    int first_empty = move_to + 1;
    // Continue with a split of the first subpart
    first_empty = division(move_from, first_empty, model);
    // Continue with a split of the second subpart
    first_empty = division(move_to, first_empty, model);

    return first_empty;
}

/**
 * Helper function for the division procedure that finds the integer representation of the bitstring with only a 1 in the position of the ith bit set to 1 in component.
 * 
 * @param component             Integer representation of the bitstring representing a component.
 * @param i                     Index of the bit to be found.
 * 
 * @return Integer representation of the bitstring with only a 1 in the position of the ith bit set to 1 in component.
 */
int find_member_i(__uint128_t component, int i){
    // Counter to keep track of the number of bits set to 1
    int counter = 0;
    // Starting value: integer representation of the bitstring with only the least significant bit set to 1
    int position = 1;
    // Continue as long as 'component' has bits set to 1
    while(component){
        // Check if the least significant bit is '1'
        if(component & 1){
            // Increase the counter of the number of bits set to 1
            counter += 1;
        }
        // Check if this is the ith bit set to 1
        if (counter == i){
            return position;
        }
        // Bitshift to the right to check the next bit
        component >>= 1;
        // Update the position with a bitshift to the left
        position <<= 1;
    }
    return position;
}

/**
 * Helper function when printing the steps of the algorithm to a file that locates the position of the (first) bit set to 1.
 * 
 * @param member                Integer representation of a bitstring
 * 
 * @return Index of the least significant bit set to 1.
 */
int index_of_member(int member){
    // Starting index
    int i=0;
    // Continue as long as the least significant bit is not a 1
    while((member & 1) == false){
        // Bitshift to the right
        member >>= 1;
        // Update the index
        i++;
    }
    return i;
}