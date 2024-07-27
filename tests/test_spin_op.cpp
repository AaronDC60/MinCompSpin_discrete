#include "gtest/gtest.h"
#include "../src/model/model.h"

TEST(spin_op, bit_count){
    EXPECT_EQ(count_set_bits(0), 0);
    EXPECT_EQ(count_set_bits(1), 1);
    EXPECT_EQ(count_set_bits(2), 1);
    EXPECT_EQ(count_set_bits(3), 2);
    EXPECT_EQ(count_set_bits(26), 3);
}

TEST(spin_op, convert_repres){
    std::vector<int> a = {0,0};
    std::vector<__uint128_t> converted_a = {0,0,0};
    EXPECT_EQ(convert_representation(a, 2, 3), converted_a);

    a = {0,1};
    converted_a = {2,0};
    EXPECT_EQ(convert_representation(a, 2, 2), converted_a);

    a = {0,2};
    converted_a = {0,2,0};
    EXPECT_EQ(convert_representation(a, 2, 3), converted_a);

    a = {1,0};
    converted_a = {1,0,0};
    EXPECT_EQ(convert_representation(a, 2, 3), converted_a);

    a = {1,1};
    converted_a = {3,0,0};
    EXPECT_EQ(convert_representation(a, 2, 3), converted_a);

    a = {1,2};
    converted_a = {1,2,0};
    EXPECT_EQ(convert_representation(a, 2, 3), converted_a);

    a = {2,1};
    converted_a = {2,1,0};
    EXPECT_EQ(convert_representation(a, 2, 3), converted_a);
    
    a = {4,7};
    converted_a = {2,2,3};
    EXPECT_EQ(convert_representation(a, 2, 3), converted_a);

    a = {7,2};
    converted_a = {1,3,1};
    EXPECT_EQ(convert_representation(a, 2, 3), converted_a);
}

TEST(spin_op, spin_value){
    std::vector<int> state;
    std::vector<__uint128_t> conv_state;

    std::vector<int> op;
    std::vector<__uint128_t> conv_op;
    

    // Base 2
    state = {0,0};
    conv_state = convert_representation(state, 2, 1);

    op = {0,0};
    conv_op = convert_representation(op, 2, 1);
    EXPECT_EQ(spin_value(conv_state, conv_op, 2, 1), 0);

    state = {0,1};
    conv_state = convert_representation(state, 2, 1);

    op = {1,0};
    conv_op = convert_representation(op, 2, 1);
    EXPECT_EQ(spin_value(conv_state, conv_op, 2, 1), 0);
    op = {0,1};
    conv_op = convert_representation(op, 2, 1);
    EXPECT_EQ(spin_value(conv_state, conv_op, 2, 1), 1);
    op = {1,1};
    conv_op = convert_representation(op, 2, 1);
    EXPECT_EQ(spin_value(conv_state, conv_op, 2, 1), 1);

    state = {1,0};
    conv_state = convert_representation(state, 2, 1);
    EXPECT_EQ(spin_value(conv_state, conv_op, 2, 1), 1);

    state = {1,1};
    conv_state = convert_representation(state, 2, 1);

    op = {1,0};
    conv_op = convert_representation(op, 2, 1);
    EXPECT_EQ(spin_value(conv_state, conv_op, 2, 1), 1);

    op = {1,1};
    conv_op = convert_representation(op, 2, 1);
    EXPECT_EQ(spin_value(conv_state, conv_op, 2, 1), 0);

    // Base 3
    state = {1,0};
    conv_state = convert_representation(state, 2, 2);

    op = {1,0};
    conv_op = convert_representation(op, 2, 2);
    EXPECT_EQ(spin_value(conv_state, conv_op, 3, 2), 1);

    state = {2,0};
    conv_state = convert_representation(state, 2, 2);
    EXPECT_EQ(spin_value(conv_state, conv_op, 3, 2), 2);

    op = {1,1};
    conv_op = convert_representation(op, 2, 2);
    EXPECT_EQ(spin_value(conv_state, conv_op, 3, 2), 2);

    state = {2,1};
    conv_state = convert_representation(state, 2, 2);
    EXPECT_EQ(spin_value(conv_state, conv_op, 3, 2), 0);

    op = {1,2};
    conv_op = convert_representation(op, 2, 2);
    EXPECT_EQ(spin_value(conv_state, conv_op, 3, 2), 1);
}

TEST(spin_op, entropy){
    std::vector<double> prob_distr = {1,0};
    EXPECT_EQ(entropy(prob_distr), 0);

    prob_distr = {0.5, 0.5};
    EXPECT_EQ(entropy(prob_distr), 1);

    prob_distr = {0.25, 0.25, 0.25, 0.25};
    EXPECT_EQ(entropy(prob_distr), 2);

    prob_distr = {0.75, 0.25};
    EXPECT_FLOAT_EQ(entropy(prob_distr), 0.8112781244591);
}

TEST(spin_op, entropy_op){
    std::vector<int> op;
    std::vector<__uint128_t> conv_op;

    std::vector<std::vector<int>> data = {{0,1,0,0},
                                {0,1,2,2},
                                {0,1,1,1},
                                {0,2,0,0}};
    
    std::vector<std::vector<__uint128_t>> conv_data;
    for(std::vector<int>& obs : data){
        conv_data.push_back(convert_representation(obs, 4, 2));
    }

    op = {0,0,0,0};
    conv_op = convert_representation(op, 4, 2);
    EXPECT_EQ(entropy_of_op(conv_data, conv_op, 3, 2), 0);

    op = {1,0,0,0};
    conv_op = convert_representation(op, 4, 2);
    EXPECT_EQ(entropy_of_op(conv_data, conv_op, 3, 2), 0);

    op = {2,0,0,0};
    conv_op = convert_representation(op, 4, 2);    
    EXPECT_EQ(entropy_of_op(conv_data, conv_op, 3, 2), 0);

    op = {0,0,2,1};
    conv_op = convert_representation(op, 4, 2);
    EXPECT_EQ(entropy_of_op(conv_data, conv_op, 3, 2), 0);

    op = {0,0,1,2};
    conv_op = convert_representation(op, 4, 2);
    EXPECT_EQ(entropy_of_op(conv_data, conv_op, 3, 2), 0);

    op = {0,1,0,0};
    conv_op = convert_representation(op, 4, 2);
    EXPECT_FLOAT_EQ(entropy_of_op(conv_data, conv_op, 3, 2), 0.8112781244591);

    op = {0,2,0,0};
    conv_op = convert_representation(op, 4, 2);
    EXPECT_FLOAT_EQ(entropy_of_op(conv_data, conv_op, 3, 2), 0.8112781244591);
}
