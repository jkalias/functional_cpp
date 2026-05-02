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

#include <gtest/gtest.h>
#include "warnings.h"
#include "map.h"
#include <string>

using namespace fcpp;

void test_contents(const map<int, std::string>& map_under_test) {
    EXPECT_EQ(3, map_under_test.size());
    EXPECT_EQ("one", map_under_test[1]);
    EXPECT_EQ("two", map_under_test[2]);
    EXPECT_EQ("three", map_under_test[3]);
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

TEST(MapTest, MapTo)
{
    const map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    const auto mapped = persons.map_to<char, std::string>([](const std::pair<const std::string, int>& element) {
        return std::make_pair(element.first[0], std::to_string(element.second) + " years");
    });

    EXPECT_EQ((map<char, std::string>({{'d', "40 years"}, {'j', "32 years"}, {'m', "26 years"}})), mapped);
}

TEST(MapTest, AllOf)
{
    const map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    EXPECT_TRUE(persons.all_of([](const std::pair<const std::string, int>& element) {
        return element.second > 20;
    }));
    EXPECT_FALSE(persons.all_of([](const std::pair<const std::string, int>& element) {
        return element.second < 35;
    }));
}

TEST(MapTest, AnyOf)
{
    const map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    EXPECT_TRUE(persons.any_of([](const std::pair<const std::string, int>& element) {
        return element.second == 40;
    }));
    EXPECT_FALSE(persons.any_of([](const std::pair<const std::string, int>& element) {
        return element.second > 50;
    }));
}

TEST(MapTest, NoneOf)
{
    const map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    EXPECT_TRUE(persons.none_of([](const std::pair<const std::string, int>& element) {
        return element.second < 18;
    }));
    EXPECT_FALSE(persons.none_of([](const std::pair<const std::string, int>& element) {
        return element.second == 26;
    }));
}

TEST(MapTest, Reduce)
{
    const map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    const auto total_age = persons.reduce(0, [](const int& partial_sum, const std::pair<const std::string, int>& element) {
        return partial_sum + element.second;
    });
    EXPECT_EQ(98, total_age);
}

TEST(MapTest, Filter)
{
    map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    persons.filter([](const std::pair<const std::string, int>& element) {
        return element.second >= 32;
    });
    EXPECT_EQ((map<std::string, int>({{"david", 40}, {"jake", 32}})), persons);
}

TEST(MapTest, Filtered)
{
    const map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    const auto filtered_persons = persons.filtered([](const std::pair<const std::string, int>& element) {
        return element.second >= 32;
    });

    EXPECT_EQ((map<std::string, int>({{"david", 40}, {"jake", 32}})), filtered_persons);
    EXPECT_EQ((map<std::string, int>({{"david", 40}, {"jake", 32}, {"mary", 26}})), persons);
}

TEST(MapTest, ForEach)
{
    const map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    auto total_age = 0;
    persons.for_each([&total_age](const std::pair<const std::string, int>& element) {
        total_age += element.second;
    });
    EXPECT_EQ(98, total_age);
}

TEST(MapTest, Keys)
{
    const map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    EXPECT_EQ(vector<std::string>({"david", "jake", "mary"}), persons.keys());
}

TEST(MapTest, Values)
{
    const map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    EXPECT_EQ(vector<int>({40, 32, 26}), persons.values());
}

TEST(MapTest, RemoveExistingElement)
{
    map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    persons.remove("mary");
    EXPECT_EQ((map<std::string, int>({{"david", 40}, {"jake", 32}})), persons);
}

TEST(MapTest, RemoveNonExistentElement)
{
    map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    persons.remove("bob");
    EXPECT_EQ((map<std::string, int>({{"david", 40}, {"jake", 32}, {"mary", 26}})), persons);
}

TEST(MapTest, RemovingExistingElement)
{
    const map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    const auto reduced_persons = persons.removing("mary");
    EXPECT_EQ((map<std::string, int>({{"david", 40}, {"jake", 32}})), reduced_persons);
    EXPECT_EQ((map<std::string, int>({{"david", 40}, {"jake", 32}, {"mary", 26}})), persons);
}

TEST(MapTest, InsertNewElement)
{
    map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    persons.insert("anna", 28);
    EXPECT_EQ((map<std::string, int>({{"anna", 28}, {"david", 40}, {"jake", 32}, {"mary", 26}})), persons);
}

TEST(MapTest, InsertingNewElement)
{
    const map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    const auto augmented_persons = persons.inserting("anna", 28);
    EXPECT_EQ((map<std::string, int>({{"anna", 28}, {"david", 40}, {"jake", 32}, {"mary", 26}})), augmented_persons);
    EXPECT_EQ((map<std::string, int>({{"david", 40}, {"jake", 32}, {"mary", 26}})), persons);
}

TEST(MapTest, InsertExistingElement)
{
    map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    persons.insert("jake", 999);
    EXPECT_EQ((map<std::string, int>({{"david", 40}, {"jake", 32}, {"mary", 26}})), persons);
}

TEST(MapTest, InsertingExistingElement)
{
    const map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    const auto augmented_persons = persons.inserting("jake", 999);
    EXPECT_EQ((map<std::string, int>({{"david", 40}, {"jake", 32}, {"mary", 26}})), augmented_persons);
    EXPECT_EQ((map<std::string, int>({{"david", 40}, {"jake", 32}, {"mary", 26}})), persons);
}

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

TEST(MapTest, EqualityOperator)
{
    const map<int, std::string> map1({{1, "one"}, {2, "two"}, {3, "three"}});
    const map<int, std::string> map2({{1, "one"}, {2, "two"}, {3, "three"}});
    EXPECT_TRUE(map1 == map2);
    EXPECT_FALSE(map1 != map2);
}

TEST(MapTest, InequalityOperator)
{
    const map<int, std::string> map1({{1, "one"}, {2, "two"}, {3, "three"}});
    const map<int, std::string> map2({{1, "one"}, {2, "two"}, {4, "four"}});
    EXPECT_FALSE(map1 == map2);
    EXPECT_TRUE(map1 != map2);
}
