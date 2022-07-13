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
#include <atomic>
#include "vector.h"
#include "set.h"
#include "index_range.h"
#include "test_types.h"
#include "warnings.h"

using namespace fcpp;

template <typename T>
void debug(const vector<T>& vec)
{
    vec.for_each([](const T& element) {
        std::cout << element << std::endl;
    });
}

TEST(VectorTest, InsertBack)
{
    vector<int> vector_under_test;
    EXPECT_EQ(0, vector_under_test.size());
    
    vector_under_test.insert_back(5);
    EXPECT_EQ(1, vector_under_test.size());
    EXPECT_EQ(5, vector_under_test[0]);
    
    vector_under_test.insert_back(-1);
    EXPECT_EQ(2, vector_under_test.size());
    EXPECT_EQ(5, vector_under_test[0]);
    EXPECT_EQ(-1, vector_under_test[1]);
}

TEST(VectorTest, InsertFront)
{
    vector<int> vector_under_test;
    EXPECT_EQ(0, vector_under_test.size());
    
    vector_under_test.insert_front(5);
    EXPECT_EQ(1, vector_under_test.size());
    EXPECT_EQ(5, vector_under_test[0]);
    
    vector_under_test.insert_front(-1);
    EXPECT_EQ(2, vector_under_test.size());
    EXPECT_EQ(-1, vector_under_test[0]);
    EXPECT_EQ(5, vector_under_test[1]);
}

TEST(VectorTest, InsertingBack)
{
    const vector<int> vector_under_test({3, 6, 2, 8});
    const auto vector_new_instance = vector_under_test.inserting_back(5);
    EXPECT_EQ(4, vector_under_test.size());
    EXPECT_EQ(3, vector_under_test[0]);
    EXPECT_EQ(8, vector_under_test[3]);
    EXPECT_EQ(vector<int>({ 3, 6, 2, 8, 5 }), vector_new_instance);
}

TEST(VectorTest, InsertingFront)
{
    const vector<int> vector_under_test({3, 6, 2, 8});
    const auto vector_new_instance = vector_under_test.inserting_front(5);
    EXPECT_EQ(4, vector_under_test.size());
    EXPECT_EQ(3, vector_under_test[0]);
    EXPECT_EQ(8, vector_under_test[3]);
    EXPECT_EQ(vector<int>({ 5, 3, 6, 2, 8}), vector_new_instance);
}

TEST(VectorTest, InsertBackFromFunctionalVector)
{
    vector<int> vector_under_test({ 4, 5, 6 });
    vector_under_test.insert_back(vector<int>({1, 2, 3}));
    EXPECT_EQ(vector<int>({ 4, 5, 6, 1, 2, 3 }), vector_under_test);
}

TEST(VectorTest, InsertBackFromStdVector)
{
    vector<int> vector_under_test({ 4, 5, 6 });
    vector_under_test.insert_back(std::vector<int>{ 1, 2, 3 });
    EXPECT_EQ(vector<int>({ 4, 5, 6, 1, 2, 3 }), vector_under_test);
}

TEST(VectorTest, InsertBackFromInitializerList)
{
    vector<int> vector_under_test({ 4, 5, 6 });
    vector_under_test.insert_back(std::initializer_list<int>{ 1, 2, 3 });
    EXPECT_EQ(vector<int>({ 4, 5, 6, 1, 2, 3 }), vector_under_test);
}

TEST(VectorTest, InsertFrontFromFunctionalVector)
{
    vector<int> vector_under_test({ 4, 5, 6 });
    vector_under_test.insert_front(vector<int>({1, 2, 3}));
    EXPECT_EQ(vector<int>({ 1, 2, 3, 4, 5, 6 }), vector_under_test);
}

TEST(VectorTest, InsertFrontFromStdVector)
{
    vector<int> vector_under_test({ 4, 5, 6 });
    vector_under_test.insert_front(std::vector<int>{ 1, 2, 3 });
    EXPECT_EQ(vector<int>({ 1, 2, 3, 4, 5, 6 }), vector_under_test);
}

TEST(VectorTest, InsertFrontFromInitializerList)
{
    vector<int> vector_under_test({ 4, 5, 6 });
    vector_under_test.insert_front(std::initializer_list<int>{ 1, 2, 3 });
    EXPECT_EQ(vector<int>({ 1, 2, 3, 4, 5, 6 }), vector_under_test);
}

TEST(VectorTest, InsertingBackFromFunctionalVector)
{
    const vector<int> vector_under_test({ 4, 5, 6 });
    const auto vector_new_instance = vector_under_test.inserting_back(vector<int>({1, 2, 3}));
    EXPECT_EQ(vector<int>({ 4, 5, 6 }), vector_under_test);
    EXPECT_EQ(vector<int>({ 4, 5, 6, 1, 2, 3 }), vector_new_instance);
}

TEST(VectorTest, InsertingBackFromStdVector)
{
    const vector<int> vector_under_test({ 4, 5, 6 });
    const auto vector_new_instance = vector_under_test.inserting_back(std::vector<int>{ 1, 2, 3 });
    EXPECT_EQ(vector<int>({ 4, 5, 6 }), vector_under_test);
    EXPECT_EQ(vector<int>({ 4, 5, 6, 1, 2, 3 }), vector_new_instance);
}

TEST(VectorTest, InsertingBackFromInitializerList)
{
    const vector<int> vector_under_test({ 4, 5, 6 });
    const auto vector_new_instance = vector_under_test.inserting_back(std::initializer_list<int>{ 1, 2, 3 });
    EXPECT_EQ(vector<int>({ 4, 5, 6 }), vector_under_test);
    EXPECT_EQ(vector<int>({ 4, 5, 6, 1, 2, 3 }), vector_new_instance);
}

TEST(VectorTest, InsertingFrontFromFunctionalVector)
{
    const vector<int> vector_under_test({ 4, 5, 6 });
    const auto vector_new_instance = vector_under_test.inserting_front(vector<int>({1, 2, 3}));
    EXPECT_EQ(vector<int>({ 4, 5, 6 }), vector_under_test);
    EXPECT_EQ(vector<int>({ 1, 2, 3, 4, 5, 6 }), vector_new_instance);
}

TEST(VectorTest, InsertingFrontFromStdVector)
{
    const vector<int> vector_under_test({ 4, 5, 6 });
    const auto vector_new_instance = vector_under_test.inserting_front(std::vector<int>{ 1, 2, 3 });
    EXPECT_EQ(vector<int>({ 4, 5, 6 }), vector_under_test);
    EXPECT_EQ(vector<int>({ 1, 2, 3, 4, 5, 6 }), vector_new_instance);
}

TEST(VectorTest, InsertingFrontFromInitializerList)
{
    const vector<int> vector_under_test({ 4, 5, 6 });
    const auto vector_new_instance = vector_under_test.inserting_front(std::initializer_list<int>{ 1, 2, 3 });
    EXPECT_EQ(vector<int>({ 4, 5, 6 }), vector_under_test);
    EXPECT_EQ(vector<int>({ 1, 2, 3, 4, 5, 6 }), vector_new_instance);
}

TEST(VectorTest, Map)
{
    const vector<int> vector_under_test({1, 3, 4});
    const auto mapped_vector = vector_under_test.map<child>([](const int& age) {
        return child(age);
    });
    EXPECT_EQ(3, mapped_vector.size());
    EXPECT_EQ(1, mapped_vector[0].age);
    EXPECT_EQ(3, mapped_vector[1].age);
    EXPECT_EQ(4, mapped_vector[2].age);
}

#ifdef PARALLEL_ALGORITHM_AVAILABLE
TEST(VectorTest, MapParallel)
{
    const functional_vector<int> vector_under_test({1, 3, 4});
    const auto mapped_vector = vector_under_test.map_parallel<child>([](const int& age) {
        return child(age);
    });
    EXPECT_EQ(3, mapped_vector.size());
    EXPECT_EQ(1, mapped_vector[0].age);
    EXPECT_EQ(3, mapped_vector[1].age);
    EXPECT_EQ(4, mapped_vector[2].age);
}
#endif

TEST(VectorTest, Filter)
{
    vector<child> vector_under_test({child(1), child(3), child(4)});
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

#ifdef PARALLEL_ALGORITHM_AVAILABLE
TEST(VectorTest, FilterParallel)
{
    functional_vector<child> vector_under_test({child(1), child(3), child(4)});
    vector_under_test.filter_parallel([](const child& child) {
        return child.age < 2;
    });
    EXPECT_EQ(1, vector_under_test.size());
    EXPECT_EQ(1, vector_under_test[0].age);
    vector_under_test.filter_parallel([](const child& child) {
        return child.age > 7;
    });
    EXPECT_EQ(0, vector_under_test.size());
}
#endif

TEST(VectorTest, Filtered)
{
    const vector<child> vector_under_test({child(1), child(3), child(4)});
    const auto filtered_vector = vector_under_test.filtered([](const child& child) {
        return child.age < 2;
    });
    EXPECT_EQ(3, vector_under_test.size());
    EXPECT_EQ(1, filtered_vector.size());
    EXPECT_EQ(1, filtered_vector[0].age);
}

#ifdef PARALLEL_ALGORITHM_AVAILABLE
TEST(VectorTest, FilteredParallel)
{
    const functional_vector<child> vector_under_test({child(1), child(3), child(4)});
    const auto filtered_vector = vector_under_test.filtered_parallel([](const child& child) {
        return child.age < 2;
    });
    EXPECT_EQ(3, vector_under_test.size());
    EXPECT_EQ(1, filtered_vector.size());
    EXPECT_EQ(1, filtered_vector[0].age);
}
#endif

TEST(VectorTest, Reverse)
{
    vector<child> vector_under_test({child(6), child(2), child(9)});
    vector_under_test.reverse();
    EXPECT_EQ(3, vector_under_test.size());
    EXPECT_EQ(9, vector_under_test[0].age);
    EXPECT_EQ(2, vector_under_test[1].age);
    EXPECT_EQ(6, vector_under_test[2].age);
}

TEST(VectorTest, Reversed)
{
    const vector<child> vector_under_test({child(6), child(2), child(9)});
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

TEST(VectorTest, ZipWithStdVectorUnequalSizesThrows)
{
    const vector<int> ages_vector({32, 25, 53, 62});
    EXPECT_DEATH({ const auto zipped_vector = ages_vector.zip(std::vector<std::string>({"Jake", "Mary"})); }, "");
}

TEST(VectorTest, ZipWithFunctionalVectorUnequalSizesThrows)
{
    const vector<int> ages_vector({32, 25, 53, 62});
    const auto names_vector = vector<std::string>({"Jake", "Mary"});
    EXPECT_DEATH({ const auto zipped_vector = ages_vector.zip(names_vector); }, "");
}

TEST(VectorTest, ZipWithFunctionalVector)
{
    const vector<int> ages_vector({32, 25, 53});
    const vector<std::string> names_vector({"Jake", "Mary", "John"});
    const auto zipped_vector = ages_vector.zip(names_vector);
    EXPECT_EQ(3, zipped_vector.size());
    
    EXPECT_EQ(32, zipped_vector[0].first);
    EXPECT_EQ("Jake", zipped_vector[0].second);
    
    EXPECT_EQ(25, zipped_vector[1].first);
    EXPECT_EQ("Mary", zipped_vector[1].second);
    
    EXPECT_EQ(53, zipped_vector[2].first);
    EXPECT_EQ("John", zipped_vector[2].second);
}

TEST(VectorTest, ZipWithStdVector)
{
    const vector<int> ages_vector({32, 25, 53});
    const auto zipped_vector = ages_vector.zip(std::vector<std::string>{"Jake", "Mary", "John"});
    EXPECT_EQ(3, zipped_vector.size());
    
    EXPECT_EQ(32, zipped_vector[0].first);
    EXPECT_EQ("Jake", zipped_vector[0].second);
    
    EXPECT_EQ(25, zipped_vector[1].first);
    EXPECT_EQ("Mary", zipped_vector[1].second);
    
    EXPECT_EQ(53, zipped_vector[2].first);
    EXPECT_EQ("John", zipped_vector[2].second);
}

TEST(VectorTest, ZipWithInitializerList)
{
    const vector<int> ages_vector({32, 25, 53});
    const auto zipped_vector = ages_vector.zip(std::initializer_list<std::string>{"Jake", "Mary", "John"});
    EXPECT_EQ(3, zipped_vector.size());
    
    EXPECT_EQ(32, zipped_vector[0].first);
    EXPECT_EQ("Jake", zipped_vector[0].second);
    
    EXPECT_EQ(25, zipped_vector[1].first);
    EXPECT_EQ("Mary", zipped_vector[1].second);
    
    EXPECT_EQ(53, zipped_vector[2].first);
    EXPECT_EQ("John", zipped_vector[2].second);
}

TEST(VectorTest, Sort)
{
    vector<person> vector_under_test({
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

TEST(VectorTest, Sorted)
{
    const vector<person> vector_under_test({
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

TEST(VectorTest, SortAscending)
{
    vector<int> vector_under_test({3, 1, 9, -4});
    vector_under_test.sort_ascending();
    EXPECT_EQ(4, vector_under_test.size());
    EXPECT_EQ(-4, vector_under_test[0]);
    EXPECT_EQ(1, vector_under_test[1]);
    EXPECT_EQ(3, vector_under_test[2]);
    EXPECT_EQ(9, vector_under_test[3]);
}

TEST(VectorTest, SortedAscending)
{
    const vector<int> vector_under_test({3, 1, 9, -4});
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

TEST(VectorTest, SortDescending)
{
    vector<int> vector_under_test({3, 1, 9, -4});
    vector_under_test.sort_descending();
    EXPECT_EQ(4, vector_under_test.size());
    EXPECT_EQ(9, vector_under_test[0]);
    EXPECT_EQ(3, vector_under_test[1]);
    EXPECT_EQ(1, vector_under_test[2]);
    EXPECT_EQ(-4, vector_under_test[3]);
}

TEST(VectorTest, SortedDescending)
{
    const vector<int> vector_under_test({3, 1, 9, -4});
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

#ifdef PARALLEL_ALGORITHM_AVAILABLE
TEST(VectorTest, SortParallel)
{
    functional_vector<person> vector_under_test({
        person(45, "Jake"), person(34, "Bob"), person(52, "Manfred"), person(8, "Alice")
    });
    vector_under_test.sort_parallel([](const person& person1, const person& person2){
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

TEST(VectorTest, SortedParallel)
{
    const functional_vector<person> vector_under_test({
        person(45, "Jake"), person(34, "Bob"), person(52, "Manfred"), person(8, "Alice")
    });
    const auto sorted_vector = vector_under_test.sorted_parallel([](const person& person1, const person& person2){
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

TEST(VectorTest, SortAscendingParallel)
{
    functional_vector<int> vector_under_test({3, 1, 9, -4});
    vector_under_test.sort_ascending_parallel();
    EXPECT_EQ(4, vector_under_test.size());
    EXPECT_EQ(-4, vector_under_test[0]);
    EXPECT_EQ(1, vector_under_test[1]);
    EXPECT_EQ(3, vector_under_test[2]);
    EXPECT_EQ(9, vector_under_test[3]);
}

TEST(VectorTest, SortedAscendingParallel)
{
    const functional_vector<int> vector_under_test({3, 1, 9, -4});
    const auto sorted_vector = vector_under_test.sorted_ascending_parallel();
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

TEST(VectorTest, SortDescendingParallel)
{
    functional_vector<int> vector_under_test({3, 1, 9, -4});
    vector_under_test.sort_descending_parallel();
    EXPECT_EQ(4, vector_under_test.size());
    EXPECT_EQ(9, vector_under_test[0]);
    EXPECT_EQ(3, vector_under_test[1]);
    EXPECT_EQ(1, vector_under_test[2]);
    EXPECT_EQ(-4, vector_under_test[3]);
}

TEST(VectorTest, SortedDescendingParallel)
{
    const functional_vector<int> vector_under_test({3, 1, 9, -4});
    const auto sorted_vector = vector_under_test.sorted_descending_parallel();
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
#endif

TEST(VectorTest, SubscriptOperatorNegativeDeath)
{
    const vector<int> vector_under_test({3, 1, 9, -4});
    EXPECT_DEATH(vector_under_test[-1], "");
}

TEST(VectorTest, SubscriptOperatorIndexEqualToSizeDeath)
{
    const vector<int> vector_under_test({3, 1, 9, -4});
    EXPECT_DEATH(vector_under_test[4], "");
}

TEST(VectorTest, SubscriptOperatorIndexLargerThanSizeDeath)
{
    const vector<int> vector_under_test({3, 1, 9, -4});
    EXPECT_DEATH(vector_under_test[5], "");
}

TEST(VectorTest, SubscriptOperatorAssignNegativeDeath)
{
    vector<int> vector_under_test({3, 1, 9, -4});
    EXPECT_DEATH(vector_under_test[-1] = 5, "");
}

TEST(VectorTest, SubscriptOperatorAssignIndexEqualSizeDeath)
{
    vector<int> vector_under_test({3, 1, 9, -4});
    EXPECT_DEATH(vector_under_test[4] = 5, "");
}

TEST(VectorTest, SubscriptOperatorAssignIndexLargerThanSizeDeath)
{
    vector<int> vector_under_test({3, 1, 9, -4});
    EXPECT_DEATH(vector_under_test[5] = -3, "");
}

TEST(VectorTest, SubscriptOperatorAssign)
{
    vector<int> vector_under_test({3, 1, 9, -4});
    vector_under_test[2] = 7;
    EXPECT_EQ(4, vector_under_test.size());
    EXPECT_EQ(3, vector_under_test[0]);
    EXPECT_EQ(1, vector_under_test[1]);
    EXPECT_EQ(7, vector_under_test[2]);
    EXPECT_EQ(-4, vector_under_test[3]);
}

TEST(VectorTest, FindFirstIndexEmptyVector)
{
    const vector<int> vector_under_test;
    EXPECT_FALSE(vector_under_test.find_first_index(-3).has_value());
}

TEST(VectorTest, FindFirstIndexFilledVectorWithoutMatch)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_FALSE(vector_under_test.find_first_index(9).has_value());
}

TEST(VectorTest, FindFirstIndexFilledVector)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_EQ(0, vector_under_test.find_first_index(1).value());
    EXPECT_EQ(7, vector_under_test.find_first_index(7).value());
    EXPECT_EQ(3, vector_under_test.find_first_index(5).value());
}

TEST(VectorTest, FindLastIndexEmptyVector)
{
    const vector<int> vector_under_test;
    EXPECT_FALSE(vector_under_test.find_last_index(-3).has_value());
}

TEST(VectorTest, FindLastIndexFilledVectorWithoutMatch)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_FALSE(vector_under_test.find_last_index(9).has_value());
}

TEST(VectorTest, FindLastIndexFilledVector)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_EQ(8, vector_under_test.find_last_index(1).value());
    EXPECT_EQ(7, vector_under_test.find_last_index(7).value());
    EXPECT_EQ(3, vector_under_test.find_last_index(5).value());
}

TEST(VectorTest, FindAllIndicesEmptyVector)
{
    const vector<int> vector_under_test;
    EXPECT_EQ(0, vector_under_test.find_all_indices(-3).size());
}

TEST(VectorTest, FindAllIndicesFilledVectorWithoutMatch)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_EQ(0, vector_under_test.find_all_indices(9).size());
}

TEST(VectorTest, FindAllIndicesFilledVector)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 9, 1});
    const auto one_indices = vector_under_test.find_all_indices(1);
    EXPECT_EQ(std::vector<size_t>({ 0, 6, 8 }), one_indices);
    const auto seven_indices = vector_under_test.find_all_indices(9);
    EXPECT_EQ(std::vector<size_t>({ 7 }), seven_indices);
}

TEST(VectorTest, RemoveAtEmptyVector)
{
    vector<int> vector_under_test;
    EXPECT_DEATH(vector_under_test.remove_at(-1), "");
    EXPECT_DEATH(vector_under_test.remove_at(0), "");
    EXPECT_DEATH(vector_under_test.remove_at(1), "");
}

TEST(VectorTest, RemoveAtFilledVectorAboveSize)
{
    auto vector_under_test = vector<int>({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_DEATH(vector_under_test.remove_at(15), "");
}

TEST(VectorTest, RemoveAtFilledVector)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.remove_at(1);
    EXPECT_EQ(vector<int>({ 1, 2, 5, 8, 3, 1, 7, 1 }), vector_under_test);
    vector_under_test.remove_at(1);
    EXPECT_EQ(vector<int>({ 1, 5, 8, 3, 1, 7, 1 }), vector_under_test);
    vector_under_test.remove_at(4);
    EXPECT_EQ(vector<int>({ 1, 5, 8, 3, 7, 1 }), vector_under_test);
}

TEST(VectorTest, RemovingAtEmptyVector)
{
    const vector<int> vector_under_test;
    EXPECT_DEATH(vector_under_test.removing_at(-1), "");
    EXPECT_DEATH(vector_under_test.removing_at(0), "");
    EXPECT_DEATH(vector_under_test.removing_at(1), "");
}

TEST(VectorTest, RemovingAtFilledVectorAboveSize)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_DEATH(vector_under_test.removing_at(15), "");
}

TEST(VectorTest, RemovingAtFilledVector)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto shorter_vector = vector_under_test.removing_at(1);
    EXPECT_EQ(vector<int>({ 1, 2, 5, 8, 3, 1, 7, 1 }), shorter_vector);
    EXPECT_EQ(vector<int>({ 1, 4, 2, 5, 8, 3, 1, 7, 1 }), vector_under_test);
}

TEST(VectorTest, RemoveBack)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.remove_back();
    EXPECT_EQ(vector<int>({ 1, 4, 2, 5, 8, 3, 1, 7 }), vector_under_test);
    vector_under_test.remove_back();
    EXPECT_EQ(vector<int>({ 1, 4, 2, 5, 8, 3, 1 }), vector_under_test);
    vector_under_test.remove_back();
    EXPECT_EQ(vector<int>({ 1, 4, 2, 5, 8, 3 }), vector_under_test);
}

TEST(VectorTest, RemovingBack)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto vector_without_last_element = vector_under_test.removing_back();
    EXPECT_EQ(vector<int>({ 1, 4, 2, 5, 8, 3, 1, 7 }), vector_without_last_element);
    EXPECT_EQ(vector<int>({ 1, 4, 2, 5, 8, 3, 1, 7, 1 }), vector_under_test);
}

TEST(VectorTest, RemoveFront)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.remove_front();
    EXPECT_EQ(vector<int>({ 4, 2, 5, 8, 3, 1, 7, 1 }), vector_under_test);
    vector_under_test.remove_front();
    EXPECT_EQ(vector<int>({ 2, 5, 8, 3, 1, 7, 1 }), vector_under_test);
    vector_under_test.remove_front();
    EXPECT_EQ(vector<int>({ 5, 8, 3, 1, 7, 1 }), vector_under_test);
}

TEST(VectorTest, RemovingFront)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto vector_without_first_element = vector_under_test.removing_front();
    EXPECT_EQ(vector<int>({ 4, 2, 5, 8, 3, 1, 7, 1 }), vector_without_first_element);
    EXPECT_EQ(vector<int>({ 1, 4, 2, 5, 8, 3, 1, 7, 1 }), vector_under_test);
}


TEST(VectorTest, InsertAtEmptyVector)
{
    vector<int> vector_under_test;
    EXPECT_DEATH(vector_under_test.insert_at(15, -1), "");
    vector_under_test.insert_at(0, -1);
    EXPECT_EQ(1, vector_under_test.size());
    EXPECT_EQ(-1, vector_under_test[0]);
}

TEST(VectorTest, InsertAtFilledVector)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_DEATH(vector_under_test.insert_at(15, -1), "");
    vector_under_test.insert_at(3, 18);
    EXPECT_EQ(vector<int>({ 1, 4, 2, 18, 5, 8, 3, 1, 7, 1 }), vector_under_test);
}

TEST(VectorTest, InsertingAtEmptyVector)
{
    const vector<int> vector_under_test;
    EXPECT_DEATH(vector_under_test.inserting_at(15, -1), "");
    const auto augmented_vector = vector_under_test.inserting_at(0, -1);
    EXPECT_EQ(1, augmented_vector.size());
    EXPECT_EQ(-1, augmented_vector[0]);
    EXPECT_EQ(0, vector_under_test.size());
}

TEST(VectorTest, InsertingAtFilledVector)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_DEATH(vector_under_test.inserting_at(15, -1), "");
    const auto augmented_vector = vector_under_test.inserting_at(3, 18);
    EXPECT_EQ(vector<int>({ 1, 4, 2, 18, 5, 8, 3, 1, 7, 1 }), augmented_vector);
    EXPECT_EQ(vector<int>({ 1, 4, 2, 5, 8, 3, 1, 7, 1 }), vector_under_test);
}

TEST(VectorTest, InsertRangeEmptyFunctionalVector)
{
    vector<int> vector_under_test;
    const vector<int> vector_to_insert({4, 7, 3, -5});
    vector_under_test.insert_at(0, vector_to_insert);
    EXPECT_EQ(vector<int>({ 4, 7, 3, -5 }), vector_under_test);
}

TEST(VectorTest, InsertRangeExistingWrongInsertionIndexFunctionalVector)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const vector<int> vector_to_insert({9, -5, 6});
    EXPECT_DEATH(vector_under_test.insert_at(-1, vector_to_insert), "");
    EXPECT_DEATH(vector_under_test.insert_at(10, vector_to_insert), "");
}

TEST(VectorTest, InsertRangeExistingFunctionalVector)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const vector<int> vector_to_insert({9, -5, 6});
    vector_under_test.insert_at(3, vector_to_insert);
    EXPECT_EQ(vector<int>({ 1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1 }), vector_under_test);
}

TEST(VectorTest, InsertRangeEmptyStdVector)
{
    vector<int> vector_under_test;
    const std::vector<int> vector_to_insert{4, 7, 3, -5};
    vector_under_test.insert_at(0, vector_to_insert);
    EXPECT_EQ(vector<int>({ 4, 7, 3, -5 }), vector_under_test);
}

TEST(VectorTest, InsertRangeExistingWrongInsertionIndexStdVector)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const std::vector<int> vector_to_insert({9, -5, 6});
    EXPECT_DEATH(vector_under_test.insert_at(-1, vector_to_insert), "");
    EXPECT_DEATH(vector_under_test.insert_at(10, vector_to_insert), "");
}

TEST(VectorTest, InsertRangeExistingStdVector)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const std::vector<int> vector_to_insert({9, -5, 6});
    vector_under_test.insert_at(3, vector_to_insert);
    EXPECT_EQ(vector<int>({ 1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1 }), vector_under_test);
}

TEST(VectorTest, InsertRangeEmptyInitializerList)
{
    vector<int> vector_under_test;
    vector_under_test.insert_at(0, {4, 7, 3, -5});
    EXPECT_EQ(vector<int>({ 4, 7, 3, -5 }), vector_under_test);
}

TEST(VectorTest, InsertRangeExistingWrongInsertionIndexInitializerList)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_DEATH(vector_under_test.insert_at(-1, { 9, -5, 6 }), "");
    EXPECT_DEATH(vector_under_test.insert_at(10, { 9, -5, 6 }), "");
}

TEST(VectorTest, InsertRangeExistingInitializerList)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.insert_at(3, {9, -5, 6});
    EXPECT_EQ(vector<int>({ 1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1 }), vector_under_test);
}

TEST(VectorTest, InsertingRangeEmptyFunctionalVector)
{
    const vector<int> vector_under_test;
    const vector<int> vector_to_insert({4, 7, 3, -5});
    const auto result_vector = vector_under_test.inserting_at(0, vector_to_insert);
    EXPECT_EQ(vector<int>({ 4, 7, 3, -5 }), result_vector);
}

TEST(VectorTest, InsertingRangeExistingWrongInsertionIndexFunctionalVector)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const vector<int> vector_to_insert({9, -5, 6});
    EXPECT_DEATH(vector_under_test.inserting_at(-1, vector_to_insert), "");
    EXPECT_DEATH(vector_under_test.inserting_at(10, vector_to_insert), "");
}

TEST(VectorTest, InsertingRangeExistingFunctionalVector)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const vector<int> vector_to_insert({9, -5, 6});
    const auto result_vector = vector_under_test.inserting_at(3, vector_to_insert);
    EXPECT_EQ(vector<int>({ 1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1 }), result_vector);
}

TEST(VectorTest, InsertingRangeEmptyStdVector)
{
    const vector<int> vector_under_test;
    const std::vector<int> vector_to_insert({4, 7, 3, -5});
    const auto result_vector = vector_under_test.inserting_at(0, vector_to_insert);
    EXPECT_EQ(vector<int>({ 4, 7, 3, -5 }), result_vector);
}

TEST(VectorTest, InsertingRangeExistingWrongInsertionIndexStdVector)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const std::vector<int> vector_to_insert({9, -5, 6});
    EXPECT_DEATH(vector_under_test.inserting_at(-1, vector_to_insert), "");
    EXPECT_DEATH(vector_under_test.inserting_at(10, vector_to_insert), "");
}

TEST(VectorTest, InsertingRangeExistingStdVector)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const std::vector<int> vector_to_insert({9, -5, 6});
    const auto result_vector = vector_under_test.inserting_at(3, vector_to_insert);
    EXPECT_EQ(vector<int>({ 1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1 }), result_vector);
}

TEST(VectorTest, InsertingRangeEmptyInitializerList)
{
    const vector<int> vector_under_test;
    const auto result_vector = vector_under_test.inserting_at(0, {4, 7, 3, -5});
    EXPECT_EQ(vector<int>({ 4, 7, 3, -5 }), result_vector);
}

TEST(VectorTest, InsertingRangeExistingWrongInsertionIndexInitializerList)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_DEATH(vector_under_test.inserting_at(-1, { 9, -5, 6 }), "");
    EXPECT_DEATH(vector_under_test.inserting_at(10, { 9, -5, 6 }), "");
}

TEST(VectorTest, InsertingRangeExistingInitializerList)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto result_vector = vector_under_test.inserting_at(3, {9, -5, 6});
    EXPECT_EQ(vector<int>({ 1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1 }), result_vector);
}

TEST(VectorTest, RemoveIndexRangeWithInvalidRange)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.remove_range(index_range::invalid);
    EXPECT_EQ(vector<int>({ 1, 4, 2, 5, 8, 3, 1, 7, 1 }), vector_under_test);
}

TEST(VectorTest, RemoveIndexRangeEmptyVector)
{
    vector<int> vector_under_test;
    vector_under_test.remove_range(index_range::start_count(1, 12));
    EXPECT_EQ(vector<int>(), vector_under_test);
}

TEST(VectorTest, RemoveIndexRangeStartCountSuccess)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.remove_range(index_range::start_count(2, 3));
    EXPECT_EQ(vector<int>({ 1, 4, 3, 1, 7, 1 }), vector_under_test);
}

TEST(VectorTest, RemoveRangeStartEndSuccess)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.remove_range(index_range::start_end(3, 6));
    EXPECT_EQ(vector<int>({ 1, 4, 2, 7, 1 }), vector_under_test);
}

TEST(VectorTest, RemovingIndexRangeWithInvalidRange)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto shorter_vector = vector_under_test.removing_range(index_range::invalid);
    EXPECT_EQ(vector<int>({ 1, 4, 2, 5, 8, 3, 1, 7, 1 }), shorter_vector);
}

TEST(VectorTest, RemovingRangeEmptyVector)
{
    const vector<int> vector_under_test;
    const auto shorter_vector = vector_under_test.removing_range(index_range::start_count(1, 12));
    EXPECT_EQ(vector<int>(), shorter_vector);
}

TEST(VectorTest, RemovingIndexRangeStartCountSuccess)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto shorter_vector = vector_under_test.removing_range(index_range::start_count(2, 3));
    EXPECT_EQ(vector<int>({ 1, 4, 3, 1, 7, 1 }), shorter_vector);
}

TEST(VectorTest, RemovingIndexRangeStartEndSuccess)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto shorter_vector = vector_under_test.removing_range(index_range::start_end(3, 6));
    EXPECT_EQ(vector<int>({ 1, 4, 2, 7, 1 }), shorter_vector);
}

TEST(VectorTest, ReplaceRangeAtWithEmptySource)
{
    vector<int> vector_under_test({5, -3, 4, -9});
    vector_under_test.replace_range_at(3, std::vector<int>());
    EXPECT_EQ(vector<int>({ 5, -3 , 4, -9 }), vector_under_test);
}

TEST(VectorTest, ReplaceRangeAtWrongIndex)
{
    vector<int> vector_under_test({5, -3, 4, -9});
    EXPECT_DEATH(vector_under_test.replace_range_at(-1, { 1, 2, 6, 4 }), "");
    EXPECT_DEATH(vector_under_test.replace_range_at(4, { 1, 2, 6, 4 }), "");
    EXPECT_DEATH(vector_under_test.replace_range_at(5, { 1, 2, 6, 4 }), "");
}

TEST(VectorTest, ReplaceRangeAtMoreElementsInSourceThanCapacity)
{
    vector<int> vector_under_test({5, -3, 4, -9});
    EXPECT_DEATH(vector_under_test.replace_range_at(2, { 1, 2, 6, 4, 8, 9, -10 }), "");
}

TEST(VectorTest, ReplaceRangeAtWithFunctionalVector)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.replace_range_at(4, vector<int>({9, -10, 8}));
    EXPECT_EQ(vector<int>({ 1, 4, 2, 5, 9, -10, 8, 7, 1 }), vector_under_test);
}

TEST(VectorTest, ReplaceRangeAtWithStdVector)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.replace_range_at(4, std::vector<int>({9, -10, 8}));
    EXPECT_EQ(vector<int>({ 1, 4, 2, 5, 9, -10, 8, 7, 1 }), vector_under_test);
}

TEST(VectorTest, ReplaceRangeAtWithInitializerList)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.replace_range_at(4, std::initializer_list<int>({9, -10, 8}));
    EXPECT_EQ(vector<int>({ 1, 4, 2, 5, 9, -10, 8, 7, 1 }), vector_under_test);
}

TEST(VectorTest, ReplacingRangeAtWithEmptySource)
{
    const vector<int> vector_under_test({5, -3, 4, -9});
    const auto replaced_vector = vector_under_test.replacing_range_at(3, std::vector<int>());
    EXPECT_EQ(vector<int>({ 5, -3 , 4, -9 }), replaced_vector);
}

TEST(VectorTest, ReplacingRangeAtWrongIndex)
{
    const vector<int> vector_under_test({5, -3, 4, -9});
    EXPECT_DEATH(vector_under_test.replacing_range_at(-1, { 1, 2, 6, 4 }), "");
    EXPECT_DEATH(vector_under_test.replacing_range_at(4, { 1, 2, 6, 4 }), "");
    EXPECT_DEATH(vector_under_test.replacing_range_at(5, { 1, 2, 6, 4 }), "");
}

TEST(VectorTest, ReplacingRangeAtMoreElementsInSourceThanCapacity)
{
    const vector<int> vector_under_test({5, -3, 4, -9});
    EXPECT_DEATH(vector_under_test.replacing_range_at(2, { 1, 2, 6, 4, 8, 9, -10 }), "");
}

TEST(VectorTest, ReplacingRangeAtWithFunctionalVector)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto replaced_vector = vector_under_test.replacing_range_at(4, vector<int>({9, -10, 8}));
    EXPECT_EQ(vector<int>({ 1, 4, 2, 5, 9, -10, 8, 7, 1 }), replaced_vector);
}

TEST(VectorTest, ReplacingRangeAtWithStdVector)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto replaced_vector = vector_under_test.replacing_range_at(4, std::vector<int>({9, -10, 8}));
    EXPECT_EQ(vector<int>({ 1, 4, 2, 5, 9, -10, 8, 7, 1 }), replaced_vector);
}

TEST(VectorTest, ReplacingRangeAtWithInitializerList)
{
    const vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto replaced_vector = vector_under_test.replacing_range_at(4, std::initializer_list<int>({9, -10, 8}));
    EXPECT_EQ(vector<int>({ 1, 4, 2, 5, 9, -10, 8, 7, 1 }), replaced_vector);
}

TEST(VectorTest, Fill)
{
    vector<int> vector_under_test({1, 3, -6, 4, -9});
    vector_under_test.fill(7);
    EXPECT_EQ(vector<int>({ 7, 7, 7, 7, 7 }), vector_under_test);
}

TEST(VectorTest, RepeatingConstructor)
{
    const vector<std::string> vector_under_test(3, "John");
    EXPECT_EQ(vector<std::string>({ "John", "John", "John" }), vector_under_test);
}

TEST(VectorTest, EqualityOperatorEmptyVectors)
{
    const vector<int> vec1;
    const vector<int> vec2;
    EXPECT_TRUE(vec1 == vec2);
    EXPECT_FALSE(vec1 != vec2);
}

TEST(VectorTest, EqualityOperatorUnequalSizes)
{
    const vector<int> vec1({1, 2, 3});
    const vector<int> vec2({1, 2, 3, 4});
    EXPECT_TRUE(vec1 != vec2);
    EXPECT_FALSE(vec1 == vec2);
}

TEST(VectorTest, EqualityOperatorEqualSizesDifferentElements)
{
    const vector<int> vec1({1, 2, 3});
    const vector<int> vec2({1, 2, 4});
    EXPECT_TRUE(vec1 != vec2);
    EXPECT_FALSE(vec1 == vec2);
}

TEST(VectorTest, EqualityOperatorEqualVectors)
{
    const vector<int> vec1({1, 2, 3});
    const vector<int> vec2({1, 2, 3});
    EXPECT_TRUE(vec1 == vec2);
    EXPECT_FALSE(vec1 != vec2);
}

TEST(VectorTest, EqualityOperatorCustomTypeEqualVectors)
{
    const vector<person> vec1({
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate")
    });
    
    const vector<person> vec2({
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate")
    });

    EXPECT_TRUE(vec1 == vec2);
    EXPECT_FALSE(vec1 != vec2);
}

TEST(VectorTest, EqualityOperatorCustomTypeUnequalSizes)
{
    const vector<person> vec1({
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate")
    });
    
    const vector<person> vec2({
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate"),
        person(50, "Barbara")
    });

    EXPECT_FALSE(vec1 == vec2);
    EXPECT_TRUE(vec1 != vec2);
}

TEST(VectorTest, EqualityOperatorCustomTypeUnequalVectors)
{
    const vector<person> vec1({
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate")
    });
    
    const vector<person> vec2({
        person(15, "Jake"),
        person(53, "Bob"),
        person(35, "Suzan")
    });

    EXPECT_FALSE(vec1 == vec2);
    EXPECT_TRUE(vec1 != vec2);
}

TEST(VectorTest, ClearEmptyVector)
{
    vector<int> vector_under_test;
    EXPECT_TRUE(vector_under_test.is_empty());
    vector_under_test.clear();
    EXPECT_EQ(0, vector_under_test.size());
    EXPECT_TRUE(vector_under_test.is_empty());
}

TEST(VectorTest, ClearFilledVector)
{
    vector<int> vector_under_test({5, -3, 4, -9});
    EXPECT_FALSE(vector_under_test.is_empty());
    vector_under_test.clear();
    EXPECT_EQ(0, vector_under_test.size());
    EXPECT_TRUE(vector_under_test.is_empty());
}

TEST(VectorTest, CapacityReserveClear)
{
    vector<int> vector_under_test;
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

TEST(VectorTest, ResizeEmptyVector)
{
    vector<int> vector_under_test;
    vector_under_test.resize(5);
    EXPECT_EQ(vector<int>({ 0, 0, 0, 0, 0 }), vector_under_test);
    EXPECT_EQ(5, vector_under_test.capacity());
    EXPECT_EQ(5, vector_under_test.size());
}

TEST(VectorTest, ResizeSmallerThanCurrentSize)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_EQ(9, vector_under_test.capacity());
    EXPECT_EQ(9, vector_under_test.size());
    
    vector_under_test.resize(5);
    EXPECT_EQ(vector<int>({1, 4, 2, 5, 8}), vector_under_test);
    EXPECT_EQ(9, vector_under_test.capacity());
    EXPECT_EQ(5, vector_under_test.size());
}

TEST(VectorTest, AllOfFalse)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_FALSE(vector_under_test.all_of([](const int& number) { return number > 5; }));
}

TEST(VectorTest, AllOfTrue)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.all_of([](const int& number) {
        return number < 10;
    });
    EXPECT_TRUE(vector_under_test.all_of([](const int& number) { return number < 10; }));
}

#ifdef PARALLEL_ALGORITHM_AVAILABLE
TEST(VectorTest, AllOfParallelFalse)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_FALSE(vector_under_test.all_of_parallel([](const int& number) { return number > 5; }));
}

TEST(VectorTest, AllOfParallelTrue)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector_under_test.all_of([](const int& number) {
        return number < 10;
    });
    EXPECT_TRUE(vector_under_test.all_of_parallel([](const int& number) { return number < 10; }));
}
#endif

TEST(VectorTest, AnyOfFalse)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_FALSE(vector_under_test.any_of([](const int& number) { return number > 20; }));
}

TEST(VectorTest, AnyOfTrue)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_TRUE(vector_under_test.any_of([](const int& number) { return number >= 7; }));
}

#ifdef PARALLEL_ALGORITHM_AVAILABLE
TEST(VectorTest, AnyOfParallelFalse)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_FALSE(vector_under_test.any_of_parallel([](const int& number) { return number > 20; }));
}

TEST(VectorTest, AnyOfParallelTrue)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_TRUE(vector_under_test.any_of_parallel([](const int& number) { return number >= 7; }));
}
#endif

TEST(VectorTest, NoneOfFalse)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_FALSE(vector_under_test.none_of([](const int& number) { return number > 7; }));
}

TEST(VectorTest, NoneOfTrue)
{
    vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_TRUE(vector_under_test.none_of([](const int& number) { return number < -2; }));
}

#ifdef PARALLEL_ALGORITHM_AVAILABLE
TEST(VectorTest, NoneOfParallelFalse)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_FALSE(vector_under_test.none_of_parallel([](const int& number) { return number > 7; }));
}

TEST(VectorTest, NoneOfParallelTrue)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_TRUE(vector_under_test.none_of_parallel([](const int& number) { return number < -2; }));
}

TEST(VectorTest, ForEachParallel)
{
    functional_vector<int> vector_under_test({1, 4, 2, 5, 8, 3, 1, 7, 1});
    EXPECT_EQ(9, vector_under_test.size());
    std::atomic<int> counter(0);
    vector_under_test.for_each_parallel([&](const int& element) { ++counter; });
    EXPECT_EQ(9, counter);
}
#endif

TEST(VectorTest, Distinct)
{
    const vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    const auto& unique_numbers = numbers.distinct();
    EXPECT_EQ(set<int>({1, 2, 3, 4, 5, 7, 8}), unique_numbers);
}

TEST(VectorTest, DistinctCustomType)
{
    const vector<person> persons({
        person(15, "Jake"),
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate"),
        person(25, "Kate"),
        person(62, "Bob")
    });
    
    const auto& unique_persons = persons.distinct<person_comparator>();
    
    const set<person, person_comparator> expected({
        person(15, "Jake"),
        person(18, "Jannet"),
        person(25, "Kate"),
        person(62, "Bob")
    });
    
    EXPECT_EQ(expected, unique_persons);
}
