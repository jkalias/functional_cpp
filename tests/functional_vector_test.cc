#include <gtest/gtest.h>
#include <string>
#include "functional_vector.h"

// Demonstrate some basic assertions.
TEST(functional_vector_test, BasicAssertions) {
    auto t = functional_vector<std::string>({"Mary", "John", "Kate"});
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}
