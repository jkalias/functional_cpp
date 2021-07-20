#include <gtest/gtest.h>
#include <string>
#include "functional_vector.h"

struct child {
    child(int age)
    : age(age)
    {
    }
    int age;
};

struct person {
    person(int age, std::string name)
    : age(age), name(std::move(name))
    {
    }
    int age;
    std::string name;
};

template<typename T>
void debug_vector(const functional_vector<T>& vec) {
    vec.for_each([](const auto& element) {
        std::cout << element << std::endl;
    });
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

TEST(FunctionalVectorTest, AddRangeFromStdVectorTest) {
    auto vector_under_test = functional_vector<int>();
    vector_under_test.add_range(std::vector {1, 2, 3});
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

TEST(FunctionalVectorTest, ZipWithStdVectorUnequalSizesThrowsTest) {
    const auto ages_vector = functional_vector({32, 25, 53, 62});
    EXPECT_DEATH(ages_vector.zip(std::vector{"Jake", "Mary"}), "");
}

TEST(FunctionalVectorTest, ZipWithFunctionalVectorUnequalSizesThrowsTest) {
    const auto ages_vector = functional_vector({32, 25, 53, 62});
    const auto names_vector = functional_vector<std::string>({"Jake", "Mary"});
    EXPECT_DEATH(ages_vector.zip(names_vector), "");
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

TEST(FunctionalVectorTest, SubscriptOperatorAssignTest) {
    auto vector_under_test = functional_vector({3, 1, 9, -4});
    vector_under_test[2] = 7;
    EXPECT_EQ(4, vector_under_test.size());
    EXPECT_EQ(3, vector_under_test[0]);
    EXPECT_EQ(1, vector_under_test[1]);
    EXPECT_EQ(7, vector_under_test[2]);
    EXPECT_EQ(-4, vector_under_test[3]);
}
