#include <gtest/gtest.h>
#include "functional_vector.h"

TEST(FunctionalVectorTest, AddTest) {
	auto my_vector = functional_vector<int>();
	EXPECT_EQ(0, my_vector.size());

	my_vector.add(5);
    EXPECT_EQ(1, my_vector.size());
	EXPECT_EQ(5, my_vector[0]);
	
	my_vector.add(-1);
	EXPECT_EQ(2, my_vector.size());
	EXPECT_EQ(-1, my_vector[1]);
}
