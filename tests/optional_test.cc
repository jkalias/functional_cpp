// MIT License
//
// Copyright (c) 2023 Ioannis Kaliakatsos
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
#include "optional.h"

using namespace fcpp;

TEST(OptionalTest, InvalidTest)
{
	const optional_t<int> index;
	EXPECT_FALSE(index.has_value());
}

TEST(OptionalTest, ValidTest)
{
	const optional_t<int> index(5);
	EXPECT_TRUE(index.has_value());
	EXPECT_EQ(5, index.value());
}

TEST(OptionalTest, AssignmentFromValueTest)
{
	optional_t<int> index(5);
	index = 3;
	EXPECT_TRUE(index.has_value());
	EXPECT_EQ(3, index.value());
}

TEST(OptionalTest, AssignmentFromOptionalTest)
{
	optional_t<int> v1(5);
	const optional_t<int> v2(2);
	v1 = v2;
	EXPECT_TRUE(v1.has_value());
	EXPECT_EQ(2, v1.value());
	EXPECT_TRUE(v2.has_value());
	EXPECT_EQ(2, v2.value());
}

TEST(OptionalTest, AssignmentFromNullOptionalTest)
{
	optional_t<int> v1(5);
	const optional_t<int> v2;
	v1 = v2;
	EXPECT_FALSE(v1.has_value());
	EXPECT_FALSE(v2.has_value());
}

TEST(OptionalTest, AssignmentToNullFromValueTest)
{
	optional_t<int> index;
	index = 3;
	EXPECT_TRUE(index.has_value());
	EXPECT_EQ(3, index.value());
}

TEST(OptionalTest, AssignmentToNullFromOptionalTest)
{
	optional_t<int> v1;
	const optional_t<int> v2(2);
	v1 = v2;
	EXPECT_TRUE(v1.has_value());
	EXPECT_EQ(2, v1.value());
	EXPECT_TRUE(v2.has_value());
	EXPECT_EQ(2, v2.value());
}

TEST(OptionalTest, AssignmentToNullFromNullOptionalTest)
{
	optional_t<int> v1;
	const optional_t<int> v2;
	v1 = v2;
	EXPECT_FALSE(v1.has_value());
	EXPECT_FALSE(v2.has_value());
}

TEST(OptionalTest, CopyConstructorTest)
{
	const optional_t<int> v2(5);
	const optional_t<int> v1(v2);
	EXPECT_TRUE(v1.has_value());
	EXPECT_EQ(5, v2.value());
}

TEST(OptionalTest, CopyConstructorNullTest)
{
	const optional_t<int> v2;
	const optional_t<int> v1(v2);
	EXPECT_FALSE(v1.has_value());
}
