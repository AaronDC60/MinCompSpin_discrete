#include "gtest/gtest.h"
#include "../src/model/model.h"
#include "../src/search_algorithms/search.h"

TEST(evidence, count_obs){
    // Read in test data + create model
    mcm model = create_model(3, 3, false);
    std::vector<std::vector<__uint128_t>> data = data_processing("../tests/test.dat", 3, model.n_ints);
    model.data = data;
    model.N = data.size();


    std::map<std::vector<__uint128_t>, unsigned int> counts;
    std::vector<__uint128_t> key = {0,0};
    // Component = 1
    counts = count_observations(model, 1);
    EXPECT_EQ(counts.size(), 3);
    EXPECT_EQ(counts[key], 1);
    key = {1,0};
    EXPECT_EQ(counts[key], 2);
    key = {0,1};
    EXPECT_EQ(counts[key], 4);
    
    // Component = 2
    counts = count_observations(model, 2);
    EXPECT_EQ(counts.size(), 3);
    key = {0,0};
    EXPECT_EQ(counts[key], 1);
    key = {2,0};
    EXPECT_EQ(counts[key], 5);
    key = {0,2};
    EXPECT_EQ(counts[key], 1);

    // Component = 4
    counts = count_observations(model, 4);
    EXPECT_EQ(counts.size(), 3);
    key = {0,0};
    EXPECT_EQ(counts[key], 4);
    key = {4,0};
    EXPECT_EQ(counts[key], 2);
    key = {0,4};
    EXPECT_EQ(counts[key], 1);

    // Component = 3
    counts = count_observations(model, 3);
    EXPECT_EQ(counts.size(), 5);
    key = {0,1};
    EXPECT_EQ(counts[key], 1);
    key = {1,2};
    EXPECT_EQ(counts[key], 1);
    key = {3,0};
    EXPECT_EQ(counts[key], 1);
    key = {2,1};
    EXPECT_EQ(counts[key], 3);
    key = {2,0};
    EXPECT_EQ(counts[key], 1);

    // Component = 7
    counts = count_observations(model, 7);
    EXPECT_EQ(counts.size(), 6);
    key = {0,1};
    EXPECT_EQ(counts[key], 1);
    key = {2,1};
    EXPECT_EQ(counts[key], 2);
    key = {1,2};
    EXPECT_EQ(counts[key], 1);
    key = {6,1};
    EXPECT_EQ(counts[key], 1);
    key = {7,0};
    EXPECT_EQ(counts[key], 1);
    key = {2,4};
    EXPECT_EQ(counts[key], 1);
}

TEST(evidence, icc){
    // Read in test data + create model
    mcm model = create_model(3, 3, false);
    std::vector<std::vector<__uint128_t>> data = data_processing("../tests/test.dat", 3, model.n_ints);
    model.data = data;
    model.N = data.size();

    EXPECT_FLOAT_EQ(calc_evidence_icc(1, model, 1), -8.769507120030227);
    EXPECT_FLOAT_EQ(calc_evidence_icc(2, model, 1), -7.670894831362117);
    EXPECT_FLOAT_EQ(calc_evidence_icc(4, model, 1), -8.769507120030227);
    EXPECT_FLOAT_EQ(calc_evidence_icc(3, model, 2), -15.982243968542207);
    EXPECT_FLOAT_EQ(calc_evidence_icc(7, model, 3), -23.324842793537606); 
}

TEST(evidence, total){
    // Read in test data + create model
    mcm model = create_model(3, 3, false);
    std::vector<std::vector<__uint128_t>> data = data_processing("../tests/test.dat", 3, model.n_ints);
    model.data = data;
    model.N = data.size();
    model.exhaustive = true;

    // Create storage
    model.evidence_storage_es.assign(7, 0);

    std::vector<__uint128_t> partition = {1,2,4};
    EXPECT_EQ(calc_evidence(partition, model), calc_evidence_icc(1, model, 1) + calc_evidence_icc(2, model, 1) + calc_evidence_icc(4, model, 1));

    partition = {7,0,0};
    EXPECT_EQ(calc_evidence(partition, model), calc_evidence_icc(7, model, 3));

    partition = {3,4,0};
    EXPECT_EQ(calc_evidence(partition, model), calc_evidence_icc(3, model, 2) + calc_evidence_icc(4, model, 1));
}

TEST(evidence, storage){
    // Read in test data + create model
    mcm model = create_model(3, 3, false);
    std::vector<std::vector<__uint128_t>> data = data_processing("../tests/test.dat", 3, model.n_ints);
    model.data = data;
    model.N = data.size();
    model.exhaustive = false;

    // Calculate the log-evidence of 1 component
    get_evidence_icc(3, model);

    EXPECT_EQ(model.evidence_storage.size(), 1);
    EXPECT_EQ(model.evidence_storage[3], calc_evidence_icc(3, model, 2));

    get_evidence_icc(7, model);

    EXPECT_EQ(model.evidence_storage.size(), 2);
    EXPECT_EQ(model.evidence_storage[7], calc_evidence_icc(7, model, 3));    
}
