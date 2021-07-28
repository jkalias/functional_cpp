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
    
    functional_vector& add(T value)
    {
        backing_vector_.push_back(value);
        return *this;
    }
    
    [[nodiscard]] functional_vector adding(T value) const
    {
        auto augmented_vector(backing_vector_);
        augmented_vector.push_back(value);
        return functional_vector(augmented_vector);
    }
    
    functional_vector& add_range(const functional_vector<T>& vector)
    {
        return add_range_impl(vector.begin(), vector.end());
    }
    
    [[nodiscard]] functional_vector adding_range(const functional_vector<T>& vector) const
    {
        return adding_range_impl(vector.begin(), vector.end());
    }
    
    functional_vector& add_range(const std::vector<T>& vector)
    {
        return add_range_impl(vector.begin(), vector.end());
    }
    
    [[nodiscard]] functional_vector adding_range(const std::vector<T>& vector) const
    {
        return adding_range_impl(vector.begin(), vector.end());
    }
    
    functional_vector& add_range(const std::initializer_list<T>& list)
    {
        return add_range(std::vector(list));
    }
    
    [[nodiscard]] functional_vector adding_range(const std::initializer_list<T>& list) const
    {
        return adding_range(std::vector(list));
    }
    
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
    
    functional_vector& filter(const std::function<bool(T)>& predicate_to_keep)
    {
        backing_vector_.erase(std::remove_if(backing_vector_.begin(),
                                             backing_vector_.end(),
                                             [&](const auto& element) { return !predicate_to_keep(element); }),
                              backing_vector_.end());
        return *this;
    }
    
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
    
    functional_vector& reverse()
    {
        std::reverse(backing_vector_.begin(), backing_vector_.end());
        return *this;
    }
    
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
    
    size_t size() const
    {
        return backing_vector_.size();
    }
    
    T& operator[] (size_t index)
    {
        assert(index < size());
        return backing_vector_[index];
    }
    
    const T& operator[] (size_t index) const
    {
        assert(index < size());
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
    
    functional_vector& remove_at(size_t index)
    {
        assert(index < size());
        backing_vector_.erase(begin() + index);
        return *this;
    }
    
    [[nodiscard]] functional_vector removing_at(size_t index) const
    {
        assert(index < size());
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
    
    functional_vector& insert_at(size_t index, const T& element)
    {
        assert(index <= size());
        backing_vector_.insert(begin() + index, element);
        return *this;
    }
    
    [[nodiscard]] functional_vector inserting_at(size_t index, const T& element) const
    {
        assert(index <= size());
        auto copy(backing_vector_);
        copy.insert(copy.begin() + index, element);
        return functional_vector(copy);
    }
    
    functional_vector& insert_at(size_t index, const functional_vector<T>& vector)
    {
        return insert_at_impl(index, vector.begin(), vector.end());
    }
    
    [[nodiscard]] functional_vector inserting_at(size_t index, const functional_vector<T>& vector) const
    {
        return inserting_at_impl(index, vector.begin(), vector.end());
    }
    
    functional_vector& insert_at(size_t index, const std::vector<T>& vector)
    {
        return insert_at_impl(index, vector.begin(), vector.end());
    }
    
    [[nodiscard]] functional_vector inserting_at(size_t index, const std::vector<T>& vector) const
    {
        return inserting_at_impl(index, vector.begin(), vector.end());
    }
    
    functional_vector& insert_at(size_t index, const std::initializer_list<T>& list)
    {
        return insert_at(index, std::vector(list));
    }
    
    [[nodiscard]] functional_vector inserting_at(size_t index, const std::initializer_list<T>& list) const
    {
        return inserting_at(index, std::vector(list));
    }
    
    functional_vector& remove_range(index_range range)
    {
        if (!range.is_valid || size() < range.end() + 1) {
            return *this;
        }
        backing_vector_.erase(begin() + range.start,
                              begin() + range.start + range.count);
        return *this;
    }
    
    [[nodiscard]] functional_vector removing_range(index_range range) const
    {
        if (!range.is_valid || size() < range.end() + 1) {
            return *this;
        }
        auto shorter_vector(backing_vector_);
        shorter_vector.erase(shorter_vector.begin() + range.start,
                             shorter_vector.begin() + range.start + range.count);
        return functional_vector(shorter_vector);
    }
    
    functional_vector& replace_range_at(size_t index, const functional_vector<T>& vector)
    {
        return replace_range_at_imp(index, vector.begin(), vector.end());
    }
    
    functional_vector& replace_range_at(size_t index, const std::vector<T>& vector)
    {
        return replace_range_at_imp(index, vector.begin(), vector.end());
    }
    
    functional_vector& replace_range_at(size_t index, const std::initializer_list<T>& list)
    {
        return replace_range_at(index, std::vector(list));
    }
    
    [[nodiscard]] functional_vector replacing_range_at(size_t index, const functional_vector<T>& vector) const
    {
        return replacing_range_at_imp(index, vector.begin(), vector.end());
    }
    
    [[nodiscard]] functional_vector replacing_range_at(size_t index, const std::vector<T>& vector) const
    {
        return replacing_range_at_imp(index, vector.begin(), vector.end());
    }
    
    [[nodiscard]] functional_vector replacing_range_at(size_t index, const std::initializer_list<T>& list) const
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
    
    functional_vector& insert_at_impl(size_t index,
                                      const typename std::vector<T>::const_iterator& vec_begin,
                                      const typename std::vector<T>::const_iterator& vec_end)
    {
        if (vec_begin != vec_end) {
            assert(index <= size());
            backing_vector_.insert(begin() + index,
                                   vec_begin,
                                   vec_end);
        }
        return *this;
    }
    
    [[nodiscard]] functional_vector inserting_at_impl(size_t index,
                                                      const typename std::vector<T>::const_iterator& vec_begin,
                                                      const typename std::vector<T>::const_iterator& vec_end) const
    {
        if (vec_begin == vec_end) {
            return *this;
        }
        assert(index <= size());
        auto augmented_vector(backing_vector_);
        augmented_vector.insert(augmented_vector.begin() + index,
                                vec_begin,
                                vec_end);
        return functional_vector(augmented_vector);
    }
    
    functional_vector& replace_range_at_imp(size_t index,
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
    
    [[nodiscard]] functional_vector replacing_range_at_imp(size_t index,
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
};