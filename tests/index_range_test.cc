// MIT License
//
// Copyright (c) 2021 Ioannis Kaliakatsos
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <gtest/gtest.h>
#include "index_range.h"

TEST(RangeTest, InvalidTest) {
    auto range = index_range::start_count(0, 0);
    EXPECT_FALSE(range.is_valid);
    EXPECT_EQ(-1, range.start);
    EXPECT_EQ(-1, range.end);
    EXPECT_EQ(-1, range.count);
    
    range = index_range::start_count(0, -1);
    EXPECT_FALSE(range.is_valid);
    EXPECT_EQ(-1, range.start);
    EXPECT_EQ(-1, range.end);
    EXPECT_EQ(-1, range.count);
    
    range = index_range::start_count(0, -5);
    EXPECT_FALSE(range.is_valid);
    EXPECT_EQ(-1, range.start);
    EXPECT_EQ(-1, range.end);
    EXPECT_EQ(-1, range.count);
    
    range = index_range::start_count(-1, 10);
    EXPECT_FALSE(range.is_valid);
    EXPECT_EQ(-1, range.start);
    EXPECT_EQ(-1, range.end);
    EXPECT_EQ(-1, range.count);
    
    range = index_range::start_count(-3, 10);
    EXPECT_FALSE(range.is_valid);
    EXPECT_EQ(-1, range.start);
    EXPECT_EQ(-1, range.end);
    EXPECT_EQ(-1, range.count);
}

TEST(RangeTest, ValidFromStartAndCountTest) {
    auto range = index_range::start_count(0, 1);
    EXPECT_TRUE(range.is_valid);
    EXPECT_EQ(0, range.start);
    EXPECT_EQ(0, range.end);
    EXPECT_EQ(1, range.count);
    
    range = index_range::start_count(13, 3);
    EXPECT_TRUE(range.is_valid);
    EXPECT_EQ(13, range.start);
    EXPECT_EQ(15, range.end);
    EXPECT_EQ(3, range.count);
}

TEST(RangeTest, ValidFromStartAndEndTest) {
    auto range = index_range::start_end(0, 1);
    EXPECT_TRUE(range.is_valid);
    EXPECT_EQ(0, range.start);
    EXPECT_EQ(1, range.end);
    EXPECT_EQ(2, range.count);
    
    range = index_range::start_end(13, 15);
    EXPECT_TRUE(range.is_valid);
    EXPECT_EQ(13, range.start);
    EXPECT_EQ(15, range.end);
    EXPECT_EQ(3, range.count);
    
    range = index_range::start_end(13, 13);
    EXPECT_TRUE(range.is_valid);
    EXPECT_EQ(13, range.start);
    EXPECT_EQ(13, range.end);
    EXPECT_EQ(1, range.count);
}

TEST(RangeTest, InvalidFromStartAndEndTest) {
    const auto range = index_range::start_end(10, 9);
    EXPECT_FALSE(range.is_valid);
    EXPECT_EQ(-1, range.start);
    EXPECT_EQ(-1, range.end);
    EXPECT_EQ(-1, range.count);
}

TEST(RangeTest, EqualityTest) {
    EXPECT_FALSE(index_range::invalid == index_range::start_end(9, 10));
    EXPECT_TRUE(index_range::invalid == index_range::start_end(10, 9));
    EXPECT_TRUE(index_range::invalid == index_range::invalid);
    EXPECT_FALSE(index_range::invalid != index_range::invalid);
    
    EXPECT_TRUE(index_range::start_end(9, 10) == index_range::start_end(9, 10));
    EXPECT_FALSE(index_range::start_end(9, 10) != index_range::start_end(9, 10));
    
    EXPECT_FALSE(index_range::start_end(9, 10) == index_range::start_end(8, 10));
    EXPECT_TRUE(index_range::start_end(9, 10) != index_range::start_end(8, 10));
}
