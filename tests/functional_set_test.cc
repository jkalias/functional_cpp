// MIT License
//
// Copyright (c) 2022 Ioannis Kaliakatsos
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
#include "warnings.h"
#include "functional_set.h"

template <typename T>
void debug(functional_set<T>& set)
{
    std::cout << "bla" << std::endl;
    for (auto &x : set) {
        std::cout << x << std::endl;
    }
//    set.for_each([](const T& element) {
//        std::cout << element << std::endl;
//    });
}

TEST(FunctionalSetTest, EmptyConstructor)
{
    functional_set<int> set_under_test;
    EXPECT_EQ(0, set_under_test.size());
}

TEST(FunctionalSetTest, StdSetConstructor)
{
    functional_set<int> set_under_test(std::set<int>({1, 5, 3, 3}));
    EXPECT_EQ(3, set_under_test.size());
}

TEST(FunctionalSetTest, Subscripting)
{
    functional_set<int> set_under_test(std::set<int>({1, 5, 3, 3}));
    EXPECT_EQ(1, set_under_test[0]);
    EXPECT_EQ(3, set_under_test[1]);
    EXPECT_EQ(5, set_under_test[2]);
}

TEST(FunctionalSetTest, ConstSubscripting)
{
    const functional_set<int> set_under_test(std::set<int>({1, 5, 3, 3}));
    EXPECT_EQ(1, set_under_test[0]);
    EXPECT_EQ(3, set_under_test[1]);
    EXPECT_EQ(5, set_under_test[2]);
}
