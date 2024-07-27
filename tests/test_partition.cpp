#include "gtest/gtest.h"
#include "../src/model/model.h"

TEST(partition, comm_size){
    EXPECT_EQ(component_size(0), 0);
    EXPECT_EQ(component_size(1), 1);
    EXPECT_EQ(component_size(2), 1);
    EXPECT_EQ(component_size(3), 2);
    EXPECT_EQ(component_size(4), 1);
    EXPECT_EQ(component_size(5), 2);
    EXPECT_EQ(component_size(6), 2);
    EXPECT_EQ(component_size(7), 3);
    EXPECT_EQ(component_size(8), 1);
}

TEST(partition, comm_string){
    EXPECT_EQ(component_as_string(0, 1), "0");
    EXPECT_EQ(component_as_string(0, 2), "00");
    EXPECT_EQ(component_as_string(1, 3), "100");
    EXPECT_EQ(component_as_string(2, 3), "010");
    EXPECT_EQ(component_as_string(3, 3), "110");
    EXPECT_EQ(component_as_string(4, 3), "001");
    EXPECT_EQ(component_as_string(5, 3), "101");
    EXPECT_EQ(component_as_string(6, 3), "011");
    EXPECT_EQ(component_as_string(7, 3), "111");
    EXPECT_EQ(component_as_string(8, 4), "0001");
}

TEST(partition, conversion){
    std::vector<__uint128_t> result(3,0);
    std::vector<__uint128_t> expected_result;

    int a[3] = {0,0,0};
    convert_partition(a, result, 3);
    expected_result = {7,0,0};

    for(int i = 0; i < 3; i++){
        EXPECT_EQ(result[i], expected_result[i]) << "Partition wrong at index " << i;
    }
    std::fill(result.begin(), result.end(), 0);

    a[1] = 1;
    a[2] = 2;
    convert_partition(a, result, 3);
    expected_result = {1,2,4};

    for(int i = 0; i < 3; i++){
        EXPECT_EQ(result[i], expected_result[i]) << "Partition wrong at index " << i;
    }
    std::fill(result.begin(), result.end(), 0);

    a[0] = 1;
    convert_partition(a, result, 3);
    expected_result = {0,3,4};

    for(int i = 0; i < 3; i++){
        EXPECT_EQ(result[i], expected_result[i]) << "Partition wrong at index " << i;
    }
}