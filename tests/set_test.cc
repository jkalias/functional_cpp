// MIT License
//
// Copyright (c) 2026 Ioannis Kaliakatsos
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

#include <initializer_list>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "set.h"
#include "test_types.h"
#include "vector.h"
#include "warnings.h"

using namespace fcpp;

void test_contents(const set<int>& set) {
    EXPECT_EQ(3, set.size());
    EXPECT_EQ(1, set[0]);
    EXPECT_EQ(3, set[1]);
    EXPECT_EQ(5, set[2]);
}

struct stateful_descending_int_compare
{
    stateful_descending_int_compare() = delete;

    explicit stateful_descending_int_compare(bool descending)
        : descending(descending)
    {
    }

    bool operator()(const int& lhs, const int& rhs) const
    {
        return descending
            ? lhs > rhs
            : lhs < rhs;
    }

    bool descending;
};

std::set<int, stateful_descending_int_compare> make_stateful_ordered_set(
    const std::initializer_list<int>& values,
    bool descending)
{
    std::set<int, stateful_descending_int_compare> result{stateful_descending_int_compare(descending)};
    result.insert(values.begin(), values.end());
    return result;
}

std::set<int, stateful_descending_int_compare> make_stateful_descending_set(
    const std::initializer_list<int>& values)
{
    return make_stateful_ordered_set(values, true);
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

TEST(SetTest, LazyMapFilterGet)
{
    const set<int> set_under_test({1, 2, 3, 4});
    int map_call_count = 0;
    int filter_call_count = 0;

    const auto lazy_set = set_under_test
        .lazy()
        .map<int>([&map_call_count](const int& value) {
            ++map_call_count;
            return value * 2;
        })
        .filter([&filter_call_count](const int& value) {
            ++filter_call_count;
            return value > 4;
        });

    EXPECT_EQ(0, map_call_count);
    EXPECT_EQ(0, filter_call_count);

    const auto materialized_set = lazy_set.get();
    EXPECT_EQ(set<int>({6, 8}), materialized_set);
    EXPECT_EQ(set<int>({1, 2, 3, 4}), set_under_test);
    EXPECT_EQ(4, map_call_count);
    EXPECT_EQ(4, filter_call_count);
}

TEST(SetTest, LazyFiltered)
{
    const set<int> set_under_test({1, 2, 3, 4});
    const auto filtered_set = set_under_test
        .lazy()
        .filtered([](const int& value) {
            return value % 2 == 0;
        })
        .get();

    EXPECT_EQ(set<int>({2, 4}), filtered_set);
    EXPECT_EQ(set<int>({1, 2, 3, 4}), set_under_test);
}

TEST(SetTest, LazyReduce)
{
    const set<int> set_under_test({1, 2, 3, 4, 5});
    int map_call_count = 0;
    int filter_call_count = 0;

    const auto result = set_under_test
        .lazy()
        .map<int>([&map_call_count](const int& value) {
            ++map_call_count;
            return value * 3;
        })
        .filter([&filter_call_count](const int& value) {
            ++filter_call_count;
            return value > 5;
        })
        .reduce(0, [](const int& partial_sum, const int& value) {
            return partial_sum + value;
        });

    EXPECT_EQ(42, result);
    EXPECT_EQ(5, map_call_count);
    EXPECT_EQ(5, filter_call_count);
}

TEST(SetTest, LazyZipWithFunctionalSet)
{
    const set<int> ages({25, 45, 30, 63});
    const set<std::string> persons({"Jake", "Bob", "Michael", "Philipp"});

    const auto zipped = ages
        .lazy()
        .zip(persons)
        .get();

    const auto expected = set<std::pair<int, std::string>>({
        std::pair<int, std::string>(25, "Bob"),
        std::pair<int, std::string>(30, "Jake"),
        std::pair<int, std::string>(45, "Michael"),
        std::pair<int, std::string>(63, "Philipp"),
    });
    EXPECT_EQ(expected, zipped);
}

TEST(SetTest, LazyZipWithStdSet)
{
    const set<int> ages({25, 45, 30, 63});
    const std::set<std::string> persons({"Jake", "Bob", "Michael", "Philipp"});

    const auto zipped = ages
        .lazy()
        .zip(persons)
        .get();

    const auto expected = set<std::pair<int, std::string>>({
        std::pair<int, std::string>(25, "Bob"),
        std::pair<int, std::string>(30, "Jake"),
        std::pair<int, std::string>(45, "Michael"),
        std::pair<int, std::string>(63, "Philipp"),
    });
    EXPECT_EQ(expected, zipped);
}

TEST(SetTest, LazyZipWithFunctionalVector)
{
    const set<int> ages({25, 45, 30, 63});
    const vector<std::string> persons({"Jake", "Bob", "Michael", "Philipp"});

    const auto zipped = ages
        .lazy()
        .zip(persons)
        .get();

    const auto expected = set<std::pair<int, std::string>>({
        std::pair<int, std::string>(25, "Bob"),
        std::pair<int, std::string>(30, "Jake"),
        std::pair<int, std::string>(45, "Michael"),
        std::pair<int, std::string>(63, "Philipp"),
    });
    EXPECT_EQ(expected, zipped);
}

TEST(SetTest, LazyZipWithStdVector)
{
    const set<int> ages({25, 45, 30, 63});
    const std::vector<std::string> persons({"Jake", "Bob", "Michael", "Philipp"});

    const auto zipped = ages
        .lazy()
        .zip(persons)
        .get();

    const auto expected = set<std::pair<int, std::string>>({
        std::pair<int, std::string>(25, "Bob"),
        std::pair<int, std::string>(30, "Jake"),
        std::pair<int, std::string>(45, "Michael"),
        std::pair<int, std::string>(63, "Philipp"),
    });
    EXPECT_EQ(expected, zipped);
}

TEST(SetTest, LazySourceCanOutliveFunctionalSet)
{
    lazy_set<int> lazy_numbers;
    {
        const set<int> numbers({1, 2, 3, 4});
        lazy_numbers = numbers
            .lazy()
            .filter([](const int& value) {
                return value > 2;
            });
    }

    EXPECT_EQ(set<int>({3, 4}), lazy_numbers.get());
}

TEST(SetTest, LazySourceCanStartFromTemporaryFunctionalSet)
{
    const auto lazy_numbers = set<int>({1, 2, 3, 4})
        .lazy()
        .map<int>([](const int& value) {
            return value * 2;
        });

    EXPECT_EQ(set<int>({2, 4, 6, 8}), lazy_numbers.get());
}

TEST(SetTest, LazyFilterPreservesComparatorState)
{
    const set<int, stateful_descending_int_compare> numbers(make_stateful_descending_set({1, 2, 3, 4}));

    const auto filtered = numbers
        .lazy()
        .filter([](const int& value) {
            return value % 2 == 0;
        })
        .get();

    EXPECT_EQ(2, filtered.size());
    EXPECT_EQ(4, filtered[0]);
    EXPECT_EQ(2, filtered[1]);
}

TEST(SetTest, LazyZipWithTemporaryFunctionalSet)
{
    const set<int> ages({25, 45, 30, 63});

    const auto lazy_zipped = ages
        .lazy()
        .zip(set<std::string>({"Jake", "Bob", "Michael", "Philipp"}));

    const auto expected = set<std::pair<int, std::string>>({
        std::pair<int, std::string>(25, "Bob"),
        std::pair<int, std::string>(30, "Jake"),
        std::pair<int, std::string>(45, "Michael"),
        std::pair<int, std::string>(63, "Philipp"),
    });
    EXPECT_EQ(expected, lazy_zipped.get());
}

TEST(SetTest, LazyZipWithFunctionalSetPreservesComparatorState)
{
    const set<int> left({1, 2, 3});
    const set<int, stateful_descending_int_compare> right(make_stateful_descending_set({10, 20, 30}));

    const auto zipped = left
        .lazy()
        .zip(right)
        .get();

    const auto expected = set<std::pair<int, int>>({
        std::pair<int, int>(1, 30),
        std::pair<int, int>(2, 20),
        std::pair<int, int>(3, 10),
    });
    EXPECT_EQ(expected, zipped);
}

TEST(SetTest, LazyZipWithTemporaryStdSet)
{
    const set<int> ages({25, 45, 30, 63});

    const auto lazy_zipped = ages
        .lazy()
        .zip(std::set<std::string>({"Jake", "Bob", "Michael", "Philipp"}));

    const auto expected = set<std::pair<int, std::string>>({
        std::pair<int, std::string>(25, "Bob"),
        std::pair<int, std::string>(30, "Jake"),
        std::pair<int, std::string>(45, "Michael"),
        std::pair<int, std::string>(63, "Philipp"),
    });
    EXPECT_EQ(expected, lazy_zipped.get());
}

TEST(SetTest, LazyZipWithLazyVector)
{
    const set<int> ages({25, 45, 30, 63});
    const vector<std::string> persons({"Jake", "Bob", "Michael", "Philipp"});
    int map_call_count = 0;

    const auto lazy_persons = persons
        .lazy()
        .map<std::string>([&map_call_count](const std::string& name) {
            ++map_call_count;
            return name + "!";
        });

    const auto lazy_zipped = ages
        .lazy()
        .zip(lazy_persons);

    EXPECT_EQ(0, map_call_count);

    const auto zipped = lazy_zipped.get();

    const auto expected = set<std::pair<int, std::string>>({
        std::pair<int, std::string>(25, "Bob!"),
        std::pair<int, std::string>(30, "Jake!"),
        std::pair<int, std::string>(45, "Michael!"),
        std::pair<int, std::string>(63, "Philipp!"),
    });
    EXPECT_EQ(expected, zipped);
    EXPECT_EQ(4, map_call_count);
}

TEST(SetTest, LazyZipWithLazySet)
{
    const set<int> ages({25, 45, 30, 63});
    const set<std::string> persons({"Jake", "Bob", "Michael", "Philipp"});
    int map_call_count = 0;

    const auto lazy_persons = persons
        .lazy()
        .map<std::string>([&map_call_count](const std::string& name) {
            ++map_call_count;
            return name + "!";
        });

    const auto lazy_zipped = ages
        .lazy()
        .zip(lazy_persons);

    EXPECT_EQ(0, map_call_count);

    const auto zipped = lazy_zipped.get();

    const auto expected = set<std::pair<int, std::string>>({
        std::pair<int, std::string>(25, "Bob!"),
        std::pair<int, std::string>(30, "Jake!"),
        std::pair<int, std::string>(45, "Michael!"),
        std::pair<int, std::string>(63, "Philipp!"),
    });
    EXPECT_EQ(expected, zipped);
    EXPECT_EQ(4, map_call_count);
}

TEST(SetTest, LazyZipWithDifferentSizesThrows)
{
    const set<int> ages({25, 45});
    const std::set<std::string> persons({"Jake"});

    EXPECT_DEATH({ const auto zipped = ages.lazy().zip(persons).get(); }, "");
}

TEST(SetTest, LazyDifferenceWithFunctionalSet)
{
    const set<int> set1({1, 2, 3, 5, 7, 8, 10});
    const set<int> set2({2, 5, 7, 10, 15, 17});
    int filter_call_count = 0;

    const auto lazy_diff = set1
        .lazy()
        .filter([&filter_call_count](const int& value) {
            ++filter_call_count;
            return value < 10;
        })
        .difference_with(set2);

    EXPECT_EQ(0, filter_call_count);

    const auto diff = lazy_diff.get();

    EXPECT_EQ(set<int>({1, 3, 8}), diff);
    EXPECT_EQ(7, filter_call_count);
}

TEST(SetTest, LazyDifferenceWithStdSet)
{
    const set<int> set1({1, 2, 3, 5, 7, 8, 10});
    const std::set<int> set2({2, 5, 7, 10, 15, 17});

    const auto diff = set1
        .lazy()
        .difference_with(set2)
        .get();

    EXPECT_EQ(set<int>({1, 3, 8}), diff);
}

TEST(SetTest, LazyDifferenceWithLazySet)
{
    const set<int> set1({1, 2, 3, 5, 7, 8, 10});
    const set<int> set2({1, 2, 3, 5, 7, 8});
    int map_call_count = 0;

    const auto lazy_set2 = set2
        .lazy()
        .map<int>([&map_call_count](const int& value) {
            ++map_call_count;
            return value + 2;
        });

    const auto lazy_diff = set1
        .lazy()
        .difference_with(lazy_set2);

    EXPECT_EQ(0, map_call_count);

    const auto diff = lazy_diff.get();

    EXPECT_EQ(set<int>({1, 2, 8}), diff);
    EXPECT_EQ(6, map_call_count);
}

TEST(SetTest, LazyDifferenceWithLazySetNormalizesRightHandComparatorState)
{
    const set<int, stateful_descending_int_compare> set1(make_stateful_ordered_set({1, 2, 3, 4}, true));
    const set<int, stateful_descending_int_compare> set2(make_stateful_ordered_set({2, 4, 6}, false));

    const auto diff = set1
        .lazy()
        .difference_with(set2.lazy())
        .get();

    EXPECT_EQ(2, diff.size());
    EXPECT_EQ(3, diff[0]);
    EXPECT_EQ(1, diff[1]);
}

TEST(SetTest, LazyDifferenceWithTemporaryFunctionalSet)
{
    const set<int> set1({1, 2, 3, 5, 7, 8, 10});

    const auto lazy_diff = set1
        .lazy()
        .difference_with(set<int>({2, 5, 7, 10, 15, 17}));

    EXPECT_EQ(set<int>({1, 3, 8}), lazy_diff.get());
}

TEST(SetTest, LazyDifferenceWithFunctionalSetPreservesComparatorState)
{
    const set<int, stateful_descending_int_compare> set1(make_stateful_descending_set({1, 2, 3, 4}));
    const set<int, stateful_descending_int_compare> set2(make_stateful_descending_set({2, 4, 6}));

    const auto diff = set1
        .lazy()
        .difference_with(set2)
        .get();

    EXPECT_EQ(2, diff.size());
    EXPECT_EQ(3, diff[0]);
    EXPECT_EQ(1, diff[1]);
}

TEST(SetTest, LazyUnionWithFunctionalSet)
{
    const set<int> set1({1, 2, 3, 5, 7, 8, 10});
    const set<int> set2({2, 5, 7, 10, 15, 17});
    int filter_call_count = 0;

    const auto lazy_combined = set1
        .lazy()
        .filter([&filter_call_count](const int& value) {
            ++filter_call_count;
            return value < 10;
        })
        .union_with(set2);

    EXPECT_EQ(0, filter_call_count);

    const auto combined = lazy_combined.get();

    EXPECT_EQ(set<int>({1, 2, 3, 5, 7, 8, 10, 15, 17}), combined);
    EXPECT_EQ(7, filter_call_count);
}

TEST(SetTest, LazyUnionWithStdSet)
{
    const set<int> set1({1, 2, 3, 5, 7, 8, 10});
    const std::set<int> set2({2, 5, 7, 10, 15, 17});

    const auto combined = set1
        .lazy()
        .union_with(set2)
        .get();

    EXPECT_EQ(set<int>({1, 2, 3, 5, 7, 8, 10, 15, 17}), combined);
}

TEST(SetTest, LazyUnionWithLazySet)
{
    const set<int> set1({1, 2, 3, 5, 7, 8, 10});
    const set<int> set2({1, 2, 3, 5, 7, 8});
    int map_call_count = 0;

    const auto lazy_set2 = set2
        .lazy()
        .map<int>([&map_call_count](const int& value) {
            ++map_call_count;
            return value + 2;
        });

    const auto lazy_combined = set1
        .lazy()
        .union_with(lazy_set2);

    EXPECT_EQ(0, map_call_count);

    const auto combined = lazy_combined.get();

    EXPECT_EQ(set<int>({1, 2, 3, 4, 5, 7, 8, 9, 10}), combined);
    EXPECT_EQ(6, map_call_count);
}

TEST(SetTest, LazyUnionWithLazySetNormalizesRightHandComparatorState)
{
    const set<int, stateful_descending_int_compare> set1(make_stateful_ordered_set({1, 2, 3, 4}, true));
    const set<int, stateful_descending_int_compare> set2(make_stateful_ordered_set({2, 4, 6}, false));

    const auto combined = set1
        .lazy()
        .union_with(set2.lazy())
        .get();

    EXPECT_EQ(5, combined.size());
    EXPECT_EQ(6, combined[0]);
    EXPECT_EQ(4, combined[1]);
    EXPECT_EQ(3, combined[2]);
    EXPECT_EQ(2, combined[3]);
    EXPECT_EQ(1, combined[4]);
}

TEST(SetTest, LazyUnionWithTemporaryFunctionalSet)
{
    const set<int> set1({1, 2, 3, 5, 7, 8, 10});

    const auto lazy_combined = set1
        .lazy()
        .union_with(set<int>({2, 5, 7, 10, 15, 17}));

    EXPECT_EQ(set<int>({1, 2, 3, 5, 7, 8, 10, 15, 17}), lazy_combined.get());
}

TEST(SetTest, LazyUnionWithFunctionalSetPreservesComparatorState)
{
    const set<int, stateful_descending_int_compare> set1(make_stateful_descending_set({1, 2, 3, 4}));
    const set<int, stateful_descending_int_compare> set2(make_stateful_descending_set({2, 4, 6}));

    const auto combined = set1
        .lazy()
        .union_with(set2)
        .get();

    EXPECT_EQ(5, combined.size());
    EXPECT_EQ(6, combined[0]);
    EXPECT_EQ(4, combined[1]);
    EXPECT_EQ(3, combined[2]);
    EXPECT_EQ(2, combined[3]);
    EXPECT_EQ(1, combined[4]);
}

TEST(SetTest, LazyIntersectionWithFunctionalSet)
{
    const set<int> set1({1, 2, 3, 5, 7, 8, 10});
    const set<int> set2({2, 5, 7, 10, 15, 17});
    int filter_call_count = 0;

    const auto lazy_intersection = set1
        .lazy()
        .filter([&filter_call_count](const int& value) {
            ++filter_call_count;
            return value < 10;
        })
        .intersect_with(set2);

    EXPECT_EQ(0, filter_call_count);

    const auto intersection = lazy_intersection.get();

    EXPECT_EQ(set<int>({2, 5, 7}), intersection);
    EXPECT_EQ(7, filter_call_count);
}

TEST(SetTest, LazyIntersectionWithStdSet)
{
    const set<int> set1({1, 2, 3, 5, 7, 8, 10});
    const std::set<int> set2({2, 5, 7, 10, 15, 17});

    const auto intersection = set1
        .lazy()
        .intersect_with(set2)
        .get();

    EXPECT_EQ(set<int>({2, 5, 7, 10}), intersection);
}

TEST(SetTest, LazyIntersectionWithLazySet)
{
    const set<int> set1({1, 2, 3, 5, 7, 8, 10});
    const set<int> set2({1, 2, 3, 5, 7, 8});
    int map_call_count = 0;

    const auto lazy_set2 = set2
        .lazy()
        .map<int>([&map_call_count](const int& value) {
            ++map_call_count;
            return value + 2;
        });

    const auto lazy_intersection = set1
        .lazy()
        .intersect_with(lazy_set2);

    EXPECT_EQ(0, map_call_count);

    const auto intersection = lazy_intersection.get();

    EXPECT_EQ(set<int>({3, 5, 7, 10}), intersection);
    EXPECT_EQ(6, map_call_count);
}

TEST(SetTest, LazyIntersectionWithLazySetNormalizesRightHandComparatorState)
{
    const set<int, stateful_descending_int_compare> set1(make_stateful_ordered_set({1, 2, 3, 4}, true));
    const set<int, stateful_descending_int_compare> set2(make_stateful_ordered_set({2, 4, 6}, false));

    const auto intersection = set1
        .lazy()
        .intersect_with(set2.lazy())
        .get();

    EXPECT_EQ(2, intersection.size());
    EXPECT_EQ(4, intersection[0]);
    EXPECT_EQ(2, intersection[1]);
}

TEST(SetTest, LazyIntersectionWithTemporaryFunctionalSet)
{
    const set<int> set1({1, 2, 3, 5, 7, 8, 10});

    const auto lazy_intersection = set1
        .lazy()
        .intersect_with(set<int>({2, 5, 7, 10, 15, 17}));

    EXPECT_EQ(set<int>({2, 5, 7, 10}), lazy_intersection.get());
}

TEST(SetTest, LazyIntersectionWithFunctionalSetPreservesComparatorState)
{
    const set<int, stateful_descending_int_compare> set1(make_stateful_descending_set({1, 2, 3, 4}));
    const set<int, stateful_descending_int_compare> set2(make_stateful_descending_set({2, 4, 6}));

    const auto intersection = set1
        .lazy()
        .intersect_with(set2)
        .get();

    EXPECT_EQ(2, intersection.size());
    EXPECT_EQ(4, intersection[0]);
    EXPECT_EQ(2, intersection[1]);
}
