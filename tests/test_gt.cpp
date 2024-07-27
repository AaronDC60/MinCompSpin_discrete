#include "gtest/gtest.h"
#include "../src/model/model.h"

TEST(gt, transform_state){
    // sigma1 = s1 * s2
    // sigma2 = s1^2 * s2

    std::vector<int> state;
    std::vector<__uint128_t> conv_state;

    std::vector<int> expected_state;
    std::vector<__uint128_t> expected_conv_state;

    std::vector<std::vector<__uint128_t>> gt(2);

    std::vector<int> op1 = {1,1};
    std::vector<int> op2 = {2,1};
    gt[0] = convert_representation(op1, 2, 2);
    gt[1] = convert_representation(op2, 2, 2);

    state = {0,0};
    expected_state = {0,0};
    conv_state = convert_representation(state, 2, 2);
    expected_conv_state = convert_representation(expected_state, 2, 2);
    gt_state(conv_state, gt, 3, 2, 2);
    EXPECT_EQ(conv_state, expected_conv_state);

    state = {0,1};
    expected_state = {1,1};
    conv_state = convert_representation(state, 2, 2);
    expected_conv_state = convert_representation(expected_state, 2, 2);
    gt_state(conv_state, gt, 3, 2, 2);
    EXPECT_EQ(conv_state, expected_conv_state);

    state = {0,2};
    expected_state = {2,2};
    conv_state = convert_representation(state, 2, 2);
    expected_conv_state = convert_representation(expected_state, 2, 2);
    gt_state(conv_state, gt, 3, 2, 2);
    EXPECT_EQ(conv_state, expected_conv_state);

    state = {1,0};
    expected_state = {1,2};
    conv_state = convert_representation(state, 2, 2);
    expected_conv_state = convert_representation(expected_state, 2, 2);
    gt_state(conv_state, gt, 3, 2, 2);
    EXPECT_EQ(conv_state, expected_conv_state);

    state = {1,1};
    expected_state = {2,0};
    conv_state = convert_representation(state, 2, 2);
    expected_conv_state = convert_representation(expected_state, 2, 2);
    gt_state(conv_state, gt, 3, 2, 2);
    EXPECT_EQ(conv_state, expected_conv_state);

    state = {1,2};
    expected_state = {0,1};
    conv_state = convert_representation(state, 2, 2);
    expected_conv_state = convert_representation(expected_state, 2, 2);
    gt_state(conv_state, gt, 3, 2, 2);
    EXPECT_EQ(conv_state, expected_conv_state);

    state = {2,0};
    expected_state = {2,1};
    conv_state = convert_representation(state, 2, 2);
    expected_conv_state = convert_representation(expected_state, 2, 2);
    gt_state(conv_state, gt, 3, 2, 2);
    EXPECT_EQ(conv_state, expected_conv_state);

    state = {2,1};
    expected_state = {0,2};
    conv_state = convert_representation(state, 2, 2);
    expected_conv_state = convert_representation(expected_state, 2, 2);
    gt_state(conv_state, gt, 3, 2, 2);
    EXPECT_EQ(conv_state, expected_conv_state);

    state = {2,2};
    expected_state = {1,0};
    conv_state = convert_representation(state, 2, 2);
    expected_conv_state = convert_representation(expected_state, 2, 2);
    gt_state(conv_state, gt, 3, 2, 2);
    EXPECT_EQ(conv_state, expected_conv_state);
}


TEST(gt, transform_data){
    std::vector<std::vector<__uint128_t>> gt(2);

    std::vector<int> op1 = {1,1};
    std::vector<int> op2 = {2,1};
    gt[0] = convert_representation(op1, 2, 2);
    gt[1] = convert_representation(op2, 2, 2);

    std::vector<std::vector<int>> data = {{0,0}, {0,1}, {0,2},
                                {1,0}, {1,1}, {1,2},
                                {2,0}, {2,1}, {2,2}};
    std::vector<std::vector<int>> expected_transform = {{0,0}, {1,1}, {2,2},
                                {1,2}, {2,0}, {0,1},
                                {2,1}, {0,2}, {1,0}};
    std::vector<std::vector<__uint128_t>> conv_data(9);
    std::vector<std::vector<__uint128_t>> conv_transform(9);
    for (int i = 0; i < 9; ++i){
        conv_data[i] = convert_representation(data[i], 2, 2);
        conv_transform[i] = convert_representation(expected_transform[i], 2, 2);
    }

    transform_data(conv_data, gt, 3, 2, 2);

    for (int i = 0; i < 9; ++i){
        EXPECT_EQ(conv_data[i][0], conv_transform[i][0]);
        EXPECT_EQ(conv_data[i][1], conv_transform[i][1]);
    }
}
