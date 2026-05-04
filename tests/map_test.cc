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

#include <map>
#include <string>
#include <utility>

#include <gtest/gtest.h>

#include "map.h"
#include "warnings.h"

using namespace fcpp;

void test_contents(const map<int, std::string>& map_under_test) {
    EXPECT_EQ(3, map_under_test.size());
    EXPECT_EQ("one", map_under_test[1]);
    EXPECT_EQ("two", map_under_test[2]);
    EXPECT_EQ("three", map_under_test[3]);
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

std::map<int, std::string, stateful_descending_int_compare> make_stateful_descending_map(
    const std::initializer_list<std::pair<const int, std::string>>& values)
{
    std::map<int, std::string, stateful_descending_int_compare> result(stateful_descending_int_compare(true));
    result.insert(values.begin(), values.end());
    return result;
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

TEST(MapTest, LazyMapToFilterGet)
{
    const map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    int map_call_count = 0;
    int filter_call_count = 0;

    const auto lazy_persons = persons
        .lazy()
        .map_to<char, std::string>([&map_call_count](const std::pair<const std::string, int>& element) {
            ++map_call_count;
            return std::make_pair(element.first[0], std::to_string(element.second) + " years");
        })
        .filter([&filter_call_count](const std::pair<const char, std::string>& element) {
            ++filter_call_count;
            return element.first != 'm';
        });

    EXPECT_EQ(0, map_call_count);
    EXPECT_EQ(0, filter_call_count);

    const auto mapped = lazy_persons.get();

    EXPECT_EQ((map<char, std::string>({{'d', "40 years"}, {'j', "32 years"}})), mapped);
    EXPECT_EQ((map<std::string, int>({{"david", 40}, {"jake", 32}, {"mary", 26}})), persons);
    EXPECT_EQ(3, map_call_count);
    EXPECT_EQ(3, filter_call_count);
}

TEST(MapTest, LazyFiltered)
{
    const map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});

    const auto filtered_persons = persons
        .lazy()
        .filtered([](const std::pair<const std::string, int>& element) {
            return element.second >= 32;
        })
        .get();

    EXPECT_EQ((map<std::string, int>({{"david", 40}, {"jake", 32}})), filtered_persons);
    EXPECT_EQ((map<std::string, int>({{"david", 40}, {"jake", 32}, {"mary", 26}})), persons);
}

TEST(MapTest, LazyFilteredPreservesComparatorState)
{
    const map<int, std::string, stateful_descending_int_compare> numbers(
        make_stateful_descending_map({{1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}}));

    const auto filtered_numbers = numbers
        .lazy()
        .filtered([](const std::pair<const int, std::string>& element) {
            return element.first % 2 == 0;
        })
        .get();

    EXPECT_EQ((vector<int>({4, 2})), filtered_numbers.keys());
    EXPECT_EQ("four", filtered_numbers[4]);
    EXPECT_EQ("two", filtered_numbers[2]);
}

TEST(MapTest, LazyReduce)
{
    const map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    int filter_call_count = 0;

    const auto total_age = persons
        .lazy()
        .filter([&filter_call_count](const std::pair<const std::string, int>& element) {
            ++filter_call_count;
            return element.second >= 32;
        })
        .reduce(0, [](const int& partial_sum, const std::pair<const std::string, int>& element) {
            return partial_sum + element.second;
        });

    EXPECT_EQ(72, total_age);
    EXPECT_EQ(3, filter_call_count);
}

TEST(MapTest, LazySourceCanOutliveFunctionalMap)
{
    lazy_map<std::string, int> lazy_persons;
    {
        const map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
        lazy_persons = persons
            .lazy()
            .filter([](const std::pair<const std::string, int>& element) {
                return element.second >= 32;
            });
    }

    EXPECT_EQ((map<std::string, int>({{"david", 40}, {"jake", 32}})), lazy_persons.get());
}

TEST(MapTest, LazySourceCanStartFromTemporaryFunctionalMap)
{
    const auto lazy_persons = map<std::string, int>({{"jake", 32}, {"mary", 26}})
        .lazy()
        .map_to<char, int>([](const std::pair<const std::string, int>& element) {
            return std::make_pair(element.first[0], element.second);
        });

    EXPECT_EQ((map<char, int>({{'j', 32}, {'m', 26}})), lazy_persons.get());
}

TEST(MapTest, LazyMapToDuplicateKeysKeepsFirst)
{
    const map<std::string, int> persons({{"anna", 28}, {"alex", 30}, {"david", 40}});

    const auto mapped = persons
        .lazy()
        .map_to<char, int>([](const std::pair<const std::string, int>& element) {
            return std::make_pair(element.first[0], element.second);
        })
        .get();

    EXPECT_EQ((map<char, int>({{'a', 30}, {'d', 40}})), mapped);
}
