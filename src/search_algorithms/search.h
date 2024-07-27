#include "../model/model.h"

// Search algorithms
void exhaustive_search(mcm& model);
void greedy_search(mcm& model);
void divide_and_conquer(mcm&model);

// Helper functions for exhaustive search
int find_j(int* a, int* b, int n);
int generate_next_partition(int* a, int* b, int n);

// Helper functions for divide and conquer
int division(int move_from, int move_to, mcm& model);
int find_member_i(__uint128_t community, int i);
int index_of_member(int member);
