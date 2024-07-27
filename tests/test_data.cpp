#include "gtest/gtest.h"
#include "../src/model/model.h"

TEST(data, read_in){
    int n = 3;
    // Read in data
    std::vector<std::vector<__uint128_t>> data = data_processing("../tests/test.dat", n, 2);

    // Check the number of observations
    EXPECT_EQ(data.size(), 7);

    // Check the number of variables
    for(std::vector<__uint128_t> obs : data){
        EXPECT_EQ(obs.size(), 2);
    }

    // Check individual observations
    std::vector<__uint128_t> obs = {2,1};
    EXPECT_EQ(data[0], obs);
    EXPECT_EQ(data[5], obs);
    
    obs = {0,1};
    EXPECT_EQ(data[1], obs);

    obs = {1,2};
    EXPECT_EQ(data[2], obs);

    obs = {6,1};
    EXPECT_EQ(data[3], obs);

    obs = {7,0};
    EXPECT_EQ(data[4], obs);

    obs = {2,4};
    EXPECT_EQ(data[6], obs);
}
