#include "model.h"

/**
 * Counts all the different observations in the dataset for a given component.
 * 
 * @param[in] model             Struct containing the characteristic of the model. 
 * @param component             Integer representation of the bitstring representing a component.
 * 
 * @return Distribution of the states that occur as a map.   
 */
std::map<std::vector<__uint128_t>, unsigned int> count_observations(mcm& model, __uint128_t component){
    // Map to keep track of the frequency of the observations
    std::map<std::vector<__uint128_t>, unsigned int> counts;
    std::vector<__uint128_t> state(model.n_ints);
    // Loop over the entire dataset
    for (const std::vector<__uint128_t>& obs : model.data){
        // Bitwise AND to extract the substring corresponding to the component
        for (int i = 0; i < model.n_ints; ++i){
            state[i] = obs[i] & component;
        }
        // Increase frequency of the state by 1
        counts[state] += 1;   
    }
    return counts;
}

/**
 * Stores and returns the log evidence of a given component.
 * 
 * @param component             Integer representation of the bitstring representing a component.
 * @param[in] model             Struct containing the characteristic of the model. 
 * 
 * @return Log evidence of the component   
 */
double get_evidence_icc(__uint128_t component, mcm& model){
    double log_evidence;
    // Check if it evidence for this component is already calculated
    if (!model.exhaustive){
        // Greedy search or divide and conquer -> Search in the storage, which is a map
        std::map<__uint128_t, double>::iterator result = model.evidence_storage.find(component);
        if (result == model.evidence_storage.end()){
            // Not found -> needs to be calculated
            int r = component_size(component);
            log_evidence = calc_evidence_icc(component, model, r);
            // Store the result
            model.evidence_storage[component] = log_evidence;
        }
        else{
            // Retrieve value from storage
            log_evidence = model.evidence_storage[component];
        }
    }
    else{
        // Exhaustive search -> Search for value in storage, which is a vector
        log_evidence = model.evidence_storage_es[component-1];
        if (log_evidence == 0){
            // Not found -> needs to be calculated
            int r = component_size(component);
            log_evidence = calc_evidence_icc(component, model, r);
            // Store the result
            model.evidence_storage_es[component-1] = log_evidence;
        }
    }
    return log_evidence;
}

/**
 * Calculates the log evidence of a given component.
 * 
 * @param component             Integer representation of the bitstring representing a component.
 * @param[in] model             Struct containing the characteristic of the model. 
 * @param r                     Size of the component.
 * 
 * @return Log evidence of the component
 */
double calc_evidence_icc(__uint128_t component, mcm& model, int r){
    double log_evidence = 0;
    // Contributions from the different observations
    std::map<std::vector<__uint128_t>, unsigned int> counts = count_observations(model, component);
    std::map<std::vector<__uint128_t>, unsigned int>::iterator count_iter = counts.begin();
    while (count_iter != counts.end()){
        log_evidence += (lgamma(count_iter->second + 0.5) - 0.5 * log(M_PI));
        ++count_iter;
    }

    // Calculate prefactor
    if (r > 25){
        // Approximate for large components because lgamma overflows
        log_evidence -= (r * log(model.q) * model.N);
    }
    else{
        log_evidence += lgamma(model.pow_q[r]/2.) - lgamma(model.N + model.pow_q[r]/2.);
    }
    
    return log_evidence;
}

/**
 * Calculates the log evidence of a given partition.
 * 
 * @param partition             Partition as a vector of n integers representing the components.
 * @param[in] model             Struct containing the characteristic of the model. 
 * @param r                     Size of the component.
 * 
 * @return Log evidence of the partition
 */
double calc_evidence(std::vector<__uint128_t>& partition, mcm& model){
    double log_evidence = 0;
    // Iterate over all the ICCs in the partition
    for (__uint128_t component : partition){
        // Calculate the evidencen of the ICC if it is non-empty
        if (component){
            log_evidence += get_evidence_icc(component, model);
        }
    }
    return log_evidence;
}
