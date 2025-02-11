#pragma once

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <map>
#include <float.h>
#include <bitset>
#include <cmath>
#include <algorithm>

/**
 * Representation of the characteristics of a Minimally Complex Model
 * 
 * @struct mcm
 * 
 * @var mcm::data
 *  The dataset
 * 
 * @var mcm::n
 *  Number of variables in the system
 * 
 * @var mcm::N
 *  Number of observations in the dataset
 * 
 * @var mcm::q
 *  Number of states
 * 
 * @var mcm::n_ints
 *  Number of 128bit integers necessary to represent the data
 * 
 * @var mcm::pow_q
 *  Array to store the first n powers of q
 * 
 * @var mcm::exhaustive
 *  Boolean to indicate if an exhaustive search will be done
 * 
 * @var mcm::evidence_storage_es
 *  Vector to store the calculated log evidence of components during an exhaustive search 
 * 
 * @var mcm::evidence_storage
 *  Map to store the calculated log evidence of components during non-exhaustive search algorithm
 * 
 * @var mcm::best_basis
 *  Vector containing n independent operators with the lowest entropy
 * 
 * @var mcm::best_mcm
 *  Vector containing the best partition(s)
 * 
 * @var mcm::best_evidence
 *  The log evidence of the partition(s) found by the search algorithm
 * 
 * @var mcm::log_file
 *  Boolean to indicate if the intermediate steps of the search algorithms should be written to a file
 * 
 * @var mcm::greedy_file
 *  Filename for the intermediate steps of the greedy search algorithm
 * 
 * @var mcm::divide_and_conquer_file
 *  Filename for the intermediate steps of the divide and conquer procedure
 * 
 * @var mcm::store_all_ev
 *  Boolean to indicate if the log evidence of all partitions encounterd during the exhaustive search should be stored
 * 
 * @var mcm::all_evidences
 *  Vector with the log evidence of all partitions encounterd during the exhaustive search
 */
struct mcm {
    std::vector<std::vector<__uint128_t>> data;
    int n;
    int N;
    int q;
    int n_ints;
    // Precomputing the first n powers of q speed up the calculation of the evidence (q^r)
    std::vector<__uint128_t> pow_q;

    bool exhaustive;
    // Store calculated log evidence in a vector when performing an exhaustive search (faster acces compared to a map)
    // Integer representation of a component is the index
    // Efficient storage because evidence of all (2^n -1) ICCs will be calculated
    std::vector<double> evidence_storage_es;

    // Store in map otherwise because not every ICC will occur (better memory efficiency)
    std::map<__uint128_t, double> evidence_storage;

    std::vector<std::vector<__uint128_t>> best_basis;
    // Store the best partition in a vector in case there are multiple partitions with the same log evidence
    // Only the exhaustive search can find multiple partitions with the same log evidence because it goes through all of them
    // In case of the a greedy search or a divide and conquer process, the resulting partition will be the only element in the vector
    std::vector<std::vector<__uint128_t>> best_mcm;
    double best_evidence = -DBL_MAX;

    // Output file to store search steps in the greedy search or divide and conquer procedure
    bool log_file;
    std::ofstream* greedy_file;
    std::ofstream* divide_and_conquer_file;

    // Store evidence of all partitions (useful when creating distribution of the evidence of all partition during an analysis)
    bool store_all_ev = false;
    std::vector<double> all_evidence;
};

// Function in model.cpp
mcm create_model(int q, int n, bool log_file);

// Functions in data.cpp
std::vector<std::vector<__uint128_t>> data_processing(std::string file, int n, int n_ints);
void convert_observation(std::vector<__uint128_t>& obs, std::string& raw_obs, int n);

// Function in evidence.cpp
std::map<std::vector<__uint128_t>, unsigned int> count_observations(mcm& model, __uint128_t component);
double get_evidence_icc(__uint128_t component, mcm& model);
double calc_evidence_icc(__uint128_t component, mcm& model, int r);
double calc_evidence(std::vector<__uint128_t>& partition, mcm& model);

// Functions in partition.cpp
std::string component_as_string(__uint128_t component, int n);
int component_size(__uint128_t component);
void convert_partition(int* a, std::vector<__uint128_t>& partition, int n);
void print_partition_to_terminal(std::vector<__uint128_t>& partition);
void print_partition_to_file(std::ofstream& file, std::vector<__uint128_t>& partition);

// Functions in spin_op.cpp
int count_set_bits(__uint128_t value);
std::vector<__uint128_t> convert_representation(std::vector<int>& a, int n, int n_ints);
int spin_value(std::vector<__uint128_t>& state, std::vector<__uint128_t>& op, int q, int n_ints);
double entropy(std::vector<double>& prob_distr);
double entropy_of_op(std::vector<std::vector<__uint128_t>>& data, std::vector<__uint128_t>& op, int q, int n_ints);

// Functions in gauge_transform.cpp
void gt_state(std::vector<__uint128_t>& state, std::vector<std::vector<__uint128_t>>& gt, int q, int n, int n_ints);
void transform_data(std::vector<std::vector<__uint128_t>>& data, std::vector<std::vector<__uint128_t>>& gt, int q, int n, int n_ints);
void sort_operators(mcm& model, std::vector<std::vector<__uint128_t>>& sorted_ops, unsigned int max_order=0);
bool comp_entropy(std::pair<std::vector<int>, double>& op1, std::pair<std::vector<int>, double>& op2);
void construct_matrix(std::vector<std::vector<unsigned int>>& matrix, std::vector<std::vector<int>>& ops, __uint128_t n_ops, int q, int n);
void find_best_basis(mcm& model, unsigned int max_order=0);

