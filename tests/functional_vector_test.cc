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

struct child
{
    child(int age)
    : age(age)
    {
    }
    
    int age;
};

struct person
{
    person(int age, std::string name)
    : age(age), name(std::move(name))
    {
    }
    
    int age;
    std::string name;
};

template <typename T>
void debug_vector(const functional_vector<T>& vec)
{
    vec.for_each([](const T& element) {
        std::cout << element << std::endl;
    });
}

TEST(FunctionalVectorTest, InsertBackTest)
{
    functional_vector<int> vector_under_test;
    EXPECT_EQ(0, vector_under_test.size());
    
    vector_under_test.insert_back(5);
    EXPECT_EQ(1, vector_under_test.size());
    EXPECT_EQ(5, vector_under_test[0]);
    
    vector_under_test.insert_back(-1);
    EXPECT_EQ(2, vector_under_test.size());
    EXPECT_EQ(5, vector_under_test[0]);
    EXPECT_EQ(-1, vector_under_test[1]);
}

TEST(FunctionalVectorTest, InsertFrontTest)
{
    functional_vector<int> vector_under_test;
    EXPECT_EQ(0, vector_under_test.size());
    
    vector_under_test.insert_front(5);
    EXPECT_EQ(1, vector_under_test.size());
    EXPECT_EQ(5, vector_under_test[0]);
    
    vector_under_test.insert_front(-1);
    EXPECT_EQ(2, vector_under_test.size());
    EXPECT_EQ(-1, vector_under_test[0]);
    EXPECT_EQ(5, vector_under_test[1]);
}

TEST(FunctionalVectorTest, InsertingBackTest)
{
    const functional_vector<int> vector_under_test({3, 6, 2, 8});
    const auto vector_new_instance = vector_under_test.inserting_back(5);
    EXPECT_EQ(4, vector_under_test.size());
    EXPECT_EQ(3, vector_under_test[0]);
    EXPECT_EQ(8, vector_under_test[3]);
    EXPECT_EQ(functional_vector<int>({ 3, 6, 2, 8, 5 }), vector_new_instance);
}

TEST(FunctionalVectorTest, InsertingFrontTest)
{
    const functional_vector<int> vector_under_test({3, 6, 2, 8});
    const auto vector_new_instance = vector_under_test.inserting_front(5);
    EXPECT_EQ(4, vector_under_test.size());
    EXPECT_EQ(3, vector_under_test[0]);
    EXPECT_EQ(8, vector_under_test[3]);
    EXPECT_EQ(functional_vector<int>({ 5, 3, 6, 2, 8}), vector_new_instance);
}

TEST(FunctionalVectorTest, InsertBackFromFunctionalVectorTest)
{
    functional_vector<int> vector_under_test({ 4, 5, 6 });
    vector_under_test.insert_back(functional_vector<int>({1, 2, 3}));
    EXPECT_EQ(functional_vector<int>({ 4, 5, 6, 1, 2, 3 }), vector_under_test);
}

TEST(FunctionalVectorTest, InsertBackFromStdVectorTest)
{
    functional_vector<int> vector_under_test({ 4, 5, 6 });
    vector_under_test.insert_back(std::vector<int>{ 1, 2, 3 });
    EXPECT_EQ(functional_vector<int>({ 4, 5, 6, 1, 2, 3 }), vector_under_test);
}

TEST(FunctionalVectorTest, InsertBackFromInitializerListTest)
{
    functional_vector<int> vector_under_test({ 4, 5, 6 });
    vector_under_test.insert_back(std::initializer_list<int>{ 1, 2, 3 });
    EXPECT_EQ(functional_vector<int>({ 4, 5, 6, 1, 2, 3 }), vector_under_test);
}

TEST(FunctionalVectorTest, InsertFrontFromFunctionalVectorTest)
{
    functional_vector<int> vector_under_test({ 4, 5, 6 });
    vector_under_test.insert_front(functional_vector<int>({1, 2, 3}));
    EXPECT_EQ(functional_vector<int>({ 1, 2, 3, 4, 5, 6 }), vector_under_test);
}

TEST(FunctionalVectorTest, InsertFrontFromStdVectorTest)
{
    functional_vector<int> vector_under_test({ 4, 5, 6 });
    vector_under_test.insert_front(std::vector<int>{ 1, 2, 3 });
    EXPECT_EQ(functional_vector<int>({ 1, 2, 3, 4, 5, 6 }), vector_under_test);
}

TEST(FunctionalVectorTest, InsertFrontFromInitializerListTest)
{
    functional_vector<int> vector_under_test({ 4, 5, 6 });
    vector_under_test.insert_front(std::initializer_list<int>{ 1, 2, 3 });
    EXPECT_EQ(functional_vector<int>({ 1, 2, 3, 4, 5, 6 }), vector_under_test);
}

TEST(FunctionalVectorTest, InsertingBackFromFunctionalVectorTest)
{
    const functional_vector<int> vector_under_test({ 4, 5, 6 });
    const auto vector_new_instance = vector_under_test.inserting_back(functional_vector<int>({1, 2, 3}));
    EXPECT_EQ(functional_vector<int>({ 4, 5, 6 }), vector_under_test);
    EXPECT_EQ(functional_vector<int>({ 4, 5, 6, 1, 2, 3 }), vector_new_instance);
}

TEST(FunctionalVectorTest, InsertingBackFromStdVectorTest)
{
    const functional_vector<int> vector_under_test({ 4, 5, 6 });
    const auto vector_new_instance = vector_under_test.inserting_back(std::vector<int>{ 1, 2, 3 });
    EXPECT_EQ(functional_vector<int>({ 4, 5, 6 }), vector_under_test);
    EXPECT_EQ(functional_vector<int>({ 4, 5, 6, 1, 2, 3 }), vector_new_instance);
}

TEST(FunctionalVectorTest, InsertingBackFromInitializerListTest)
{
    const functional_vector<int> vector_under_test({ 4, 5, 6 });
    const auto vector_new_instance = vector_under_test.inserting_back(std::initializer_list<int>{ 1, 2, 3 });
    EXPECT_EQ(functional_vector<int>({ 4, 5, 6 }), vector_under_test);
    EXPECT_EQ(functional_vector<int>({ 4, 5, 6, 1, 2, 3 }), vector_new_instance);
}

TEST(FunctionalVectorTest, InsertingFrontFromFunctionalVectorTest)
{
    const functional_vector<int> vector_under_test({ 4, 5, 6 });
    const auto vector_new_instance = vector_under_test.inserting_front(functional_vector<int>({1, 2, 3}));
    EXPECT_EQ(functional_vector<int>({ 4, 5, 6 }), vector_under_test);
    EXPECT_EQ(functional_vector<int>({ 1, 2, 3, 4, 5, 6 }), vector_new_instance);
}

TEST(FunctionalVectorTest, InsertingFrontFromStdVectorTest)
{
    const functional_vector<int> vector_under_test({ 4, 5, 6 });
    const auto vector_new_instance = vector_under_test.inserting_front(std::vector<int>{ 1, 2, 3 });
    EXPECT_EQ(functional_vector<int>({ 4, 5, 6 }), vector_under_test);
    EXPECT_EQ(functional_vector<int>({ 1, 2, 3, 4, 5, 6 }), vector_new_instance);
}

TEST(FunctionalVectorTest, InsertingFrontFromInitializerListTest)
{
    const functional_vector<int> vector_under_test({ 4, 5, 6 });
    const auto vector_new_instance = vector_under_test.inserting_front(std::initializer_list<int>{ 1, 2, 3 });
    EXPECT_EQ(functional_vector<int>({ 4, 5, 6 }), vector_under_test);
    EXPECT_EQ(functional_vector<int>({ 1, 2, 3, 4, 5, 6 }), vector_new_instance);
}

TEST(FunctionalVectorTest, MapTest)
{
    const functional_vector<int> vector_under_test({1, 3, 4});
    const auto mapped_vector = vector_under_test.map<child>([](const int& age) {
        return child(age);
    });
    EXPECT_EQ(3, mapped_vector.size());
    EXPECT_EQ(1, mapped_vector[0].age);
    EXPECT_EQ(3, mapped_vector[1].age);
    EXPECT_EQ(4, mapped_vector[2].age);
}

TEST(FunctionalVectorTest, FilterTest)
{
    functional_vector<child> vector_under_test({child(1), child(3), child(4)});
    vector_under_test.filter([](const child& child) {
        return child.age < 2;
    });
    EXPECT_EQ(1, vector_under_test.size());
    EXPECT_EQ(1, vector_under_test[0].age);
    vector_under_test.filter([](const child& child) {
        return child.age > 7;
    });
    EXPECT_EQ(0, vector_under_test.size());
}

TEST(FunctionalVectorTest, FilteredTest)
{
    const functional_vector<child> vector_under_test({child(1), child(3), child(4)});
    const auto filtered_vector = vector_under_test.filtered([](const child& child) {
        return child.age < 2;
    });
    EXPECT_EQ(3, vector_under_test.size());
    EXPECT_EQ(1, filtered_vector.size());
    EXPECT_EQ(1, filtered_vector[0].age);
}

TEST(FunctionalVectorTest, ReverseTest)
{
    functional_vector<child> vector_under_test({child(6), child(2), child(9)});
    vector_under_test.reverse();
    EXPECT_EQ(3, vector_under_test.size());
    EXPECT_EQ(9, vector_under_test[0].age);
    EXPECT_EQ(2, vector_under_test[1].age);
    EXPECT_EQ(6, vector_under_test[2].age);
}

TEST(FunctionalVectorTest, ReversedTest)
{
    const functional_vector<child> vector_under_test({child(6), child(2), child(9)});
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

TEST(FunctionalVectorTest, ZipWithStdVectorUnequalSizesThrowsTest)
{
    const functional_vector<int> ages_vector({32, 25, 53, 62});
    EXPECT_DEATH({ const auto zipped_vector = ages_vector.zip(std::vector<std::string>({"Jake", "Mary"})); }, "");
}

TEST(FunctionalVectorTest, ZipWithFunctionalVectorUnequalSizesThrowsTest)
{
    const functional_vector<int> ages_vector({32, 25, 53, 62});
    const auto names_vector = functional_vector<std::string>({"Jake", "Mary"});
    EXPECT_DEATH({ const auto zipped_vector = ages_vector.zip(names_vector); }, "");
}

TEST(FunctionalVectorTest, ZipWithFunctionalVectorTest)
{
    const functional_vector<int> ages_vector({32, 25, 53});
    const functional_vector<std::string> names_vector({"Jake", "Mary", "John"});
    const auto zipped_vector = ages_vector.zip(names_vector);
    EXPECT_EQ(3, zipped_vector.size());
    
    EXPECT_EQ(32, zipped_vector[0].first);
    EXPECT_EQ("Jake", zipped_vector[0].second);
    
    EXPECT_EQ(25, zipped_vector[1].first);
    EXPECT_EQ("Mary", zipped_vector[1].second);
    
    EXPECT_EQ(53, zipped_vector[2].first);
    EXPECT_EQ("John", zipped_vector[2].second);
}

TEST(FunctionalVectorTest, ZipWithStdVectorTest)
{
    const functional_vector<int> ages_vector({32, 25, 53});
    const auto zipped_vector = ages_vector.zip(std::vector<std::string>{"Jake", "Mary", "John"});
    EXPECT_EQ(3, zipped_vector.size());
    
    EXPECT_EQ(32, zipped_vector[0].first);
    EXPECT_EQ("Jake", zipped_vector[0].second);
    
    EXPECT_EQ(25, zipped_vector[1].first);
    EXPECT_EQ("Mary", zipped_vector[1].second);
    
    EXPECT_EQ(53, zipped_vector[2].first);
    EXPECT_EQ("John", zipped_vector[2].second);
}

TEST(FunctionalVectorTest, ZipWithInitializerListTest)
{
    const functional_vector<int> ages_vector({32, 25, 53});
    const auto zipped_vector = ages_vector.zip(std::initializer_list<std::string>{"Jake", "Mary", "John"});
    EXPECT_EQ(3, zipped_vector.size());
    
    EXPECT_EQ(32, zipped_vector[0].first);
    EXPECT_EQ("Jake", zipped_vector[0].second);
    
    EXPECT_EQ(25, zipped_vector[1].first);
    EXPECT_EQ("Mary", zipped_vector[1].second);
    
    EXPECT_EQ(53, zipped_vector[2].first);
    EXPECT_EQ("John", zipped_vector[2].second);
}

TEST(FunctionalVectorTest, SortTest)
{
    functional_vector<person> vector_under_test({
        person(45, "Jake"), person(34, "Bob"), person(52, "Manfred"), person(8, "Alice")
    });
    vector_under_test.sort([](const person& person1, const person& person2)
                           {
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

TEST(FunctionalVectorTest, SortedTest)
{
    const functional_vector<person> vector_under_test({
        person(45, "Jake"), person(34, "Bob"), person(52, "Manfred"), person(8, "Alice")
    });
    const auto sorted_vector = vector_under_test.sorted([](const person& person1, const person& person2) {
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

TEST(FunctionalVectorTest, SortAscendingTest)
{
    functional_vector<int> vector_under_test({3, 1, 9, -4});
    vector_under_test.sort_ascending();
    EXPECT_EQ(4, vector_under_test.size());
    EXPECT_EQ(-4, vector_under_test[0]);
    EXPECT_EQ(1, vector_under_test[1]);
    EXPECT_EQ(3, vector_under_test[2]);
    EXPECT_EQ(9, vector_under_test[3]);
}

TEST(FunctionalVectorTest, SortedAscendingTest)
{
    const functional_vector<int> vector_under_test({3, 1, 9, -4});
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

TEST(FunctionalVectorTest, SortDescendingTest)
{
    functional_vector<int> vector_under_test({3, 1, 9, -4});
    vector_under_test.sort_descending();
    EXPECT_EQ(4, vector_under_test.size());
    EXPECT_EQ(9, vector_under_test[0]);
    EXPECT_EQ(3, vector_under_test[1]);
    EXPECT_EQ(1, vector_under_test[2]);
    EXPECT_EQ(-4, vector_under_test[3]);
}

TEST(FunctionalVectorTest, SortedDescendingTest)
{
    const functional_vector<int> vector_under_test({3, 1, 9, -4});
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

TEST(FunctionalVectorTest, SubscriptOperatorNegativeDeathTest)
{
    const functional_vector<int> vector_under_test({3, 1, 9, -4});
    EXPECT_DEATH(vector_under_test[-1], "");
}

TEST(FunctionalVectorTest, SubscriptOperatorIndexEqualToSizeDeathTest)
{
    const functional_vector<int> vector_under_test({3, 1, 9, -4});
    EXPECT_DEATH(vector_under_test[4], "");
}

TEST(FunctionalVectorTest, SubscriptOperatorIndexLargerThanSizeDeathTest)
{
    const functional_vector<int> vector_under_test({3, 1, 9, -4});
    EXPECT_DEATH(vector_under_test[5], "");
}

TEST(FunctionalVectorTest, SubscriptOperatorAssignNegativeDeathTest)
{
    functional_vector<int> vector_under_test({3, 1, 9, -4});
    EXPECT_DEATH(vector_under_test[-1] = 5, "");
}

TEST(FunctionalVectorTest, SubscriptOperatorAssignIndexEqualSizeDeathTest)
{
    functional_vector<int> vector_under_test({3, 1, 9, -4});
    EXPECT_DEATH(vector_under_test[4] = 5, "");
}

TEST(FunctionalVectorTest, SubscriptOperatorAssignIndexLargerThanSizeDeathTest)
{
    functional_vector<int> vector_under_test({3, 1, 9, -4});
    EXPECT_DEATH(vector_under_test[5] = -3, "");
}

TEST(FunctionalVectorTest, SubscriptOperatorAssignTest)
{
    functional_vector<int> vector_under_test({3, 1, 9, -4});
    vector_under_test[2] = 7;
    EXPECT_EQ(4, vector_under_test.size());
    EXPECT_EQ(3, vector_under_test[0]);
    EXPECT_EQ(1, vector_under_test[1]);
    EXPECT_EQ(7, vector_under_test[2]);
    EXPECT_EQ(-4, vector_under_test[3]);
}

TEST(FunctionalVectorTest, FindFirstIndexEmptyVectorTest)
{
    const functional_vector<int> vector_under_test;
    EXPECT_FALSE(vector_under_test.find_first_index(-3).has_value());
}

TEST(FunctionalVectorTest, FindFirstIndexFilledVectorWithoutMatchTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_FALSE(vector_under_test.find_first_index(9).has_value());
}

TEST(FunctionalVectorTest, FindFirstIndexFilledVectorTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_EQ(0, vector_under_test.find_first_index(1).value());
    EXPECT_EQ(7, vector_under_test.find_first_index(7).value());
    EXPECT_EQ(3, vector_under_test.find_first_index(5).value());
}

TEST(FunctionalVectorTest, FindLastIndexEmptyVectorTest)
{
    const functional_vector<int> vector_under_test;
    EXPECT_FALSE(vector_under_test.find_last_index(-3).has_value());
}

TEST(FunctionalVectorTest, FindLastIndexFilledVectorWithoutMatchTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_FALSE(vector_under_test.find_last_index(9).has_value());
}

TEST(FunctionalVectorTest, FindLastIndexFilledVectorTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_EQ(8, vector_under_test.find_last_index(1).value());
    EXPECT_EQ(7, vector_under_test.find_last_index(7).value());
    EXPECT_EQ(3, vector_under_test.find_last_index(5).value());
}

TEST(FunctionalVectorTest, FindAllIndicesEmptyVectorTest)
{
    const functional_vector<int> vector_under_test;
    EXPECT_EQ(0, vector_under_test.find_all_indices(-3).size());
}

TEST(FunctionalVectorTest, FindAllIndicesFilledVectorWithoutMatchTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_EQ(0, vector_under_test.find_all_indices(9).size());
}

TEST(FunctionalVectorTest, FindAllIndicesFilledVectorTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 9, 1});
    const auto one_indices = vector_under_test.find_all_indices(1);
    EXPECT_EQ(std::vector<size_t>({ 0, 6, 8 }), one_indices);
    const auto seven_indices = vector_under_test.find_all_indices(9);
    EXPECT_EQ(std::vector<size_t>({ 7 }), seven_indices);
}

TEST(FunctionalVectorTest, RemoveAtEmptyVectorTest)
{
    functional_vector<int> vector_under_test;
    EXPECT_DEATH(vector_under_test.remove_at(-1), "");
    EXPECT_DEATH(vector_under_test.remove_at(0), "");
    EXPECT_DEATH(vector_under_test.remove_at(1), "");
}

TEST(FunctionalVectorTest, RemoveAtFilledVectorAboveSizeTest)
{
    auto vector_under_test = functional_vector<int>({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_DEATH(vector_under_test.remove_at(15), "");
}

TEST(FunctionalVectorTest, RemoveAtFilledVectorTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.remove_at(1);
    EXPECT_EQ(functional_vector<int>({ 1, 2, 5, 8, 3, 1, 7, 1 }), vector_under_test);
    vector_under_test.remove_at(1);
    EXPECT_EQ(functional_vector<int>({ 1, 5, 8, 3, 1, 7, 1 }), vector_under_test);
    vector_under_test.remove_at(4);
    EXPECT_EQ(functional_vector<int>({ 1, 5, 8, 3, 7, 1 }), vector_under_test);
}

TEST(FunctionalVectorTest, RemovingAtEmptyVectorTest)
{
    const functional_vector<int> vector_under_test;
    EXPECT_DEATH(vector_under_test.removing_at(-1), "");
    EXPECT_DEATH(vector_under_test.removing_at(0), "");
    EXPECT_DEATH(vector_under_test.removing_at(1), "");
}

TEST(FunctionalVectorTest, RemovingAtFilledVectorAboveSizeTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_DEATH(vector_under_test.removing_at(15), "");
}

TEST(FunctionalVectorTest, RemovingAtFilledVectorTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto shorter_vector = vector_under_test.removing_at(1);
    EXPECT_EQ(functional_vector<int>({ 1, 2, 5, 8, 3, 1, 7, 1 }), shorter_vector);
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 5, 8, 3, 1, 7, 1 }), vector_under_test);
}

TEST(FunctionalVectorTest, RemoveBackTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.remove_back();
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 5, 8, 3, 1, 7 }), vector_under_test);
    vector_under_test.remove_back();
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 5, 8, 3, 1 }), vector_under_test);
    vector_under_test.remove_back();
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 5, 8, 3 }), vector_under_test);
}

TEST(FunctionalVectorTest, RemovingBackTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto vector_without_last_element = vector_under_test.removing_back();
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 5, 8, 3, 1, 7 }), vector_without_last_element);
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 5, 8, 3, 1, 7, 1 }), vector_under_test);
}

TEST(FunctionalVectorTest, RemoveFrontTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.remove_front();
    EXPECT_EQ(functional_vector<int>({ 4, 2, 5, 8, 3, 1, 7, 1 }), vector_under_test);
    vector_under_test.remove_front();
    EXPECT_EQ(functional_vector<int>({ 2, 5, 8, 3, 1, 7, 1 }), vector_under_test);
    vector_under_test.remove_front();
    EXPECT_EQ(functional_vector<int>({ 5, 8, 3, 1, 7, 1 }), vector_under_test);
}

TEST(FunctionalVectorTest, RemovingFrontTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto vector_without_first_element = vector_under_test.removing_front();
    EXPECT_EQ(functional_vector<int>({ 4, 2, 5, 8, 3, 1, 7, 1 }), vector_without_first_element);
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 5, 8, 3, 1, 7, 1 }), vector_under_test);
}


TEST(FunctionalVectorTest, InsertAtEmptyVectorTest)
{
    functional_vector<int> vector_under_test;
    EXPECT_DEATH(vector_under_test.insert_at(15, -1), "");
    vector_under_test.insert_at(0, -1);
    EXPECT_EQ(1, vector_under_test.size());
    EXPECT_EQ(-1, vector_under_test[0]);
}

TEST(FunctionalVectorTest, InsertAtFilledVectorTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_DEATH(vector_under_test.insert_at(15, -1), "");
    vector_under_test.insert_at(3, 18);
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 18, 5, 8, 3, 1, 7, 1 }), vector_under_test);
}

TEST(FunctionalVectorTest, InsertingAtEmptyVectorTest)
{
    const functional_vector<int> vector_under_test;
    EXPECT_DEATH(vector_under_test.inserting_at(15, -1), "");
    const auto augmented_vector = vector_under_test.inserting_at(0, -1);
    EXPECT_EQ(1, augmented_vector.size());
    EXPECT_EQ(-1, augmented_vector[0]);
    EXPECT_EQ(0, vector_under_test.size());
}

TEST(FunctionalVectorTest, InsertingAtFilledVectorTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_DEATH(vector_under_test.inserting_at(15, -1), "");
    const auto augmented_vector = vector_under_test.inserting_at(3, 18);
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 18, 5, 8, 3, 1, 7, 1 }), augmented_vector);
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 5, 8, 3, 1, 7, 1 }), vector_under_test);
}

TEST(FunctionalVectorTest, InsertRangeEmptyFunctionalVectorTest)
{
    functional_vector<int> vector_under_test;
    const functional_vector<int> vector_to_insert({4, 7, 3, -5});
    vector_under_test.insert_at(0, vector_to_insert);
    EXPECT_EQ(functional_vector<int>({ 4, 7, 3, -5 }), vector_under_test);
}

TEST(FunctionalVectorTest, InsertRangeExistingWrongInsertionIndexFunctionalVectorTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const functional_vector<int> vector_to_insert({9, -5, 6});
    EXPECT_DEATH(vector_under_test.insert_at(-1, vector_to_insert), "");
    EXPECT_DEATH(vector_under_test.insert_at(10, vector_to_insert), "");
}

TEST(FunctionalVectorTest, InsertRangeExistingFunctionalVectorTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const functional_vector<int> vector_to_insert({9, -5, 6});
    vector_under_test.insert_at(3, vector_to_insert);
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1 }), vector_under_test);
}

TEST(FunctionalVectorTest, InsertRangeEmptyStdVectorTest)
{
    functional_vector<int> vector_under_test;
    const std::vector<int> vector_to_insert{4, 7, 3, -5};
    vector_under_test.insert_at(0, vector_to_insert);
    EXPECT_EQ(functional_vector<int>({ 4, 7, 3, -5 }), vector_under_test);
}

TEST(FunctionalVectorTest, InsertRangeExistingWrongInsertionIndexStdVectorTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const std::vector<int> vector_to_insert({9, -5, 6});
    EXPECT_DEATH(vector_under_test.insert_at(-1, vector_to_insert), "");
    EXPECT_DEATH(vector_under_test.insert_at(10, vector_to_insert), "");
}

TEST(FunctionalVectorTest, InsertRangeExistingStdVectorTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const std::vector<int> vector_to_insert({9, -5, 6});
    vector_under_test.insert_at(3, vector_to_insert);
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1 }), vector_under_test);
}

TEST(FunctionalVectorTest, InsertRangeEmptyInitializerListTest)
{
    functional_vector<int> vector_under_test;
    vector_under_test.insert_at(0, {4, 7, 3, -5});
    EXPECT_EQ(functional_vector<int>({ 4, 7, 3, -5 }), vector_under_test);
}

TEST(FunctionalVectorTest, InsertRangeExistingWrongInsertionIndexInitializerListTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_DEATH(vector_under_test.insert_at(-1, { 9, -5, 6 }), "");
    EXPECT_DEATH(vector_under_test.insert_at(10, { 9, -5, 6 }), "");
}

TEST(FunctionalVectorTest, InsertRangeExistingInitializerListTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.insert_at(3, {9, -5, 6});
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1 }), vector_under_test);
}

TEST(FunctionalVectorTest, InsertingRangeEmptyFunctionalVectorTest)
{
    const functional_vector<int> vector_under_test;
    const functional_vector<int> vector_to_insert({4, 7, 3, -5});
    const auto result_vector = vector_under_test.inserting_at(0, vector_to_insert);
    EXPECT_EQ(functional_vector<int>({ 4, 7, 3, -5 }), result_vector);
}

TEST(FunctionalVectorTest, InsertingRangeExistingWrongInsertionIndexFunctionalVectorTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const functional_vector<int> vector_to_insert({9, -5, 6});
    EXPECT_DEATH(vector_under_test.inserting_at(-1, vector_to_insert), "");
    EXPECT_DEATH(vector_under_test.inserting_at(10, vector_to_insert), "");
}

TEST(FunctionalVectorTest, InsertingRangeExistingFunctionalVectorTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const functional_vector<int> vector_to_insert({9, -5, 6});
    const auto result_vector = vector_under_test.inserting_at(3, vector_to_insert);
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1 }), result_vector);
}

TEST(FunctionalVectorTest, InsertingRangeEmptyStdVectorTest)
{
    const functional_vector<int> vector_under_test;
    const std::vector<int> vector_to_insert({4, 7, 3, -5});
    const auto result_vector = vector_under_test.inserting_at(0, vector_to_insert);
    EXPECT_EQ(functional_vector<int>({ 4, 7, 3, -5 }), result_vector);
}

TEST(FunctionalVectorTest, InsertingRangeExistingWrongInsertionIndexStdVectorTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const std::vector<int> vector_to_insert({9, -5, 6});
    EXPECT_DEATH(vector_under_test.inserting_at(-1, vector_to_insert), "");
    EXPECT_DEATH(vector_under_test.inserting_at(10, vector_to_insert), "");
}

TEST(FunctionalVectorTest, InsertingRangeExistingStdVectorTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const std::vector<int> vector_to_insert({9, -5, 6});
    const auto result_vector = vector_under_test.inserting_at(3, vector_to_insert);
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1 }), result_vector);
}

TEST(FunctionalVectorTest, InsertingRangeEmptyInitializerListTest)
{
    const functional_vector<int> vector_under_test;
    const auto result_vector = vector_under_test.inserting_at(0, {4, 7, 3, -5});
    EXPECT_EQ(functional_vector<int>({ 4, 7, 3, -5 }), result_vector);
}

TEST(FunctionalVectorTest, InsertingRangeExistingWrongInsertionIndexInitializerListTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_DEATH(vector_under_test.inserting_at(-1, { 9, -5, 6 }), "");
    EXPECT_DEATH(vector_under_test.inserting_at(10, { 9, -5, 6 }), "");
}

TEST(FunctionalVectorTest, InsertingRangeExistingInitializerListTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto result_vector = vector_under_test.inserting_at(3, {9, -5, 6});
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1 }), result_vector);
}

TEST(FunctionalVectorTest, RemoveIndexRangeWithInvalidRangeTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.remove_range(index_range::invalid);
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 5, 8, 3, 1, 7, 1 }), vector_under_test);
}

TEST(FunctionalVectorTest, RemoveIndexRangeEmptyVectorTest)
{
    functional_vector<int> vector_under_test;
    vector_under_test.remove_range(index_range::start_count(1, 12));
    EXPECT_EQ(functional_vector<int>(), vector_under_test);
}

TEST(FunctionalVectorTest, RemoveIndexRangeStartCountSuccessTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.remove_range(index_range::start_count(2, 3));
    EXPECT_EQ(functional_vector<int>({ 1, 4, 3, 1, 7, 1 }), vector_under_test);
}

TEST(FunctionalVectorTest, RemoveRangeStartEndSuccessTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.remove_range(index_range::start_end(3, 6));
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 7, 1 }), vector_under_test);
}

TEST(FunctionalVectorTest, RemovingIndexRangeWithInvalidRangeTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto shorter_vector = vector_under_test.removing_range(index_range::invalid);
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 5, 8, 3, 1, 7, 1 }), shorter_vector);
}

TEST(FunctionalVectorTest, RemovingRangeEmptyVectorTest)
{
    const functional_vector<int> vector_under_test;
    const auto shorter_vector = vector_under_test.removing_range(index_range::start_count(1, 12));
    EXPECT_EQ(functional_vector<int>(), shorter_vector);
}

TEST(FunctionalVectorTest, RemovingIndexRangeStartCountSuccessTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto shorter_vector = vector_under_test.removing_range(index_range::start_count(2, 3));
    EXPECT_EQ(functional_vector<int>({ 1, 4, 3, 1, 7, 1 }), shorter_vector);
}

TEST(FunctionalVectorTest, RemovingIndexRangeStartEndSuccessTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto shorter_vector = vector_under_test.removing_range(index_range::start_end(3, 6));
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 7, 1 }), shorter_vector);
}

TEST(FunctionalVectorTest, ReplaceRangeAtWithEmptySource)
{
    functional_vector<int> vector_under_test({5, -3, 4, -9});
    vector_under_test.replace_range_at(3, std::vector<int>());
    EXPECT_EQ(functional_vector<int>({ 5, -3 , 4, -9 }), vector_under_test);
}

TEST(FunctionalVectorTest, ReplaceRangeAtWrongIndex)
{
    functional_vector<int> vector_under_test({5, -3, 4, -9});
    EXPECT_DEATH(vector_under_test.replace_range_at(-1, { 1, 2, 6, 4 }), "");
    EXPECT_DEATH(vector_under_test.replace_range_at(4, { 1, 2, 6, 4 }), "");
    EXPECT_DEATH(vector_under_test.replace_range_at(5, { 1, 2, 6, 4 }), "");
}

TEST(FunctionalVectorTest, ReplaceRangeAtMoreElementsInSourceThanCapacityTest)
{
    functional_vector<int> vector_under_test({5, -3, 4, -9});
    EXPECT_DEATH(vector_under_test.replace_range_at(2, { 1, 2, 6, 4, 8, 9, -10 }), "");
}

TEST(FunctionalVectorTest, ReplaceRangeAtWithFunctionalVectorTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.replace_range_at(4, functional_vector<int>({9, -10, 8}));
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 5, 9, -10, 8, 7, 1 }), vector_under_test);
}

TEST(FunctionalVectorTest, ReplaceRangeAtWithStdVectorTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.replace_range_at(4, std::vector<int>({9, -10, 8}));
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 5, 9, -10, 8, 7, 1 }), vector_under_test);
}

TEST(FunctionalVectorTest, ReplaceRangeAtWithInitializerListTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.replace_range_at(4, std::initializer_list<int>({9, -10, 8}));
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 5, 9, -10, 8, 7, 1 }), vector_under_test);
}

TEST(FunctionalVectorTest, ReplacingRangeAtWithEmptySource)
{
    const functional_vector<int> vector_under_test({5, -3, 4, -9});
    const auto replaced_vector = vector_under_test.replacing_range_at(3, std::vector<int>());
    EXPECT_EQ(functional_vector<int>({ 5, -3 , 4, -9 }), replaced_vector);
}

TEST(FunctionalVectorTest, ReplacingRangeAtWrongIndex)
{
    const functional_vector<int> vector_under_test({5, -3, 4, -9});
    EXPECT_DEATH(vector_under_test.replacing_range_at(-1, { 1, 2, 6, 4 }), "");
    EXPECT_DEATH(vector_under_test.replacing_range_at(4, { 1, 2, 6, 4 }), "");
    EXPECT_DEATH(vector_under_test.replacing_range_at(5, { 1, 2, 6, 4 }), "");
}

TEST(FunctionalVectorTest, ReplacingRangeAtMoreElementsInSourceThanCapacityTest)
{
    const functional_vector<int> vector_under_test({5, -3, 4, -9});
    EXPECT_DEATH(vector_under_test.replacing_range_at(2, { 1, 2, 6, 4, 8, 9, -10 }), "");
}

TEST(FunctionalVectorTest, ReplacingRangeAtWithFunctionalVectorTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto replaced_vector = vector_under_test.replacing_range_at(4, functional_vector<int>({9, -10, 8}));
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 5, 9, -10, 8, 7, 1 }), replaced_vector);
}

TEST(FunctionalVectorTest, ReplacingRangeAtWithStdVectorTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto replaced_vector = vector_under_test.replacing_range_at(4, std::vector<int>({9, -10, 8}));
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 5, 9, -10, 8, 7, 1 }), replaced_vector);
}

TEST(FunctionalVectorTest, ReplacingRangeAtWithInitializerListTest)
{
    const functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto replaced_vector = vector_under_test.replacing_range_at(4, std::initializer_list<int>({9, -10, 8}));
    EXPECT_EQ(functional_vector<int>({ 1, 4, 2, 5, 9, -10, 8, 7, 1 }), replaced_vector);
}

TEST(FunctionalVectorTest, FillTest)
{
    functional_vector<int> vector_under_test({1, 3, -6, 4, -9});
    vector_under_test.fill(7);
    EXPECT_EQ(functional_vector<int>({ 7, 7, 7, 7, 7 }), vector_under_test);
}

TEST(FunctionalVectorTest, RepeatingConstructorTest)
{
    const functional_vector<std::string> vector_under_test(3, "John");
    EXPECT_EQ(functional_vector<std::string>({ "John", "John", "John" }), vector_under_test);
}

TEST(FunctionalVectorTest, EqualityOperatorEmptyVectorsTest)
{
    const functional_vector<int> vec1;
    const functional_vector<int> vec2;
    EXPECT_TRUE(vec1 == vec2);
    EXPECT_FALSE(vec1 != vec2);
}

TEST(FunctionalVectorTest, EqualityOperatorUnequalSizesTest)
{
    const functional_vector<int> vec1({1, 2, 3});
    const functional_vector<int> vec2({1, 2, 3, 4});
    EXPECT_TRUE(vec1 != vec2);
    EXPECT_FALSE(vec1 == vec2);
}

TEST(FunctionalVectorTest, EqualityOperatorEqualSizesDifferentElementsTest)
{
    const functional_vector<int> vec1({1, 2, 3});
    const functional_vector<int> vec2({1, 2, 4});
    EXPECT_TRUE(vec1 != vec2);
    EXPECT_FALSE(vec1 == vec2);
}

TEST(FunctionalVectorTest, EqualityOperatorEqualVectorsTest)
{
    const functional_vector<int> vec1({1, 2, 3});
    const functional_vector<int> vec2({1, 2, 3});
    EXPECT_TRUE(vec1 == vec2);
    EXPECT_FALSE(vec1 != vec2);
}

TEST(FunctionalVectorTest, ClearEmptyVectorTest)
{
    functional_vector<int> vector_under_test;
    EXPECT_TRUE(vector_under_test.is_empty());
    vector_under_test.clear();
    EXPECT_EQ(0, vector_under_test.size());
    EXPECT_TRUE(vector_under_test.is_empty());
}

TEST(FunctionalVectorTest, ClearFilledVectorTest)
{
    functional_vector<int> vector_under_test({5, -3, 4, -9});
    EXPECT_FALSE(vector_under_test.is_empty());
    vector_under_test.clear();
    EXPECT_EQ(0, vector_under_test.size());
    EXPECT_TRUE(vector_under_test.is_empty());
}

TEST(FunctionalVectorTest, CapacityReserveClearTest)
{
    functional_vector<int> vector_under_test;
    EXPECT_EQ(0, vector_under_test.capacity());
    EXPECT_EQ(0, vector_under_test.size());
    
    vector_under_test.reserve(5);
    EXPECT_EQ(5, vector_under_test.capacity());
    EXPECT_EQ(0, vector_under_test.size());
    
    vector_under_test.insert_back({1, 4, -5, 2});
    EXPECT_EQ(5, vector_under_test.capacity());
    EXPECT_EQ(4, vector_under_test.size());
    
    vector_under_test.clear();
    EXPECT_EQ(5, vector_under_test.capacity());
    EXPECT_EQ(0, vector_under_test.size());
}

TEST(FunctionalVectorTest, ResizeEmptyVectorTest)
{
    functional_vector<int> vector_under_test;
    vector_under_test.resize(5);
    EXPECT_EQ(functional_vector<int>({ 0, 0, 0, 0, 0 }), vector_under_test);
    EXPECT_EQ(5, vector_under_test.capacity());
    EXPECT_EQ(5, vector_under_test.size());
}

TEST(FunctionalVectorTest, ResizeSmallerThanCurrentSizeTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_EQ(9, vector_under_test.capacity());
    EXPECT_EQ(9, vector_under_test.size());
    
    vector_under_test.resize(5);
    EXPECT_EQ(functional_vector<int>({1, 4, 2, 5, 8}), vector_under_test);
    EXPECT_EQ(9, vector_under_test.capacity());
    EXPECT_EQ(5, vector_under_test.size());
}

TEST(FunctionalVectorTest, AllOfFalseTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_FALSE(vector_under_test.all_of([](const int& number) { return number > 5; }));
}

TEST(FunctionalVectorTest, AllOfTrueTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.all_of([](const int& number) {
        return number < 10;
    });
    EXPECT_TRUE(vector_under_test.all_of([](const int& number) { return number < 10; }));
}

TEST(FunctionalVectorTest, AnyOfFalseTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_FALSE(vector_under_test.any_of([](const int& number) { return number > 20; }));
}

TEST(FunctionalVectorTest, AnyOfTrueTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_TRUE(vector_under_test.any_of([](const int& number) { return number >= 7; }));
}

TEST(FunctionalVectorTest, NoneOfFalseTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_FALSE(vector_under_test.none_of([](const int& number) { return number > 7; }));
}

TEST(FunctionalVectorTest, NoneOfTrueTest)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_TRUE(vector_under_test.none_of([](const int& number) { return number < -2; }));
}
