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
#include <functional>
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
class functional_vector {
public:
    functional_vector()
    : backing_vector_()
    {
    }
    
    explicit functional_vector(const std::vector<T> &vector)
    : backing_vector_(std::move(vector))
    {
    }
    
    functional_vector(const std::initializer_list<T>& list)
    : backing_vector_(std::move(list))
    {
    }

	// Adds a value at the end of the vector in place (mutating)
    functional_vector& push_back(T value)
    {
        backing_vector_.push_back(value);
        return *this;
    }

    // Makes a copy of the vector, adds value at the end of the copy and returns the copy (non-mutating)
    [[nodiscard]] functional_vector pushing_back(T value) const
    {
        auto augmented_vector(backing_vector_);
        augmented_vector.push_back(value);
        return functional_vector(augmented_vector);
    }

    // Adds a range of values at the end of the vector in place (mutating)
    functional_vector& push_back(const functional_vector<T>& vector)
    {
        return add_range_impl(vector.begin(), vector.end());
    }

    // Makes a copy of the vector, adds a range of values at the end of the copy, and returns the copy (non-mutating)
    [[nodiscard]] functional_vector pushing_back(const functional_vector<T>& vector) const
    {
        return adding_range_impl(vector.begin(), vector.end());
    }

    // Adds a range of values at the end of the vector in place (mutating)
    functional_vector& push_back(const std::vector<T>& vector)
    {
        return add_range_impl(vector.begin(), vector.end());
    }
    
    // Makes a copy of the vector, adds a range of values at the end of the copy, and returns the copy (non-mutating)
	[[nodiscard]] functional_vector pushing_back(const std::vector<T>& vector) const
    {
        return adding_range_impl(vector.begin(), vector.end());
    }

    // Adds a range of values at the end of the vector in place (mutating)
    functional_vector& push_back(const std::initializer_list<T>& list)
    {
        return push_back(std::vector(list));
    }

    // Makes a copy of the vector, adds a range of values at the end of the copy, and returns the copy (non-mutating)
    [[nodiscard]] functional_vector pushing_back(const std::initializer_list<T>& list) const
    {
        return pushing_back(std::vector(list));
    }

	// Performs the functional `map` algorithm, in which every element of the result vector is the
	// output of applying the transform function on every element of this instance.
    //
	// example:
	//      const auto input_vector = functional_vector<int>({ 1, 3, -5 });
	//      const auto output_vector = input_vector.map<std::string>([](const auto& element) {
	//      	return std::to_string(element);
	//      });
    //
    // outcome:
    //      output_vector -> functional_vector<std::string>({ "1", "3", "-5" })
	//
	// is equivalent to:
	//      const auto input_vector = functional_vector<int>({ 1, 3, -5 });
	//      auto output_vector = functional_vector<std::string>();
	//      for (auto i = 0; i < input_vector.size(); i++) {
	//      	output_vector.push_back(std::to_string(input_vector[i]));
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
    // which do match the given predicate are filtered out, i.e. removed (mutating)
    //
    // example:
    //      auto input_vector = functional_vector<int>({ 1, 3, -5, 2, -1, 9, -4 });
    //      input_vector.filter([](const auto& element) {
    //          return element >= 1.5;
    //      });
    //
    // outcome:
    //      input_vector -> functional_vector<std::string>({ 3, 2, 9 });
    //
    // is equivalent to:
    //      auto input_vector = functional_vector<int>({ 1, 3, -5, 2, -1, 9, -4 });
    //      for (auto i = 0; i < input_vector.size(); i++) {
    //          if (input_vector[i] >= 1.5) {
    //              continue;
    //          }
    //          input_vector.remove_at(i);
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
    // the copy which do match the given predicate are filtered out, i.e. removed (non-mutating)
    //
    // example:
    //      const auto input_vector = functional_vector<int>({ 1, 3, -5, 2, -1, 9, -4 });
    //      const auto filtered_vector = input_vector.filtered([](const auto& element) {
    //          return element >= 1.5;
    //      });
    //
    // outcome:
    //      input_vector -> functional_vector<int>({ 1, 3, -5, 2, -1, 9, -4 })
    //      filtered_vector -> functional_vector<std::string>({ 3, 2, 9 })
    //
    // is equivalent to:
    //      const auto input_vector = functional_vector<int>({ 1, 3, -5, 2, -1, 9, -4 });
    //      auto filtered_vector(input_vector);
    //      for (auto i = 0; i < filtered_vector.size(); i++) {
    //          if (filtered_vector[i] >= 1.5) {
    //              continue;
    //          }
    //          filtered_vector.remove_at(i);
    //          i--;
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
    //      auto my_vector = functional_vector<int>({ 1, 3, -5, 2, -1, 9, -4 });
    //      my_vector.reverse();
    //
    // outcome:
    //      my_vector -> functional_vector<int>({ -4, 9, -1, 2, -5, 3, 1 })
    functional_vector& reverse()
    {
        std::reverse(backing_vector_.begin(), backing_vector_.end());
        return *this;
    }
    
    // Makes a copy of this instance, whose elements are in reverse order (non-mutating)
    //
    // example:
    //      const auto input_vector = functional_vector<int>({ 1, 3, -5, 2, -1, 9, -4 });
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
    struct functional_vector_tuple {
    public:
        T first;
        U second;
    };
    
    template <typename U>
    [[nodiscard]] functional_vector<functional_vector_tuple<U>> zip(const functional_vector<U>& vector) const
    {
        return zip_impl<U>(vector.begin(), vector.end());
    }
    
    template <typename U>
    [[nodiscard]] functional_vector<functional_vector_tuple<U>> zip(const std::vector<U>& vector) const
    {
        return zip_impl<U>(vector.begin(), vector.end());
    }
    
    template <typename U>
    [[nodiscard]] functional_vector<functional_vector_tuple<U>> zip(const std::initializer_list<U>& list) const
    {
        return zip(std::vector(list));
    }
    
    functional_vector& sort(const std::function<bool(T, T)>& comparison_predicate)
    {
        std::sort(backing_vector_.begin(),
                  backing_vector_.end(),
                  comparison_predicate);
        return *this;
    }
    
    functional_vector& sort_ascending()
    {
        return sort(std::less_equal<T>());
    }
    
    functional_vector& sort_descending()
    {
        return sort(std::greater_equal<T>());
    }
    
    functional_vector sorted(const std::function<bool(T, T)>& comparison_predicate) const
    {
        auto sorted_vector(backing_vector_);
        std::sort(sorted_vector.begin(),
                  sorted_vector.end(),
                  comparison_predicate);
        return functional_vector(sorted_vector);
    }
    
    [[nodiscard]] functional_vector sorted_ascending() const
    {
        return sorted(std::less_equal<T>());
    }
    
    [[nodiscard]] functional_vector sorted_descending() const
    {
        return sorted(std::greater_equal<T>());
    }

    [[nodiscard]] size_t size() const
    {
        return backing_vector_.size();
    }
    
    T& operator[] (int index)
    {
        assert_smaller_size(index);
        return backing_vector_[index];
    }
    
    const T& operator[] (int index) const
    {
        assert_smaller_size(index);
        return backing_vector_[index];
    }
    
    [[nodiscard]] typename std::vector<T>::const_iterator begin() const
    {
        return backing_vector_.begin();
    }
    
    [[nodiscard]] typename std::vector<T>::const_iterator end() const
    {
        return backing_vector_.end();
    }
    
    const functional_vector& for_each(const std::function<void(T)>& operation) const
    {
        std::for_each(backing_vector_.begin(),
                      backing_vector_.end(),
                      operation);
        return *this;
    }
    
    [[nodiscard]] std::optional<size_t> find_first_index(const T& element) const
    {
        if (auto it = std::find(backing_vector_.begin(),
                                backing_vector_.end(),
                                element); it != backing_vector_.end()) {
            return std::distance(backing_vector_.begin(), it);
        }
        return std::nullopt;
    }
    
    [[nodiscard]] std::optional<size_t> find_last_index(const T& element) const
    {
        if (auto it = std::find(backing_vector_.rbegin(),
                                backing_vector_.rend(),
                                element); it != backing_vector_.rend()) {
            return std::distance(it, backing_vector_.rend()) - 1;
        }
        return std::nullopt;
    }
    
    [[nodiscard]] std::vector<size_t> find_all_indices(const T& element) const
    {
        std::vector<size_t> indices;
        auto it = std::find(backing_vector_.begin(),
                            backing_vector_.end(),
                            element);
        while (it != backing_vector_.end()) {
            indices.push_back(std::distance(backing_vector_.begin(), it));
            ++it;
            it = std::find(it, backing_vector_.end(), element);
        }
        return indices;
    }
    
    functional_vector& remove_at(int index)
    {
        assert_smaller_size(index);
        backing_vector_.erase(begin() + index);
        return *this;
    }
    
    [[nodiscard]] functional_vector removing_at(int index) const
    {
        assert_smaller_size(index);
        auto copy(backing_vector_);
        copy.erase(copy.begin() + index);
        return functional_vector(copy);
    }
    
    functional_vector& remove_last()
    {
        if (size() == 0) {
            return *this;
        }
        return remove_at(size() - 1);
    }
    
    [[nodiscard]] functional_vector removing_last() const
    {
        if (size() == 0) {
            return *this;
        }
        return removing_at(size() - 1);
    }
    
    functional_vector& remove_first()
    {
        if (size() == 0) {
            return *this;
        }
        return remove_at(0);
    }
    
    [[nodiscard]] functional_vector removing_first() const
    {
        if (size() == 0) {
            return *this;
        }
        return removing_at(0);
    }
    
    functional_vector& insert_at(int index, const T& element)
    {
        assert_smaller_or_equal_size(index);
        backing_vector_.insert(begin() + index, element);
        return *this;
    }
    
    [[nodiscard]] functional_vector inserting_at(int index, const T& element) const
    {
        assert_smaller_or_equal_size(index);
        auto copy(backing_vector_);
        copy.insert(copy.begin() + index, element);
        return functional_vector(copy);
    }
    
    functional_vector& insert_at(int index, const functional_vector<T>& vector)
    {
        return insert_at_impl(index, vector.begin(), vector.end());
    }
    
    [[nodiscard]] functional_vector inserting_at(int index, const functional_vector<T>& vector) const
    {
        return inserting_at_impl(index, vector.begin(), vector.end());
    }
    
    functional_vector& insert_at(int index, const std::vector<T>& vector)
    {
        return insert_at_impl(index, vector.begin(), vector.end());
    }
    
    [[nodiscard]] functional_vector inserting_at(int index, const std::vector<T>& vector) const
    {
        return inserting_at_impl(index, vector.begin(), vector.end());
    }
    
    functional_vector& insert_at(int index, const std::initializer_list<T>& list)
    {
        return insert_at(index, std::vector(list));
    }
    
    [[nodiscard]] functional_vector inserting_at(int index, const std::initializer_list<T>& list) const
    {
        return inserting_at(index, std::vector(list));
    }
    
    functional_vector& remove_range(index_range range)
    {
        if (!range.is_valid || size() < range.end + 1) {
            return *this;
        }
        backing_vector_.erase(begin() + range.start,
                              begin() + range.start + range.count);
        return *this;
    }
    
    [[nodiscard]] functional_vector removing_range(index_range range) const
    {
        if (!range.is_valid || size() < range.end + 1) {
            return *this;
        }
        auto shorter_vector(backing_vector_);
        shorter_vector.erase(shorter_vector.begin() + range.start,
                             shorter_vector.begin() + range.start + range.count);
        return functional_vector(shorter_vector);
    }
    
    functional_vector& replace_range_at(int index, const functional_vector<T>& vector)
    {
        return replace_range_at_imp(index, vector.begin(), vector.end());
    }
    
    functional_vector& replace_range_at(int index, const std::vector<T>& vector)
    {
        return replace_range_at_imp(index, vector.begin(), vector.end());
    }
    
    functional_vector& replace_range_at(int index, const std::initializer_list<T>& list)
    {
        return replace_range_at(index, std::vector(list));
    }
    
    [[nodiscard]] functional_vector replacing_range_at(int index, const functional_vector<T>& vector) const
    {
        return replacing_range_at_imp(index, vector.begin(), vector.end());
    }
    
    [[nodiscard]] functional_vector replacing_range_at(int index, const std::vector<T>& vector) const
    {
        return replacing_range_at_imp(index, vector.begin(), vector.end());
    }
    
    [[nodiscard]] functional_vector replacing_range_at(int index, const std::initializer_list<T>& list) const
    {
        return replacing_range_at(index, std::vector(list));
    }
    
    functional_vector& fill(const T& element)
    {
        std::fill(backing_vector_.begin(),
                  backing_vector_.end(),
                  element);
        return *this;
    }
    
    [[nodiscard]] static functional_vector filled(const T& element, size_t count)
    {
        auto backing_vector = std::vector<T>();
        backing_vector.resize(count);
        std::fill(backing_vector.begin(),
                  backing_vector.end(),
                  element);
        return functional_vector(backing_vector);
    }
    
    bool operator == (const functional_vector<T>& rhs) const
    {
        return std::equal(backing_vector_.begin(),
                          backing_vector_.end(),
                          rhs.begin(),
                          rhs.end());
    }
    
    bool operator != (const functional_vector<T>& rhs) const
    {
        return !((*this) == rhs);
    }
    
private:
    std::vector<T> backing_vector_;
    
    functional_vector& add_range_impl(const typename std::vector<T>::const_iterator& vec_begin,
                                      const typename std::vector<T>::const_iterator& vec_end)
    {
        backing_vector_.insert(backing_vector_.end(),
                               vec_begin,
                               vec_end);
        return *this;
    }
    
    [[nodiscard]] functional_vector adding_range_impl(const typename std::vector<T>::const_iterator& vec_begin,
                                                      const typename std::vector<T>::const_iterator& vec_end) const
    {
        auto augmented_vector(backing_vector_);
        augmented_vector.reserve(augmented_vector.size() + std::distance(vec_begin, vec_end));
        augmented_vector.insert(augmented_vector.end(),
                                vec_begin,
                                vec_end);
        return functional_vector(augmented_vector);
    }
    
    template <typename U>
    [[nodiscard]] functional_vector<functional_vector_tuple<U>> zip_impl(const typename std::vector<U>::const_iterator& vec_begin,
                                                                         const typename std::vector<U>::const_iterator& vec_end) const
    {
        const auto vec_size = std::distance(vec_begin, vec_end);
        assert(backing_vector_.size() == vec_size);
        std::vector<functional_vector_tuple<U>> combined_vector;
        combined_vector.reserve(vec_size);
        for (size_t i = 0; i < vec_size; i++) {
            combined_vector.push_back({ backing_vector_[i], *(vec_begin + i) });
        }
        return functional_vector<functional_vector_tuple<U>>(combined_vector);
    }
    
    functional_vector& insert_at_impl(int index,
                                      const typename std::vector<T>::const_iterator& vec_begin,
                                      const typename std::vector<T>::const_iterator& vec_end)
    {
        if (vec_begin != vec_end) {
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
        if (vec_begin == vec_end) {
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
