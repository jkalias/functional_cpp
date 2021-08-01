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

#pragma once
#include <vector>
#include <algorithm>
#include <optional>
#include "index_range.h"

// A lightweight wrapper around std::vector, enabling fluent and functional
// programming on the vector itself, rather than using the more procedural style
// of the standard library algorithms.
//
// Member functions can be mutating (eg. my_vector.reverse()) or
// non-mutating (eg. my_vector.reversed()) enforcing thread safety if needed
template <typename T>
class functional_vector
{
public:
	functional_vector()
		: backing_vector_()
	{
	}

	explicit functional_vector(const std::vector<T>& vector)
		: backing_vector_(std::move(vector))
	{
	}

    explicit functional_vector(const std::initializer_list<T>& list)
		: backing_vector_(std::move(list))
	{
	}

	// Performs the functional `map` algorithm, in which every element of the resulting vector is the
	// output of applying the transform function on every element of this instance.
	//
	// example:
	//      const functional_vector<int> input_vector({ 1, 3, -5 });
	//      const auto output_vector = input_vector.map<std::string>([](const auto& element) {
	//      	return std::to_string(element);
	//      });
	//
	// outcome:
	//      output_vector -> functional_vector<std::string>({ "1", "3", "-5" })
	//
	// is equivalent to:
	//      const functional_vector<int> input_vector({ 1, 3, -5 });
	//      functional_vector<std::string> output_vector;
	//      for (auto i = 0; i < input_vector.size(); ++i) {
	//      	output_vector.insert_back(std::to_string(input_vector[i]));
	//      }
	template <typename U>
	functional_vector<U> map(const std::function<U(T)>& transform) const
	{
		std::vector<U> transformed_vector;
		transformed_vector.reserve(backing_vector_.size());
		std::transform(backing_vector_.begin(),
		               backing_vector_.end(),
		               std::back_inserter(transformed_vector),
		               transform);
		return functional_vector<U>(transformed_vector);
	}

	// Performs the functional `filter` algorithm, in which all elements of this instance
	// which match the given predicate are kept (mutating)
	//
	// example:
	//      functional_vector<int> numbers({ 1, 3, -5, 2, -1, 9, -4 });
	//      numbers.filter([](const auto& element) {
	//          return element >= 1.5;
	//      });
	//
	// outcome:
	//      numbers -> functional_vector<int>({ 3, 2, 9 });
	//
	// is equivalent to:
	//      functional_vector<int> numbers({ 1, 3, -5, 2, -1, 9, -4 });
	//      for (auto i = 0; i < numbers.size(); ++i) {
	//          if (numbers[i] >= 1.5) {
	//              continue;
	//          }
	//          numbers.remove_at(i);
	//          i--;
	//      }
	functional_vector& filter(const std::function<bool(T)>& predicate_to_keep)
	{
		backing_vector_.erase(std::remove_if(backing_vector_.begin(),
		                                     backing_vector_.end(),
		                                     [&](const auto& element) { return !predicate_to_keep(element); }),
		                      backing_vector_.end());
		return *this;
	}

	// Performs the functional `filter` algorithm in a copy of this instance, in which all elements of
	// the copy which match the given predicate (non-mutating)
	//
	// example:
	//      const functional_vector<int> numbers({ 1, 3, -5, 2, -1, 9, -4 });
	//      const auto filtered_numbers = numbers.filtered([](const auto& element) {
	//          return element >= 1.5;
	//      });
	//
	// outcome:
	//      numbers -> functional_vector<int>({ 1, 3, -5, 2, -1, 9, -4 })
	//      filtered_numbers -> functional_vector<int>({ 3, 2, 9 })
	//
	// is equivalent to:
	//      const functional_vector<int> numbers({ 1, 3, -5, 2, -1, 9, -4 });
	//      functional_vector<int> filtered_numbers;
	//      for (auto i = 0; i < numbers.size(); ++i) {
	//          if (numbers[i] >= 1.5) {
	//              filtered_numbers.insert_back(numbers[i]);
	//          }
	//      }
	functional_vector filtered(const std::function<bool(T)>& predicate_to_keep) const
	{
		std::vector<T> filtered_vector;
		filtered_vector.reserve(backing_vector_.size());
		std::copy_if(backing_vector_.begin(),
		             backing_vector_.end(),
		             std::back_inserter(filtered_vector),
		             predicate_to_keep);
		return functional_vector(filtered_vector);
	}

	// Reverses the order of the elements in place (mutating)
	//
	// example:
	//      functional_vector<int> my_vector({ 1, 3, -5, 2, -1, 9, -4 });
	//      my_vector.reverse();
	//
	// outcome:
	//      my_vector -> functional_vector<int>({ -4, 9, -1, 2, -5, 3, 1 })
	functional_vector& reverse()
	{
		std::reverse(backing_vector_.begin(), backing_vector_.end());
		return *this;
	}

	// Returns a copy of this instance, whose elements are in reverse order (non-mutating)
	//
	// example:
	//      const functional_vector<int> input_vector({ 1, 3, -5, 2, -1, 9, -4 });
	//      const auto reversed_vector = input_vector.reversed();
	//
	// outcome:
	//      input_vector -> functional_vector<int>({ 1, 3, -5, 2, -1, 9, -4 });
	//      reversed_vector -> functional_vector<int>({ -4, 9, -1, 2, -5, 3, 1 })
	[[nodiscard]] functional_vector reversed() const
	{
		std::vector<T> reversed_vector(backing_vector_);
		std::reverse(reversed_vector.begin(), reversed_vector.end());
		return functional_vector(reversed_vector);
	}

	template <typename U>
	struct functional_tuple
	{
		T first;
		U second;
	};

	// Performs the functional `zip` algorithm, in which every element of the resulting vector is a
	// tuple of this instance's element (first) and the second vector's element (second) at the same
	// index. The sizes of the two vectors must be equal.
	//
	// example:
	//      const functional_vector ages_vector({32, 25, 53});
	//      const functional_vector<std::string> names_vector({"Jake", "Mary", "John"});
	//      const auto zipped_vector = ages_vector.zip(names_vector);
	//
	// outcome:
	//      zipped_vector -> functional_vector<functional_vector<int>::functional_tuple<std::string>>({
	//                          (32, "Jake"),
	//                          (25, "Mary"),
	//                          (53, "John"),
	//                       })
	//
	// is equivalent to:
	//      const functional_vector ages_vector({32, 25, 53});
	//      const functional_vector<std::string> names_vector({"Jake", "Mary", "John"});
	//      functional_vector<functional_vector<int>::functional_tuple<std::string>> zipped_vector;
	//      for (auto i = 0; i < ages_vector.size(); ++i) {
	//          functional_vector<int>::functional_tuple<std::string> tuple;
	//          tuple.first = ages_vector[i];
	//          tuple.second = names_vector[i];
	//          zipped_vector.insert_back(tuple);
	//      }
	template <typename U>
	[[nodiscard]] functional_vector<functional_tuple<U>> zip(const functional_vector<U>& vector) const
	{
		return zip_impl<U>(vector.begin(), vector.end());
	}

	// Performs the functional `zip` algorithm, in which every element of the resulting vector is a
	// tuple of this instance's element (first) and the second vector's element (second) at the same
	// index. The sizes of the two vectors must be equal.
	//
    // example:
    //      const functional_vector ages_vector({32, 25, 53});
    //      const std::vector<std::string> names_vector({"Jake", "Mary", "John"});
    //      const auto zipped_vector = ages_vector.zip(names_vector);
    //
    // outcome:
    //      zipped_vector -> functional_vector<functional_vector<int>::functional_tuple<std::string>>({
    //                          (32, "Jake"),
    //                          (25, "Mary"),
    //                          (53, "John"),
    //                       })
    //
    // is equivalent to:
    //      const functional_vector ages_vector({32, 25, 53});
    //      const std::vector<std::string> names_vector({"Jake", "Mary", "John"});
    //      functional_vector<functional_vector<int>::functional_tuple<std::string>> zipped_vector;
    //      for (auto i = 0; i < ages_vector.size(); ++i) {
    //          functional_vector<int>::functional_tuple<std::string> tuple;
    //          tuple.first = ages_vector[i];
    //          tuple.second = names_vector[i];
    //          zipped_vector.insert_back(tuple);
    //      }
	template <typename U>
	[[nodiscard]] functional_vector<functional_tuple<U>> zip(const std::vector<U>& vector) const
	{
		return zip_impl<U>(vector.begin(), vector.end());
	}

    // example:
    //      const functional_vector ages_vector({32, 25, 53});
    //      const std::initializer_list<std::string> names_vector({"Jake", "Mary", "John"});
    //      const auto zipped_vector = ages_vector.zip(names_vector);
    //
    // outcome:
    //      zipped_vector -> functional_vector<functional_vector<int>::functional_tuple<std::string>>({
    //                          (32, "Jake"),
    //                          (25, "Mary"),
    //                          (53, "John"),
    //                       })
    //
    // is equivalent to:
    //      const functional_vector ages_vector({32, 25, 53});
    //      const std::initializer_list<std::string> names_vector({"Jake", "Mary", "John"});
    //      functional_vector<functional_vector<int>::functional_tuple<std::string>> zipped_vector;
    //      for (auto i = 0; i < ages_vector.size(); ++i) {
    //          functional_vector<int>::functional_tuple<std::string> tuple;
    //          tuple.first = ages_vector[i];
    //          tuple.second = names_vector[i];
    //          zipped_vector.insert_back(tuple);
    //      }
	template <typename U>
	[[nodiscard]] functional_vector<functional_tuple<U>> zip(const std::initializer_list<U>& list) const
	{
		return zip(std::vector(list));
	}

	// Sorts the vector in place (mutating). The comparison predicate takes two elements
	// `v1` and `v2` and returns true if the first element `v1` should appear before `v2`.
	//
	// example:
	//      struct person
	//      {
	//          int age;
	//          std::string name;
	//      };
	//      ...
	//      functional_vector persons_vector({
	//          person(45, "Jake"), person(34, "Bob"), person(52, "Manfred"), person(8, "Alice")
	//      });
	//      persons_vector.sort([](const auto& person1, const auto& person2) {
	//          return person1.name < person2.name;
	//      });
	//
	// outcome:
	//      person_vector -> functional_vector({
	//          person(8, "Alice"), person(34, "Bob"), person(45, "Jake"), person(52, "Manfred")
	//      });
	functional_vector& sort(const std::function<bool(T, T)>& comparison_predicate)
	{
		std::sort(backing_vector_.begin(),
		          backing_vector_.end(),
		          comparison_predicate);
		return *this;
	}

	// Sorts the vector in place in ascending order, when its elements support comparison by std::less_equal [<=] (mutating).
	//
	// example:
	//      functional_vector numbers({3, 1, 9, -4});
	//      numbers.sort_ascending();
	//
	// outcome:
	//      numbers -> functional_vector({-4, 1, 3, 9});
	functional_vector& sort_ascending()
	{
		return sort(std::less_equal<T>());
	}

	// Sorts the vector in place in descending order, when its elements support comparison by std::greater_equal [>=] (mutating).
	//
	// example:
	//      functional_vector numbers({3, 1, 9, -4});
	//      numbers.sort_ascending();
	//
	// outcome:
	//      numbers -> functional_vector({9, 3, 1, -4});
	functional_vector& sort_descending()
	{
		return sort(std::greater_equal<T>());
	}

	// Returns its elements copied and sorted (non-mutating). The comparison predicate takes two elements
	// `v1` and `v2` and returns true if the first element `v1` should appear before `v2`.
	//
	// example:
	//      struct person
	//      {
	//          int age;
	//          std::string name;
	//      };
	//      ...
	//      const functional_vector persons_vector({
	//          person(45, "Jake"), person(34, "Bob"), person(52, "Manfred"), person(8, "Alice")
	//      });
	//      auto sorted_persons_vector = persons_vector.sorted([](const auto& person1, const auto& person2) {
	//          return person1.name < person2.name;
	//      });
	//
	// outcome:
	//      sorted_persons_vector -> functional_vector({
	//          person(8, "Alice"), person(34, "Bob"), person(45, "Jake"), person(52, "Manfred")
	//      });
	functional_vector sorted(const std::function<bool(T, T)>& comparison_predicate) const
	{
		auto sorted_vector(backing_vector_);
		std::sort(sorted_vector.begin(),
		          sorted_vector.end(),
		          comparison_predicate);
		return functional_vector(sorted_vector);
	}

	// Sorts its elements copied and sorted in ascending order, when its elements support comparison by std::less_equal [<=] (non-mutating).
	//
	// example:
	//      const functional_vector numbers({3, 1, 9, -4});
	//      auto sorted_numbers = numbers.sorted_ascending();
	//
	// outcome:
	//      sorted_numbers -> functional_vector({-4, 1, 3, 9});
	[[nodiscard]] functional_vector sorted_ascending() const
	{
		return sorted(std::less_equal<T>());
	}

	// Sorts its elements copied and sorted in descending order, when its elements support comparison by std::greater_equal [>=] (non-mutating).
	//
	// example:
	//      const functional_vector numbers({3, 1, 9, -4});
	//      auto sorted_numbers = numbers.sorted_descending();
	//
	// outcome:
	//      sorted_numbers -> functional_vector({9, 3, 1, -4});
	[[nodiscard]] functional_vector sorted_descending() const
	{
		return sorted(std::greater_equal<T>());
	}

	// Executes the given operation for each element of the vector. The operation must not
	// change the vector's contents during execution.
	const functional_vector& for_each(const std::function<void(T)>& operation) const
	{
		std::for_each(backing_vector_.begin(),
		              backing_vector_.end(),
		              operation);
		return *this;
	}

	// Returns the first index in which the given element is found in the vector.
	// In case of multiple occurrences, only the first index is returned
	// (see find_all_indices for multiple occurences).
	//
	// example:
	//      const functional_vector numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
	//      const auto index_of_one = numbers.find_first_index(1);
	//      const auto index_of_nine = numbers.find_first_index(9);
	//
	// outcome:
	//      index_of_one.value() -> 0
	//      index_of_one.has_value() -> true
	//      index_of_nine.has_value() -> false
	[[nodiscard]] std::optional<size_t> find_first_index(const T& element) const
	{
		if (auto it = std::find(backing_vector_.begin(),
		                        backing_vector_.end(),
		                        element); it != backing_vector_.end())
		{
			return std::distance(backing_vector_.begin(), it);
		}
		return std::nullopt;
	}

	// Returns the last index in which the given element is found in the vector.
	// In case of multiple occurrences, only the last index is returned
	// (see find_all_indices for multiple occurences).
	//
	// example:
	//      const functional_vector numbers({1, 4, 2, 5, -6, 3, 1, 7, 1});
	//      const auto index_of_one = numbers.find_last_index(1);
	//      const auto index_of_nine = numbers.find_last_index(9);
	//
	// outcome:
	//      index_of_one.value() -> 8
	//      index_of_one.has_value() -> true
	//      index_of_nine.has_value() -> false
	[[nodiscard]] std::optional<size_t> find_last_index(const T& element) const
	{
		if (auto it = std::find(backing_vector_.rbegin(),
		                        backing_vector_.rend(),
		                        element); it != backing_vector_.rend())
		{
			return std::distance(it, backing_vector_.rend()) - 1;
		}
		return std::nullopt;
	}

	// Returns all indices in which the given element is found in the vector.
	//
	// example:
	//      const functional_vector numbers({1, 4, 2, 5, 8, 3, 1, 9, 1});
	//      const auto indices_of_one = numbers.find_all_indices(1);
	//      const auto indices_of_ten = numbers.find_all_indices(10);
	//
	// outcome:
	//      indices_of_one -> { 0, 6, 8 }
	//      indices_of_ten -> empty vector
	[[nodiscard]] std::vector<size_t> find_all_indices(const T& element) const
	{
		std::vector<size_t> indices;
		auto it = std::find(backing_vector_.begin(),
		                    backing_vector_.end(),
		                    element);
		while (it != backing_vector_.end())
		{
			indices.push_back(std::distance(backing_vector_.begin(), it));
			++it;
			it = std::find(it, backing_vector_.end(), element);
		}
		return indices;
	}

	// Removes the element at `index` (mutating)
    //
    // example:
    //      functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      numbers.remove_at(4);
    //
    // outcome:
    //      numbers -> functional_vector<int>({1, 4, 2, 5, 3, 1, 7, 1});
	functional_vector& remove_at(int index)
	{
		assert_smaller_size(index);
		backing_vector_.erase(begin() + index);
		return *this;
	}

	// Returns a copy of itself in which the element at `index` is removed (non-mutating)
    //
    // example:
    //      const functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      auto shorter_vector = numbers.removing_at(4);
    //
    // outcome:
    //      shorter_vector -> functional_vector<int>({1, 4, 2, 5, 3, 1, 7, 1});
	[[nodiscard]] functional_vector removing_at(int index) const
	{
		assert_smaller_size(index);
		auto copy(backing_vector_);
		copy.erase(copy.begin() + index);
		return functional_vector(copy);
	}

	// Removes the last element, if present (mutating)
    //
    // example:
    //      functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      numbers.remove_last();
    //
    // outcome:
    //      numbers -> functional_vector<int>({1, 4, 2, 5, 8, 3, 1, 7});
	functional_vector& remove_last()
	{
		backing_vector_.pop_back();
		return *this;
	}

	// Returns a copy of itself in which the last element is removed (non-mutating)
    //
    // example:
    //      const functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      auto shorter_vector = numbers.removing_last();
    //
    // outcome:
    //      shorter_vector -> functional_vector<int>({1, 4, 2, 5, 8, 3, 1, 7});
	[[nodiscard]] functional_vector removing_last() const
	{
		auto copy(backing_vector_);
		copy.pop_back();
		return functional_vector(copy);
	}

	// Removes the first element, if present (mutating)
    //
    // example:
    //      functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      numbers.remove_first();
    //
    // outcome:
    //      numbers -> functional_vector<int>({4, 2, 5, 8, 3, 1, 7, 1});
	functional_vector& remove_first()
	{
		if (size() == 0)
		{
			return *this;
		}
		return remove_at(0);
	}

	// Returns a copy of itself in which the first element is removed (non-mutating)
    //
    // example:
    //      const functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      auto shorter_numbers = numbers.removing_first();
    //
    // outcome:
    //      shorter_numbers -> functional_vector<int>({4, 2, 5, 8, 3, 1, 7, 1});
	[[nodiscard]] functional_vector removing_first() const
	{
		if (size() == 0)
		{
			return *this;
		}
		return removing_at(0);
	}

	// Removes the elements whose index is contained in the given index range (mutating)
	//
	// example:
	//		functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
	//		numbers.remove_range(index_range::start_count(2, 3));
	//
	// outcome:
	//		numbers -> functional_vector<int>({ 1, 4, 2, 7, 1 })
	functional_vector& remove_range(index_range range)
	{
		if (!range.is_valid || size() < range.end + 1)
		{
			return *this;
		}
		backing_vector_.erase(begin() + range.start,
		                      begin() + range.start + range.count);
		return *this;
	}

	// Returns a copy by removing the elements whose index is contained in the given index range (non-mutating)
	//
	// example:
	//		const functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
	//		const auto shorter_vector = numbers.removing_range(index_range::start_count(2, 3));
	//
	// outcome:
	//		shorter_vector -> functional_vector<int>({ 1, 4, 3, 1, 7, 1 })
	[[nodiscard]] functional_vector removing_range(index_range range) const
	{
		if (!range.is_valid || size() < range.end + 1)
		{
			return *this;
		}
		auto shorter_vector(backing_vector_);
		shorter_vector.erase(shorter_vector.begin() + range.start,
		                     shorter_vector.begin() + range.start + range.count);
		return functional_vector(shorter_vector);
	}

	// Inserts an element at the given index, therefore changing the vector's contents (mutating)
	//
	// example:
	//      functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
	//      numbers.insert_at(3, 18);
	//
	// outcome:
	//      numbers -> functional_vector({1, 4, 2, 18, 5, 8, 3, 1, 7, 1});
	functional_vector& insert_at(int index, const T& element)
	{
		assert_smaller_or_equal_size(index);
		backing_vector_.insert(begin() + index, element);
		return *this;
	}

	// Returns a copy by inserting an element at the given index (non-mutating)
	//
	// example:
	//      const functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
	//      auto augmented_numbers = numbers.inserting_at(3, 18);
	//
	// outcome:
	//      augmented_numbers -> functional_vector({1, 4, 2, 18, 5, 8, 3, 1, 7, 1});
	[[nodiscard]] functional_vector inserting_at(int index, const T& element) const
	{
		assert_smaller_or_equal_size(index);
		auto copy(backing_vector_);
		copy.insert(copy.begin() + index, element);
		return functional_vector(copy);
	}

	// Inserts a range of elements starting at the given index, therefore changing the vector's contents (mutating)
	//
	// example:
	//      functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
	//      const functional_vector vector_to_insert({9, -5, 6});
	//      numbers.insert_at(3, vector_to_insert);
	//
	// outcome:
	//      numbers -> functional_vector({1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1});
	functional_vector& insert_at(int index, const functional_vector<T>& vector)
	{
		return insert_at_impl(index, vector.begin(), vector.end());
	}

	// Returns a copy by inserting a range of elements starting at the given index (non-mutating)
	//
	// example:
	//      const functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
	//      const functional_vector vector_to_insert({9, -5, 6});
	//      auto augmented_numbers = numbers.inserting_at(3, vector_to_insert);
	//
	// outcome:
	//      augmented_numbers -> functional_vector({1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1});
	[[nodiscard]] functional_vector inserting_at(int index, const functional_vector<T>& vector) const
	{
		return inserting_at_impl(index, vector.begin(), vector.end());
	}

	// Inserts a range of elements starting at the given index, therefore changing the vector's contents (mutating)
	//
	// example:
	//      functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
	//      const std::vector vector_to_insert({9, -5, 6});
	//      numbers.insert_at(3, vector_to_insert);
	//
	// outcome:
	//      numbers -> functional_vector({1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1});
	functional_vector& insert_at(int index, const std::vector<T>& vector)
	{
		return insert_at_impl(index, vector.begin(), vector.end());
	}

	// Returns a copy by inserting a range of elements starting at the given index (non-mutating)
	//
	// example:
	//      const functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
	//      const std::vector vector_to_insert({9, -5, 6});
	//      auto augmented_numbers = numbers.inserting_at(3, vector_to_insert);
	//
	// outcome:
	//      augmented_numbers -> functional_vector({1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1});
	[[nodiscard]] functional_vector inserting_at(int index, const std::vector<T>& vector) const
	{
		return inserting_at_impl(index, vector.begin(), vector.end());
	}

	// Inserts a range of elements starting at the given index, therefore changing the vector's contents (mutating)
	//
	// example:
	//      functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
	//      const std::initializer_list vector_to_insert({9, -5, 6});
	//      numbers.insert_at(3, vector_to_insert);
	//
	// outcome:
	//      numbers -> functional_vector({1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1});
	functional_vector& insert_at(int index, const std::initializer_list<T>& list)
	{
		return insert_at(index, std::vector(list));
	}

	// Returns a copy by inserting a range of elements starting at the given index (non-mutating)
	//
	// example:
	//      const functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
	//      const std::initializer_list vector_to_insert({9, -5, 6});
	//      auto augmented_numbers = numbers.inserting_at(3, vector_to_insert);
	//
	// outcome:
	//      augmented_numbers -> functional_vector({1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1});
	[[nodiscard]] functional_vector inserting_at(int index, const std::initializer_list<T>& list) const
	{
		return inserting_at(index, std::vector(list));
	}

	// Inserts a value at the end of the vector in place (mutating)
    //
    // example:
    //      functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      numbers.insert_back(18);
    //
    // outcome:
    //      numbers -> functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1, 18});
	functional_vector& insert_back(T value)
	{
		backing_vector_.push_back(value);
		return *this;
	}

	// Inserts a value at the beginning of the vector in place (mutating)
    //
    // example:
    //      functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      numbers.insert_front(18);
    //
    // outcome:
    //      numbers -> functional_vector({18, 1, 4, 2, 5, 8, 3, 1, 7, 1});
	functional_vector& insert_front(T value)
	{
		return insert_at(0, value);
	}

	// Makes a copy of the vector, inserts value at the end of the copy and returns the copy (non-mutating)
    //
    // example:
    //      const functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      auto augmented_numbers = numbers.inserting_last(18);
    //
    // outcome:
    //      augmented_numbers -> functional_vector({1, 4, 2, 5, 8, 3, 1, 7, 1, 18});
	[[nodiscard]] functional_vector inserting_last(T value) const
	{
		auto augmented_vector(backing_vector_);
		augmented_vector.push_back(value);
		return functional_vector(augmented_vector);
	}

	// Makes a copy of the vector, inserts value at the beginning of the copy and returns the copy (non-mutating)
    //
    // example:
    //      const functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      auto augmented_numbers = numbers.inserting_first(18);
    //
    // outcome:
    //      augmented_numbers -> functional_vector({18, 1, 4, 2, 5, 8, 3, 1, 7, 1});
	[[nodiscard]] functional_vector inserting_first(T value) const
	{
		return inserting_at(0, value);
	}

	// Inserts a range of values at the end of the vector in place (mutating)
    //
    // example:
    //      functional_vector<int> numbers({ 4, 5, 6 });
    //      numbers.insert_back(functional_vector({1, 2, 3}));
    //
    // outcome:
    //      numbers -> functional_vector<int> numbers({ 4, 5, 6, 1, 2, 3 });
	functional_vector& insert_back(const functional_vector<T>& vector)
	{
		return insert_last_range_impl(vector.begin(), vector.end());
	}

	// Inserts a range of values at the beginning of the vector in place (mutating)
    //
    // example:
    //      functional_vector<int> numbers({ 4, 5, 6 });
    //      numbers.insert_front(functional_vector({1, 2, 3}));
    //
    // outcome:
    //      numbers -> functional_vector<int> numbers({ 1, 2, 3, 4, 5, 6 });
	functional_vector& insert_front(const functional_vector<T>& vector)
	{
		return insert_first_range_impl(vector.begin(), vector.end());
	}

	// Makes a copy of the vector, inserts a range of values at the end of the copy, and returns the copy (non-mutating)
    //
    // example:
    //      const functional_vector<int> numbers({ 4, 5, 6 });
    //      auto augmented_numbers = numbers.inserting_last(functional_vector({1, 2, 3}));
    //
    // outcome:
    //      augmented_numbers -> functional_vector<int> numbers({ 4, 5, 6, 1, 2, 3 });
	[[nodiscard]] functional_vector inserting_last(const functional_vector<T>& vector) const
	{
		return inserting_last_range_impl(vector.begin(), vector.end());
	}

	// Makes a copy of the vector, inserts a range of values at the beginning of the copy, and returns the copy (non-mutating)
    //
    // example:
    //      const functional_vector<int> numbers({ 4, 5, 6 });
    //      auto augmented_numbers = numbers.inserting_first(functional_vector({1, 2, 3}));
    //
    // outcome:
    //      augmented_numbers -> functional_vector<int> numbers({ 1, 2, 3, 4, 5, 6 });
	[[nodiscard]] functional_vector inserting_first(const functional_vector<T>& vector) const
	{
		return inserting_first_range_impl(vector.begin(), vector.end());
	}

	// Inserts a range of values at the end of the vector in place (mutating)
    //
    // example:
    //      functional_vector<int> numbers({ 4, 5, 6 });
    //      numbers.insert_back(std::vector({1, 2, 3}));
    //
    // outcome:
    //      numbers -> functional_vector<int> numbers({ 4, 5, 6, 1, 2, 3 });
	functional_vector& insert_back(const std::vector<T>& vector)
	{
		return insert_last_range_impl(vector.begin(), vector.end());
	}

	// Inserts a range of values at the beginning of the vector in place (mutating)
    //
    // example:
    //      functional_vector<int> numbers({ 4, 5, 6 });
    //      numbers.insert_front(std::vector({1, 2, 3}));
    //
    // outcome:
    //      numbers -> functional_vector<int> numbers({ 1, 2, 3, 4, 5, 6 });
	functional_vector& insert_front(const std::vector<T>& vector)
	{
		return insert_first_range_impl(vector.begin(), vector.end());
	}

	// Makes a copy of the vector, inserts a range of values at the end of the copy, and returns the copy (non-mutating)
    //
    // example:
    //      const functional_vector<int> numbers({ 4, 5, 6 });
    //      auto augmented_numbers = numbers.inserting_last(std::vector({1, 2, 3}));
    //
    // outcome:
    //      augmented_numbers -> functional_vector<int> numbers({ 4, 5, 6, 1, 2, 3 });
	[[nodiscard]] functional_vector inserting_last(const std::vector<T>& vector) const
	{
		return inserting_last_range_impl(vector.begin(), vector.end());
	}

	// Makes a copy of the vector, inserts a range of values at the beginning of the copy, and returns the copy (non-mutating)
    //
    // example:
    //      const functional_vector<int> numbers({ 4, 5, 6 });
    //      auto augmented_numbers = numbers.inserting_first(std::vector({1, 2, 3}));
    //
    // outcome:
    //      augmented_numbers -> functional_vector<int> numbers({ 1, 2, 3, 4, 5, 6 });
	[[nodiscard]] functional_vector inserting_first(const std::vector<T>& vector) const
	{
		return inserting_first_range_impl(vector.begin(), vector.end());
	}

	// Inserts a range of values at the end of the vector in place (mutating)
    //
    // example:
    //      functional_vector<int> numbers({ 4, 5, 6 });
    //      numbers.insert_back(std::initializer_list({1, 2, 3}));
    //
    // outcome:
    //      numbers -> functional_vector<int> numbers({ 4, 5, 6, 1, 2, 3 });
	functional_vector& insert_back(const std::initializer_list<T>& list)
	{
		return insert_back(std::vector(list));
	}

	// Inserts a range of values at the beginning of the vector in place (mutating)
    //
    // example:
    //      functional_vector<int> numbers({ 4, 5, 6 });
    //      numbers.insert_front(std::initializer_list({1, 2, 3}));
    //
    // outcome:
    //      numbers -> functional_vector<int> numbers({ 1, 2, 3, 4, 5, 6 });
	functional_vector& insert_front(const std::initializer_list<T>& list)
	{
		return insert_front(std::vector(list));
	}

	// Makes a copy of the vector, inserts a range of values at the end of the copy, and returns the copy (non-mutating)
    //
    // example:
    //      const functional_vector<int> numbers({ 4, 5, 6 });
    //      auto augmented_numbers = numbers.inserting_last(std::initializer_list({1, 2, 3}));
    //
    // outcome:
    //      augmented_numbers -> functional_vector<int> numbers({ 4, 5, 6, 1, 2, 3 });
	[[nodiscard]] functional_vector inserting_last(const std::initializer_list<T>& list) const
	{
		return inserting_last(std::vector(list));
	}

	// Makes a copy of the vector, inserts a range of values at the beginning of the copy, and returns the copy (non-mutating)
    //
    // example:
    //      const functional_vector<int> numbers({ 4, 5, 6 });
    //      auto augmented_numbers = numbers.inserting_first(std::initializer_list({1, 2, 3}));
    //
    // outcome:
    //      augmented_numbers -> functional_vector<int> numbers({ 1, 2, 3, 4, 5, 6 });
	[[nodiscard]] functional_vector inserting_first(const std::initializer_list<T>& list) const
	{
		return inserting_first(std::vector(list));
	}

	// Replaces the existing contents starting at `index` with the contents of the given vector (mutating)
    //
    // example:
    //      functional_vector numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      numbers.replace_range_at(4, functional_vector({9, -10, 8}));
    //
    // outcome:
    //      numbers -> functional_vector({ 1, 4, 2, 5, 9, -10, 8, 7, 1 })
	functional_vector& replace_range_at(int index, const functional_vector<T>& vector)
	{
		return replace_range_at_imp(index, vector.begin(), vector.end());
	}

    // Replaces the existing contents starting at `index` with the contents of the given vector (mutating)
    //
    // example:
    //      functional_vector numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      numbers.replace_range_at(4, std::vector({9, -10, 8}));
    //
    // outcome:
    //      numbers -> functional_vector({ 1, 4, 2, 5, 9, -10, 8, 7, 1 })
	functional_vector& replace_range_at(int index, const std::vector<T>& vector)
	{
		return replace_range_at_imp(index, vector.begin(), vector.end());
	}

    // Replaces the existing contents starting at `index` with the contents of the given vector (mutating)
    //
    // example:
    //      functional_vector numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      numbers.replace_range_at(4, std::initializer_list({9, -10, 8}));
    //
    // outcome:
    //      numbers -> functional_vector({ 1, 4, 2, 5, 9, -10, 8, 7, 1 })
	functional_vector& replace_range_at(int index, const std::initializer_list<T>& list)
	{
		return replace_range_at(index, std::vector(list));
	}

    // Returns a copy whose elements starting at `index` are replaced with the contents of the given vector (non-mutating)
    //
    // example:
    //      const functional_vector numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      auto replaced_numbers = numbers.replacing_range_at(4, functional_vector({9, -10, 8}));
    //
    // outcome:
    //      replaced_numbers -> functional_vector({ 1, 4, 2, 5, 9, -10, 8, 7, 1 })
	[[nodiscard]] functional_vector replacing_range_at(int index, const functional_vector<T>& vector) const
	{
		return replacing_range_at_imp(index, vector.begin(), vector.end());
	}

    // Returns a copy whose elements starting at `index` are replaced with the contents of the given vector (non-mutating)
    //
    // example:
    //      const functional_vector numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      auto replaced_numbers = numbers.replacing_range_at(4, std::vector({9, -10, 8}));
    //
    // outcome:
    //      replaced_numbers -> functional_vector({ 1, 4, 2, 5, 9, -10, 8, 7, 1 })
	[[nodiscard]] functional_vector replacing_range_at(int index, const std::vector<T>& vector) const
	{
		return replacing_range_at_imp(index, vector.begin(), vector.end());
	}

    // Returns a copy whose elements starting at `index` are replaced with the contents of the given vector (non-mutating)
    //
    // example:
    //      const functional_vector numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      auto replaced_numbers = numbers.replacing_range_at(4, std::initializer_list({9, -10, 8}));
    //
    // outcome:
    //      replaced_numbers -> functional_vector({ 1, 4, 2, 5, 9, -10, 8, 7, 1 })
	[[nodiscard]] functional_vector replacing_range_at(int index, const std::initializer_list<T>& list) const
	{
		return replacing_range_at(index, std::vector(list));
	}

    // Replaces all existing elements with a constant element (mutating)
    //
    // example:
    //      functional_vector numbers({1, 3, -6, 4, -9});
    //      numbers.fill(7);
    //
    // outcome:
    //      numbers -> functional_vector({ 7, 7, 7, 7, 7 })
	functional_vector& fill(const T& element)
	{
		std::fill(backing_vector_.begin(),
		          backing_vector_.end(),
		          element);
		return *this;
	}

    // Creates a new vector of a given size, where all elements are equal
    //
    // example:
    //      const auto filled_vector = functional_vector<std::string>::filled("John", 3);
    //
    // outcome:
    //      filled_vector -> functional_vector<std::string>({ "John", "John", "John" })
	[[nodiscard]] static functional_vector filled(const T& element, size_t count)
	{
		auto backing_vector = std::vector<T>();
		backing_vector.resize(count);
		std::fill(backing_vector.begin(),
		          backing_vector.end(),
		          element);
		return functional_vector(backing_vector);
	}
	
    // Returns the size of the vector (how many elements it contains)
	size_t size() const
	{
		return backing_vector_.size();
	}

    // Returns the constant begin iterator, useful for other standard library algorithms
	[[nodiscard]] typename std::vector<T>::const_iterator begin() const
	{
		return backing_vector_.begin();
	}

    // Returns the constant end iterator, useful for other standard library algorithms
	[[nodiscard]] typename std::vector<T>::const_iterator end() const
	{
		return backing_vector_.end();
	}
	
    // Returns a reference to the element in the given index, allowing subscripting and value editing.
    // Bounds checking (assert) is enabled for debug builds.
	T& operator[](int index)
	{
		assert_smaller_size(index);
		return backing_vector_[index];
	}

    // Returns a constant reference to the element in the given index, allowing subscripting.
    // Bounds checking (assert) is enabled for debug builds.
	const T& operator[](int index) const
	{
		assert_smaller_size(index);
		return backing_vector_[index];
	}
	
    // Returns true if both instances have equal sizes and the corresponding elements (same index) are equal
	bool operator ==(const functional_vector<T>& rhs) const
	{
		return std::equal(backing_vector_.begin(),
		                  backing_vector_.end(),
		                  rhs.begin(),
		                  rhs.end());
	}

    // Returns false if either the sizes are not equal or at least corresponding element (same index) is not equal
	bool operator !=(const functional_vector<T>& rhs) const
	{
		return !((*this) == rhs);
	}

private:
	std::vector<T> backing_vector_;

	functional_vector& insert_last_range_impl(const typename std::vector<T>::const_iterator& vec_begin,
	                                          const typename std::vector<T>::const_iterator& vec_end)
	{
		backing_vector_.insert(backing_vector_.end(),
		                       vec_begin,
		                       vec_end);
		return *this;
	}

	functional_vector& insert_first_range_impl(const typename std::vector<T>::const_iterator& vec_begin,
	                                           const typename std::vector<T>::const_iterator& vec_end)
	{
		backing_vector_.insert(backing_vector_.begin(),
		                       vec_begin,
		                       vec_end);
		return *this;
	}

	[[nodiscard]] functional_vector inserting_last_range_impl(const typename std::vector<T>::const_iterator& vec_begin,
	                                                          const typename std::vector<T>::const_iterator& vec_end)
	const
	{
		auto augmented_vector(backing_vector_);
		augmented_vector.reserve(augmented_vector.size() + std::distance(vec_begin, vec_end));
		augmented_vector.insert(augmented_vector.end(),
		                        vec_begin,
		                        vec_end);
		return functional_vector(augmented_vector);
	}

	[[nodiscard]] functional_vector inserting_first_range_impl(const typename std::vector<T>::const_iterator& vec_begin,
	                                                           const typename std::vector<T>::const_iterator& vec_end)
	const
	{
		auto augmented_vector(backing_vector_);
		augmented_vector.reserve(augmented_vector.size() + std::distance(vec_begin, vec_end));
		augmented_vector.insert(augmented_vector.begin(),
		                        vec_begin,
		                        vec_end);
		return functional_vector(augmented_vector);
	}

	template <typename U>
	[[nodiscard]] functional_vector<functional_tuple<U>> zip_impl(
		const typename std::vector<U>::const_iterator& vec_begin,
		const typename std::vector<U>::const_iterator& vec_end) const
	{
		const auto vec_size = std::distance(vec_begin, vec_end);
		assert(backing_vector_.size() == vec_size);
		std::vector<functional_tuple<U>> combined_vector;
		combined_vector.reserve(vec_size);
		for (size_t i = 0; i < vec_size; ++i)
		{
			combined_vector.push_back({backing_vector_[i], *(vec_begin + i)});
		}
		return functional_vector<functional_tuple<U>>(combined_vector);
	}

	functional_vector& insert_at_impl(int index,
	                                  const typename std::vector<T>::const_iterator& vec_begin,
	                                  const typename std::vector<T>::const_iterator& vec_end)
	{
		if (vec_begin != vec_end)
		{
			assert_smaller_or_equal_size(index);
			backing_vector_.insert(begin() + index,
			                       vec_begin,
			                       vec_end);
		}
		return *this;
	}

	[[nodiscard]] functional_vector inserting_at_impl(int index,
	                                                  const typename std::vector<T>::const_iterator& vec_begin,
	                                                  const typename std::vector<T>::const_iterator& vec_end) const
	{
		if (vec_begin == vec_end)
		{
			return *this;
		}
		assert_smaller_or_equal_size(index);
		auto augmented_vector(backing_vector_);
		augmented_vector.insert(augmented_vector.begin() + index,
		                        vec_begin,
		                        vec_end);
		return functional_vector(augmented_vector);
	}

	functional_vector& replace_range_at_imp(int index,
	                                        const typename std::vector<T>::const_iterator& vec_begin,
	                                        const typename std::vector<T>::const_iterator& vec_end)
	{
		const auto vec_size = std::distance(vec_begin, vec_end);
		assert(index + vec_size >= vec_size && index + vec_size <= size());
		std::copy(vec_begin,
		          vec_end,
		          backing_vector_.begin() + index);
		return *this;
	}

	[[nodiscard]] functional_vector replacing_range_at_imp(int index,
	                                                       const typename std::vector<T>::const_iterator& vec_begin,
	                                                       const typename std::vector<T>::const_iterator& vec_end) const
	{
		const auto vec_size = std::distance(vec_begin, vec_end);
		assert(index + vec_size >= vec_size && index + vec_size <= size());
		auto replaced_vector(backing_vector_);
		std::copy(vec_begin,
                  vec_end,
		          replaced_vector.begin() + index);
		return functional_vector(replaced_vector);
	}

	void assert_smaller_size(int index) const
	{
		assert(index < size() && index >= 0);
	}

	void assert_smaller_or_equal_size(int index) const
	{
		assert(index <= size() && index >= 0);
	}
};
