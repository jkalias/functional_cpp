#include <gtest/gtest.h>
#include <string>
#include "functional_vector.h"

struct Child {
    Child(int age)
    : age(age)
    {
    }
    int age;
};

struct Person {
    Person(int age, std::string name)
    : age(age), name(name)
    {
    }
    int age;
    std::string name;
};

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
    const auto mapped_vector = vector_under_test.map<Child>([](const auto& age) {
        return Child(age);
    });
    EXPECT_EQ(3, mapped_vector.size());
    EXPECT_EQ(1, mapped_vector[0].age);
    EXPECT_EQ(3, mapped_vector[1].age);
    EXPECT_EQ(4, mapped_vector[2].age);
}

TEST(FunctionalVectorTest, FilterTest) {
    auto vector_under_test = functional_vector({Child(1), Child(3), Child(4)});
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
    const auto vector_under_test = functional_vector({Child(1), Child(3), Child(4)});
    const auto filtered_vector = vector_under_test.filtered([](const auto& child) {
        return child.age < 2;
    });
    EXPECT_EQ(3, vector_under_test.size());
    EXPECT_EQ(1, filtered_vector.size());
    EXPECT_EQ(1, filtered_vector[0].age);
}

TEST(FunctionalVectorTest, ReverseTest) {
    auto vector_under_test = functional_vector({Child(6), Child(2), Child(9)});
    vector_under_test.reverse();
    EXPECT_EQ(3, vector_under_test.size());
    EXPECT_EQ(9, vector_under_test[0].age);
    EXPECT_EQ(2, vector_under_test[1].age);
    EXPECT_EQ(6, vector_under_test[2].age);
}

TEST(FunctionalVectorTest, ReversedTest) {
    const auto vector_under_test = functional_vector({Child(6), Child(2), Child(9)});
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
    const auto zipped_vector = ages_vector.zip(std::vector{"Jake", "Mary", "John"});
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
    const auto zipped_vector = ages_vector.zip(std::vector{"Jake", "Mary", "John"});
    EXPECT_EQ(3, zipped_vector.size());
    
    EXPECT_EQ(32, zipped_vector[0].first);
    EXPECT_EQ("Jake", zipped_vector[0].second);
    
    EXPECT_EQ(25, zipped_vector[1].first);
    EXPECT_EQ("Mary", zipped_vector[1].second);
    
    EXPECT_EQ(53, zipped_vector[2].first);
    EXPECT_EQ("John", zipped_vector[2].second);
}
