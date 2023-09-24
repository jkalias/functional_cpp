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
    EXPECT_EQ("one", map[1]);
    EXPECT_EQ("two", map[2]);
    EXPECT_EQ("three", map[3]);
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

TEST(MapTest, AccessConstOperator)
{
    const map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    EXPECT_EQ(32, persons["jake"]);
}

TEST(MapTest, AccessOperator)
{
    map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    EXPECT_EQ(0, persons["john"]);
}

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

TEST(MapTest, Clear)
{
    map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    persons.clear();
    EXPECT_EQ(0, persons.size());
}

TEST(MapTest, IsEmpty)
{
    const map<int, std::string> data({{1, "one"}, {2, "two"}, {3, "three"}});
    EXPECT_FALSE(data.is_empty());
    map<int, std::string> empty_map;
    EXPECT_TRUE(empty_map.is_empty());
}

TEST(MapTest, Contains)
{
    const map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    EXPECT_TRUE(persons.contains("jake"));
    EXPECT_FALSE(persons.contains("bob"));
}

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
