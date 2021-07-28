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
#include <string>
#include "functional_vector.h"
#include "index_range.h"

struct child {
    child(int age)
    : age(age)
    {}
    int age;
};

struct person {
    person(int age, std::string name)
    : age(age), name(std::move(name))
    {}
    int age;
    std::string name;
};

template<typename T>
void debug_vector(const functional_vector<T>& vec) {
    vec.for_each([](const auto& element) {
        std::cout << element << std::endl;
    });
}

TEST(FunctionalVectorTest, EqualityOperatorEmptyVectorsTest) {
    const auto vec1 = functional_vector<int>();
    const auto vec2 = functional_vector<int>();
    EXPECT_TRUE(vec1 == vec2);
    EXPECT_FALSE(vec1 != vec2);
}

TEST(FunctionalVectorTest, EqualityOperatorUnequalSizesTest) {
    const auto vec1 = functional_vector({1, 2, 3});
    const auto vec2 = functional_vector({1, 2, 3, 4});
    EXPECT_TRUE(vec1 != vec2);
    EXPECT_FALSE(vec1 == vec2);
}

TEST(FunctionalVectorTest, EqualityOperatorEqualSizesDifferentElementsTest) {
    const auto vec1 = functional_vector({1, 2, 3});
    const auto vec2 = functional_vector({1, 2, 4});
    EXPECT_TRUE(vec1 != vec2);
    EXPECT_FALSE(vec1 == vec2);
}

TEST(FunctionalVectorTest, EqualityOperatorEqualVectorsTest) {
    const auto vec1 = functional_vector({1, 2, 3});
    const auto vec2 = functional_vector({1, 2, 3});
    EXPECT_TRUE(vec1 == vec2);
    EXPECT_FALSE(vec1 != vec2);
}

TEST(FunctionalVectorTest, AddTest) {
	auto vector_under_test = functional_vector<int>();
	EXPECT_EQ(0, vector_under_test.size());

	vector_under_test.add(5);
    EXPECT_EQ(1, vector_under_test.size());
	EXPECT_EQ(5, vector_under_test[0]);
	
	vector_under_test.add(-1);
	EXPECT_EQ(2, vector_under_test.size());
	EXPECT_EQ(-1, vector_under_test[1]);
}

TEST(FunctionalVectorTest, AddingTest) {
    const auto vector_under_test = functional_vector<int>();
    const auto vector_new_instance = vector_under_test.adding(5);
    EXPECT_EQ(0, vector_under_test.size());
    EXPECT_EQ(1, vector_new_instance.size());
    EXPECT_EQ(5, vector_new_instance[0]);
}

TEST(FunctionalVectorTest, AddRangeFromFunctionalVectorTest) {
    auto vector_under_test = functional_vector<int>();
    vector_under_test.add_range(functional_vector({1, 2, 3}));
    EXPECT_EQ(3, vector_under_test.size());
    EXPECT_EQ(1, vector_under_test[0]);
    EXPECT_EQ(2, vector_under_test[1]);
    EXPECT_EQ(3, vector_under_test[2]);
}

TEST(FunctionalVectorTest, AddRangeFromStdVectorTest) {
    auto vector_under_test = functional_vector<int>();
    vector_under_test.add_range(std::vector({1, 2, 3}));
    EXPECT_EQ(3, vector_under_test.size());
    EXPECT_EQ(1, vector_under_test[0]);
    EXPECT_EQ(2, vector_under_test[1]);
    EXPECT_EQ(3, vector_under_test[2]);
}

TEST(FunctionalVectorTest, AddRangeFromInitializerListTest) {
    auto vector_under_test = functional_vector<int>();
    vector_under_test.add_range({1, 2, 3});
    EXPECT_EQ(3, vector_under_test.size());
    EXPECT_EQ(1, vector_under_test[0]);
    EXPECT_EQ(2, vector_under_test[1]);
    EXPECT_EQ(3, vector_under_test[2]);
}

TEST(FunctionalVectorTest, AddingRangeFromFunctionalVectorTest) {
    const auto vector_under_test = functional_vector<int>();
    const auto vector_new_instance = vector_under_test.adding_range(functional_vector({1, 2, 3}));
    EXPECT_EQ(0, vector_under_test.size());
    EXPECT_EQ(3, vector_new_instance.size());
    EXPECT_EQ(1, vector_new_instance[0]);
    EXPECT_EQ(2, vector_new_instance[1]);
    EXPECT_EQ(3, vector_new_instance[2]);
}

TEST(FunctionalVectorTest, AddingRangeFromStdVectorTest) {
    const auto vector_under_test = functional_vector<int>();
    const auto vector_new_instance = vector_under_test.adding_range(std::vector {1, 2, 3});
    EXPECT_EQ(0, vector_under_test.size());
    EXPECT_EQ(3, vector_new_instance.size());
    EXPECT_EQ(1, vector_new_instance[0]);
    EXPECT_EQ(2, vector_new_instance[1]);
    EXPECT_EQ(3, vector_new_instance[2]);
}

TEST(FunctionalVectorTest, AddingRangeFromInitializerListTest) {
    const auto vector_under_test = functional_vector<int>();
    const auto vector_new_instance = vector_under_test.adding_range({1, 2, 3});
    EXPECT_EQ(0, vector_under_test.size());
    EXPECT_EQ(3, vector_new_instance.size());
    EXPECT_EQ(1, vector_new_instance[0]);
    EXPECT_EQ(2, vector_new_instance[1]);
    EXPECT_EQ(3, vector_new_instance[2]);
}

TEST(FunctionalVectorTest, MapTest) {
    const auto vector_under_test = functional_vector({1, 3, 4});
    const auto mapped_vector = vector_under_test.map<child>([](const auto& age) {
        return child(age);
    });
    EXPECT_EQ(3, mapped_vector.size());
    EXPECT_EQ(1, mapped_vector[0].age);
    EXPECT_EQ(3, mapped_vector[1].age);
    EXPECT_EQ(4, mapped_vector[2].age);
}

TEST(FunctionalVectorTest, FilterTest) {
    auto vector_under_test = functional_vector({child(1), child(3), child(4)});
    vector_under_test.filter([](const auto& child) {
        return child.age < 2;
    });
    EXPECT_EQ(1, vector_under_test.size());
    EXPECT_EQ(1, vector_under_test[0].age);
    vector_under_test.filter([](const auto& child) {
        return child.age > 7;
    });
    EXPECT_EQ(0, vector_under_test.size());
}

TEST(FunctionalVectorTest, FilteredTest) {
    const auto vector_under_test = functional_vector({child(1), child(3), child(4)});
    const auto filtered_vector = vector_under_test.filtered([](const auto& child) {
        return child.age < 2;
    });
    EXPECT_EQ(3, vector_under_test.size());
    EXPECT_EQ(1, filtered_vector.size());
    EXPECT_EQ(1, filtered_vector[0].age);
}

TEST(FunctionalVectorTest, ReverseTest) {
    auto vector_under_test = functional_vector({child(6), child(2), child(9)});
    vector_under_test.reverse();
    EXPECT_EQ(3, vector_under_test.size());
    EXPECT_EQ(9, vector_under_test[0].age);
    EXPECT_EQ(2, vector_under_test[1].age);
    EXPECT_EQ(6, vector_under_test[2].age);
}

TEST(FunctionalVectorTest, ReversedTest) {
    const auto vector_under_test = functional_vector({child(6), child(2), child(9)});
    const auto reversed_vector = vector_under_test.reversed();
    EXPECT_EQ(3, reversed_vector.size());
    EXPECT_EQ(9, reversed_vector[0].age);
    EXPECT_EQ(2, reversed_vector[1].age);
    EXPECT_EQ(6, reversed_vector[2].age);
    
    EXPECT_EQ(3, vector_under_test.size());
    EXPECT_EQ(6, vector_under_test[0].age);
    EXPECT_EQ(2, vector_under_test[1].age);
    EXPECT_EQ(9, vector_under_test[2].age);
}

#ifndef NDEBUG
TEST(FunctionalVectorTest, ZipWithStdVectorUnequalSizesThrowsTest) {
    const auto ages_vector = functional_vector({32, 25, 53, 62});
    EXPECT_DEATH({ const auto zipped_vector = ages_vector.zip(std::vector{"Jake", "Mary"}); }, "");
}

TEST(FunctionalVectorTest, ZipWithFunctionalVectorUnequalSizesThrowsTest) {
    const auto ages_vector = functional_vector({32, 25, 53, 62});
    const auto names_vector = functional_vector<std::string>({"Jake", "Mary"});
    EXPECT_DEATH({ const auto zipped_vector = ages_vector.zip(names_vector); }, "");
}
#endif

TEST(FunctionalVectorTest, ZipWithFunctionalVectorTest) {
    const auto ages_vector = functional_vector({32, 25, 53});
    const auto names_vector = functional_vector<std::string>({"Jake", "Mary", "John"});
    const auto zipped_vector = ages_vector.zip(names_vector);
    EXPECT_EQ(3, zipped_vector.size());
    
    EXPECT_EQ(32, zipped_vector[0].first);
    EXPECT_EQ("Jake", zipped_vector[0].second);
    
    EXPECT_EQ(25, zipped_vector[1].first);
    EXPECT_EQ("Mary", zipped_vector[1].second);
    
    EXPECT_EQ(53, zipped_vector[2].first);
    EXPECT_EQ("John", zipped_vector[2].second);
}

TEST(FunctionalVectorTest, ZipWithStdVectorTest) {
    const auto ages_vector = functional_vector({32, 25, 53});
    const auto zipped_vector = ages_vector.zip(std::vector<std::string>{"Jake", "Mary", "John"});
    EXPECT_EQ(3, zipped_vector.size());
    
    EXPECT_EQ(32, zipped_vector[0].first);
    EXPECT_EQ("Jake", zipped_vector[0].second);
    
    EXPECT_EQ(25, zipped_vector[1].first);
    EXPECT_EQ("Mary", zipped_vector[1].second);
    
    EXPECT_EQ(53, zipped_vector[2].first);
    EXPECT_EQ("John", zipped_vector[2].second);
}

TEST(FunctionalVectorTest, ZipWithInitializerListTest) {
    const auto ages_vector = functional_vector({32, 25, 53});
    const auto zipped_vector = ages_vector.zip(std::initializer_list<std::string>{"Jake", "Mary", "John"});
    EXPECT_EQ(3, zipped_vector.size());
    
    EXPECT_EQ(32, zipped_vector[0].first);
    EXPECT_EQ("Jake", zipped_vector[0].second);
    
    EXPECT_EQ(25, zipped_vector[1].first);
    EXPECT_EQ("Mary", zipped_vector[1].second);
    
    EXPECT_EQ(53, zipped_vector[2].first);
    EXPECT_EQ("John", zipped_vector[2].second);
}

TEST(FunctionalVectorTest, SortTest) {
    auto vector_under_test = functional_vector({person(45, "Jake"), person(34, "Bob"), person(52, "Manfred"), person(8, "Alice")});
    vector_under_test.sort([](const auto& person1, const auto& person2) {
        return person1.name < person2.name;
    });
    EXPECT_EQ(4, vector_under_test.size());
    
    EXPECT_EQ("Alice", vector_under_test[0].name);
    EXPECT_EQ(8, vector_under_test[0].age);
    
    EXPECT_EQ("Bob", vector_under_test[1].name);
    EXPECT_EQ(34, vector_under_test[1].age);
    
    EXPECT_EQ("Jake", vector_under_test[2].name);
    EXPECT_EQ(45, vector_under_test[2].age);
    
    EXPECT_EQ("Manfred", vector_under_test[3].name);
    EXPECT_EQ(52, vector_under_test[3].age);
}

TEST(FunctionalVectorTest, SortedTest) {
    const auto vector_under_test = functional_vector({person(45, "Jake"), person(34, "Bob"), person(52, "Manfred"), person(8, "Alice")});
    const auto sorted_vector = vector_under_test.sorted([](const auto& person1, const auto& person2) {
        return person1.name < person2.name;
    });
    
    EXPECT_EQ(4, vector_under_test.size());
    EXPECT_EQ("Jake", vector_under_test[0].name);
    EXPECT_EQ(45, vector_under_test[0].age);
    EXPECT_EQ("Bob", vector_under_test[1].name);
    EXPECT_EQ(34, vector_under_test[1].age);
    EXPECT_EQ("Manfred", vector_under_test[2].name);
    EXPECT_EQ(52, vector_under_test[2].age);
    EXPECT_EQ("Alice", vector_under_test[3].name);
    EXPECT_EQ(8, vector_under_test[3].age);
    
    EXPECT_EQ(4, sorted_vector.size());
    EXPECT_EQ("Alice", sorted_vector[0].name);
    EXPECT_EQ(8, sorted_vector[0].age);
    EXPECT_EQ("Bob", sorted_vector[1].name);
    EXPECT_EQ(34, sorted_vector[1].age);
    EXPECT_EQ("Jake", sorted_vector[2].name);
    EXPECT_EQ(45, sorted_vector[2].age);
    EXPECT_EQ("Manfred", sorted_vector[3].name);
    EXPECT_EQ(52, sorted_vector[3].age);
}

TEST(FunctionalVectorTest, SortAscendingTest) {
    auto vector_under_test = functional_vector({3, 1, 9, -4});
    vector_under_test.sort_ascending();
    EXPECT_EQ(4, vector_under_test.size());
    EXPECT_EQ(-4, vector_under_test[0]);
    EXPECT_EQ(1, vector_under_test[1]);
    EXPECT_EQ(3, vector_under_test[2]);
    EXPECT_EQ(9, vector_under_test[3]);
}

TEST(FunctionalVectorTest, SortedAscendingTest) {
    const auto vector_under_test = functional_vector({3, 1, 9, -4});
    const auto sorted_vector = vector_under_test.sorted_ascending();
    EXPECT_EQ(4, vector_under_test.size());
    EXPECT_EQ(3, vector_under_test[0]);
    EXPECT_EQ(1, vector_under_test[1]);
    EXPECT_EQ(9, vector_under_test[2]);
    EXPECT_EQ(-4, vector_under_test[3]);
    
    EXPECT_EQ(4, sorted_vector.size());
    EXPECT_EQ(-4, sorted_vector[0]);
    EXPECT_EQ(1, sorted_vector[1]);
    EXPECT_EQ(3, sorted_vector[2]);
    EXPECT_EQ(9, sorted_vector[3]);
}

TEST(FunctionalVectorTest, SortDescendingTest) {
    auto vector_under_test = functional_vector({3, 1, 9, -4});
    vector_under_test.sort_descending();
    EXPECT_EQ(4, vector_under_test.size());
    EXPECT_EQ(9, vector_under_test[0]);
    EXPECT_EQ(3, vector_under_test[1]);
    EXPECT_EQ(1, vector_under_test[2]);
    EXPECT_EQ(-4, vector_under_test[3]);
}

TEST(FunctionalVectorTest, SortedDescendingTest) {
    const auto vector_under_test = functional_vector({3, 1, 9, -4});
    const auto sorted_vector = vector_under_test.sorted_descending();
    EXPECT_EQ(4, vector_under_test.size());
    EXPECT_EQ(3, vector_under_test[0]);
    EXPECT_EQ(1, vector_under_test[1]);
    EXPECT_EQ(9, vector_under_test[2]);
    EXPECT_EQ(-4, vector_under_test[3]);
    
    EXPECT_EQ(4, sorted_vector.size());
    EXPECT_EQ(9, sorted_vector[0]);
    EXPECT_EQ(3, sorted_vector[1]);
    EXPECT_EQ(1, sorted_vector[2]);
    EXPECT_EQ(-4, sorted_vector[3]);
}

#ifndef NDEBUG
TEST(FunctionalVectorTest, SubscriptOperatorNegativeDeathTest) {
    const auto vector_under_test = functional_vector({3, 1, 9, -4});
    EXPECT_DEATH(vector_under_test[-1], "");
}

TEST(FunctionalVectorTest, SubscriptOperatorIndexEqualToSizeDeathTest) {
    const auto vector_under_test = functional_vector({3, 1, 9, -4});
    EXPECT_DEATH(vector_under_test[4], "");
}

TEST(FunctionalVectorTest, SubscriptOperatorIndexLargerThanSizeDeathTest) {
    const auto vector_under_test = functional_vector({3, 1, 9, -4});
    EXPECT_DEATH(vector_under_test[5], "");
}

TEST(FunctionalVectorTest, SubscriptOperatorAssignNegativeDeathTest) {
    auto vector_under_test = functional_vector({3, 1, 9, -4});
    EXPECT_DEATH(vector_under_test[-1] = 5, "");
}

TEST(FunctionalVectorTest, SubscriptOperatorAssignIndexEqualSizeDeathTest) {
    auto vector_under_test = functional_vector({3, 1, 9, -4});
    EXPECT_DEATH(vector_under_test[4] = 5, "");
}

TEST(FunctionalVectorTest, SubscriptOperatorAssignIndexLargerThanSizeDeathTest) {
    auto vector_under_test = functional_vector({3, 1, 9, -4});
    EXPECT_DEATH(vector_under_test[5] = -3, "");
}
#endif

TEST(FunctionalVectorTest, SubscriptOperatorAssignTest) {
    auto vector_under_test = functional_vector({3, 1, 9, -4});
    vector_under_test[2] = 7;
    EXPECT_EQ(4, vector_under_test.size());
    EXPECT_EQ(3, vector_under_test[0]);
    EXPECT_EQ(1, vector_under_test[1]);
    EXPECT_EQ(7, vector_under_test[2]);
    EXPECT_EQ(-4, vector_under_test[3]);
}

TEST(FunctionalVectorTest, FindFirstIndexEmptyVectorTest) {
    const auto vector_under_test = functional_vector<int>();
    EXPECT_FALSE(vector_under_test.find_first_index(-3).has_value());
}

TEST(FunctionalVectorTest, FindFirstIndexFilledVectorWithoutMatchTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_FALSE(vector_under_test.find_first_index(9).has_value());
}

TEST(FunctionalVectorTest, FindFirstIndexFilledVectorTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_EQ(0, vector_under_test.find_first_index(1).value());
    EXPECT_EQ(7, vector_under_test.find_first_index(7).value());
    EXPECT_EQ(3, vector_under_test.find_first_index(5).value());
}

TEST(FunctionalVectorTest, FindLastIndexEmptyVectorTest) {
    const auto vector_under_test = functional_vector<int>();
    EXPECT_FALSE(vector_under_test.find_last_index(-3).has_value());
}

TEST(FunctionalVectorTest, FindLastIndexFilledVectorWithoutMatchTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_FALSE(vector_under_test.find_last_index(9).has_value());
}

TEST(FunctionalVectorTest, FindLastIndexFilledVectorTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_EQ(8, vector_under_test.find_last_index(1).value());
    EXPECT_EQ(7, vector_under_test.find_last_index(7).value());
    EXPECT_EQ(3, vector_under_test.find_last_index(5).value());
}

TEST(FunctionalVectorTest, FindAllIndicesEmptyVectorTest) {
    const auto vector_under_test = functional_vector<int>();
    EXPECT_EQ(0, vector_under_test.find_all_indices(-3).size());
}

TEST(FunctionalVectorTest, FindAllIndicesFilledVectorWithoutMatchTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_EQ(0, vector_under_test.find_all_indices(9).size());
}

TEST(FunctionalVectorTest, FindAllIndicesFilledVectorTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 9, 1});
    const auto one_indices = vector_under_test.find_all_indices(1);
    EXPECT_EQ(std::vector<size_t>({0, 6, 8}), one_indices);
    const auto seven_indices = vector_under_test.find_all_indices(9);
    EXPECT_EQ(std::vector<size_t>({7}), seven_indices);
}

#ifndef NDEBUG
TEST(FunctionalVectorTest, RemoveAtEmptyVectorTest) {
    auto vector_under_test = functional_vector<int>();
    EXPECT_DEATH(vector_under_test.remove_at(-1),"");
    EXPECT_DEATH(vector_under_test.remove_at(0),"");
    EXPECT_DEATH(vector_under_test.remove_at(1),"");
}

TEST(FunctionalVectorTest, RemoveAtFilledVectorAboveSizeTest) {
    auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_DEATH(vector_under_test.remove_at(15),"");
}
#endif

TEST(FunctionalVectorTest, RemoveAtFilledVectorTest) {
    auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.remove_at(1);
    EXPECT_EQ(functional_vector({1, 2, 5, 8, 3, 1, 7, 1}), vector_under_test);
    vector_under_test.remove_at(1);
    EXPECT_EQ(functional_vector({1, 5, 8, 3, 1, 7, 1}), vector_under_test);
    vector_under_test.remove_at(4);
    EXPECT_EQ(functional_vector({1, 5, 8, 3, 7, 1}), vector_under_test);
}

#ifndef NDEBUG
TEST(FunctionalVectorTest, RemovingAtEmptyVectorTest) {
    const auto vector_under_test = functional_vector<int>();
    EXPECT_DEATH(vector_under_test.removing_at(-1),"");
    EXPECT_DEATH(vector_under_test.removing_at(0),"");
    EXPECT_DEATH(vector_under_test.removing_at(1),"");
}

TEST(FunctionalVectorTest, RemovingAtFilledVectorAboveSizeTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_DEATH(vector_under_test.removing_at(15),"");
}
#endif

TEST(FunctionalVectorTest, RemovingAtFilledVectorTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto shorter_vector = vector_under_test.removing_at(1);
    EXPECT_EQ(functional_vector({1, 2, 5, 8, 3, 1, 7, 1}), shorter_vector);
    EXPECT_EQ(functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1}), vector_under_test);
}

TEST(FunctionalVectorTest, RemoveLastTest) {
    auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.remove_last();
    EXPECT_EQ(functional_vector({1, 4, 2, 5, 8, 3, 1, 7}), vector_under_test);
    vector_under_test.remove_last();
    EXPECT_EQ(functional_vector({1, 4, 2, 5, 8, 3, 1}), vector_under_test);
    vector_under_test.remove_last();
    EXPECT_EQ(functional_vector({1, 4, 2, 5, 8, 3}), vector_under_test);
}

TEST(FunctionalVectorTest, RemovingLastTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto vector_without_last_element = vector_under_test.removing_last();
    EXPECT_EQ(functional_vector({1, 4, 2, 5, 8, 3, 1, 7}), vector_without_last_element);
    EXPECT_EQ(functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1}), vector_under_test);
}

TEST(FunctionalVectorTest, RemoveFirstTest) {
    auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.remove_first();
    EXPECT_EQ(functional_vector({4, 2, 5, 8, 3, 1, 7, 1}), vector_under_test);
    vector_under_test.remove_first();
    EXPECT_EQ(functional_vector({2, 5, 8, 3, 1, 7, 1}), vector_under_test);
    vector_under_test.remove_first();
    EXPECT_EQ(functional_vector({5, 8, 3, 1, 7, 1}), vector_under_test);
}

TEST(FunctionalVectorTest, RemovingFirstTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto vector_without_first_element = vector_under_test.removing_first();
    EXPECT_EQ(functional_vector({4, 2, 5, 8, 3, 1, 7, 1}), vector_without_first_element);
    EXPECT_EQ(functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1}), vector_under_test);
}


TEST(FunctionalVectorTest, InsertAtEmptyVectorTest) {
    auto vector_under_test = functional_vector<int>();
#ifndef NDEBUG
    EXPECT_DEATH(vector_under_test.insert_at(15, -1), "");
#endif
    vector_under_test.insert_at(0, -1);
    EXPECT_EQ(1, vector_under_test.size());
    EXPECT_EQ(-1, vector_under_test[0]);
}

TEST(FunctionalVectorTest, InsertAtFilledVectorTest) {
    auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
#ifndef NDEBUG
    EXPECT_DEATH(vector_under_test.insert_at(15, -1), "");
#endif
    vector_under_test.insert_at(3, 18);
    EXPECT_EQ(functional_vector({1, 4, 2, 18, 5, 8, 3, 1, 7, 1}), vector_under_test);
}

TEST(FunctionalVectorTest, InsertingAtEmptyVectorTest) {
    const auto vector_under_test = functional_vector<int>();
#ifndef NDEBUG
    EXPECT_DEATH(vector_under_test.inserting_at(15, -1), "");
#endif
    const auto augmented_vector = vector_under_test.inserting_at(0, -1);
    EXPECT_EQ(1, augmented_vector.size());
    EXPECT_EQ(-1, augmented_vector[0]);
    EXPECT_EQ(0, vector_under_test.size());
}

TEST(FunctionalVectorTest, InsertingAtFilledVectorTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
#ifndef NDEBUG
    EXPECT_DEATH(vector_under_test.inserting_at(15, -1), "");
#endif
    const auto augmented_vector = vector_under_test.inserting_at(3, 18);
    EXPECT_EQ(functional_vector({1, 4, 2, 18, 5, 8, 3, 1, 7, 1}), augmented_vector);
    EXPECT_EQ(functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1}), vector_under_test);
}

TEST(FunctionalVectorTest, InsertRangeEmptyFunctionalVectorTest) {
    auto vector_under_test = functional_vector<int>();
    const auto vector_to_insert = functional_vector({4, 7, 3, -5});
    vector_under_test.insert_at(0, vector_to_insert);
    EXPECT_EQ(functional_vector({4, 7, 3, -5}), vector_under_test);
}

#ifndef NDEBUG
TEST(FunctionalVectorTest, InsertRangeExistingWrongInsertionIndexFunctionalVectorTest) {
    auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto vector_to_insert = functional_vector({9, -5, 6});
    EXPECT_DEATH(vector_under_test.insert_at(-1, vector_to_insert), "");
    EXPECT_DEATH(vector_under_test.insert_at(10, vector_to_insert), "");
}
#endif

TEST(FunctionalVectorTest, InsertRangeExistingFunctionalVectorTest) {
    auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto vector_to_insert = functional_vector({9, -5, 6});
    vector_under_test.insert_at(3, vector_to_insert);
    EXPECT_EQ(functional_vector({1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1}), vector_under_test);
}

TEST(FunctionalVectorTest, InsertRangeEmptyStdVectorTest) {
    auto vector_under_test = functional_vector<int>();
    const auto vector_to_insert = std::vector {4, 7, 3, -5};
    vector_under_test.insert_at(0, vector_to_insert);
    EXPECT_EQ(functional_vector({4, 7, 3, -5}), vector_under_test);
}

#ifndef NDEBUG
TEST(FunctionalVectorTest, InsertRangeExistingWrongInsertionIndexStdVectorTest) {
    auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto vector_to_insert = std::vector {9, -5, 6};
    EXPECT_DEATH(vector_under_test.insert_at(-1, vector_to_insert), "");
    EXPECT_DEATH(vector_under_test.insert_at(10, vector_to_insert), "");
}
#endif

TEST(FunctionalVectorTest, InsertRangeExistingStdVectorTest) {
    auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto vector_to_insert = std::vector {9, -5, 6};
    vector_under_test.insert_at(3, vector_to_insert);
    EXPECT_EQ(functional_vector({1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1}), vector_under_test);
}

TEST(FunctionalVectorTest, InsertRangeEmptyInitializerListTest) {
    auto vector_under_test = functional_vector<int>();
    vector_under_test.insert_at(0, {4, 7, 3, -5});
    EXPECT_EQ(functional_vector({4, 7, 3, -5}), vector_under_test);
}

#ifndef NDEBUG
TEST(FunctionalVectorTest, InsertRangeExistingWrongInsertionIndexInitializerListTest) {
    auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_DEATH(vector_under_test.insert_at(-1, {9, -5, 6}), "");
    EXPECT_DEATH(vector_under_test.insert_at(10, {9, -5, 6}), "");
}
#endif

TEST(FunctionalVectorTest, InsertRangeExistingInitializerListTest) {
    auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.insert_at(3, {9, -5, 6});
    EXPECT_EQ(functional_vector({1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1}), vector_under_test);
}

TEST(FunctionalVectorTest, InsertingRangeEmptyFunctionalVectorTest) {
    const auto vector_under_test = functional_vector<int>();
    const auto vector_to_insert = functional_vector({4, 7, 3, -5});
    const auto result_vector = vector_under_test.inserting_at(0, vector_to_insert);
    EXPECT_EQ(functional_vector({4, 7, 3, -5}), result_vector);
}

#ifndef NDEBUG
TEST(FunctionalVectorTest, InsertingRangeExistingWrongInsertionIndexFunctionalVectorTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto vector_to_insert = functional_vector({9, -5, 6});
    EXPECT_DEATH(vector_under_test.inserting_at(-1, vector_to_insert), "");
    EXPECT_DEATH(vector_under_test.inserting_at(10, vector_to_insert), "");
}
#endif

TEST(FunctionalVectorTest, InsertingRangeExistingFunctionalVectorTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto vector_to_insert = functional_vector({9, -5, 6});
    const auto result_vector = vector_under_test.inserting_at(3, vector_to_insert);
    EXPECT_EQ(functional_vector({1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1}), result_vector);
}

TEST(FunctionalVectorTest, InsertingRangeEmptyStdVectorTest) {
    const auto vector_under_test = functional_vector<int>();
    const auto vector_to_insert = std::vector {4, 7, 3, -5};
    const auto result_vector = vector_under_test.inserting_at(0, vector_to_insert);
    EXPECT_EQ(functional_vector({4, 7, 3, -5}), result_vector);
}

#ifndef NDEBUG
TEST(FunctionalVectorTest, InsertingRangeExistingWrongInsertionIndexStdVectorTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto vector_to_insert = std::vector {9, -5, 6};
    EXPECT_DEATH(vector_under_test.inserting_at(-1, vector_to_insert), "");
    EXPECT_DEATH(vector_under_test.inserting_at(10, vector_to_insert), "");
}
#endif

TEST(FunctionalVectorTest, InsertingRangeExistingStdVectorTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto vector_to_insert = std::vector {9, -5, 6};
    const auto result_vector = vector_under_test.inserting_at(3, vector_to_insert);
    EXPECT_EQ(functional_vector({1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1}), result_vector);
}

TEST(FunctionalVectorTest, InsertingRangeEmptyInitializerListTest) {
    const auto vector_under_test = functional_vector<int>();
    const auto result_vector = vector_under_test.inserting_at(0, {4, 7, 3, -5});
    EXPECT_EQ(functional_vector({4, 7, 3, -5}), result_vector);
}

#ifndef NDEBUG
TEST(FunctionalVectorTest, InsertingRangeExistingWrongInsertionIndexInitializerListTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_DEATH(vector_under_test.inserting_at(-1, {9, -5, 6}), "");
    EXPECT_DEATH(vector_under_test.inserting_at(10, {9, -5, 6}), "");
}
#endif

TEST(FunctionalVectorTest, InsertingRangeExistingInitializerListTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto result_vector = vector_under_test.inserting_at(3, {9, -5, 6});
    EXPECT_EQ(functional_vector({1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1}), result_vector);
}

TEST(FunctionalVectorTest, RemoveRangeWithInvalidRangeTest) {
    auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.remove_range(index_range::invalid);
    EXPECT_EQ(functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1}), vector_under_test);
}

TEST(FunctionalVectorTest, RemoveRangeEmptyVectorTest) {
    auto vector_under_test = functional_vector<int>();
    vector_under_test.remove_range(index_range::from_start_and_count(1, 12));
    EXPECT_EQ(functional_vector<int>(), vector_under_test);
}

TEST(FunctionalVectorTest, RemoveRangeStartCountSuccessTest) {
    auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.remove_range(index_range::from_start_and_count(2, 3));
    EXPECT_EQ(functional_vector({1, 4, 3, 1, 7, 1}), vector_under_test);
}

TEST(FunctionalVectorTest, RemoveRangeStartEndSuccessTest) {
    auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.remove_range(index_range::from_start_and_end(3, 6));
    EXPECT_EQ(functional_vector({1, 4, 2, 7, 1}), vector_under_test);
}

TEST(FunctionalVectorTest, RemovingRangeWithInvalidRangeTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto shorter_vector = vector_under_test.removing_range(index_range::invalid);
    EXPECT_EQ(functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1}), shorter_vector);
}

TEST(FunctionalVectorTest, RemovingRangeEmptyVectorTest) {
    const auto vector_under_test = functional_vector<int>();
    const auto shorter_vector = vector_under_test.removing_range(index_range::from_start_and_count(1, 12));
    EXPECT_EQ(functional_vector<int>(), shorter_vector);
}

TEST(FunctionalVectorTest, RemovingRangeStartCountSuccessTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto shorter_vector = vector_under_test.removing_range(index_range::from_start_and_count(2, 3));
    EXPECT_EQ(functional_vector({1, 4, 3, 1, 7, 1}), shorter_vector);
}

TEST(FunctionalVectorTest, RemovingRangeStartEndSuccessTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto shorter_vector = vector_under_test.removing_range(index_range::from_start_and_end(3, 6));
    EXPECT_EQ(functional_vector({1, 4, 2, 7, 1}), shorter_vector);
}

TEST(FunctionalVectorTest, ReplaceRangeAtWithEmptySource) {
    auto vector_under_test = functional_vector({5, -3 , 4, -9});
    vector_under_test.replace_range_at(3, std::vector<int>());
    EXPECT_EQ(functional_vector({5, -3 , 4, -9}), vector_under_test);
}

#ifndef NDEBUG
TEST(FunctionalVectorTest, ReplaceRangeAtWrongIndex) {
    auto vector_under_test = functional_vector({5, -3 , 4, -9});
    EXPECT_DEATH(vector_under_test.replace_range_at(-1, {1, 2, 6, 4}), "");
    EXPECT_DEATH(vector_under_test.replace_range_at(4, {1, 2, 6, 4}), "");
    EXPECT_DEATH(vector_under_test.replace_range_at(5, {1, 2, 6, 4}), "");
}

TEST(FunctionalVectorTest, ReplaceRangeAtMoreElementsInSourceThanCapacityTest) {
    auto vector_under_test = functional_vector({5, -3 , 4, -9});
    EXPECT_DEATH(vector_under_test.replace_range_at(2, {1, 2, 6, 4, 8, 9, -10}), "");
}
#endif

TEST(FunctionalVectorTest, ReplaceRangeAtWithFunctionalVectorTest) {
    auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.replace_range_at(4, functional_vector({9, -10, 8}));
    EXPECT_EQ(functional_vector({1, 4, 2, 5, 9, -10, 8, 7, 1}), vector_under_test);
}

TEST(FunctionalVectorTest, ReplaceRangeAtWithStdVectorTest) {
    auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.replace_range_at(4, std::vector({9, -10, 8}));
    EXPECT_EQ(functional_vector({1, 4, 2, 5, 9, -10, 8, 7, 1}), vector_under_test);
}

TEST(FunctionalVectorTest, ReplaceRangeAtWithInitializerListTest) {
    auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.replace_range_at(4, std::initializer_list({9, -10, 8}));
    EXPECT_EQ(functional_vector({1, 4, 2, 5, 9, -10, 8, 7, 1}), vector_under_test);
}

TEST(FunctionalVectorTest, ReplacingRangeAtWithEmptySource) {
    const auto vector_under_test = functional_vector({5, -3 , 4, -9});
    const auto replaced_vector = vector_under_test.replacing_range_at(3, std::vector<int>());
    EXPECT_EQ(functional_vector({5, -3 , 4, -9}), replaced_vector);
}

#ifndef NDEBUG
TEST(FunctionalVectorTest, ReplacingRangeAtWrongIndex) {
    const auto vector_under_test = functional_vector({5, -3 , 4, -9});
    EXPECT_DEATH(vector_under_test.replacing_range_at(-1, {1, 2, 6, 4}), "");
    EXPECT_DEATH(vector_under_test.replacing_range_at(4, {1, 2, 6, 4}), "");
    EXPECT_DEATH(vector_under_test.replacing_range_at(5, {1, 2, 6, 4}), "");
}

TEST(FunctionalVectorTest, ReplacingRangeAtMoreElementsInSourceThanCapacityTest) {
    const auto vector_under_test = functional_vector({5, -3 , 4, -9});
    EXPECT_DEATH(vector_under_test.replacing_range_at(2, {1, 2, 6, 4, 8, 9, -10}), "");
}
#endif

TEST(FunctionalVectorTest, ReplacingRangeAtWithFunctionalVectorTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto replaced_vector = vector_under_test.replacing_range_at(4, functional_vector({9, -10, 8}));
    EXPECT_EQ(functional_vector({1, 4, 2, 5, 9, -10, 8, 7, 1}), replaced_vector);
}

TEST(FunctionalVectorTest, ReplacingRangeAtWithStdVectorTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto replaced_vector = vector_under_test.replacing_range_at(4, std::vector({9, -10, 8}));
    EXPECT_EQ(functional_vector({1, 4, 2, 5, 9, -10, 8, 7, 1}), replaced_vector);
}

TEST(FunctionalVectorTest, ReplacingRangeAtWithInitializerListTest) {
    const auto vector_under_test = functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto replaced_vector = vector_under_test.replacing_range_at(4, std::initializer_list({9, -10, 8}));
    EXPECT_EQ(functional_vector({1, 4, 2, 5, 9, -10, 8, 7, 1}), replaced_vector);
}

TEST(FunctionalVectorTest, FillTest) {
    auto vector_under_test = functional_vector({1, 3, -6, 4, -9});
    vector_under_test.fill(7);
    EXPECT_EQ(functional_vector({7, 7, 7, 7, 7}), vector_under_test);
}

TEST(FunctionalVectorTest, FilledTest) {
    const auto vector_under_test = functional_vector<std::string>::filled("John", 3);
    EXPECT_EQ(functional_vector<std::string>({"John", "John", "John"}), vector_under_test);
}
