#include <gtest/gtest.h>
#include <string>
#include "functional_vector.h"

TEST(functional_vector_test, default_constructor_test) {
    auto my_vector = functional_vector<int>();
    EXPECT_EQ(0, my_vector.size());
}
