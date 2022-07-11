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
#include "functional_vector.h"
#include "test_types.h""

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

void testContents(const functional_set<int>& set) {
    EXPECT_EQ(3, set.size());
    EXPECT_EQ(1, set[0]);
    EXPECT_EQ(3, set[1]);
    EXPECT_EQ(5, set[2]);
}

TEST(FunctionalSetTest, EmptyConstructor)
{
    functional_set<int> set_under_test;
    EXPECT_EQ(0, set_under_test.size());
}

TEST(FunctionalSetTest, StdSetConstructor)
{
    functional_set<int> set_under_test(std::set<int>({1, 5, 3, 3}));
    testContents(set_under_test);
}

TEST(FunctionalSetTest, StdVectorConstructor)
{
    functional_set<int> set_under_test(std::vector<int>({1, 5, 3, 3}));
    testContents(set_under_test);
}

TEST(FunctionalSetTest, FunctionalVectorConstructor)
{
    functional_set<int> set_under_test(functional_vector<int>({1, 5, 3, 3}));
    testContents(set_under_test);
}

TEST(FunctionalSetTest, StdInitializerListConstructor)
{
    functional_set<int> set_under_test(std::initializer_list<int>({1, 5, 3, 3}));
    testContents(set_under_test);
}

TEST(FunctionalSetTest, Subscripting)
{
    functional_set<int> set_under_test(std::set<int>({1, 5, 3, 3}));
    testContents(set_under_test);
}

TEST(FunctionalSetTest, ConstSubscripting)
{
    const functional_set<int> set_under_test(std::set<int>({1, 5, 3, 3}));
    testContents(set_under_test);
}

TEST(FunctionalSetTest, Difference)
{
    const functional_set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
    const functional_set<int> set2(std::set<int>({2, 5, 7, 10, 15, 17}));
    const auto& diff = set1.difference_with(set2);
    EXPECT_EQ(functional_set<int>({1, 3, 8}), diff);
}

TEST(FunctionalSetTest, DifferenceStdSet)
{
    const functional_set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
    const std::set<int> set2({2, 5, 7, 10, 15, 17});
    const auto& diff = set1.difference_with(set2);
    EXPECT_EQ(functional_set<int>({1, 3, 8}), diff);
}

TEST(FunctionalSetTest, DifferenceFunctionalSet)
{
    const functional_set<int> set1({1, 2, 3, 5, 7, 8, 10});
    const functional_set<int> set2({2, 5, 7, 10, 15, 17});
    const auto& diff = set1.difference_with(set2);
    EXPECT_EQ(functional_set<int>({1, 3, 8}), diff);
}

TEST(FunctionalSetTest, DifferenceFunctionalSetCustomType)
{
    const functional_set<person, person_comparator> set1({
        person(51, "George"),
        person(81, "Jackie"),
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate")
    });
    
    const functional_set<person, person_comparator> set2({
        person(51, "George"),
        person(81, "Jackie"),
    });
    
    const auto& diff = set1.difference_with(set2);
    
    const functional_set<person, person_comparator> expected({
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate")
    });
    
    EXPECT_EQ(expected, diff);
}

TEST(FunctionalSetTest, Union)
{
    const functional_set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
    const functional_set<int> set2(std::set<int>({2, 5, 7, 10, 15, 17}));
    const auto& combined = set1.union_with(set2);
    EXPECT_EQ(functional_set<int>({1, 2, 3, 5, 7, 8, 10, 15, 17}), combined);
}

TEST(FunctionalSetTest, UnionStdSet)
{
    const functional_set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
    const std::set<int> set2({2, 5, 7, 10, 15, 17});
    const auto& combined = set1.union_with(set2);
    EXPECT_EQ(functional_set<int>({1, 2, 3, 5, 7, 8, 10, 15, 17}), combined);
}

TEST(FunctionalSetTest, UnionFunctionalSet)
{
    const functional_set<int> set1({1, 2, 3, 5, 7, 8, 10});
    const functional_set<int> set2({2, 5, 7, 10, 15, 17});
    const auto& combined = set1.union_with(set2);
    EXPECT_EQ(functional_set<int>({1, 2, 3, 5, 7, 8, 10, 15, 17}), combined);
}

TEST(FunctionalSetTest, UnionFunctionalSetCustomType)
{
    const functional_set<person, person_comparator> set1({
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate")
    });
    
    const functional_set<person, person_comparator> set2({
        person(51, "George"),
        person(81, "Jackie"),
    });
    
    const auto& combined = set1.union_with(set2);
    
    const functional_set<person, person_comparator> expected({
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate"),
        person(51, "George"),
        person(81, "Jackie"),
    });
    
    EXPECT_EQ(expected, combined);
}

TEST(FunctionalSetTest, Intersection)
{
    const functional_set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
    const functional_set<int> set2(std::set<int>({2, 5, 7, 10, 15, 17}));
    const auto& intersection = set1.intersect_with(set2);
    EXPECT_EQ(functional_set<int>({2, 5, 7, 10}), intersection);
}

TEST(FunctionalSetTest, IntersectionStdSet)
{
    const functional_set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
    const std::set<int> set2({2, 5, 7, 10, 15, 17});
    const auto& intersection = set1.intersect_with(set2);
    EXPECT_EQ(functional_set<int>({2, 5, 7, 10}), intersection);
}

TEST(FunctionalSetTest, IntersectionFunctionalSet)
{
    const functional_set<int> set1({1, 2, 3, 5, 7, 8, 10});
    const functional_set<int> set2({2, 5, 7, 10, 15, 17});
    const auto& intersection = set1.intersect_with(set2);
    EXPECT_EQ(functional_set<int>({2, 5, 7, 10}), intersection);
}

TEST(FunctionalSetTest, IntersectionFunctionalSetCustomType)
{
    const functional_set<person, person_comparator> set1({
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate"),
        person(51, "George"),
        person(81, "Jackie"),
    });
    
    const functional_set<person, person_comparator> set2({
        person(39, "Robert"),
        person(18, "Jannet"),
        person(25, "Kate"),
        person(52, "Anna"),
        person(63, "Simon"),
    });
    
    const auto& intersection = set1.intersect_with(set2);
    
    const functional_set<person, person_comparator> expected({
        person(18, "Jannet"),
        person(25, "Kate"),
    });
    
    EXPECT_EQ(expected, intersection);
}

TEST(FunctionalSetTest, Min)
{
    const functional_set<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    auto minimum = numbers.min();
    EXPECT_TRUE(minimum.has_value());
    EXPECT_EQ(1, minimum.value());
}

TEST(FunctionalSetTest, MinCustomType)
{
    const functional_set<person, person_comparator> persons({
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate"),
        person(62, "Bob")
    });
    auto minimum = persons.min();
    EXPECT_EQ(person(18, "Jannet"), minimum.value());
}

TEST(FunctionalSetTest, MinEmptySet)
{
    const functional_set<int> numbers;
    auto minimum = numbers.min();
    EXPECT_FALSE(minimum.has_value());
}

TEST(FunctionalSetTest, Max)
{
    const functional_set<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    auto maximum = numbers.max();
    EXPECT_TRUE(maximum.has_value());
    EXPECT_EQ(8, maximum.value());
}

TEST(FunctionalSetTest, MaxCustomType)
{
    const functional_set<person, person_comparator> persons({
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate"),
        person(62, "Bob")
    });
    auto maximum = persons.max();
    EXPECT_EQ(person(25, "Kate"), maximum.value());
}

TEST(FunctionalSetTest, MaxEmptySet)
{
    const functional_set<int> numbers;
    auto maximum = numbers.max();
    EXPECT_FALSE(maximum.has_value());
}

TEST(FunctionalSetTest, Map)
{
    const functional_set<int> numbers({4, 1, 3});
    const auto mapped_set = numbers.map<child>([](const int& age) {
        return child(age);
    });
    EXPECT_EQ(3, mapped_set.size());
    EXPECT_EQ(1, mapped_set[0].age);
    EXPECT_EQ(3, mapped_set[1].age);
    EXPECT_EQ(4, mapped_set[2].age);
}

TEST(FunctionalSetTest, RemoveExistingElement)
{
    functional_set<int> numbers({1, 4, 2});
    numbers.remove(4);
    EXPECT_EQ(functional_set<int>({1, 2}), numbers);
}

TEST(FunctionalSetTest, RemoveNonExistentElement)
{
    functional_set<int> numbers({1, 4, 2});
    numbers.remove(18);
    EXPECT_EQ(functional_set<int>({1, 2, 4}), numbers);
}

TEST(FunctionalSetTest, RemovingExistingElement)
{
    const functional_set<int> numbers({1, 4, 2});
    auto less_numbers = numbers.removing(4);
    EXPECT_EQ(functional_set<int>({1, 2}), less_numbers);
    EXPECT_EQ(functional_set<int>({1, 2, 4}), numbers);
}

TEST(FunctionalSetTest, InsertNewElement)
{
    functional_set<int> numbers({1, 4, 2});
    numbers.insert(18);
    EXPECT_EQ(functional_set<int>({1, 2, 4, 18}), numbers);
}

TEST(FunctionalSetTest, InsertingNewElement)
{
    const functional_set<int> numbers({1, 4, 2});
    auto augmented_numbers =  numbers.inserting(18);
    EXPECT_EQ(functional_set<int>({1, 2, 4, 18}), augmented_numbers);
    EXPECT_EQ(functional_set<int>({1, 2, 4}), numbers);
}

TEST(FunctionalSetTest, InsertExistingElement)
{
    functional_set<int> numbers({1, 4, 2});
    numbers.insert(2);
    EXPECT_EQ(functional_set<int>({1, 2, 4}), numbers);
}

TEST(FunctionalSetTest, InsertingExistingElement)
{
    const functional_set<int> numbers({1, 4, 2});
    auto augmented_numbers =  numbers.inserting(2);
    EXPECT_EQ(functional_set<int>({1, 2, 4}), augmented_numbers);
    EXPECT_EQ(functional_set<int>({1, 2, 4}), numbers);
}

TEST(FunctionalSetTest, EqualityOperator)
{
    const functional_set<int> set1(std::set<int>({1, 2, 3}));
    const functional_set<int> set2(std::set<int>({1, 2, 3, 2, 3}));
    EXPECT_TRUE(set1 == set2);
    EXPECT_FALSE(set1 != set2);
}

TEST(FunctionalSetTest, InequalityOperator)
{
    const functional_set<int> set1(std::set<int>({1, 2, 3}));
    const functional_set<int> set2(std::set<int>({1, 2, 3, 4}));
    EXPECT_FALSE(set1 == set2);
    EXPECT_TRUE(set1 != set2);
}

TEST(FunctionalSetTest, EqualityOperatorCustomType)
{
    const functional_set<person, person_comparator> vec1({
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate")
    });
    
    const functional_set<person, person_comparator> vec2({
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate")
    });

    EXPECT_TRUE(vec1 == vec2);
    EXPECT_FALSE(vec1 != vec2);
}
