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
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include "index_range.h"
#include <algorithm>
#include <optional>
#include <type_traits>
#include <vector>

template <typename T> class functional_vector {
public:
  functional_vector() : backing_vector_() {}

  explicit functional_vector(const std::vector<T> &vector)
      : backing_vector_(vector) {}

  explicit functional_vector(std::vector<T> &&vector)
      : backing_vector_(std::move(vector)) {}

  functional_vector(std::initializer_list<T> list)
      : backing_vector_(std::move(list)) {}

  template <typename U,
            typename = std::enable_if_t<std::is_constructible_v<T, U>>>
  functional_vector &add(U &&value) {
    backing_vector_.push_back(std::forward<U>(value));
    return *this;
  }

  template <typename U,
            typename = std::enable_if_t<std::is_constructible_v<T, U>>>
  [[nodiscard]] functional_vector adding(U &&value) const {
    auto augmented_vector(backing_vector_);
    augmented_vector.push_back(std::forward<U>(value));
    return functional_vector(augmented_vector);
  }

  functional_vector &add_range(const functional_vector<T> &vector) {
    return add_range_impl(vector.cbegin(), vector.cend());
  }

  [[nodiscard]] functional_vector
  adding_range(const functional_vector<T> &vector) const {
    return adding_range_impl(vector.cbegin(), vector.cend());
  }

  functional_vector &add_range(const std::vector<T> &vector) {
    return add_range_impl(vector.cbegin(), vector.cend());
  }

  [[nodiscard]] functional_vector
  adding_range(const std::vector<T> &vector) const {
    return adding_range_impl(vector.cbegin(), vector.cend());
  }

  functional_vector &add_range(const std::initializer_list<T> &list) {
    return add_range(std::vector(list));
  }

  [[nodiscard]] functional_vector
  adding_range(const std::initializer_list<T> &list) const {
    return adding_range(std::vector(list));
  }

  template <
      typename U, typename Transform,
      typename = std::enable_if_t<std::is_invocable_r<U, Transform, T>::value>>
  functional_vector<U> map(Transform transform) const {
    std::vector<U> transformed_vector;
    transformed_vector.reserve(backing_vector_.size());
    std::transform(backing_vector_.begin(), backing_vector_.end(),
                   std::back_inserter(transformed_vector), transform);
    return functional_vector<U>(transformed_vector);
  }

  template <typename Filter, typename = std::enable_if_t<
                                 std::is_invocable_r<bool, Filter, T>::value>>
  functional_vector &filter(Filter predicate_to_keep) {
    backing_vector_.erase(std::remove_if(backing_vector_.begin(),
                                         backing_vector_.end(),
                                         [&](const auto &element) {
                                           return !predicate_to_keep(element);
                                         }),
                          backing_vector_.end());
    return *this;
  }

  template <typename Callable,
            typename =
                std::enable_if_t<std::is_invocable_r<bool, Callable, T>::value>>
  functional_vector filtered(Callable predicate_to_keep) const {
    std::vector<T> filtered_vector;
    filtered_vector.reserve(backing_vector_.size());
    std::copy_if(backing_vector_.begin(), backing_vector_.end(),
                 std::back_inserter(filtered_vector), predicate_to_keep);
    return functional_vector(filtered_vector);
  }

  functional_vector &reverse() {
    std::reverse(backing_vector_.begin(), backing_vector_.end());
    return *this;
  }

  [[nodiscard]] functional_vector reversed() const {
    std::vector<T> reversed_vector(backing_vector_.crbegin(),
                                   backing_vector_.crend());
    return functional_vector(std::move(reversed_vector));
  }

  template <typename U> struct functional_vector_pair {
    T first;
    U second;
  };

  template <typename U>
  [[nodiscard]] functional_vector<functional_vector_pair<U>>
  zip(const functional_vector<U> &vector) const {
    return zip_impl<U>(vector.cbegin(), vector.cend());
  }

  template <typename U>
  [[nodiscard]] functional_vector<functional_vector_pair<U>>
  zip(const std::vector<U> &vector) const {
    return zip_impl<U>(vector.cbegin(), vector.cend());
  }

  template <typename U>
  [[nodiscard]] functional_vector<functional_vector_pair<U>>
  zip(const std::initializer_list<U> &list) const {
    return zip(std::vector(list));
  }

  template <typename Sortable, typename = std::enable_if_t<std::is_invocable_r<
                                   bool, Sortable, T, T>::value>>
  functional_vector &sort(Sortable comparison_predicate) {
    std::sort(backing_vector_.begin(), backing_vector_.end(),
              comparison_predicate);
    return *this;
  }

  functional_vector &sort_ascending() { return sort(std::less_equal<T>()); }

  functional_vector &sort_descending() { return sort(std::greater_equal<T>()); }

  template <typename Sortable, typename = std::enable_if_t<std::is_invocable_r<
                                   bool, Sortable, T, T>::value>>
  functional_vector sorted(Sortable comparison_predicate) const {
    auto sorted_vector(backing_vector_);
    std::sort(sorted_vector.begin(), sorted_vector.end(), comparison_predicate);
    return functional_vector(sorted_vector);
  }

  [[nodiscard]] functional_vector sorted_ascending() const {
    return sorted(std::less_equal<T>());
  }

  [[nodiscard]] functional_vector sorted_descending() const {
    return sorted(std::greater_equal<T>());
  }

  [[nodiscard]] auto size() const { return backing_vector_.size(); }

  T &operator[](int index) {
    assert_smaller_size(index);
    return backing_vector_[index];
  }

  const T &operator[](int index) const {
    assert_smaller_size(index);
    return backing_vector_[index];
  }

  [[nodiscard]] auto begin() const { return backing_vector_.begin(); }

  [[nodiscard]] auto end() const { return backing_vector_.end(); }

  [[nodiscard]] auto cbegin() const { return backing_vector_.cbegin(); }
  [[nodiscard]] auto cend() const { return backing_vector_.cend(); }

  template <typename Callable,
            typename =
                std::enable_if_t<std::is_invocable_r<void, Callable, T>::value>>
  const functional_vector &for_each(Callable operation) const {
    std::for_each(backing_vector_.cbegin(), backing_vector_.cend(), operation);
    return *this;
  }

  [[nodiscard]] std::optional<size_t> find_first_index(const T &element) const {
    auto const cbegin_iter = cbegin();
    auto const cend_iter = cend();
    if (auto const it = std::find(cbegin_iter, cend_iter, element);
        it != cend_iter) {
      return std::distance(cbegin_iter, it);
    }
    return std::nullopt;
  }

  [[nodiscard]] std::optional<size_t> find_last_index(const T &element) const {
    auto const crend_iter = backing_vector_.crend();
    if (auto const it =
            std::find(backing_vector_.rbegin(), crend_iter, element);
        it != crend_iter) {
      return std::distance(it, crend_iter) - 1;
    }
    return std::nullopt;
  }

  [[nodiscard]] std::vector<size_t> find_all_indices(const T &element) const {
    // cache the iterators so we don't end up calling them all the time
    auto const cbegin_iter = cbegin();
    auto const cend_iter = cend();

    std::vector<size_t> indices;
    auto it = std::find(cbegin_iter, cend_iter, element);
    while (it != cend_iter) {
      indices.push_back(std::distance(cbegin_iter, it));
      ++it;
      it = std::find(it, cend_iter, element);
    }
    return indices;
  }

  functional_vector &remove_at(int const index) {
    assert_smaller_size(index);
    backing_vector_.erase(begin() + index);
    return *this;
  }

  [[nodiscard]] functional_vector removing_at(int const index) const {
    assert_smaller_size(index);
    auto copy(backing_vector_);
    copy.erase(copy.begin() + index);
    return functional_vector(copy);
  }

  functional_vector &remove_last() {
    if (size() == 0) {
      return *this;
    }
    backing_vector_.pop_back();
    return *this;
  }

  [[nodiscard]] functional_vector removing_last() const {
    if (size() == 0) {
      return *this;
    }
    return removing_at(size() - 1);
  }

  functional_vector &remove_first() {
    if (size() == 0) {
      return *this;
    }
    return remove_at(0);
  }

  [[nodiscard]] functional_vector removing_first() const {
    if (size() == 0) {
      return *this;
    }
    return removing_at(0);
  }

  functional_vector &insert_at(int index, const T &element) {
    assert_smaller_or_equal_size(index);
    backing_vector_.insert(begin() + index, element);
    return *this;
  }

  [[nodiscard]] functional_vector inserting_at(int index,
                                               const T &element) const {
    assert_smaller_or_equal_size(index);
    auto copy(backing_vector_);
    copy.insert(copy.begin() + index, element);
    return functional_vector(copy);
  }

  functional_vector &insert_at(int index, const functional_vector<T> &vector) {
    return insert_at_impl(index, vector.begin(), vector.end());
  }

  [[nodiscard]] functional_vector
  inserting_at(int index, const functional_vector<T> &vector) const {
    return inserting_at_impl(index, vector.begin(), vector.end());
  }

  functional_vector &insert_at(int index, const std::vector<T> &vector) {
    return insert_at_impl(index, vector.begin(), vector.end());
  }

  [[nodiscard]] functional_vector
  inserting_at(int index, const std::vector<T> &vector) const {
    return inserting_at_impl(index, vector.begin(), vector.end());
  }

  functional_vector &insert_at(int index,
                               const std::initializer_list<T> &list) {
    return insert_at(index, std::vector(list));
  }

  [[nodiscard]] functional_vector
  inserting_at(int index, const std::initializer_list<T> &list) const {
    return inserting_at(index, std::vector(list));
  }

  functional_vector &remove_range(index_range range) {
    if (!range.is_valid || size() < range.end + 1) {
      return *this;
    }
    backing_vector_.erase(begin() + range.start,
                          begin() + range.start + range.count);
    return *this;
  }

  [[nodiscard]] functional_vector removing_range(index_range range) const {
    if (!range.is_valid || size() < range.end + 1) {
      return *this;
    }
    auto shorter_vector(backing_vector_);
    shorter_vector.erase(shorter_vector.begin() + range.start,
                         shorter_vector.begin() + range.start + range.count);
    return functional_vector(shorter_vector);
  }

  functional_vector &replace_range_at(int index,
                                      const functional_vector<T> &vector) {
    return replace_range_at_imp(index, vector.begin(), vector.end());
  }

  functional_vector &replace_range_at(int index, const std::vector<T> &vector) {
    return replace_range_at_imp(index, vector.begin(), vector.end());
  }

  functional_vector &replace_range_at(int index,
                                      const std::initializer_list<T> &list) {
    return replace_range_at(index, std::vector(list));
  }

  [[nodiscard]] functional_vector
  replacing_range_at(int index, const functional_vector<T> &vector) const {
    return replacing_range_at_imp(index, vector.begin(), vector.end());
  }

  [[nodiscard]] functional_vector
  replacing_range_at(int index, const std::vector<T> &vector) const {
    return replacing_range_at_imp(index, vector.begin(), vector.end());
  }

  [[nodiscard]] functional_vector
  replacing_range_at(int index, const std::initializer_list<T> &list) const {
    return replacing_range_at(index, std::vector(list));
  }

  functional_vector &fill(const T &element) {
    std::fill(backing_vector_.begin(), backing_vector_.end(), element);
    return *this;
  }

  [[nodiscard]] static functional_vector filled(const T &element,
                                                size_t count) {
    auto backing_vector = std::vector<T>(count, element);
    return functional_vector(std::move(backing_vector));
  }

  bool operator==(const functional_vector<T> &rhs) const {
    return std::equal(backing_vector_.cbegin(), backing_vector_.cend(),
                      rhs.cbegin(), rhs.cend());
  }

  bool operator!=(const functional_vector<T> &rhs) const {
    return !((*this) == rhs);
  }

private:
  functional_vector &
  add_range_impl(const typename std::vector<T>::const_iterator &vec_begin,
                 const typename std::vector<T>::const_iterator &vec_end) {
    backing_vector_.insert(backing_vector_.end(), vec_begin, vec_end);
    return *this;
  }

  [[nodiscard]] functional_vector adding_range_impl(
      const typename std::vector<T>::const_iterator &vec_begin,
      const typename std::vector<T>::const_iterator &vec_end) const {
    auto augmented_vector(backing_vector_);
    augmented_vector.reserve(augmented_vector.size() +
                             std::distance(vec_begin, vec_end));
    augmented_vector.insert(augmented_vector.end(), vec_begin, vec_end);
    return functional_vector(augmented_vector);
  }

  template <typename U>
  [[nodiscard]] functional_vector<functional_vector_pair<U>>
  zip_impl(const typename std::vector<U>::const_iterator &vec_begin,
           const typename std::vector<U>::const_iterator &vec_end) const {
    const auto vec_size = std::distance(vec_begin, vec_end);
    assert(backing_vector_.size() == vec_size);
    std::vector<functional_vector_pair<U>> combined_vector;
    combined_vector.reserve(vec_size);
    for (size_t i = 0; i < vec_size; i++) {
      combined_vector.push_back({backing_vector_[i], *(vec_begin + i)});
    }
    return functional_vector<functional_vector_pair<U>>(combined_vector);
  }

  functional_vector &
  insert_at_impl(int index,
                 const typename std::vector<T>::const_iterator &vec_begin,
                 const typename std::vector<T>::const_iterator &vec_end) {
    if (vec_begin != vec_end) {
      assert_smaller_or_equal_size(index);
      backing_vector_.insert(begin() + index, vec_begin, vec_end);
    }
    return *this;
  }

  [[nodiscard]] functional_vector inserting_at_impl(
      int const index, const typename std::vector<T>::const_iterator &vec_begin,
      const typename std::vector<T>::const_iterator &vec_end) const {
    if (vec_begin == vec_end) {
      return *this;
    }
    assert_smaller_or_equal_size(index);
    auto augmented_vector(backing_vector_);
    augmented_vector.insert(augmented_vector.begin() + index, vec_begin,
                            vec_end);
    return functional_vector(augmented_vector);
  }

  functional_vector &
  replace_range_at_imp(int index,
                       const typename std::vector<T>::const_iterator &vec_begin,
                       const typename std::vector<T>::const_iterator &vec_end) {
    const auto vec_size = std::distance(vec_begin, vec_end);
    assert(index + vec_size >= vec_size && index + vec_size <= size());
    std::copy(vec_begin, vec_end, backing_vector_.begin() + index);
    return *this;
  }

  [[nodiscard]] functional_vector replacing_range_at_imp(
      int index, const typename std::vector<T>::const_iterator &vec_begin,
      const typename std::vector<T>::const_iterator &vec_end) const {
    const auto vec_size = std::distance(vec_begin, vec_end);
    assert(index + vec_size >= vec_size && index + vec_size <= size());
    auto replaced_vector(backing_vector_);
    std::copy(vec_begin, vec_end, replaced_vector.begin() + index);
    return functional_vector(replaced_vector);
  }

  void assert_smaller_size(int index) const {
    assert(index < size() && index >= 0);
  }

  void assert_smaller_or_equal_size(int index) const {
    assert(index <= size() && index >= 0);
  }

private:
  std::vector<T> backing_vector_;
};
