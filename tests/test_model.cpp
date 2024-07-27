#include "gtest/gtest.h"
#include "../src/model/model.h"

TEST(model, construction){
    int q = 3;
    int n = 3;
    // Construct model
    mcm model = create_model(q, n, false);
    // Read in data
    std::vector<std::vector<__uint128_t>> data = data_processing("../tests/test.dat", n, model.n_ints);
    model.data = data;
    model.N = data.size();

    // Check initializations
    EXPECT_EQ(model.N, 7);
    EXPECT_EQ(model.q, 3);
    EXPECT_EQ(model.n, 3);
    EXPECT_EQ(model.n_ints, 2);

    // Check powers of q
    std::vector<__uint128_t> powers = {1,3,9,27};
    EXPECT_EQ(powers, model.pow_q);
    for(int i = 0; i < n+1; i++){
        EXPECT_EQ(powers[i], model.pow_q[i]) << "Wrong power at index " << i;
    }

    // Check the number of integers for different values of q
    q = 2;
    model = create_model(q, n, false);
    EXPECT_EQ(model.n_ints, 1);

    q = 4;
    model = create_model(q, n, false);
    EXPECT_EQ(model.n_ints, 2);

    q = 5;
    model = create_model(q, n, false);
    EXPECT_EQ(model.n_ints, 3);

    q = 6;
    model = create_model(q, n, false);
    EXPECT_EQ(model.n_ints, 3);

    q = 7;
    model = create_model(q, n, false);
    EXPECT_EQ(model.n_ints, 3);

    q = 8;
    model = create_model(q, n, false);
    EXPECT_EQ(model.n_ints, 3);

    q = 9;
    model = create_model(q, n, false);
    EXPECT_EQ(model.n_ints, 4);
}