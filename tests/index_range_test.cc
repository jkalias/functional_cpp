#include <gtest/gtest.h>
#include "index_range.h"

TEST(RangeTest, InvalidTest) {
    auto range = index_range::from_start_and_count(0, 0);
    EXPECT_FALSE(range.is_valid);
    EXPECT_EQ(-1, range.start);
    EXPECT_EQ(-1, range.count);
    
    range = index_range::from_start_and_count(0, -1);
    EXPECT_FALSE(range.is_valid);
    EXPECT_EQ(-1, range.start);
    EXPECT_EQ(-1, range.count);
    
    range = index_range::from_start_and_count(-1, 10);
    EXPECT_FALSE(range.is_valid);
    EXPECT_EQ(-1, range.start);
    EXPECT_EQ(-1, range.count);
}

TEST(RangeTest, ValidFromStartAndCountTest) {
    auto range = index_range::from_start_and_count(0, 1);
    EXPECT_TRUE(range.is_valid);
    EXPECT_EQ(0, range.start);
    EXPECT_EQ(1, range.count);
    
    range = index_range::from_start_and_count(13, 3);
    EXPECT_TRUE(range.is_valid);
    EXPECT_EQ(13, range.start);
    EXPECT_EQ(3, range.count);
}

TEST(RangeTest, ValidFromStartAndEndTest) {
    auto range = index_range::from_start_and_end(0, 1);
    EXPECT_TRUE(range.is_valid);
    EXPECT_EQ(0, range.start);
    EXPECT_EQ(2, range.count);
    
    range = index_range::from_start_and_end(13, 15);
    EXPECT_TRUE(range.is_valid);
    EXPECT_EQ(13, range.start);
    EXPECT_EQ(3, range.count);
    
    range = index_range::from_start_and_end(13, 13);
    EXPECT_TRUE(range.is_valid);
    EXPECT_EQ(13, range.start);
    EXPECT_EQ(1, range.count);
}

TEST(RangeTest, InvalidFromStartAndEndTest) {
    auto range = index_range::from_start_and_end(10, 9);
    EXPECT_FALSE(range.is_valid);
    EXPECT_EQ(-1, range.start);
    EXPECT_EQ(-1, range.count);
}

TEST(RangeTest, EqualityTest) {
    EXPECT_FALSE(index_range::invalid == index_range::from_start_and_end(9, 10));
    EXPECT_TRUE(index_range::invalid == index_range::from_start_and_end(10, 9));
    EXPECT_TRUE(index_range::invalid == index_range::invalid);
    EXPECT_FALSE(index_range::invalid != index_range::invalid);
    
    EXPECT_TRUE(index_range::from_start_and_end(9, 10) == index_range::from_start_and_end(9, 10));
    EXPECT_FALSE(index_range::from_start_and_end(9, 10) != index_range::from_start_and_end(9, 10));
    
    EXPECT_FALSE(index_range::from_start_and_end(9, 10) == index_range::from_start_and_end(8, 10));
    EXPECT_TRUE(index_range::from_start_and_end(9, 10) != index_range::from_start_and_end(8, 10));
}
