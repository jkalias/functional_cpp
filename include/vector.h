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

#pragma once
#include <algorithm>
#include <type_traits>
#include <vector>
#include <iterator>
#include "index_range.h"
#include "optional.h"
#ifdef PARALLEL_ALGORITHM_AVAILABLE
#include <execution>
#endif

namespace fcpp {

template <class T, class Compare>
class set;

// A lightweight wrapper around std::vector, enabling fluent and functional
// programming on the vector itself, rather than using the more procedural style
// of the standard library algorithms.
//
// Member functions can be mutating (eg. my_vector.reverse()) or
// non-mutating (eg. my_vector.reversed()) enforcing thread safety if needed
template <typename T>
class vector
{
public:
    vector()
    : m_vector()
    {
    }
    
    explicit vector(const std::vector<T>& vector)
    : m_vector(vector)
    {
    }
    
    explicit vector(std::vector<T>&& vector)
    : m_vector(std::move(vector))
    {
    }
    
    explicit vector(std::initializer_list<T> list)
    : m_vector(std::move(list))
    {
    }
    
    // Creates a new vector by repeating a given element.
    //
    // example:
    //      const fcpp::vector<std::string> filled_vector(3, "John");
    //
    // outcome:
    //      filled_vector -> fcpp::vector<std::string>({ "John", "John", "John" })
    explicit vector(size_t count, const T& element)
    : m_vector(count, element)
    {
    }
    
    // Performs the functional `map` algorithm, in which every element of the resulting vector is the
    // output of applying the transform function on every element of this instance.
    //
    // example:
    //      const fcpp::vector<int> input_vector({ 1, 3, -5 });
    //      const auto output_vector = input_vector.map<std::string>([](const auto& element) {
    //      	return std::to_string(element);
    //      });
    //
    // outcome:
    //      output_vector -> fcpp::vector<std::string>({ "1", "3", "-5" })
    //
    // is equivalent to:
    //      const fcpp::vector<int> input_vector({ 1, 3, -5 });
    //      fcpp::vector<std::string> output_vector;
    //      for (auto i = 0; i < input_vector.size(); ++i) {
    //      	output_vector.insert_back(std::to_string(input_vector[i]));
    //      }
#ifdef CPP17_AVAILABLE
    template <typename U, typename Transform, typename = std::enable_if_t<std::is_invocable_r_v<U, Transform, T>>>
#else
    template <typename U, typename Transform>
#endif
    vector<U> map(Transform && transform) const
    {
        std::vector<U> transformed_vector;
        transformed_vector.reserve(m_vector.size());
        std::transform(m_vector.begin(),
                       m_vector.end(),
                       std::back_inserter(transformed_vector),
                       std::forward<Transform>(transform));
        return vector<U>(transformed_vector);
    }
        
#ifdef PARALLEL_ALGORITHM_AVAILABLE
    // Performs the functional `map` algorithm in parallel.
    // See also the sequential version for more documentation.
    template <typename U, typename Transform, typename = std::enable_if_t<std::is_invocable_r_v<U, Transform, T>>>
    vector<U> map_parallel(Transform && transform) const
    {
        std::vector<U> transformed_vector;
        transformed_vector.resize(m_vector.size());
        std::transform(std::execution::par,
                       m_vector.cbegin(),
                       m_vector.cend(),
                       transformed_vector.begin(),
                       std::forward<Transform>(transform));
        return vector<U>(transformed_vector);
    }
#endif
    
    // Returns true if all elements match the predicate (return true)
    //
    // example:
    //      fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //
    //      // returns true
    //      numbers.all_of([](const auto &number) {
    //          return number < 10;
    //      });
    //
    //      // returns false
    //      numbers.all_of([](const auto &number) {
    //          return number > 2;
    //      });
#ifdef CPP17_AVAILABLE
    template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<bool, Callable, T>>>
#else
    template <typename Callable>
#endif
    bool all_of(Callable && unary_predicate) const
    {
        return std::all_of(begin(),
                           end(),
                           std::forward<Callable>(unary_predicate));
    }
        
#ifdef PARALLEL_ALGORITHM_AVAILABLE
    // Performs the `all_of` algorithm in parallel.
    // See also the sequential version for more documentation.
    template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<bool, Callable, T>>>
    bool all_of_parallel(Callable && unary_predicate) const
    {
        return std::all_of(std::execution::par,
                           begin(),
                           end(),
                           std::forward<Callable>(unary_predicate));
    }
#endif
    
    // Returns true if at least one of the elements matches the predicate (returns true)
    //
    // example:
    //      fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //
    //      // returns true
    //      numbers.any_of([](const auto &number) {
    //          return number < 5;
    //      });
    //
    //      // returns false
    //      numbers.any_of([](const auto &number) {
    //          return number > 9;
    //      });
#ifdef CPP17_AVAILABLE
    template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<bool, Callable, T>>>
#else
    template <typename Callable>
#endif
    bool any_of(Callable && unary_predicate) const
    {
        return std::any_of(begin(),
                           end(),
                           std::forward<Callable>(unary_predicate));
    }
        
#ifdef PARALLEL_ALGORITHM_AVAILABLE
    // Performs the `any_of` algorithm in parallel.
    // See also the sequential version for more documentation.
    template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<bool, Callable, T>>>
    bool any_of_parallel(Callable && unary_predicate) const
    {
        return std::any_of(std::execution::par,
                           begin(),
                           end(),
                           std::forward<Callable>(unary_predicate));
    }
#endif
    
    // Returns true if no element matches the predicate (all return false)
    //
    // example:
    //      fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //
    //      // returns true
    //      numbers.none_of([](const auto &number) {
    //          return number < -2;
    //      });
    //
    //      // returns false
    //      numbers.none_of([](const auto &number) {
    //          return number > 7;
    //      });
#ifdef CPP17_AVAILABLE
    template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<bool, Callable, T>>>
#else
    template <typename Callable>
#endif
    bool none_of(Callable && unary_predicate) const
    {
        return std::none_of(begin(),
                            end(),
                            std::forward<Callable>(unary_predicate));
    }
        
#ifdef PARALLEL_ALGORITHM_AVAILABLE
    // Performs the `none_of` algorithm in parallel.
    // See also the sequential version for more documentation.
    template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<bool, Callable, T>>>
    bool none_of_parallel(Callable && unary_predicate) const
    {
        return std::none_of(std::execution::par,
                            begin(),
                            end(),
                            std::forward<Callable>(unary_predicate));
    }
#endif

    // Performs the functional `reduce` (fold/accumulate) algorithm, by returning the result of
    // accumulating all the values in the vector to an initial value. (non-mutating)
    //
    // example:
    //      const fcpp::vector<std::string> tokens({ "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "brown", "dog" });
    //      const auto sentence = tokens.reduce<std::string>("", [](const std::string& partial, const std::string& token) {
    //          return partial.length() != 0
    //              ? partial + " " + token
    //              : token;
    //      });
    //
    // outcome:
    //      sentence -> std::string("the quick brown fox jumps over the lazy brown dog");
#ifdef CPP17_AVAILABLE
    template <typename U, typename Reduce, typename = std::enable_if_t<std::is_invocable_r_v<U, Reduce, U, T>>>
#else
    template <typename U, typename Reduce>
#endif
    U reduce(const U& initial, Reduce&& reduction) const
    {
        auto result = initial;
        for(const auto &x: m_vector)
        {
            result = reduction(result, x);
        }
        return result;
    }

    // Performs the functional `filter` algorithm, in which all elements of this instance
    // which match the given predicate are kept (mutating)
    //
    // example:
    //      fcpp::vector<int> numbers({ 1, 3, -5, 2, -1, 9, -4 });
    //      numbers.filter([](const auto& element) {
    //          return element >= 1.5;
    //      });
    //
    // outcome:
    //      numbers -> fcpp::vector<int>({ 3, 2, 9 });
    //
    // is equivalent to:
    //      fcpp::vector<int> numbers({ 1, 3, -5, 2, -1, 9, -4 });
    //      for (auto i = 0; i < numbers.size(); ++i) {
    //          if (numbers[i] >= 1.5) {
    //              continue;
    //          }
    //          numbers.remove_at(i);
    //          i--;
    //      }
#ifdef CPP17_AVAILABLE
    template <typename Filter, typename = std::enable_if_t<std::is_invocable_r_v<bool, Filter, T>>>
#else
    template <typename Filter>
#endif
    vector& filter(Filter && predicate_to_keep)
    {
        m_vector.erase(std::remove_if(m_vector.begin(),
                                             m_vector.end(),
                                             [predicate=std::forward<Filter>(predicate_to_keep)](const T& element) {
            return !predicate(element);
        }), m_vector.end());
        return *this;
    }
        
#ifdef PARALLEL_ALGORITHM_AVAILABLE
    // Performs the functional `filter` algorithm in parallel.
    // See also the sequential version for more documentation.
    template <typename Filter, typename = std::enable_if_t<std::is_invocable_r_v<bool, Filter, T>>>
    vector& filter_parallel(Filter && predicate_to_keep)
    {
        m_vector.erase(std::remove_if(std::execution::par,
                                             m_vector.begin(),
                                             m_vector.end(),
                                             [predicate=std::forward<Filter>(predicate_to_keep)](const T& element) {
            return !predicate(element);
        }), m_vector.end());
        return *this;
    }
#endif
    
    // Performs the functional `filter` algorithm in a copy of this instance, in which all elements of
    // the copy which match the given predicate are kept (non-mutating)
    //
    // example:
    //      const fcpp::vector<int> numbers({ 1, 3, -5, 2, -1, 9, -4 });
    //      const auto filtered_numbers = numbers.filtered([](const auto& element) {
    //          return element >= 1.5;
    //      });
    //
    // outcome:
    //      numbers -> fcpp::vector<int>({ 1, 3, -5, 2, -1, 9, -4 })
    //      filtered_numbers -> fcpp::vector<int>({ 3, 2, 9 })
    //
    // is equivalent to:
    //      const fcpp::vector<int> numbers({ 1, 3, -5, 2, -1, 9, -4 });
    //      fcpp::vector<int> filtered_numbers;
    //      for (auto i = 0; i < numbers.size(); ++i) {
    //          if (numbers[i] >= 1.5) {
    //              filtered_numbers.insert_back(numbers[i]);
    //          }
    //      }
#ifdef CPP17_AVAILABLE
    template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<bool, Callable, T>>>
#else
    template <typename Callable>
#endif
    vector filtered(Callable && predicate_to_keep) const
    {
        std::vector<T> filtered_vector;
        filtered_vector.reserve(m_vector.size());
        std::copy_if(m_vector.begin(),
                     m_vector.end(),
                     std::back_inserter(filtered_vector),
                     std::forward<Callable>(predicate_to_keep));
        return vector(filtered_vector);
    }
    
#ifdef PARALLEL_ALGORITHM_AVAILABLE
    // Performs the `filtered` algorithm in parallel.
    // See also the sequential version for more documentation.
    template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<bool, Callable, T>>>
    vector filtered_parallel(Callable && predicate_to_keep) const
    {
#ifdef _MSC_VER
        // Visual Studio compiler is stricter than GCC in its use of iterators, so back_inserter wouldn't work here
        auto copy(*this);
        copy.filter_parallel(predicate_to_keep);
        return copy;
#else
        std::vector<T> filtered_vector;
        filtered_vector.reserve(m_vector.size());
        std::copy_if(std::execution::par,
                     m_vector.begin(),
                     m_vector.end(),
                     std::back_inserter(filtered_vector),
                     std::forward<Callable>(predicate_to_keep));
        return vector(filtered_vector);
#endif
    }
#endif
        
    // Reverses the order of the elements in place (mutating)
    //
    // example:
    //      fcpp::vector<int> numbers_vector({ 1, 3, -5, 2, -1, 9, -4 });
    //      numbers_vector.reverse();
    //
    // outcome:
    //      numbers_vector -> fcpp::vector<int>({ -4, 9, -1, 2, -5, 3, 1 })
    vector& reverse()
    {
        std::reverse(m_vector.begin(), m_vector.end());
        return *this;
    }
    
    // Returns a copy of this instance, whose elements are in reverse order (non-mutating)
    //
    // example:
    //      const fcpp::vector<int> input_vector({ 1, 3, -5, 2, -1, 9, -4 });
    //      const auto reversed_vector = input_vector.reversed();
    //
    // outcome:
    //      input_vector -> fcpp::vector<int>({ 1, 3, -5, 2, -1, 9, -4 });
    //      reversed_vector -> fcpp::vector<int>({ -4, 9, -1, 2, -5, 3, 1 })
    [[nodiscard]] vector reversed() const
    {
        std::vector<T> reversed_vector(m_vector.crbegin(), m_vector.crend());
        return vector(std::move(reversed_vector));
    }
    
#ifdef CPP17_AVAILABLE
    template<typename Iterator>
    using deref_type = typename std::iterator_traits<Iterator>::value_type;
    
    template<typename Iterator>
    struct is_valid_iterator {
        static bool const value = std::is_constructible_v<deref_type<Iterator>>;
    };
#endif
    
    // Performs the functional `zip` algorithm, in which every element of the resulting vector is a
    // tuple of this instance's element (first) and the second vector's element (second) at the same
    // index. The sizes of the two vectors must be equal.
    //
    // example:
    //      const fcpp::vector ages_vector({32, 25, 53});
    //      const fcpp::vector<std::string> names_vector({"Jake", "Mary", "John"});
    //      const auto zipped_vector = ages_vector.zip(names_vector);
    //
    // outcome:
    //      zipped_vector -> fcpp::vector<std::pair<int, std::string>>({
    //                          (32, "Jake"),
    //                          (25, "Mary"),
    //                          (53, "John"),
    //                       })
    //
    // is equivalent to:
    //      const fcpp::vector ages_vector({32, 25, 53});
    //      const fcpp::vector<std::string> names_vector({"Jake", "Mary", "John"});
    //      fcpp::vector<std::pair<int, std::string>> zipped_vector;
    //      for (auto i = 0; i < ages_vector.size(); ++i) {
    //          fcpp::vector<int>::pair<std::string> tuple;
    //          tuple.first = ages_vector[i];
    //          tuple.second = names_vector[i];
    //          zipped_vector.insert_back(tuple);
    //      }
    template <typename U>
    [[nodiscard]] vector<std::pair<T, U>> zip(const vector<U>& vector) const
    {
#ifdef CPP17_AVAILABLE
        return zip_impl(vector.begin(), vector.end());
#else
        return zip_impl<U>(vector.begin(), vector.end());
#endif
    }
    
    // Performs the functional `zip` algorithm, in which every element of the resulting vector is a
    // tuple of this instance's element (first) and the second vector's element (second) at the same
    // index. The sizes of the two vectors must be equal.
    //
    // example:
    //      const fcpp::vector ages_vector({32, 25, 53});
    //      const std::vector<std::string> names_vector({"Jake", "Mary", "John"});
    //      const auto zipped_vector = ages_vector.zip(names_vector);
    //
    // outcome:
    //      zipped_vector -> fcpp::vector<std::pair<int, std::string>>({
    //                          (32, "Jake"),
    //                          (25, "Mary"),
    //                          (53, "John"),
    //                       })
    //
    // is equivalent to:
    //      const fcpp::vector ages_vector({32, 25, 53});
    //      const std::vector<std::string> names_vector({"Jake", "Mary", "John"});
    //      fcpp::vector<std::pair<int, std::string>> zipped_vector;
    //      for (auto i = 0; i < ages_vector.size(); ++i) {
    //          fcpp::vector<int>::pair<std::string> tuple;
    //          tuple.first = ages_vector[i];
    //          tuple.second = names_vector[i];
    //          zipped_vector.insert_back(tuple);
    //      }
    template <typename U>
    [[nodiscard]] vector<std::pair<T, U>> zip(const std::vector<U>& vector) const
    {
#ifdef CPP17_AVAILABLE
        return zip_impl(vector.cbegin(), vector.cend());
#else
        return zip_impl<U>(vector.cbegin(), vector.cend());
#endif
    }
    
    // example:
    //      const fcpp::vector ages_vector({32, 25, 53});
    //      const std::initializer_list<std::string> names_vector({"Jake", "Mary", "John"});
    //      const auto zipped_vector = ages_vector.zip(names_vector);
    //
    // outcome:
    //      zipped_vector -> fcpp::vector<std::pair<int, std::string>>({
    //                          (32, "Jake"),
    //                          (25, "Mary"),
    //                          (53, "John"),
    //                       })
    //
    // is equivalent to:
    //      const fcpp::vector ages_vector({32, 25, 53});
    //      const std::initializer_list<std::string> names_vector({"Jake", "Mary", "John"});
    //      fcpp::vector<std::pair<int, std::string>> zipped_vector;
    //      for (auto i = 0; i < ages_vector.size(); ++i) {
    //          fcpp::vector<int>::pair<std::string> tuple;
    //          tuple.first = ages_vector[i];
    //          tuple.second = names_vector[i];
    //          zipped_vector.insert_back(tuple);
    //      }
    template <typename U>
    [[nodiscard]] vector<std::pair<T, U>> zip(const std::initializer_list<U>& list) const
    {
#ifdef CPP17_AVAILABLE
        return zip_impl(list.begin(), list.end());
#else
        return zip(std::vector<U>(list));
#endif
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
    //      fcpp::vector persons_vector({
    //          person(45, "Jake"), person(34, "Bob"), person(52, "Manfred"), person(8, "Alice")
    //      });
    //      persons_vector.sort([](const auto& person1, const auto& person2) {
    //          return person1.name < person2.name;
    //      });
    //
    // outcome:
    //      person_vector -> fcpp::vector({
    //          person(8, "Alice"), person(34, "Bob"), person(45, "Jake"), person(52, "Manfred")
    //      });
#ifdef CPP17_AVAILABLE
    template <typename Sortable, typename = std::enable_if_t<std::is_invocable_r_v<bool, Sortable, T, T>>>
#else
    template <typename Sortable>
#endif
    vector& sort(Sortable && comparison_predicate)
    {
        std::sort(m_vector.begin(),
                  m_vector.end(),
                  std::forward<Sortable>(comparison_predicate));
        return *this;
    }
        
#ifdef PARALLEL_ALGORITHM_AVAILABLE
    // Performs the `sort` algorithm in parallel.
    // See also the sequential version for more documentation.
    template <typename Sortable, typename = std::enable_if_t<std::is_invocable_r_v<bool, Sortable, T, T>>>
    vector& sort_parallel(Sortable && comparison_predicate)
    {
        std::sort(std::execution::par,
                  m_vector.begin(),
                  m_vector.end(),
                  std::forward<Sortable>(comparison_predicate));
        return *this;
    }
#endif
    
    // Sorts the vector in place in ascending order, when its elements support comparison by std::less_equal [<=] (mutating).
    //
    // example:
    //      fcpp::vector numbers({3, 1, 9, -4});
    //      numbers.sort_ascending();
    //
    // outcome:
    //      numbers -> fcpp::vector({-4, 1, 3, 9});
    vector& sort_ascending()
    {
        return sort(std::less_equal<T>());
    }
        
#ifdef PARALLEL_ALGORITHM_AVAILABLE
    // Performs the `sort_ascending` algorithm in parallel.
    // See also the sequential version for more documentation.
    vector& sort_ascending_parallel()
    {
        return sort_parallel(std::less_equal<T>());
    }
#endif
    
    // Sorts the vector in place in descending order, when its elements support comparison by std::greater_equal [>=] (mutating).
    //
    // example:
    //      fcpp::vector numbers({3, 1, 9, -4});
    //      numbers.sort_ascending();
    //
    // outcome:
    //      numbers -> fcpp::vector({9, 3, 1, -4});
    vector& sort_descending()
    {
        return sort(std::greater_equal<T>());
    }
        
#ifdef PARALLEL_ALGORITHM_AVAILABLE
    // Performs the `sort_ascending` algorithm in parallel.
    // See also the sequential version for more documentation.
    vector& sort_descending_parallel()
    {
        return sort_parallel(std::greater_equal<T>());
    }
#endif
    
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
    //      const fcpp::vector persons_vector({
    //          person(45, "Jake"), person(34, "Bob"), person(52, "Manfred"), person(8, "Alice")
    //      });
    //      auto sorted_persons_vector = persons_vector.sorted([](const auto& person1, const auto& person2) {
    //          return person1.name < person2.name;
    //      });
    //
    // outcome:
    //      sorted_persons_vector -> fcpp::vector({
    //          person(8, "Alice"), person(34, "Bob"), person(45, "Jake"), person(52, "Manfred")
    //      });
#ifdef CPP17_AVAILABLE
    template <typename Sortable, typename = std::enable_if_t<std::is_invocable_r_v<bool, Sortable, T, T>>>
#else
    template <typename Sortable>
#endif
    vector sorted(Sortable && comparison_predicate) const
    {
        auto sorted_vector(m_vector);
        std::sort(sorted_vector.begin(),
                  sorted_vector.end(),
                  std::forward<Sortable>(comparison_predicate));
        return vector(sorted_vector);
    }
        
#ifdef PARALLEL_ALGORITHM_AVAILABLE
    // Performs the `sorted` algorithm in parallel.
    // See also the sequential version for more documentation.
    template <typename Sortable, typename = std::enable_if_t<std::is_invocable_r_v<bool, Sortable, T, T>>>
    vector sorted_parallel(Sortable && comparison_predicate) const
    {
        auto sorted_vector(m_vector);
        std::sort(std::execution::par,
                  sorted_vector.begin(),
                  sorted_vector.end(),
                  std::forward<Sortable>(comparison_predicate));
        return fcpp::vector(sorted_vector);
    }
#endif
    
    // Sorts its elements copied and sorted in ascending order, when its elements support comparison by std::less_equal [<=] (non-mutating).
    //
    // example:
    //      const fcpp::vector numbers({3, 1, 9, -4});
    //      auto sorted_numbers = numbers.sorted_ascending();
    //
    // outcome:
    //      sorted_numbers -> fcpp::vector({-4, 1, 3, 9});
    [[nodiscard]] vector sorted_ascending() const
    {
        return sorted(std::less_equal<T>());
    }
        
#ifdef PARALLEL_ALGORITHM_AVAILABLE
    // Performs the `sorted_ascending` algorithm in parallel.
    // See also the sequential version for more documentation.
    [[nodiscard]] vector sorted_ascending_parallel() const
    {
        return sorted_parallel(std::less_equal<T>());
    }
#endif
    
    // Sorts its elements copied and sorted in descending order, when its elements support comparison by std::greater_equal [>=] (non-mutating).
    //
    // example:
    //      const fcpp::vector numbers({3, 1, 9, -4});
    //      auto sorted_numbers = numbers.sorted_descending();
    //
    // outcome:
    //      sorted_numbers -> fcpp::vector({9, 3, 1, -4});
    [[nodiscard]] vector sorted_descending() const
    {
        return sorted(std::greater_equal<T>());
    }
        
#ifdef PARALLEL_ALGORITHM_AVAILABLE
    // Performs the `sorted_descending` algorithm in parallel.
    // See also the sequential version for more documentation.
    [[nodiscard]] vector sorted_descending_parallel() const
    {
        return sorted_parallel(std::greater_equal<T>());
    }
#endif
    
    // Executes the given operation for each element of the vector. The operation must not
    // change the vector's contents during execution.
#ifdef CPP17_AVAILABLE
    template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<void, Callable, T const &>>>
#else
    template <typename Callable>
#endif
    const vector& for_each(Callable && operation) const
    {
        std::for_each(m_vector.cbegin(),
                      m_vector.cend(),
                      std::forward<Callable>(operation));
        return *this;
    }
    
#ifdef PARALLEL_ALGORITHM_AVAILABLE
    // Executes the given operation for each element of the vector in parallel. The operation must not
    // change the vector's contents during execution.
    template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<void, Callable, T const &>>>
    const vector& for_each_parallel(Callable && operation) const
    {
        std::for_each(std::execution::par,
                      m_vector.cbegin(),
                      m_vector.cend(),
                      std::forward<Callable>(operation));
        return *this;
    }
#endif
        
    // Returns the first index in which the given element is found in the vector.
    // In case of multiple occurrences, only the first index is returned
    // (see find_all_indices for multiple occurrences).
    //
    // example:
    //      const fcpp::vector numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      const auto index_of_one = numbers.find_first_index(1);
    //      const auto index_of_nine = numbers.find_first_index(9);
    //
    // outcome:
    //      index_of_one.value() -> 0
    //      index_of_one.has_value() -> true
    //      index_of_nine.has_value() -> false
    [[nodiscard]] fcpp::optional_t<size_t> find_first_index(const T& element) const
    {
        auto const it = std::find(m_vector.cbegin(),
                                  m_vector.cend(),
                                  element);
        if (it != m_vector.cend()) {
            auto index = std::distance(m_vector.cbegin(), it);
            return index;
        }
        return fcpp::optional_t<size_t>();
    }
    
    // Returns the last index in which the given element is found in the vector.
    // In case of multiple occurrences, only the last index is returned
    // (see find_all_indices for multiple occurrences).
    //
    // example:
    //      const fcpp::vector numbers({1, 4, 2, 5, -6, 3, 1, 7, 1});
    //      const auto index_of_one = numbers.find_last_index(1);
    //      const auto index_of_nine = numbers.find_last_index(9);
    //
    // outcome:
    //      index_of_one.value() -> 8
    //      index_of_one.has_value() -> true
    //      index_of_nine.has_value() -> false
    [[nodiscard]]  fcpp::optional_t<size_t> find_last_index(const T& element) const
    {
        auto const it = std::find(m_vector.crbegin(),
                                  m_vector.crend(),
                                  element);
        if (it != m_vector.crend()) {
            auto index = std::distance(it, m_vector.crend()) - 1;
            return index;
        }
        return  fcpp::optional_t<size_t>();
    }
    
    // Returns all indices in which the given element is found in the vector.
    //
    // example:
    //      const fcpp::vector numbers({1, 4, 2, 5, 8, 3, 1, 9, 1});
    //      const auto indices_of_one = numbers.find_all_indices(1);
    //      const auto indices_of_ten = numbers.find_all_indices(10);
    //
    // outcome:
    //      indices_of_one -> { 0, 6, 8 }
    //      indices_of_ten -> empty vector
    [[nodiscard]] std::vector<size_t> find_all_indices(const T& element) const
    {
        std::vector<size_t> indices;
        auto it = std::find(m_vector.cbegin(),
                            m_vector.cend(),
                            element);
        while (it != m_vector.cend())
        {
            indices.push_back(std::distance(m_vector.cbegin(), it));
            ++it;
            it = std::find(it, m_vector.cend(), element);
        }
        return indices;
    }
    
    // Removes the element at `index` (mutating)
    //
    // example:
    //      fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      numbers.remove_at(4);
    //
    // outcome:
    //      numbers -> fcpp::vector<int>({1, 4, 2, 5, 3, 1, 7, 1});
    vector& remove_at(size_t index)
    {
        assert_smaller_size(index);
        m_vector.erase(begin() + index);
        return *this;
    }
    
    // Returns a copy of itself in which the element at `index` is removed (non-mutating)
    //
    // example:
    //      const fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      auto shorter_vector = numbers.removing_at(4);
    //
    // outcome:
    //      shorter_vector -> fcpp::vector<int>({1, 4, 2, 5, 3, 1, 7, 1});
    [[nodiscard]] vector removing_at(size_t index) const
    {
        assert_smaller_size(index);
        auto copy(m_vector);
        copy.erase(copy.begin() + index);
        return vector(copy);
    }
    
    // Removes the last element, if present (mutating)
    //
    // example:
    //      fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      numbers.remove_back();
    //
    // outcome:
    //      numbers -> fcpp::vector<int>({1, 4, 2, 5, 8, 3, 1, 7});
    vector& remove_back()
    {
        m_vector.pop_back();
        return *this;
    }
    
    // Returns a copy of itself in which the last element is removed (non-mutating)
    //
    // example:
    //      const fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      auto shorter_vector = numbers.removing_back();
    //
    // outcome:
    //      shorter_vector -> fcpp::vector<int>({1, 4, 2, 5, 8, 3, 1, 7});
    [[nodiscard]] vector removing_back() const
    {
        auto copy(m_vector);
        copy.pop_back();
        return vector(copy);
    }
    
    // Removes the first element, if present (mutating)
    //
    // example:
    //      fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      numbers.remove_front();
    //
    // outcome:
    //      numbers -> fcpp::vector<int>({4, 2, 5, 8, 3, 1, 7, 1});
    vector& remove_front()
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
    //      const fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      auto shorter_numbers = numbers.removing_front();
    //
    // outcome:
    //      shorter_numbers -> fcpp::vector<int>({4, 2, 5, 8, 3, 1, 7, 1});
    [[nodiscard]] vector removing_front() const
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
    //		fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //		numbers.remove_range(index_range::start_count(2, 3));
    //
    // outcome:
    //		numbers -> fcpp::vector<int>({ 1, 4, 2, 7, 1 })
    vector& remove_range(index_range range)
    {
        if (!range.is_valid || size() < range.end + 1)
        {
            return *this;
        }
        m_vector.erase(begin() + range.start,
                              begin() + range.start + range.count);
        return *this;
    }
    
    // Returns a copy by removing the elements whose index is contained in the given index range (non-mutating)
    //
    // example:
    //		const fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //		const auto shorter_vector = numbers.removing_range(index_range::start_count(2, 3));
    //
    // outcome:
    //		shorter_vector -> fcpp::vector<int>({ 1, 4, 3, 1, 7, 1 })
    [[nodiscard]] vector removing_range(index_range range) const
    {
        if (!range.is_valid || size() < range.end + 1)
        {
            return *this;
        }
        auto shorter_vector(m_vector);
        shorter_vector.erase(shorter_vector.begin() + range.start,
                             shorter_vector.begin() + range.start + range.count);
        return vector(shorter_vector);
    }
    
    // Inserts an element at the given index, therefore changing the vector's contents (mutating)
    //
    // example:
    //      fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      numbers.insert_at(3, 18);
    //
    // outcome:
    //      numbers -> fcpp::vector({1, 4, 2, 18, 5, 8, 3, 1, 7, 1});
    vector& insert_at(size_t index, const T& element)
    {
        assert_smaller_or_equal_size(index);
        m_vector.insert(begin() + index, element);
        return *this;
    }
    
    // Returns a copy by inserting an element at the given index (non-mutating)
    //
    // example:
    //      const fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      auto augmented_numbers = numbers.inserting_at(3, 18);
    //
    // outcome:
    //      augmented_numbers -> fcpp::vector({1, 4, 2, 18, 5, 8, 3, 1, 7, 1});
    [[nodiscard]] vector inserting_at(size_t index, const T& element) const
    {
        assert_smaller_or_equal_size(index);
        auto copy(m_vector);
        copy.insert(copy.begin() + index, element);
        return vector(copy);
    }
    
    // Inserts a range of elements starting at the given index, therefore changing the vector's contents (mutating)
    //
    // example:
    //      fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      const fcpp::vector vector_to_insert({9, -5, 6});
    //      numbers.insert_at(3, vector_to_insert);
    //
    // outcome:
    //      numbers -> fcpp::vector({1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1});
    vector& insert_at(size_t index, const vector<T>& vector)
    {
        return insert_at_impl(index, vector.begin(), vector.end());
    }
    
    // Returns a copy by inserting a range of elements starting at the given index (non-mutating)
    //
    // example:
    //      const fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      const fcpp::vector vector_to_insert({9, -5, 6});
    //      auto augmented_numbers = numbers.inserting_at(3, vector_to_insert);
    //
    // outcome:
    //      augmented_numbers -> fcpp::vector({1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1});
    [[nodiscard]] vector inserting_at(size_t index, const vector<T>& vector) const
    {
        return inserting_at_impl(index, vector.begin(), vector.end());
    }
    
    // Inserts a range of elements starting at the given index, therefore changing the vector's contents (mutating)
    //
    // example:
    //      fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      const std::vector vector_to_insert({9, -5, 6});
    //      numbers.insert_at(3, vector_to_insert);
    //
    // outcome:
    //      numbers -> fcpp::vector({1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1});
    vector& insert_at(size_t index, const std::vector<T>& vector)
    {
        return insert_at_impl(index, vector.cbegin(), vector.cend());
    }
    
    // Returns a copy by inserting a range of elements starting at the given index (non-mutating)
    //
    // example:
    //      const fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      const std::vector vector_to_insert({9, -5, 6});
    //      auto augmented_numbers = numbers.inserting_at(3, vector_to_insert);
    //
    // outcome:
    //      augmented_numbers -> fcpp::vector({1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1});
    [[nodiscard]] vector inserting_at(size_t index, const std::vector<T>& vector) const
    {
        return inserting_at_impl(index, vector.cbegin(), vector.cend());
    }
    
    // Inserts a range of elements starting at the given index, therefore changing the vector's contents (mutating)
    //
    // example:
    //      fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      const std::initializer_list vector_to_insert({9, -5, 6});
    //      numbers.insert_at(3, vector_to_insert);
    //
    // outcome:
    //      numbers -> fcpp::vector({1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1});
    vector& insert_at(size_t index, std::initializer_list<T> list)
    {
        return insert_at(index, std::vector<T>(std::move(list)));
    }
    
    // Returns a copy by inserting a range of elements starting at the given index (non-mutating)
    //
    // example:
    //      const fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      const std::initializer_list vector_to_insert({9, -5, 6});
    //      auto augmented_numbers = numbers.inserting_at(3, vector_to_insert);
    //
    // outcome:
    //      augmented_numbers -> fcpp::vector({1, 4, 2, 9, -5, 6, 5, 8, 3, 1, 7, 1});
    [[nodiscard]] vector inserting_at(size_t index, std::initializer_list<T> list) const
    {
        return inserting_at(index, std::vector<T>(std::move(list)));
    }
    
    // Inserts a value at the end of the vector in place (mutating)
    //
    // example:
    //      fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      numbers.insert_back(18);
    //
    // outcome:
    //      numbers -> fcpp::vector({1, 4, 2, 5, 8, 3, 1, 7, 1, 18});
    vector& insert_back(T value)
    {
        m_vector.push_back(value);
        return *this;
    }
    
    // Inserts a value at the beginning of the vector in place (mutating)
    //
    // example:
    //      fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      numbers.insert_front(18);
    //
    // outcome:
    //      numbers -> fcpp::vector({18, 1, 4, 2, 5, 8, 3, 1, 7, 1});
    vector& insert_front(T value)
    {
        return insert_at(0, value);
    }
    
    // Makes a copy of the vector, inserts value at the end of the copy and returns the copy (non-mutating)
    //
    // example:
    //      const fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      auto augmented_numbers = numbers.inserting_back(18);
    //
    // outcome:
    //      augmented_numbers -> fcpp::vector({1, 4, 2, 5, 8, 3, 1, 7, 1, 18});
    [[nodiscard]] vector inserting_back(T value) const
    {
        auto augmented_vector(m_vector);
        augmented_vector.push_back(value);
        return vector(augmented_vector);
    }
    
    // Makes a copy of the vector, inserts value at the beginning of the copy and returns the copy (non-mutating)
    //
    // example:
    //      const fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      auto augmented_numbers = numbers.inserting_front(18);
    //
    // outcome:
    //      augmented_numbers -> fcpp::vector({18, 1, 4, 2, 5, 8, 3, 1, 7, 1});
    [[nodiscard]] vector inserting_front(T value) const
    {
        return inserting_at(0, value);
    }
    
    // Inserts a range of values at the end of the vector in place (mutating)
    //
    // example:
    //      fcpp::vector<int> numbers({ 4, 5, 6 });
    //      numbers.insert_back(fcpp::vector({1, 2, 3}));
    //
    // outcome:
    //      numbers -> fcpp::vector<int> numbers({ 4, 5, 6, 1, 2, 3 });
    vector& insert_back(const vector<T>& vector)
    {
        return insert_back_range_impl(vector.begin(), vector.end());
    }
    
    // Inserts a range of values at the beginning of the vector in place (mutating)
    //
    // example:
    //      fcpp::vector<int> numbers({ 4, 5, 6 });
    //      numbers.insert_front(fcpp::vector({1, 2, 3}));
    //
    // outcome:
    //      numbers -> fcpp::vector<int> numbers({ 1, 2, 3, 4, 5, 6 });
    vector& insert_front(const vector<T>& vector)
    {
        return insert_front_range_impl(vector.begin(), vector.end());
    }
    
    // Makes a copy of the vector, inserts a range of values at the end of the copy, and returns the copy (non-mutating)
    //
    // example:
    //      const fcpp::vector<int> numbers({ 4, 5, 6 });
    //      auto augmented_numbers = numbers.inserting_back(fcpp::vector({1, 2, 3}));
    //
    // outcome:
    //      augmented_numbers -> fcpp::vector<int> numbers({ 4, 5, 6, 1, 2, 3 });
    [[nodiscard]] vector inserting_back(const vector<T>& vector) const
    {
        return inserting_back_range_impl(vector.begin(), vector.end());
    }
    
    // Makes a copy of the vector, inserts a range of values at the beginning of the copy, and returns the copy (non-mutating)
    //
    // example:
    //      const fcpp::vector<int> numbers({ 4, 5, 6 });
    //      auto augmented_numbers = numbers.inserting_front(fcpp::vector({1, 2, 3}));
    //
    // outcome:
    //      augmented_numbers -> fcpp::vector<int> numbers({ 1, 2, 3, 4, 5, 6 });
    [[nodiscard]] vector inserting_front(const vector<T>& vector) const
    {
        return inserting_front_range_impl(vector.begin(), vector.end());
    }
    
    // Inserts a range of values at the end of the vector in place (mutating)
    //
    // example:
    //      fcpp::vector<int> numbers({ 4, 5, 6 });
    //      numbers.insert_back(std::vector({1, 2, 3}));
    //
    // outcome:
    //      numbers -> fcpp::vector<int> numbers({ 4, 5, 6, 1, 2, 3 });
    vector& insert_back(const std::vector<T>& vector)
    {
        return insert_back_range_impl(vector.cbegin(), vector.cend());
    }
    
    // Inserts a range of values at the beginning of the vector in place (mutating)
    //
    // example:
    //      fcpp::vector<int> numbers({ 4, 5, 6 });
    //      numbers.insert_front(std::vector({1, 2, 3}));
    //
    // outcome:
    //      numbers -> fcpp::vector<int> numbers({ 1, 2, 3, 4, 5, 6 });
    vector& insert_front(const std::vector<T>& vector)
    {
        return insert_front_range_impl(vector.cbegin(), vector.cend());
    }
    
    // Makes a copy of the vector, inserts a range of values at the end of the copy, and returns the copy (non-mutating)
    //
    // example:
    //      const fcpp::vector<int> numbers({ 4, 5, 6 });
    //      auto augmented_numbers = numbers.inserting_back(std::vector({1, 2, 3}));
    //
    // outcome:
    //      augmented_numbers -> fcpp::vector<int> numbers({ 4, 5, 6, 1, 2, 3 });
    [[nodiscard]] vector inserting_back(const std::vector<T>& vector) const
    {
        return inserting_back_range_impl(vector.cbegin(), vector.cend());
    }
    
    // Makes a copy of the vector, inserts a range of values at the beginning of the copy, and returns the copy (non-mutating)
    //
    // example:
    //      const fcpp::vector<int> numbers({ 4, 5, 6 });
    //      auto augmented_numbers = numbers.inserting_front(std::vector({1, 2, 3}));
    //
    // outcome:
    //      augmented_numbers -> fcpp::vector<int> numbers({ 1, 2, 3, 4, 5, 6 });
    [[nodiscard]] vector inserting_front(const std::vector<T>& vector) const
    {
        return inserting_front_range_impl(vector.cbegin(), vector.cend());
    }
    
    // Inserts a range of values at the end of the vector in place (mutating)
    //
    // example:
    //      fcpp::vector<int> numbers({ 4, 5, 6 });
    //      numbers.insert_back(std::initializer_list({1, 2, 3}));
    //
    // outcome:
    //      numbers -> fcpp::vector<int> numbers({ 4, 5, 6, 1, 2, 3 });
    vector& insert_back(const std::initializer_list<T>& list)
    {
        return insert_back(std::vector<T>(list));
    }
    
    // Inserts a range of values at the beginning of the vector in place (mutating)
    //
    // example:
    //      fcpp::vector<int> numbers({ 4, 5, 6 });
    //      numbers.insert_front(std::initializer_list({1, 2, 3}));
    //
    // outcome:
    //      numbers -> fcpp::vector<int> numbers({ 1, 2, 3, 4, 5, 6 });
    vector& insert_front(const std::initializer_list<T>& list)
    {
        return insert_front(std::vector<T>(list));
    }
    
    // Makes a copy of the vector, inserts a range of values at the end of the copy, and returns the copy (non-mutating)
    //
    // example:
    //      const fcpp::vector<int> numbers({ 4, 5, 6 });
    //      auto augmented_numbers = numbers.inserting_back(std::initializer_list({1, 2, 3}));
    //
    // outcome:
    //      augmented_numbers -> fcpp::vector<int> numbers({ 4, 5, 6, 1, 2, 3 });
    [[nodiscard]] vector inserting_back(const std::initializer_list<T>& list) const
    {
        return inserting_back(std::vector<T>(list));
    }
    
    // Makes a copy of the vector, inserts a range of values at the beginning of the copy, and returns the copy (non-mutating)
    //
    // example:
    //      const fcpp::vector<int> numbers({ 4, 5, 6 });
    //      auto augmented_numbers = numbers.inserting_front(std::initializer_list({1, 2, 3}));
    //
    // outcome:
    //      augmented_numbers -> fcpp::vector<int> numbers({ 1, 2, 3, 4, 5, 6 });
    [[nodiscard]] vector inserting_front(const std::initializer_list<T>& list) const
    {
        return inserting_front(std::vector<T>(list));
    }
    
    // Replaces the existing contents starting at `index` with the contents of the given vector (mutating)
    //
    // example:
    //      fcpp::vector numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      numbers.replace_range_at(4, fcpp::vector({9, -10, 8}));
    //
    // outcome:
    //      numbers -> fcpp::vector({ 1, 4, 2, 5, 9, -10, 8, 7, 1 })
    vector& replace_range_at(size_t index, const vector<T>& vector)
    {
        return replace_range_at_imp(index, vector.begin(), vector.end());
    }
    
    // Replaces the existing contents starting at `index` with the contents of the given vector (mutating)
    //
    // example:
    //      fcpp::vector numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      numbers.replace_range_at(4, std::vector({9, -10, 8}));
    //
    // outcome:
    //      numbers -> fcpp::vector({ 1, 4, 2, 5, 9, -10, 8, 7, 1 })
    vector& replace_range_at(size_t index, const std::vector<T>& vector)
    {
        return replace_range_at_imp(index, vector.cbegin(), vector.cend());
    }
    
    // Replaces the existing contents starting at `index` with the contents of the given vector (mutating)
    //
    // example:
    //      fcpp::vector numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      numbers.replace_range_at(4, std::initializer_list({9, -10, 8}));
    //
    // outcome:
    //      numbers -> fcpp::vector({ 1, 4, 2, 5, 9, -10, 8, 7, 1 })
    vector& replace_range_at(size_t index, const std::initializer_list<T>& list)
    {
        return replace_range_at(index, std::vector<T>(list));
    }
    
    // Returns a copy whose elements starting at `index` are replaced with the contents of the given vector (non-mutating)
    //
    // example:
    //      const fcpp::vector numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      auto replaced_numbers = numbers.replacing_range_at(4, fcpp::vector({9, -10, 8}));
    //
    // outcome:
    //      replaced_numbers -> fcpp::vector({ 1, 4, 2, 5, 9, -10, 8, 7, 1 })
    [[nodiscard]] vector replacing_range_at(size_t index, const vector<T>& vector) const
    {
        return replacing_range_at_imp(index, vector.begin(), vector.end());
    }
    
    // Returns a copy whose elements starting at `index` are replaced with the contents of the given vector (non-mutating)
    //
    // example:
    //      const fcpp::vector numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      auto replaced_numbers = numbers.replacing_range_at(4, std::vector({9, -10, 8}));
    //
    // outcome:
    //      replaced_numbers -> fcpp::vector({ 1, 4, 2, 5, 9, -10, 8, 7, 1 })
    [[nodiscard]] vector replacing_range_at(size_t index, const std::vector<T>& vector) const
    {
        return replacing_range_at_imp(index, vector.cbegin(), vector.cend());
    }
    
    // Returns a copy whose elements starting at `index` are replaced with the contents of the given vector (non-mutating)
    //
    // example:
    //      const fcpp::vector numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      auto replaced_numbers = numbers.replacing_range_at(4, std::initializer_list({9, -10, 8}));
    //
    // outcome:
    //      replaced_numbers -> fcpp::vector({ 1, 4, 2, 5, 9, -10, 8, 7, 1 })
    [[nodiscard]] vector replacing_range_at(size_t index, const std::initializer_list<T>& list) const
    {
        return replacing_range_at(index, std::vector<T>(list));
    }
    
    // Replaces all existing elements with a constant element (mutating)
    //
    // example:
    //      fcpp::vector numbers({1, 3, -6, 4, -9});
    //      numbers.fill(7);
    //
    // outcome:
    //      numbers -> fcpp::vector({ 7, 7, 7, 7, 7 })
    vector& fill(const T& element)
    {
        std::fill(m_vector.begin(),
                  m_vector.end(),
                  element);
        return *this;
    }
    
    // Returns the size of the vector (how many elements it contains, it may be different from its capacity)
    [[nodiscard]] size_t size() const
    {
        return m_vector.size();
    }
    
    // Clears the vector by removing all elements (mutating)
    vector& clear()
    {
        m_vector.clear();
        return *this;
    }
    
    // Returns true if the vector has no elements
    [[nodiscard]] bool is_empty() const
    {
        return m_vector.empty();
    }
    
    // Returns the underlying capacity of the vector, which can be larger from its size
    [[nodiscard]] size_t capacity() const
    {
        return m_vector.capacity();
    }
    
    // Reserves the necessary memory for `count` elements, so that subsequent changes in the
    // vector's size due to addition/removal of elements is more performant
    vector& reserve(size_t count)
    {
        m_vector.reserve(count);
        return *this;
    }
    
    // Resizes the vector to have given number of elements
    // If `count` is larger than the current `size`, then `count-size` default elements are inserted at the back
    // If `count` is smaller than the current `size`, then the last `size - count` elements are removed
    //
    // example:
    //      // numbers.capacity() = 9
    //      // numbers.size() = 9
    //      fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    
    //      // numbers.capacity() = 9
    //      // numbers.size() = 5
    //      // numbers -> fcpp::vector<int>({1, 4, 2, 5, 8});
    //      numbers.resize(5);
    //
    //      // empty_numbers.capacity() = 0
    //      // empty_numbers.size() = 0
    //      fcpp::vector<int> empty_numbers;
    //
    //      // empty_numbers.capacity() = 5
    //      // empty_numbers.size() = 5
    //      // empty_numbers -> fcpp::vector<int>({0, 0, 0, 0, 0});
    //      empty_numbers.resize(5);
    vector& resize(size_t count)
    {
        m_vector.resize(count);
        return *this;
    }
    
    // Returns the begin iterator, useful for other standard library algorithms
    [[nodiscard]] typename std::vector<T>::iterator begin()
    {
        return m_vector.begin();
    }
    
    // Returns the const begin iterator, useful for other standard library algorithms
    [[nodiscard]] typename std::vector<T>::const_iterator begin() const
    {
        return m_vector.begin();
    }
    
    // Returns the end iterator, useful for other standard library algorithms
    [[nodiscard]] typename std::vector<T>::iterator end()
    {
        return m_vector.end();
    }
    
    // Returns the const end iterator, useful for other standard library algorithms
    [[nodiscard]] typename std::vector<T>::const_iterator end() const
    {
        return m_vector.end();
    }
    
    // Returns a set, whose elements are the elements of the vector, removing any potential duplicates
    //
    // example:
    //      const fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      const auto& unique_numbers = numbers.distinct();
    //
    // outcome:
    //      unique_numbers -> fcpp::set<int>({1, 2, 3, 4, 5, 7, 8})
    template <typename UCompare = std::less<T>>
    set<T, UCompare> distinct() const {
        return set<T, UCompare>(*this);
    }
    
    // Returns a reference to the element in the given index, allowing subscripting and value editing.
    // Bounds checking (assert) is enabled for debug builds.
    T& operator[](size_t index)
    {
        assert_smaller_size(index);
        return m_vector[index];
    }
    
    // Returns a constant reference to the element in the given index, allowing subscripting.
    // Bounds checking (assert) is enabled for debug builds.
    const T& operator[](size_t index) const
    {
        assert_smaller_size(index);
        return m_vector[index];
    }
    
    // Returns true if both instances have equal sizes and the corresponding elements (same index) are equal
    bool operator ==(const vector<T>& rhs) const
    {
#ifdef CPP17_AVAILABLE
        return std::equal(begin(),
                          end(),
                          rhs.begin(),
                          rhs.end());
#else
        if (size() != rhs.size()) {
            return false;
        }
        
        for (auto i = 0; i < size(); ++i) {
            if (!((*this)[i] == rhs[i])) {
                return false;
            }
        }
        
        return true;
#endif
    }
    
    // Returns false if either the sizes are not equal or at least one corresponding element (same index) is not equal
    bool operator !=(const vector<T>& rhs) const
    {
        return !((*this) == rhs);
    }
    
private:
    std::vector<T> m_vector;
    // The iterator passed here may not necessarily be from std::vector as long as it's a valid iterable range
#ifdef CPP17_AVAILABLE
    template<typename Iterator, typename = std::enable_if_t<std::is_constructible_v<T, typename std::iterator_traits<Iterator>::value_type>>>
#else
    template<typename Iterator>
#endif
    vector& insert_back_range_impl(const Iterator& vec_begin, const Iterator& vec_end)
    {
        m_vector.insert(m_vector.end(),
                               vec_begin,
                               vec_end);
        return *this;
    }
    
#ifdef CPP17_AVAILABLE
    template<typename Iterator, typename = std::enable_if_t<std::is_constructible_v<T, typename std::iterator_traits<Iterator>::value_type>>>
#else
    template<typename Iterator>
#endif
    vector& insert_front_range_impl(const Iterator& vec_begin, const Iterator& vec_end)
    {
        m_vector.insert(m_vector.begin(),
                               vec_begin,
                               vec_end);
        return *this;
    }
    
#ifdef CPP17_AVAILABLE
    template<typename Iterator, typename = std::enable_if_t<std::is_constructible_v<T, typename std::iterator_traits<Iterator>::value_type>>>
#else
    template<typename Iterator>
#endif
    [[nodiscard]] vector inserting_back_range_impl(const Iterator& vec_begin, const Iterator& vec_end) const
    {
        auto augmented_vector(m_vector);
        augmented_vector.reserve(augmented_vector.size() + std::distance(vec_begin, vec_end));
        augmented_vector.insert(augmented_vector.end(),
                                vec_begin,
                                vec_end);
        return vector(augmented_vector);
    }
    
#ifdef CPP17_AVAILABLE
    template<typename Iterator, typename = std::enable_if_t<std::is_constructible_v<T, typename std::iterator_traits<Iterator>::value_type>>>
#else
    template<typename Iterator>
#endif
    [[nodiscard]] vector inserting_front_range_impl(const Iterator& vec_begin, const Iterator& vec_end) const
    {
        auto augmented_vector(m_vector);
        augmented_vector.reserve(augmented_vector.size() + std::distance(vec_begin, vec_end));
        augmented_vector.insert(augmented_vector.begin(),
                                vec_begin,
                                vec_end);
        return vector(augmented_vector);
    }
    
#ifdef CPP17_AVAILABLE
    template<typename Iterator, typename = std::enable_if_t<is_valid_iterator<Iterator>::value>>
    [[nodiscard]] auto zip_impl( const Iterator& vec_begin, const Iterator& vec_end) const ->
    vector<std::pair<T, deref_type<Iterator>>>
    {
        using U = deref_type<Iterator>;
#else
    template <typename U>
    [[nodiscard]] vector<std::pair<T, U>> zip_impl(const typename std::vector<U>::const_iterator& vec_begin,
                                                      const typename std::vector<U>::const_iterator& vec_end) const
    {
#endif
        const auto vec_size = std::distance(vec_begin, vec_end);
        assert(m_vector.size() == vec_size);
        std::vector<std::pair<T, U>> combined_vector;
        combined_vector.reserve(vec_size);
        for (size_t i = 0; i < vec_size; ++i)
        {
            combined_vector.push_back({m_vector[i], *(vec_begin + i)});
        }
        return vector<std::pair<T, U>>(std::move(combined_vector));
    }
    
#ifdef CPP17_AVAILABLE
    // checks if the value of dereferencing the passed Iterators is convertible to type T
    template<typename Iterator, typename = std::enable_if_t<std::is_constructible_v<T, typename std::iterator_traits<Iterator>::value_type>>>
#else
    template<typename Iterator>
#endif
    vector& insert_at_impl(size_t index,
                                      const Iterator& vec_begin,
                                      const Iterator& vec_end)
    {
        if (vec_begin != vec_end)
        {
            assert_smaller_or_equal_size(index);
            m_vector.insert(begin() + index,
                                   vec_begin,
                                   vec_end);
        }
        return *this;
    }
    
#ifdef CPP17_AVAILABLE
    template<typename Iterator, typename = std::enable_if_t<std::is_constructible_v<T, typename std::iterator_traits<Iterator>::value_type>>>
#else
    template<typename Iterator>
#endif
    [[nodiscard]] vector inserting_at_impl(size_t index,
                                                      const Iterator& vec_begin,
                                                      const Iterator& vec_end) const
    {
        if (vec_begin == vec_end)
        {
            return *this;
        }
        assert_smaller_or_equal_size(index);
        auto augmented_vector(m_vector);
        augmented_vector.insert(augmented_vector.begin() + index,
                                vec_begin,
                                vec_end);
        return vector(std::move(augmented_vector));
    }
    
#ifdef CPP17_AVAILABLE
    template<typename Iterator, typename = std::enable_if_t<std::is_constructible_v<T, typename std::iterator_traits<Iterator>::value_type>>>
#else
    template<typename Iterator>
#endif
    vector& replace_range_at_imp(size_t index,
                                            const Iterator& vec_begin,
                                            const Iterator& vec_end)
    {
        const auto vec_size = std::distance(vec_begin, vec_end);
        assert(index + vec_size >= vec_size && index + vec_size <= size());
        std::copy(vec_begin,
                  vec_end,
                  m_vector.begin() + index);
        return *this;
    }
    
#ifdef CPP17_AVAILABLE
    template<typename Iterator, typename = std::enable_if_t<std::is_constructible_v<T, typename std::iterator_traits<Iterator>::value_type>>>
#else
    template<typename Iterator>
#endif
    [[nodiscard]] vector replacing_range_at_imp(size_t index,
                                                           const Iterator& vec_begin,
                                                           const Iterator& vec_end) const
    {
        const auto vec_size = std::distance(vec_begin, vec_end);
        assert(index + vec_size >= vec_size && index + vec_size <= size());
        auto replaced_vector(m_vector);
        std::copy(vec_begin,
                  vec_end,
                  replaced_vector.begin() + index);
        return vector(replaced_vector);
    }
    
    void assert_smaller_size(size_t index) const
    {
        assert(index < size());
    }
    
    void assert_smaller_or_equal_size(size_t index) const
    {
        assert(index <= size());
    }
};
    
}
