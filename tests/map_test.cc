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
#include "map.h"
#include "vector.h"
#include "test_types.h"
#include <string>

using namespace fcpp;

void test_contents(const map<int, std::string>& map) {
    EXPECT_EQ(3, map.size());
    EXPECT_EQ(std::string("one"), map[1]);
    EXPECT_EQ(std::string("two"), map[2]);
    EXPECT_EQ(std::string("three"), map[3]);
}

TEST(MapTest, EmptyConstructor)
{
    const map<int, std::string> map_under_test;
    EXPECT_EQ(0, map_under_test.size());
}

TEST(MapTest, StdMapConstructor)
{
    const map<int, std::string> map_under_test(std::map<int, std::string>{{1, "one"}, {2, "two"}, {3, "three"}});
    test_contents(map_under_test);
}

TEST(MapTest, StdInitializerListConstructor)
{
    const map<int, std::string> map_under_test({{1, "one"}, {2, "two"}, {3, "three"}});
    test_contents(map_under_test);
}

//TEST(MapTest, DifferenceFunctionalSetCustomType)
//{
//    const set<person, person_comparator> set1({
//        person(51, "George"),
//        person(81, "Jackie"),
//        person(15, "Jake"),
//        person(18, "Jannet"),
//        person(25, "Kate")
//    });
//    
//    const set<person, person_comparator> set2({
//        person(51, "George"),
//        person(81, "Jackie"),
//    });
//    
//    const auto& diff = set1.difference_with(set2);
//    
//    const set<person, person_comparator> expected({
//        person(15, "Jake"),
//        person(18, "Jannet"),
//        person(25, "Kate")
//    });
//    
//    EXPECT_EQ(expected, diff);
//}
//
//TEST(MapTest, Union)
//{
//    const set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
//    const set<int> set2(std::set<int>({2, 5, 7, 10, 15, 17}));
//    const auto& combined = set1.union_with(set2);
//    EXPECT_EQ(set<int>({1, 2, 3, 5, 7, 8, 10, 15, 17}), combined);
//}
//
//TEST(MapTest, UnionStdSet)
//{
//    const set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
//    const std::set<int> set2({2, 5, 7, 10, 15, 17});
//    const auto& combined = set1.union_with(set2);
//    EXPECT_EQ(set<int>({1, 2, 3, 5, 7, 8, 10, 15, 17}), combined);
//}
//
//TEST(MapTest, UnionFunctionalSet)
//{
//    const set<int> set1({1, 2, 3, 5, 7, 8, 10});
//    const set<int> set2({2, 5, 7, 10, 15, 17});
//    const auto& combined = set1.union_with(set2);
//    EXPECT_EQ(set<int>({1, 2, 3, 5, 7, 8, 10, 15, 17}), combined);
//}
//
//TEST(MapTest, UnionFunctionalSetCustomType)
//{
//    const set<person, person_comparator> set1({
//        person(15, "Jake"),
//        person(18, "Jannet"),
//        person(25, "Kate")
//    });
//    
//    const set<person, person_comparator> set2({
//        person(51, "George"),
//        person(81, "Jackie"),
//    });
//    
//    const auto& combined = set1.union_with(set2);
//    
//    const set<person, person_comparator> expected({
//        person(15, "Jake"),
//        person(18, "Jannet"),
//        person(25, "Kate"),
//        person(51, "George"),
//        person(81, "Jackie"),
//    });
//    
//    EXPECT_EQ(expected, combined);
//}
//
//TEST(MapTest, Intersection)
//{
//    const set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
//    const set<int> set2(std::set<int>({2, 5, 7, 10, 15, 17}));
//    const auto& intersection = set1.intersect_with(set2);
//    EXPECT_EQ(set<int>({2, 5, 7, 10}), intersection);
//}
//
//TEST(MapTest, IntersectionStdSet)
//{
//    const set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
//    const std::set<int> set2({2, 5, 7, 10, 15, 17});
//    const auto& intersection = set1.intersect_with(set2);
//    EXPECT_EQ(set<int>({2, 5, 7, 10}), intersection);
//}
//
//TEST(MapTest, IntersectionFunctionalSet)
//{
//    const set<int> set1({1, 2, 3, 5, 7, 8, 10});
//    const set<int> set2({2, 5, 7, 10, 15, 17});
//    const auto& intersection = set1.intersect_with(set2);
//    EXPECT_EQ(set<int>({2, 5, 7, 10}), intersection);
//}
//
//TEST(MapTest, IntersectionFunctionalSetCustomType)
//{
//    const set<person, person_comparator> set1({
//        person(15, "Jake"),
//        person(18, "Jannet"),
//        person(25, "Kate"),
//        person(51, "George"),
//        person(81, "Jackie"),
//    });
//    
//    const set<person, person_comparator> set2({
//        person(39, "Robert"),
//        person(18, "Jannet"),
//        person(25, "Kate"),
//        person(52, "Anna"),
//        person(63, "Simon"),
//    });
//    
//    const auto& intersection = set1.intersect_with(set2);
//    
//    const set<person, person_comparator> expected({
//        person(18, "Jannet"),
//        person(25, "Kate"),
//    });
//    
//    EXPECT_EQ(expected, intersection);
//}
//
//TEST(MapTest, Min)
//{
//    const set<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
//    const auto minimum = numbers.min();
//    EXPECT_TRUE(minimum.has_value());
//    EXPECT_EQ(1, minimum.value());
//}
//
//TEST(MapTest, MinCustomType)
//{
//    const set<person, person_comparator> persons({
//        person(15, "Jake"),
//        person(18, "Jannet"),
//        person(25, "Kate"),
//        person(62, "Bob")
//    });
//    const auto minimum = persons.min();
//#if defined(__clang__)
//    EXPECT_EQ(person(18, "Jannet"), minimum.value());
//#else
//    EXPECT_EQ(person(62, "Bob"), minimum.value());
//#endif
//}
//
//TEST(MapTest, MinEmptySet)
//{
//    const set<int> numbers;
//    const auto minimum = numbers.min();
//    EXPECT_FALSE(minimum.has_value());
//}
//
//TEST(MapTest, Max)
//{
//    const set<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
//    const auto maximum = numbers.max();
//    EXPECT_TRUE(maximum.has_value());
//    EXPECT_EQ(8, maximum.value());
//}
//
//TEST(MapTest, MaxCustomType)
//{
//    const set<person, person_comparator> persons({
//        person(15, "Jake"),
//        person(18, "Jannet"),
//        person(25, "Kate"),
//        person(62, "Bob")
//    });
//    const auto maximum = persons.max();
//    std::cout << maximum.value().name << std::endl;
//#if __linux__  // NOLINT(clang-diagnostic-undef)
//    EXPECT_EQ(person(18, "Jannet"), maximum.value());
//#else
//    EXPECT_EQ(person(25, "Kate"), maximum.value());
//#endif
//}
//
//TEST(MapTest, MaxEmptySet)
//{
//    const set<int> numbers;
//    const auto maximum = numbers.max();
//    EXPECT_FALSE(maximum.has_value());
//}
//
//TEST(MapTest, Map)
//{
//    const set<int> numbers({4, 1, 3});
//    const auto mapped_set = numbers.map<child>([](const int& age) {
//        return child(age);
//    });
//    EXPECT_EQ(3, mapped_set.size());
//    EXPECT_EQ(1, mapped_set[0].age);
//    EXPECT_EQ(3, mapped_set[1].age);
//    EXPECT_EQ(4, mapped_set[2].age);
//}
//
//TEST(MapTest, AllOf)
//{
//    const set<int> numbers({1, 4, 2, 5, 8, 3});
//    EXPECT_TRUE(numbers.all_of([](const int &number) { return number < 10; }));
//    EXPECT_FALSE(numbers.all_of([](const int &number) { return number > 2; }));
//}
//
//TEST(MapTest, AnyOf)
//{
//    const set<int> numbers({1, 4, 2, 5, 8, 3});
//    EXPECT_TRUE(numbers.any_of([](const int &number) { return number < 5; }));
//    EXPECT_FALSE(numbers.any_of([](const int &number) { return number > 10; }));
//}
//
//TEST(MapTest, NoneOf)
//{
//    const set<int> numbers({1, 4, 2, 5, 8, 3});
//    EXPECT_TRUE(numbers.none_of([](const int &number) { return number > 10; }));
//    EXPECT_FALSE(numbers.none_of([](const int &number) { return number < 6; }));
//}
//
//TEST(MapTest, Reduce)
//{
//    const set<std::string> tokens({ "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "brown", "dog" });
//    const auto sentence = tokens.reduce(std::string(""), [](const std::string& partial, const std::string& token) {
//        return partial.length() != 0
//            ? partial + " " + token
//            : token;
//    });
//    EXPECT_EQ("brown dog fox jumps lazy over quick the", sentence);
//}
//
//TEST(MapTest, Filter)
//{
//    set<int> numbers({ 1, 3, -5, 2, -1, 9, -4 });
//    numbers.filter([](const int& element) {
//        return element >= 1.5;
//    });
//    EXPECT_EQ(set<int>({2, 3, 9}), numbers);
//}
//
//TEST(MapTest, Filtered)
//{
//    const set<int> numbers({ 1, 3, -5, 2, -1, 9, -4 });
//    auto filtered_numbers = numbers.filtered([](const int& element) {
//        return element >= 1.5;
//    });
//    EXPECT_EQ(set<int>({2, 3, 9}), filtered_numbers);
//    EXPECT_EQ(set<int>({ 1, 3, -5, 2, -1, 9, -4 }), numbers);
//}
//
//TEST(MapTest, ZipWithFunctionalSet)
//{
//    const set<int> ages({ 25, 45, 30, 63 });
//    const set<std::string> persons({ "Jake", "Bob", "Michael", "Philipp" });
//    const auto zipped = ages.zip(persons);
//    const auto expected = set<std::pair<int, std::string>>({
//        std::pair<int, std::string>(25, "Bob"),
//        std::pair<int, std::string>(30, "Jake"),
//        std::pair<int, std::string>(45, "Michael"),
//        std::pair<int, std::string>(63, "Philipp"),
//    });
//    EXPECT_EQ(expected, zipped);
//}
//
//TEST(MapTest, ZipWithFunctionalSetDifferentSizes)
//{
//    const set<int> ages({ 25, 45, 30, 63 });
//    const set<std::string> persons({ "Jake" });
//    EXPECT_DEATH(ages.zip(persons), "");
//}
//
//TEST(MapTest, ZipWithStdSet)
//{
//    const set<int> ages({ 25, 45, 30, 63 });
//    const std::set<std::string> persons({ "Jake", "Bob", "Michael", "Philipp" });
//    const auto zipped = ages.zip(persons);
//    const auto expected = set<std::pair<int, std::string>>({
//        std::pair<int, std::string>(25, "Bob"),
//        std::pair<int, std::string>(30, "Jake"),
//        std::pair<int, std::string>(45, "Michael"),
//        std::pair<int, std::string>(63, "Philipp"),
//    });
//    EXPECT_EQ(expected, zipped);
//}
//
//TEST(MapTest, ZipWithStdSetDifferentSizes)
//{
//    const set<int> ages({ 25, 45, 30, 63 });
//    const std::set<std::string> persons({ "Jake" });
//    EXPECT_DEATH(ages.zip(persons), "");
//}
//
//TEST(MapTest, ZipWithFunctionalVector)
//{
//    const set<int> ages({ 25, 45, 30, 63 });
//    const vector<std::string> persons({ "Jake", "Bob", "Michael", "Philipp" });
//    const auto zipped = ages.zip(persons);
//    const auto expected = set<std::pair<int, std::string>>({
//        std::pair<int, std::string>(25, "Bob"),
//        std::pair<int, std::string>(30, "Jake"),
//        std::pair<int, std::string>(45, "Michael"),
//        std::pair<int, std::string>(63, "Philipp"),
//    });
//    EXPECT_EQ(expected, zipped);
//}
//
//TEST(MapTest, ZipWithFunctionalVectorDifferentSizes)
//{
//    const set<int> ages({ 25, 45, 30, 63 });
//    const vector<std::string> persons({ "Jake" });
//    EXPECT_DEATH(ages.zip(persons), "");
//}
//
//TEST(MapTest, ZipWithStdVector)
//{
//    const set<int> ages({ 25, 45, 30, 63 });
//    const std::vector<std::string> persons({ "Jake", "Bob", "Michael", "Philipp" });
//    const auto zipped = ages.zip(persons);
//    const auto expected = set<std::pair<int, std::string>>({
//        std::pair<int, std::string>(25, "Bob"),
//        std::pair<int, std::string>(30, "Jake"),
//        std::pair<int, std::string>(45, "Michael"),
//        std::pair<int, std::string>(63, "Philipp"),
//    });
//    EXPECT_EQ(expected, zipped);
//}
//
//TEST(MapTest, ZipWithStdVectorDifferentSizes)
//{
//    const set<int> ages({ 25, 45, 30, 63 });
//    const std::vector<std::string> persons({ "Jake" });
//    EXPECT_DEATH(ages.zip(persons), "");
//}
//
//TEST(MapTest, Keys)
//{
//    const set<int> numbers({ 25, 45, 30, 63 });
//    const auto keys = numbers.keys();
//    EXPECT_EQ(vector<int>({25, 30, 45, 63}), keys);
//}
//
//TEST(MapTest, RemoveExistingElement)
//{
//    set<int> numbers({1, 4, 2});
//    numbers.remove(4);
//    EXPECT_EQ(set<int>({1, 2}), numbers);
//}
//
//TEST(MapTest, RemoveNonExistentElement)
//{
//    set<int> numbers({1, 4, 2});
//    numbers.remove(18);
//    EXPECT_EQ(set<int>({1, 2, 4}), numbers);
//}
//
//TEST(MapTest, RemovingExistingElement)
//{
//    const set<int> numbers({1, 4, 2});
//    const auto less_numbers = numbers.removing(4);
//    EXPECT_EQ(set<int>({1, 2}), less_numbers);
//    EXPECT_EQ(set<int>({1, 2, 4}), numbers);
//}
//
//TEST(MapTest, InsertNewElement)
//{
//    set<int> numbers({1, 4, 2});
//    numbers.insert(18);
//    EXPECT_EQ(set<int>({1, 2, 4, 18}), numbers);
//}
//
//TEST(MapTest, InsertingNewElement)
//{
//    const set<int> numbers({1, 4, 2});
//    const auto augmented_numbers =  numbers.inserting(18);
//    EXPECT_EQ(set<int>({1, 2, 4, 18}), augmented_numbers);
//    EXPECT_EQ(set<int>({1, 2, 4}), numbers);
//}
//
//TEST(MapTest, InsertExistingElement)
//{
//    set<int> numbers({1, 4, 2});
//    numbers.insert(2);
//    EXPECT_EQ(set<int>({1, 2, 4}), numbers);
//}
//
//TEST(MapTest, InsertingExistingElement)
//{
//    const set<int> numbers({1, 4, 2});
//    const auto augmented_numbers =  numbers.inserting(2);
//    EXPECT_EQ(set<int>({1, 2, 4}), augmented_numbers);
//    EXPECT_EQ(set<int>({1, 2, 4}), numbers);
//}
//
//TEST(MapTest, Clear)
//{
//    set<int> numbers({1, 4, 2});
//    numbers.clear();
//    EXPECT_EQ(0, numbers.size());
//}
//
//TEST(MapTest, Clearing)
//{
//    const set<int> numbers({1, 4, 2});
//    const auto cleared_numbers = numbers.clearing();
//    EXPECT_EQ(0, cleared_numbers.size());
//    EXPECT_EQ(3, numbers.size());
//}

TEST(MapTest, IsEmpty)
{
    const map<int, std::string> data({{1, "one"}, {2, "two"}, {3, "three"}});
    EXPECT_FALSE(data.is_empty());
    map<int, std::string> empty_map;
    EXPECT_TRUE(empty_map.is_empty());
}

//TEST(MapTest, Contains)
//{
//    const set<int> numbers({1, 4, 2});
//    EXPECT_TRUE(numbers.contains(1));
//    EXPECT_FALSE(numbers.contains(15));
//}
//
//TEST(MapTest, EqualityOperator)
//{
//    const set<int> set1(std::set<int>({1, 2, 3}));
//    const set<int> set2(std::set<int>({1, 2, 3, 2, 3}));
//    EXPECT_TRUE(set1 == set2);
//    EXPECT_FALSE(set1 != set2);
//}
//
//TEST(MapTest, InequalityOperator)
//{
//    const set<int> set1(std::set<int>({1, 2, 3}));
//    const set<int> set2(std::set<int>({1, 2, 3, 4}));
//    EXPECT_FALSE(set1 == set2);
//    EXPECT_TRUE(set1 != set2);
//}
//
//TEST(MapTest, EqualityOperatorCustomType)
//{
//    const set<person, person_comparator> set1({
//        person(15, "Jake"),
//        person(18, "Jannet"),
//        person(25, "Kate")
//    });
//    
//    const set<person, person_comparator> set2({
//        person(15, "Jake"),
//        person(18, "Jannet"),
//        person(25, "Kate")
//    });
//
//    EXPECT_TRUE(set1 == set2);
//    EXPECT_FALSE(set1 != set2);
//}
