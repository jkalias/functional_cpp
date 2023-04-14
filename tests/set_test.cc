// MIT License
//
// Copyright (c) 2023 Ioannis Kaliakatsos
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
#include "set.h"
#include "vector.h"
#include "test_types.h"

using namespace fcpp;

template <typename T>
void debug(set<T>& set)
{
    set.for_each([](const T& element) {
        std::cout << element << std::endl;
    });
}

void test_contents(const set<int>& set) {
    EXPECT_EQ(3, set.size());
    EXPECT_EQ(1, set[0]);
    EXPECT_EQ(3, set[1]);
    EXPECT_EQ(5, set[2]);
}

TEST(SetTest, EmptyConstructor)
{
	const set<int> set_under_test;
    EXPECT_EQ(0, set_under_test.size());
}

TEST(SetTest, StdSetConstructor)
{
	const set<int> set_under_test(std::set<int>({1, 5, 3, 3}));
    test_contents(set_under_test);
}

TEST(SetTest, StdVectorConstructor)
{
	const set<int> set_under_test(std::vector<int>({1, 5, 3, 3}));
    test_contents(set_under_test);
}

TEST(SetTest, FunctionalVectorConstructor)
{
	const set<int> set_under_test(vector<int>({1, 5, 3, 3}));
    test_contents(set_under_test);
}

TEST(SetTest, StdInitializerListConstructor)
{
	const set<int> set_under_test(std::initializer_list<int>({1, 5, 3, 3}));
    test_contents(set_under_test);
}

TEST(SetTest, Subscripting)
{
	const set<int> set_under_test(std::set<int>({1, 5, 3, 3}));
    test_contents(set_under_test);
}

TEST(SetTest, ConstSubscripting)
{
    const set<int> set_under_test(std::set<int>({1, 5, 3, 3}));
    test_contents(set_under_test);
}

TEST(SetTest, Difference)
{
    const set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
    const set<int> set2(std::set<int>({2, 5, 7, 10, 15, 17}));
    const auto& diff = set1.difference_with(set2);
    EXPECT_EQ(set<int>({1, 3, 8}), diff);
}

TEST(SetTest, DifferenceStdSet)
{
    const set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
    const std::set<int> set2({2, 5, 7, 10, 15, 17});
    const auto& diff = set1.difference_with(set2);
    EXPECT_EQ(set<int>({1, 3, 8}), diff);
}

TEST(SetTest, DifferenceFunctionalSet)
{
    const set<int> set1({1, 2, 3, 5, 7, 8, 10});
    const set<int> set2({2, 5, 7, 10, 15, 17});
    const auto& diff = set1.difference_with(set2);
    EXPECT_EQ(set<int>({1, 3, 8}), diff);
}

TEST(SetTest, DifferenceFunctionalSetCustomType)
{
    const set<person, person_comparator> set1({
        person(51, "George"),
        person(81, "Jackie"),
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate")
    });
    
    const set<person, person_comparator> set2({
        person(51, "George"),
        person(81, "Jackie"),
    });
    
    const auto& diff = set1.difference_with(set2);
    
    const set<person, person_comparator> expected({
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate")
    });
    
    EXPECT_EQ(expected, diff);
}

TEST(SetTest, Union)
{
    const set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
    const set<int> set2(std::set<int>({2, 5, 7, 10, 15, 17}));
    const auto& combined = set1.union_with(set2);
    EXPECT_EQ(set<int>({1, 2, 3, 5, 7, 8, 10, 15, 17}), combined);
}

TEST(SetTest, UnionStdSet)
{
    const set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
    const std::set<int> set2({2, 5, 7, 10, 15, 17});
    const auto& combined = set1.union_with(set2);
    EXPECT_EQ(set<int>({1, 2, 3, 5, 7, 8, 10, 15, 17}), combined);
}

TEST(SetTest, UnionFunctionalSet)
{
    const set<int> set1({1, 2, 3, 5, 7, 8, 10});
    const set<int> set2({2, 5, 7, 10, 15, 17});
    const auto& combined = set1.union_with(set2);
    EXPECT_EQ(set<int>({1, 2, 3, 5, 7, 8, 10, 15, 17}), combined);
}

TEST(SetTest, UnionFunctionalSetCustomType)
{
    const set<person, person_comparator> set1({
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate")
    });
    
    const set<person, person_comparator> set2({
        person(51, "George"),
        person(81, "Jackie"),
    });
    
    const auto& combined = set1.union_with(set2);
    
    const set<person, person_comparator> expected({
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate"),
        person(51, "George"),
        person(81, "Jackie"),
    });
    
    EXPECT_EQ(expected, combined);
}

TEST(SetTest, Intersection)
{
    const set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
    const set<int> set2(std::set<int>({2, 5, 7, 10, 15, 17}));
    const auto& intersection = set1.intersect_with(set2);
    EXPECT_EQ(set<int>({2, 5, 7, 10}), intersection);
}

TEST(SetTest, IntersectionStdSet)
{
    const set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
    const std::set<int> set2({2, 5, 7, 10, 15, 17});
    const auto& intersection = set1.intersect_with(set2);
    EXPECT_EQ(set<int>({2, 5, 7, 10}), intersection);
}

TEST(SetTest, IntersectionFunctionalSet)
{
    const set<int> set1({1, 2, 3, 5, 7, 8, 10});
    const set<int> set2({2, 5, 7, 10, 15, 17});
    const auto& intersection = set1.intersect_with(set2);
    EXPECT_EQ(set<int>({2, 5, 7, 10}), intersection);
}

TEST(SetTest, IntersectionFunctionalSetCustomType)
{
    const set<person, person_comparator> set1({
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate"),
        person(51, "George"),
        person(81, "Jackie"),
    });
    
    const set<person, person_comparator> set2({
        person(39, "Robert"),
        person(18, "Jannet"),
        person(25, "Kate"),
        person(52, "Anna"),
        person(63, "Simon"),
    });
    
    const auto& intersection = set1.intersect_with(set2);
    
    const set<person, person_comparator> expected({
        person(18, "Jannet"),
        person(25, "Kate"),
    });
    
    EXPECT_EQ(expected, intersection);
}

TEST(SetTest, Min)
{
    const set<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto minimum = numbers.min();
    EXPECT_TRUE(minimum.has_value());
    EXPECT_EQ(1, minimum.value());
}

TEST(SetTest, MinCustomType)
{
    const set<person, person_comparator> persons({
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate"),
        person(62, "Bob")
    });
    const auto minimum = persons.min();
#if defined(__clang__)
    EXPECT_EQ(person(18, "Jannet"), minimum.value());
#else
    EXPECT_EQ(person(62, "Bob"), minimum.value());
#endif
}

TEST(SetTest, MinEmptySet)
{
    const set<int> numbers;
    const auto minimum = numbers.min();
    EXPECT_FALSE(minimum.has_value());
}

TEST(SetTest, Max)
{
    const set<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto maximum = numbers.max();
    EXPECT_TRUE(maximum.has_value());
    EXPECT_EQ(8, maximum.value());
}

TEST(SetTest, MaxCustomType)
{
    const set<person, person_comparator> persons({
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate"),
        person(62, "Bob")
    });
    const auto maximum = persons.max();
    std::cout << maximum.value().name << std::endl;
#if __linux__  // NOLINT(clang-diagnostic-undef)
    EXPECT_EQ(person(18, "Jannet"), maximum.value());
#else
    EXPECT_EQ(person(25, "Kate"), maximum.value());
#endif
}

TEST(SetTest, MaxEmptySet)
{
    const set<int> numbers;
    const auto maximum = numbers.max();
    EXPECT_FALSE(maximum.has_value());
}

TEST(SetTest, Map)
{
    const set<int> numbers({4, 1, 3});
    const auto mapped_set = numbers.map<child>([](const int& age) {
        return child(age);
    });
    EXPECT_EQ(3, mapped_set.size());
    EXPECT_EQ(1, mapped_set[0].age);
    EXPECT_EQ(3, mapped_set[1].age);
    EXPECT_EQ(4, mapped_set[2].age);
}

TEST(SetTest, AllOf)
{
    const set<int> numbers({1, 4, 2, 5, 8, 3});
    EXPECT_TRUE(numbers.all_of([](const int &number) { return number < 10; }));
    EXPECT_FALSE(numbers.all_of([](const int &number) { return number > 2; }));
}

TEST(SetTest, AnyOf)
{
    const set<int> numbers({1, 4, 2, 5, 8, 3});
    EXPECT_TRUE(numbers.any_of([](const int &number) { return number < 5; }));
    EXPECT_FALSE(numbers.any_of([](const int &number) { return number > 10; }));
}

TEST(SetTest, NoneOf)
{
    const set<int> numbers({1, 4, 2, 5, 8, 3});
    EXPECT_TRUE(numbers.none_of([](const int &number) { return number > 10; }));
    EXPECT_FALSE(numbers.none_of([](const int &number) { return number < 6; }));
}

TEST(SetTest, Reduce)
{
    const set<std::string> tokens({ "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "brown", "dog" });
    const auto sentence = tokens.reduce(std::string(""), [](const std::string& partial, const std::string& token) {
        return partial.length() != 0
    		? partial + " " + token
    		: token;
    });
    EXPECT_EQ("brown dog fox jumps lazy over quick the", sentence);
}

TEST(SetTest, Filter)
{
    set<int> numbers({ 1, 3, -5, 2, -1, 9, -4 });
    numbers.filter([](const int& element) {
        return element >= 1.5;
    });
    EXPECT_EQ(set<int>({2, 3, 9}), numbers);
}

TEST(SetTest, Filtered)
{
    const set<int> numbers({ 1, 3, -5, 2, -1, 9, -4 });
    auto filtered_numbers = numbers.filtered([](const int& element) {
        return element >= 1.5;
    });
    EXPECT_EQ(set<int>({2, 3, 9}), filtered_numbers);
    EXPECT_EQ(set<int>({ 1, 3, -5, 2, -1, 9, -4 }), numbers);
}

TEST(SetTest, ZipWithFunctionalSet)
{
    const set<int> ages({ 25, 45, 30, 63 });
    const set<std::string> persons({ "Jake", "Bob", "Michael", "Philipp" });
    const auto zipped = ages.zip(persons);
    const auto expected = set<std::pair<int, std::string>>({
        std::pair<int, std::string>(25, "Bob"),
        std::pair<int, std::string>(30, "Jake"),
        std::pair<int, std::string>(45, "Michael"),
        std::pair<int, std::string>(63, "Philipp"),
    });
    EXPECT_EQ(expected, zipped);
}

TEST(SetTest, ZipWithFunctionalSetDifferentSizes)
{
    const set<int> ages({ 25, 45, 30, 63 });
    const set<std::string> persons({ "Jake" });
    EXPECT_DEATH(ages.zip(persons), "");
}

TEST(SetTest, ZipWithStdSet)
{
    const set<int> ages({ 25, 45, 30, 63 });
    const std::set<std::string> persons({ "Jake", "Bob", "Michael", "Philipp" });
    const auto zipped = ages.zip(persons);
    const auto expected = set<std::pair<int, std::string>>({
        std::pair<int, std::string>(25, "Bob"),
        std::pair<int, std::string>(30, "Jake"),
        std::pair<int, std::string>(45, "Michael"),
        std::pair<int, std::string>(63, "Philipp"),
    });
    EXPECT_EQ(expected, zipped);
}

TEST(SetTest, ZipWithStdSetDifferentSizes)
{
    const set<int> ages({ 25, 45, 30, 63 });
    const std::set<std::string> persons({ "Jake" });
    EXPECT_DEATH(ages.zip(persons), "");
}

TEST(SetTest, ZipWithFunctionalVector)
{
    const set<int> ages({ 25, 45, 30, 63 });
    const vector<std::string> persons({ "Jake", "Bob", "Michael", "Philipp" });
    const auto zipped = ages.zip(persons);
    const auto expected = set<std::pair<int, std::string>>({
        std::pair<int, std::string>(25, "Bob"),
        std::pair<int, std::string>(30, "Jake"),
        std::pair<int, std::string>(45, "Michael"),
        std::pair<int, std::string>(63, "Philipp"),
    });
    EXPECT_EQ(expected, zipped);
}

TEST(SetTest, ZipWithFunctionalVectorDifferentSizes)
{
    const set<int> ages({ 25, 45, 30, 63 });
    const vector<std::string> persons({ "Jake" });
    EXPECT_DEATH(ages.zip(persons), "");
}

TEST(SetTest, ZipWithStdVector)
{
    const set<int> ages({ 25, 45, 30, 63 });
    const std::vector<std::string> persons({ "Jake", "Bob", "Michael", "Philipp" });
    const auto zipped = ages.zip(persons);
    const auto expected = set<std::pair<int, std::string>>({
        std::pair<int, std::string>(25, "Bob"),
        std::pair<int, std::string>(30, "Jake"),
        std::pair<int, std::string>(45, "Michael"),
        std::pair<int, std::string>(63, "Philipp"),
    });
    EXPECT_EQ(expected, zipped);
}

TEST(SetTest, ZipWithStdVectorDifferentSizes)
{
    const set<int> ages({ 25, 45, 30, 63 });
    const std::vector<std::string> persons({ "Jake" });
    EXPECT_DEATH(ages.zip(persons), "");
}

TEST(SetTest, Keys)
{
    const set<int> numbers({ 25, 45, 30, 63 });
    const auto keys = numbers.keys();
    EXPECT_EQ(vector<int>({25, 30, 45, 63}), keys);
}

TEST(SetTest, RemoveExistingElement)
{
    set<int> numbers({1, 4, 2});
    numbers.remove(4);
    EXPECT_EQ(set<int>({1, 2}), numbers);
}

TEST(SetTest, RemoveNonExistentElement)
{
    set<int> numbers({1, 4, 2});
    numbers.remove(18);
    EXPECT_EQ(set<int>({1, 2, 4}), numbers);
}

TEST(SetTest, RemovingExistingElement)
{
    const set<int> numbers({1, 4, 2});
    const auto less_numbers = numbers.removing(4);
    EXPECT_EQ(set<int>({1, 2}), less_numbers);
    EXPECT_EQ(set<int>({1, 2, 4}), numbers);
}

TEST(SetTest, InsertNewElement)
{
    set<int> numbers({1, 4, 2});
    numbers.insert(18);
    EXPECT_EQ(set<int>({1, 2, 4, 18}), numbers);
}

TEST(SetTest, InsertingNewElement)
{
    const set<int> numbers({1, 4, 2});
    const auto augmented_numbers =  numbers.inserting(18);
    EXPECT_EQ(set<int>({1, 2, 4, 18}), augmented_numbers);
    EXPECT_EQ(set<int>({1, 2, 4}), numbers);
}

TEST(SetTest, InsertExistingElement)
{
    set<int> numbers({1, 4, 2});
    numbers.insert(2);
    EXPECT_EQ(set<int>({1, 2, 4}), numbers);
}

TEST(SetTest, InsertingExistingElement)
{
    const set<int> numbers({1, 4, 2});
    const auto augmented_numbers =  numbers.inserting(2);
    EXPECT_EQ(set<int>({1, 2, 4}), augmented_numbers);
    EXPECT_EQ(set<int>({1, 2, 4}), numbers);
}

TEST(SetTest, Clear)
{
    set<int> numbers({1, 4, 2});
    numbers.clear();
    EXPECT_EQ(0, numbers.size());
}

TEST(SetTest, Clearing)
{
    const set<int> numbers({1, 4, 2});
    const auto cleared_numbers = numbers.clearing();
    EXPECT_EQ(0, cleared_numbers.size());
    EXPECT_EQ(3, numbers.size());
}

TEST(SetTest, IsEmpty)
{
    const set<int> numbers({1, 4, 2});
    EXPECT_FALSE(numbers.is_empty());
    EXPECT_TRUE(set<int>().is_empty());
}

TEST(SetTest, Contains)
{
    const set<int> numbers({1, 4, 2});
    EXPECT_TRUE(numbers.contains(1));
    EXPECT_FALSE(numbers.contains(15));
}

TEST(SetTest, EqualityOperator)
{
    const set<int> set1(std::set<int>({1, 2, 3}));
    const set<int> set2(std::set<int>({1, 2, 3, 2, 3}));
    EXPECT_TRUE(set1 == set2);
    EXPECT_FALSE(set1 != set2);
}

TEST(SetTest, InequalityOperator)
{
    const set<int> set1(std::set<int>({1, 2, 3}));
    const set<int> set2(std::set<int>({1, 2, 3, 4}));
    EXPECT_FALSE(set1 == set2);
    EXPECT_TRUE(set1 != set2);
}

TEST(SetTest, EqualityOperatorCustomType)
{
    const set<person, person_comparator> set1({
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate")
    });
    
    const set<person, person_comparator> set2({
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate")
    });

    EXPECT_TRUE(set1 == set2);
    EXPECT_FALSE(set1 != set2);
}
