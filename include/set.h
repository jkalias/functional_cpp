// MIT License
//
// Copyright (c) 2026 Ioannis Kaliakatsos
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
#include "compatibility.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <set>
#include <type_traits>
#include <utility>
#include <vector>
#include "optional.h"

namespace fcpp {
    template <typename T>
    class vector;

    template <typename T>
    class lazy_vector;

    template <class TKey, class TCompare>
    class set;

    // A lightweight wrapper representing a deferred set pipeline, enabling fluent and functional
    // programming while avoiding intermediate set materialization.
    //
    // Member functions are non-mutating and keep extending the pipeline. Terminal functions such as
    // `get` and `reduce` execute the stored operations.
    template <class TKey, class TCompare = std::less<TKey>>
    class lazy_set
    {
    public:
        lazy_set()
            : m_compare()
            , m_operation([](const std::function<void(const TKey&)>&) {})
        {
        }

        // Creates a lazy set by copying the provided std::set as an owned source.
        explicit lazy_set(const std::set<TKey, TCompare>& set)
            : m_compare(set.key_comp())
        {
            auto source = std::make_shared<std::set<TKey, TCompare>>(set);
            m_operation = [source](const std::function<void(const TKey&)>& consumer) {
                std::for_each(source->begin(), source->end(), consumer);
            };
        }

        // Creates a lazy set by moving the provided std::set as an owned source.
        explicit lazy_set(std::set<TKey, TCompare>&& set)
            : m_compare(set.key_comp())
        {
            auto source = std::make_shared<std::set<TKey, TCompare>>(std::move(set));
            m_operation = [source](const std::function<void(const TKey&)>& consumer) {
                std::for_each(source->begin(), source->end(), consumer);
            };
        }

        // Creates a lazy set by directly providing the deferred operation.
        // This constructor is mostly useful for composing lazy_set instances.
        explicit lazy_set(std::function<void(const std::function<void(const TKey&)>&)> operation)
            : m_compare()
            , m_operation(std::move(operation))
        {
        }

        // Creates a lazy set by directly providing the deferred operation and comparator.
        // This constructor is mostly useful for preserving comparator state while composing
        // lazy_set instances.
        lazy_set(std::function<void(const std::function<void(const TKey&)>&)> operation,
                 const TCompare& compare)
            : m_compare(compare)
            , m_operation(std::move(operation))
        {
        }

        // Performs the functional `map` algorithm lazily. The transform is not applied until
        // a terminal operation, such as `get` or `reduce`, is called.
        //
        // example:
        //      const fcpp::set<int> input_set({ 1, 3, -5 });
        //      const auto output_set = input_set
        //          .lazy()
        //          .map<std::string>([](const int& element) {
        //              return std::to_string(element);
        //          })
        //          .get();
        //
        // outcome:
        //      output_set -> fcpp::set<std::string>({ "-5", "1", "3" })
#ifdef CPP17_AVAILABLE
        template <class UKey, class UCompare = std::less<UKey>, typename Transform, typename = std::enable_if_t<
                      std::is_invocable_r_v<UKey, Transform, TKey>>>
#else
        template <typename UKey, class UCompare = std::less<UKey>, typename Transform>
#endif
        [[nodiscard]] lazy_set<UKey, UCompare> map(Transform&& transform) const
        {
            const auto previous = m_operation;
            typename std::decay<Transform>::type transform_copy(std::forward<Transform>(transform));
            return lazy_set<UKey, UCompare>(
                [previous, transform_copy](const std::function<void(const UKey&)>& consumer) mutable {
                    previous([&consumer, &transform_copy](const TKey& key) {
                        consumer(transform_copy(key));
                    });
                });
        }

        // Performs the functional `filter` algorithm lazily, in which all keys which match
        // the given predicate are kept. The predicate is not applied until a terminal operation,
        // such as `get` or `reduce`, is called.
        //
        // example:
        //      const fcpp::set<int> numbers({ 1, 3, -5, 2, -1, 9, -4 });
        //      const auto filtered_numbers = numbers
        //          .lazy()
        //          .filter([](const int& element) {
        //              return element >= 1.5;
        //          })
        //          .get();
        //
        // outcome:
        //      filtered_numbers -> fcpp::set<int>({ 2, 3, 9 })
#ifdef CPP17_AVAILABLE
        template <typename Filter, typename = std::enable_if_t<std::is_invocable_r_v<bool, Filter, TKey>>>
#else
        template <typename Filter>
#endif
        [[nodiscard]] lazy_set filter(Filter&& predicate_to_keep) const
        {
            const auto previous = m_operation;
            typename std::decay<Filter>::type predicate_copy(std::forward<Filter>(predicate_to_keep));
            return lazy_set(
                [previous, predicate_copy](const std::function<void(const TKey&)>& consumer) mutable {
                    previous([&consumer, &predicate_copy](const TKey& key) {
                        if (predicate_copy(key)) {
                            consumer(key);
                        }
                    });
                },
                m_compare);
        }

        // Performs the functional `filter` algorithm lazily.
        // See also `filter` for more documentation.
#ifdef CPP17_AVAILABLE
        template <typename Filter, typename = std::enable_if_t<std::is_invocable_r_v<bool, Filter, TKey>>>
#else
        template <typename Filter>
#endif
        [[nodiscard]] lazy_set filtered(Filter&& predicate_to_keep) const
        {
            return filter(std::forward<Filter>(predicate_to_keep));
        }

        // Returns the lazy set of elements which belong to this lazy set but not in the other set.
        // The operation is deferred until a terminal operation, such as `get` or `reduce`, is called.
        //
        // example:
        //      const fcpp::set<int> set1({1, 2, 3, 5, 7, 8, 10});
        //      const fcpp::set<int> set2({2, 5, 7, 10, 15, 17});
        //      const auto diff = set1
        //          .lazy()
        //          .difference_with(set2)
        //          .get();
        //
        // outcome:
        //      diff -> fcpp::set<int>({1, 3, 8})
        [[nodiscard]] lazy_set difference_with(const set<TKey, TCompare>& other) const
        {
            return difference_with(lazy_set(std::set<TKey, TCompare>(other.begin(),
                                                                     other.end(),
                                                                     other.key_comp())));
        }

        // Returns the lazy set of elements which belong to this lazy set but not in the std::set.
        // The operation is deferred until a terminal operation is called.
        [[nodiscard]] lazy_set difference_with(const std::set<TKey, TCompare>& other) const
        {
            return difference_with(lazy_set(other));
        }

        // Returns the lazy set of elements which belong to this lazy set but not in the other lazy set.
        // Both lazy sets are materialized internally when a terminal operation is called.
        [[nodiscard]] lazy_set difference_with(const lazy_set& other) const
        {
            const auto previous = m_operation;
            const auto compare = m_compare;
            return lazy_set(
                [previous, other, compare](const std::function<void(const TKey&)>& consumer) {
                    std::set<TKey, TCompare> current(compare);
                    previous([&current](const TKey& key) {
                        current.insert(key);
                    });

                    const auto materialized_other = materialize_with_compare(other, compare);
                    std::set<TKey, TCompare> diff(compare);
                    std::set_difference(current.begin(),
                                        current.end(),
                                        materialized_other.begin(),
                                        materialized_other.end(),
                                        std::inserter(diff, diff.begin()),
                                        compare);
                    std::for_each(diff.begin(), diff.end(), consumer);
                },
                compare);
        }

        // Returns the lazy set of elements which belong either to this lazy set or the other set.
        // The operation is deferred until a terminal operation, such as `get` or `reduce`, is called.
        //
        // example:
        //      const fcpp::set<int> set1({1, 2, 3, 5, 7, 8, 10});
        //      const fcpp::set<int> set2({2, 5, 7, 10, 15, 17});
        //      const auto combined = set1
        //          .lazy()
        //          .union_with(set2)
        //          .get();
        //
        // outcome:
        //      combined -> fcpp::set<int>({1, 2, 3, 5, 7, 8, 10, 15, 17})
        [[nodiscard]] lazy_set union_with(const set<TKey, TCompare>& other) const
        {
            return union_with(lazy_set(std::set<TKey, TCompare>(other.begin(),
                                                                other.end(),
                                                                other.key_comp())));
        }

        // Returns the lazy set of elements which belong either to this lazy set or the std::set.
        // The operation is deferred until a terminal operation is called.
        [[nodiscard]] lazy_set union_with(const std::set<TKey, TCompare>& other) const
        {
            return union_with(lazy_set(other));
        }

        // Returns the lazy set of elements which belong either to this lazy set or the other lazy set.
        // Both lazy sets are materialized internally when a terminal operation is called.
        [[nodiscard]] lazy_set union_with(const lazy_set& other) const
        {
            const auto previous = m_operation;
            const auto compare = m_compare;
            return lazy_set(
                [previous, other, compare](const std::function<void(const TKey&)>& consumer) {
                    std::set<TKey, TCompare> current(compare);
                    previous([&current](const TKey& key) {
                        current.insert(key);
                    });

                    const auto materialized_other = materialize_with_compare(other, compare);
                    std::set<TKey, TCompare> combined(compare);
                    std::set_union(current.begin(),
                                   current.end(),
                                   materialized_other.begin(),
                                   materialized_other.end(),
                                   std::inserter(combined, combined.begin()),
                                   compare);
                    std::for_each(combined.begin(), combined.end(), consumer);
                },
                compare);
        }

        // Returns the lazy set of elements which belong to both this lazy set and the other set.
        // The operation is deferred until a terminal operation, such as `get` or `reduce`, is called.
        //
        // example:
        //      const fcpp::set<int> set1({1, 2, 3, 5, 7, 8, 10});
        //      const fcpp::set<int> set2({2, 5, 7, 10, 15, 17});
        //      const auto combined = set1
        //          .lazy()
        //          .intersect_with(set2)
        //          .get();
        //
        // outcome:
        //      combined -> fcpp::set<int>({2, 5, 7, 10})
        [[nodiscard]] lazy_set intersect_with(const set<TKey, TCompare>& other) const
        {
            return intersect_with(lazy_set(std::set<TKey, TCompare>(other.begin(),
                                                                    other.end(),
                                                                    other.key_comp())));
        }

        // Returns the lazy set of elements which belong to both this lazy set and the std::set.
        // The operation is deferred until a terminal operation is called.
        [[nodiscard]] lazy_set intersect_with(const std::set<TKey, TCompare>& other) const
        {
            return intersect_with(lazy_set(other));
        }

        // Returns the lazy set of elements which belong to both this lazy set and the other lazy set.
        // Both lazy sets are materialized internally when a terminal operation is called.
        [[nodiscard]] lazy_set intersect_with(const lazy_set& other) const
        {
            const auto previous = m_operation;
            const auto compare = m_compare;
            return lazy_set(
                [previous, other, compare](const std::function<void(const TKey&)>& consumer) {
                    std::set<TKey, TCompare> current(compare);
                    previous([&current](const TKey& key) {
                        current.insert(key);
                    });

                    const auto materialized_other = materialize_with_compare(other, compare);
                    std::set<TKey, TCompare> intersection(compare);
                    std::set_intersection(current.begin(),
                                          current.end(),
                                          materialized_other.begin(),
                                          materialized_other.end(),
                                          std::inserter(intersection, intersection.begin()),
                                          compare);
                    std::for_each(intersection.begin(), intersection.end(), consumer);
                },
                compare);
        }

        // Performs the functional `zip` algorithm lazily, in which every key of the resulting
        // lazy set is a tuple of this instance's key (first) and the second set's key (second).
        // The sizes of the two sets must be equal.
        template <typename UKey, typename UCompare>
        [[nodiscard]] lazy_set<std::pair<TKey, UKey>> zip(const set<UKey, UCompare>& set) const
        {
            return zip(lazy_set<UKey, UCompare>(std::set<UKey, UCompare>(set.begin(),
                                                                         set.end(),
                                                                         set.key_comp())));
        }

        // Performs the functional `zip` algorithm lazily.
        // The sizes of the two sets must be equal.
        template <typename UKey, typename UCompare>
        [[nodiscard]] lazy_set<std::pair<TKey, UKey>> zip(const std::set<UKey, UCompare>& set) const
        {
            return zip(lazy_set<UKey, UCompare>(set));
        }

        // Performs the functional `zip` algorithm lazily where duplicates are removed before zipping.
        // The input vector must contain the same number of distinct values as the set size.
        template <typename UKey>
        [[nodiscard]] lazy_set<std::pair<TKey, UKey>> zip(const vector<UKey>& vector) const
        {
            std::set<UKey> distinct_values(vector.begin(), vector.end());
            return zip(lazy_set<UKey>(std::move(distinct_values)));
        }

        // Performs the functional `zip` algorithm lazily where duplicates are removed before zipping.
        // The input vector must contain the same number of distinct values as the set size.
        template <typename UKey>
        [[nodiscard]] lazy_set<std::pair<TKey, UKey>> zip(const std::vector<UKey>& vector) const
        {
            std::set<UKey> distinct_values(vector.begin(), vector.end());
            return zip(lazy_set<UKey>(std::move(distinct_values)));
        }

        // Performs the functional `zip` algorithm lazily where the lazy vector is materialized
        // and duplicates are removed when a terminal operation is called. The lazy vector must
        // contain the same number of distinct values as the set size.
        template <typename UKey>
        [[nodiscard]] lazy_set<std::pair<TKey, UKey>> zip(const lazy_vector<UKey>& vector) const
        {
            const auto previous = m_operation;
            return lazy_set<std::pair<TKey, UKey>>(
                [previous, vector](const std::function<void(const std::pair<TKey, UKey>&)>& consumer) {
                    const auto materialized_vector = vector.get();
                    std::set<UKey> distinct_values(materialized_vector.begin(), materialized_vector.end());
                    auto it = distinct_values.begin();
                    previous([&distinct_values, &it, &consumer](const TKey& key) {
                        assert(it != distinct_values.end());
                        consumer({key, *it});
                        ++it;
                    });
                    assert(it == distinct_values.end());
                });
        }

        // Performs the functional `zip` algorithm lazily, in which every key of the resulting
        // lazy set is a tuple of this instance's key (first) and the second lazy set's key (second).
        // The sizes of the two lazy sets must be equal.
        // The right-hand lazy set is materialized internally when a terminal operation is called.
        template <typename UKey, typename UCompare>
        [[nodiscard]] lazy_set<std::pair<TKey, UKey>> zip(const lazy_set<UKey, UCompare>& set) const
        {
            const auto previous = m_operation;
            return lazy_set<std::pair<TKey, UKey>>(
                [previous, set](const std::function<void(const std::pair<TKey, UKey>&)>& consumer) {
                    const auto materialized_set = set.get();
                    size_t index = 0;
                    previous([&materialized_set, &consumer, &index](const TKey& key) {
                        if (index >= materialized_set.size()) {
                            assert(false);
                            std::abort();
                        }
                        consumer({key, materialized_set[index]});
                        ++index;
                    });
                    if (index != materialized_set.size()) {
                        assert(false);
                        std::abort();
                    }
                });
        }

        // Performs the functional `reduce` (fold/accumulate) algorithm, by returning the result of
        // accumulating all the values in this lazy set to an initial value.
        //
        // example:
        //      const fcpp::set<int> numbers({ 1, 3, -5, 2, -1, 9, -4 });
        //      const auto sum = numbers
        //          .lazy()
        //          .filter([](const int& element) {
        //              return element > 0;
        //          })
        //          .reduce(0, [](const int& partial_sum, const int& number) {
        //              return partial_sum + number;
        //          });
        //
        // outcome:
        //      sum -> 15
#ifdef CPP17_AVAILABLE
        template <typename U, typename Reduce, typename = std::enable_if_t<std::is_invocable_r_v<U, Reduce, U, TKey>>>
#else
        template <typename U, typename Reduce>
#endif
        U reduce(const U& initial, Reduce&& reduction) const
        {
            auto result = initial;
            m_operation([&result, &reduction](const TKey& key) {
                result = reduction(result, key);
            });
            return result;
        }

        // Materializes this lazy set to a functional set, executing all stored operations.
        [[nodiscard]] set<TKey, TCompare> get() const;

    private:
        [[nodiscard]] static std::set<TKey, TCompare> materialize_with_compare(const lazy_set& other,
                                                                               const TCompare& compare)
        {
            const auto materialized_other = other.get();
            return std::set<TKey, TCompare>(materialized_other.begin(),
                                            materialized_other.end(),
                                            compare);
        }

        TCompare m_compare;
        std::function<void(const std::function<void(const TKey&)>&)> m_operation;
    };

    // A lightweight wrapper around std::set, enabling fluent and functional
    // programming on the set itself, rather than using the more procedural style
    // of the standard library algorithms.
    //
    // Member functions can be mutating (eg. my_set.insert()) or
    // non-mutating (eg. my_set.inserting()) enforcing thread safety if needed
    template <class TKey, class TCompare = std::less<TKey>>
    class set
    {
    public:
        set()
            : m_set()
        {
        }

        explicit set(std::set<TKey, TCompare> set)
            : m_set(std::move(set))
        {
        }

        explicit set(const std::vector<TKey>& vector)
            : m_set(vector.begin(), vector.end())
        {
        }

        explicit set(const vector<TKey>& vector)
            : m_set(vector.begin(), vector.end())
        {
        }

        explicit set(const std::initializer_list<TKey>& list)
            : m_set(list.begin(), list.end())
        {
        }

        // Returns the set of elements which belong to the current set but not in the other set.
        // In Venn diagram notation, if A is the current set and B is the other set, then
        // the difference is the operation A – B = {x : x ∈ A and x ∉ B}
        //
        // example:
        //      const fcpp::set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
        //      const fcpp::set<int> set2(std::set<int>({2, 5, 7, 10, 15, 17}));
        //      const auto diff = set1.difference_with(set2);
        //
        // outcome:
        //      diff -> fcpp::set<int>({1, 3, 8})
        [[nodiscard]] set difference_with(const set<TKey, TCompare>& other) const
        {
            std::set<TKey, TCompare> diff;
            std::set_difference(begin(),
                                end(),
                                other.begin(),
                                other.end(),
                                std::inserter(diff, diff.begin()));
            return set(diff);
        }

        [[nodiscard]] set difference_with(const std::set<TKey, TCompare>& other) const
        {
            return difference_with(set(other));
        }

        // Returns the set of elements which belong either to the current or the other set.
        // In Venn diagram notation, if A is the current set and B is the other set, then
        // the union is the operation A ∪ B = {x : x ∈ A or x ∈ B}
        //
        // example:
        //      const fcpp::set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
        //      const fcpp::set<int> set2(std::set<int>({2, 5, 7, 10, 15, 17}));
        //      const auto combined = set1.union_with(set2);
        //
        // outcome:
        //      combined -> fcpp::set<int>({1, 2, 3, 5, 7, 8, 10, 15, 17})
        [[nodiscard]] set union_with(const set<TKey, TCompare>& other) const
        {
            std::set<TKey, TCompare> combined;
            std::set_union(begin(),
                           end(),
                           other.begin(),
                           other.end(),
                           std::inserter(combined, combined.begin()));
            return set(combined);
        }

        [[nodiscard]] set union_with(const std::set<TKey, TCompare>& other) const
        {
            return union_with(set(other));
        }

        // Returns the set of elements which belong to both the current and the other set.
        // In Venn diagram notation, if A is the current set and B is the other set, then
        // the intersection is the operation A ∩ B = {x : x ∈ A and x ∈ B}
        //
        // example:
        //      const fcpp::set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
        //      const fcpp::set<int> set2(std::set<int>({2, 5, 7, 10, 15, 17}));
        //      const auto combined = set1.intersect_with(set2);
        //
        // outcome:
        //      combined -> fcpp::set<int>({2, 5, 7, 10})
        [[nodiscard]] set intersect_with(const set<TKey, TCompare>& other) const
        {
            std::set<TKey, TCompare> intersection;
            std::set_intersection(begin(),
                                  end(),
                                  other.begin(),
                                  other.end(),
                                  std::inserter(intersection, intersection.begin()));
            return set(intersection);
        }

        [[nodiscard]] set intersect_with(const std::set<TKey, TCompare>& other) const
        {
            return intersect_with(set(other));
        }

        // Returns the minimum key in the set, if it's not empty.
        //
        // example:
        //      const fcpp::set<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
        //      auto minimum = numbers.min();
        //
        //      // an empty set's minimum value
        //      fcpp::set<int>().min().has_value() // false
        //
        // outcome:
        //      minimum.has_value() -> true
        //      minimum.value() -> 1
        [[nodiscard]] fcpp::optional_t<TKey> min() const
        {
            const auto& it = std::min_element(begin(), end());
            if (it != end()) {
                return *it;
            }
            return fcpp::optional_t<TKey>();
        }

        // Returns the maximum key in the set, if it's not empty.
        //
        // example:
        //      const fcpp::set<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
        //      auto maximum = numbers.max();
        //
        //      // an empty set's maximum value
        //      fcpp::set<int>().max().has_value() // false
        //
        // outcome:
        //      maximum.has_value() -> true
        //      maximum.value() -> 8
        [[nodiscard]] fcpp::optional_t<TKey> max() const
        {
            const auto& it = std::max_element(begin(), end());
            if (it != end()) {
                return *it;
            }
            return fcpp::optional_t<TKey>();
        }

        // Performs the functional `map` algorithm, in which every element of the resulting set is the
        // output of applying the transform function on every element of this instance.
        //
        // example:
        //      const fcpp::set<int> input_set({ 1, 3, -5 });
        //      const auto output_set = input_set.map<std::string>([](const int& element) {
        //          return std::to_string(element);
        //      });
        //
        // outcome:
        //      output_set -> fcpp::set<std::string>({ "-5", "1", "3" })
        //
        // is equivalent to:
        //      const fcpp::set<int> input_set({ 1, 3, -5 });
        //      fcpp::set<std::string> output_set;
        //      for (auto const& key: input_set) {
        //          output_set.insert(std::to_string(key));
        //      }
#ifdef CPP17_AVAILABLE
        template <class UKey, class UCompare = std::less<UKey>, typename Transform, typename = std::enable_if_t<
                      std::is_invocable_r_v<UKey, Transform, TKey>>>
#else
        template <typename UKey, class UCompare = std::less<UKey>, typename Transform>
#endif
        set<UKey, UCompare> map(Transform&& transform) const
        {
            std::set<UKey, UCompare> transformed_set;
            for (const auto& key : m_set) {
                transformed_set.insert(transform(key));
            }
            return set<UKey, UCompare>(transformed_set);
        }

        // Returns true if all keys match the predicate (return true)
        //
        // example:
        //      const fcpp::set<int> numbers({1, 4, 2, 5, 8, 3});
        //
        //      // returns true
        //      numbers.all_of([](const int &number) {
        //          return number < 10;
        //      });
        //
        //      // returns false
        //      numbers.all_of([](const int &number) {
        //          return number > 2;
        //      });
#ifdef CPP17_AVAILABLE
        template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<bool, Callable, TKey>>>
#else
        template <typename Callable>
#endif
        bool all_of(Callable&& unary_predicate) const
        {
            return std::all_of(begin(),
                               end(),
                               std::forward<Callable>(unary_predicate));
        }

        // Returns true if at least one key matches the predicate (returns true)
        //
        // example:
        //      const fcpp::set<int> numbers({1, 4, 2, 5, 8, 3});
        //
        //      // returns true
        //      numbers.any_of([](const int &number) {
        //          return number < 5;
        //      });
        //
        //      // returns false
        //      numbers.any_of([](const int &number) {
        //          return number > 10;
        //      });
#ifdef CPP17_AVAILABLE
        template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<bool, Callable, TKey>>>
#else
        template <typename Callable>
#endif
        bool any_of(Callable&& unary_predicate) const
        {
            return std::any_of(begin(),
                               end(),
                               std::forward<Callable>(unary_predicate));
        }

        // Returns true if none of the keys match the predicate (all return false)
        //
        // example:
        //      const fcpp::set<int> numbers({1, 4, 2, 5, 8, 3});
        //
        //      // returns true
        //      numbers.none_of([](const int &number) {
        //          return number > 10;
        //      });
        //
        //      // returns false
        //      numbers.none_of([](const int &number) {
        //          return number < 6;
        //      });
#ifdef CPP17_AVAILABLE
        template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<bool, Callable, TKey>>>
#else
        template <typename Callable>
#endif
        bool none_of(Callable&& unary_predicate) const
        {
            return std::none_of(begin(),
                                end(),
                                std::forward<Callable>(unary_predicate));
        }

        // Performs the functional `reduce` (fold/accumulate) algorithm, by returning the result of
        // accumulating all the values in the set to an initial value. (non-mutating)
        //
        // example:
        //      const fcpp::set<std::string> tokens({ "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "brown", "dog" });
        //      const auto sentence = tokens.reduce<std::string>("", [](const std::string& partial, const std::string& token) {
        //          return partial.length() != 0
        //              ? partial + " " + token
        //              : token;
        //      });
        //
        // outcome: (a set does not allow multiple entries, and its elements are internally managed, order can vary)
        //
        //      sentence -> std::string("brown dog fox jumps lazy over quick the");
#ifdef CPP17_AVAILABLE
        template <typename U, typename Reduce, typename = std::enable_if_t<std::is_invocable_r_v<U, Reduce, U, TKey>>>
#else
        template <typename U, typename Reduce>
#endif
        U reduce(const U& initial, Reduce&& reduction) const
        {
            auto result = initial;
            for (const auto& x : m_set) {
                result = reduction(result, x);
            }
            return result;
        }

        // Performs the functional `filter` algorithm, in which all keys of this instance
        // which match the given predicate are kept (mutating)
        //
        // example:
        //      fcpp::set<int> numbers({ 1, 3, -5, 2, -1, 9, -4 });
        //      numbers.filter([](const int& element) {
        //          return element >= 1.5;
        //      });
        //
        // outcome:
        //      numbers -> fcpp::set<int>({ 2, 3, 9 });
        //
        // is equivalent to:
        //      fcpp::set<int> numbers({ 1, 3, -5, 2, -1, 9, -4 });
        //      for (auto i = 0; i < numbers.size(); ++i) {
        //          if (numbers[i] >= 1.5) {
        //              continue;
        //          }
        //          numbers.remove(i);
        //          i--;
        //      }
#ifdef CPP17_AVAILABLE
        template <typename Filter, typename = std::enable_if_t<std::is_invocable_r_v<bool, Filter, TKey>>>
#else
        template <typename Filter>
#endif
        set& filter(Filter&& predicate_to_keep)
        {
            std::set<TKey, TCompare> copy;
            auto it = begin();
            for (; it != end(); ++it) {
                if (predicate_to_keep(*it)) {
                    copy.insert(*it);
                }
            }
            m_set = std::move(copy);
            return *this;
        }

        // Performs the functional `filter` algorithm in a copy of this instance, in which all keys
        // of the copy which match the given predicate are kept (non-mutating)
        //
        // example:
        //      const fcpp::set<int> numbers({ 1, 3, -5, 2, -1, 9, -4 });
        //      auto filtered_numbers = numbers.filtered([](const int& element) {
        //          return element >= 1.5;
        //      });
        //
        // outcome:
        //      filtered_numbers -> fcpp::set<int>({ 2, 3, 9 });
        //      numbers -> fcpp::set<int>({ 1, 3, -5, 2, -1, 9, -4 });
#ifdef CPP17_AVAILABLE
        template <typename Filter, typename = std::enable_if_t<std::is_invocable_r_v<bool, Filter, TKey>>>
#else
        template <typename Filter>
#endif
        set filtered(Filter&& predicate_to_keep) const
        {
            std::set<TKey, TCompare> copy;
            auto it = begin();
            for (; it != end(); ++it) {
                if (predicate_to_keep(*it)) {
                    copy.insert(*it);
                }
            }
            return set(copy);
        }

#ifdef CPP17_AVAILABLE
        template <typename Iterator>
        using deref_type = typename std::iterator_traits<Iterator>::value_type;

        template <typename Iterator>
        struct is_valid_iterator
        {
            static bool const value = std::is_constructible_v<deref_type<Iterator>>;
        };
#endif

        // Performs the functional `zip` algorithm, in which every key of the resulting set is a
        // tuple of this instance's key (first) and the second set's key (second).
        // The sizes of the two sets must be equal.
        //
        // example:
        //      const fcpp::set<int> ages({ 25, 45, 30, 63 });
        //      const fcpp::set<std::string> persons({ "Jake", "Bob", "Michael", "Philipp" });
        //      const auto zipped = ages.zip(persons);
        //
        // outcome:
        //      zipped -> fcpp::set<std::pair<int, std::string>>({
        //                          std::pair<int, std::string>(25, "Bob"),
        //                          std::pair<int, std::string>(30, "Jake"),
        //                          std::pair<int, std::string>(45, "Michael"),
        //                          std::pair<int, std::string>(63, "Philipp"),
        //                       })
        template <typename UKey>
        [[nodiscard]] set<std::pair<TKey, UKey>> zip(const set<UKey>& set) const
        {
#ifdef CPP17_AVAILABLE
            return zip_impl(set.begin(), set.end());
#else
            return zip_impl<UKey>(set.begin(), set.end());
#endif
        }

        // Performs the functional `zip` algorithm.
        // The number of keys must match the set's size.
        // For more details, see the zip function which accepts a fcpp::set as input.
        template <typename UKey>
        [[nodiscard]] set<std::pair<TKey, UKey>> zip(const std::set<UKey>& set) const
        {
            return zip(fcpp::set<UKey>(set));
        }

        // Performs the functional `zip` algorithm where duplicates are removed before zipping.
        // The input vector must contain the same number of distinct values as the set size.
        // For more details, see the zip function which accepts a fcpp::set as input.
        template <typename UKey>
        [[nodiscard]] set<std::pair<TKey, UKey>> zip(const vector<UKey>& vector) const
        {
            const auto distinct_values = vector.distinct();
            return zip(distinct_values);
        }

        // Performs the functional `zip` algorithm where duplicates are removed before zipping.
        // The input vector must contain the same number of distinct values as the set size.
        // For more details, see the zip function which accepts a fcpp::set as input.
        template <typename UKey>
        [[nodiscard]] set<std::pair<TKey, UKey>> zip(const std::vector<UKey>& vector) const
        {
            return zip(fcpp::vector<UKey>(vector));
        }

        // Executes the given operation for each key of the set.
        // The operation must not change the set's contents during execution.
#ifdef CPP17_AVAILABLE
        template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<void, Callable, TKey const&>>>
#else
        template <typename Callable>
#endif
        const set& for_each(Callable&& operation) const
        {
            std::for_each(begin(),
                          end(),
                          std::forward<Callable>(operation));
            return *this;
        }

        // Returns a vector whose elements are the set's keys in sorted order.
        //
        // example:
        //      fcpp::set<int> numbers({1, 4, 2});
        //      const auto keys = numbers.keys();
        //
        // outcome:
        //      keys -> fcpp::vector<int>({1, 2, 4})
        vector<TKey> keys() const
        {
            vector<TKey> vec;
            vec.reserve(size());
            for_each([&vec](const TKey& key){
                vec.insert_back(key);
            });
            return std::move(vec);
        }

        // Removes an element from the set, if it exists, potentially changing the set's contents (mutating)
        //
        // example:
        //      fcpp::set<int> numbers({1, 4, 2});
        //      numbers.remove(4);
        //
        // outcome:
        //      numbers -> fcpp::set<int>({1, 2})
        set& remove(const TKey& element)
        {
            m_set.erase(element);
            return *this;
        }

        // Returns a copy by removing an element from the set, if it exists (non-mutating)
        //
        // example:
        //      const fcpp::set<int> numbers({1, 4, 2});
        //      auto less_numbers = numbers.removing(4);
        //
        // outcome:
        //      less_numbers -> fcpp::set<int>({1, 2})
        //      numbers -> fcpp::set<int>({1, 2, 4})
        [[nodiscard]] set removing(const TKey& element) const
        {
            auto copy(m_set);
            copy.erase(element);
            return set(copy);
        }

        // Inserts an element in the set, if it does not already exist, potentially changing the set's contents (mutating)
        //
        // example:
        //      fcpp::set<int> numbers({1, 4, 2});
        //      numbers.insert(18);
        //
        // outcome:
        //      numbers -> fcpp::set<int>({1, 2, 4, 18})
        set& insert(const TKey& element)
        {
            m_set.insert(element);
            return *this;
        }

        // Returns a copy by inserting an element in the set, if it does not already exist (non-mutating)
        //
        // example:
        //      const fcpp::set<int> numbers({1, 4, 2});
        //      auto augmented_numbers =  numbers.inserting(18);
        //
        // outcome:
        //      augmented_numbers -> fcpp::set<int>({1, 2, 4, 18})
        //      numbers -> fcpp::set<int>({1, 2, 4})
        [[nodiscard]] set inserting(const TKey& element) const
        {
            auto copy(m_set);
            copy.insert(element);
            return set(copy);
        }

        // Removes all keys from the set (mutating)
        //
        // example:
        //      fcpp::set<int> numbers({1, 4, 2});
        //      numbers.clear();
        //
        // outcome:
        //      numbers -> fcpp::set<int>({})
        set& clear()
        {
            m_set.clear();
            return *this;
        }

        // Returns a new set by clearing all keys from the current set (non-mutating)
        //
        // example:
        //      const fcpp::set<int> numbers({1, 4, 2});
        //      auto cleared_numbers = numbers.clearing();
        //
        // outcome:
        //      cleared_numbers -> fcpp::set<int>({})
        //      numbers -> fcpp::set<int>({1, 4, 2})
        [[nodiscard]] set clearing() const
        {
            return set();
        }

        // Returns true if the set is empty
        //
        // example:
        //      const fcpp::set<int> numbers({1, 4, 2});
        //
        //      // returns false
        //      numbers.is_empty();
        //
        //      // returns true
        //      fcpp::set<int>().is_empty();
        [[nodiscard]] bool is_empty() const
        {
            return m_set.empty();
        }

        // Returns true if the key is present in the set, otherwise false
        //
        // example:
        //      const fcpp::set<int> numbers({1, 4, 2});
        //      numbers.contains(1); // true
        //      numbers.contains(15); // false
        [[nodiscard]] bool contains(const TKey& key) const
        {
            return m_set.count(key) != 0;
        }

        // Returns the number of elements in the set
        [[nodiscard]] size_t size() const
        {
            return m_set.size();
        }

        // Returns a copy of the comparator used to order this set's keys.
        [[nodiscard]] TCompare key_comp() const
        {
            return m_set.key_comp();
        }

        // Starts a lazy pipeline. The returned lazy set defers following map/filter/zip
        // transformations until a terminal operation, such as get() or reduce(), is called.
        [[nodiscard]] lazy_set<TKey, TCompare> lazy() const
        {
            return lazy_set<TKey, TCompare>(m_set);
        }

        // Returns the begin iterator, useful for other standard library algorithms
        [[nodiscard]] typename std::set<TKey>::iterator begin()
        {
            return m_set.begin();
        }

        // Returns the const begin iterator, useful for other standard library algorithms
        [[nodiscard]] typename std::set<TKey>::const_iterator begin() const
        {
            return m_set.begin();
        }

        // Returns the end iterator, useful for other standard library algorithms
        [[nodiscard]] typename std::set<TKey>::iterator end()
        {
            return m_set.end();
        }

        // Returns the const end iterator, useful for other standard library algorithms
        [[nodiscard]] typename std::set<TKey>::const_iterator end() const
        {
            return m_set.end();
        }

        // Returns a copy of the key at the given sorted position.
        // Bounds checking (assert) is enabled for debug builds.
        // Performance is O(n), so be careful for performance critical code sections.
        TKey operator[](size_t index)
        {
            assert_smaller_size(index);
#ifdef CPP17_AVAILABLE
            auto it = std::advance(begin(), index);
            return *it;
#else
            auto count = 0;
            auto it = begin();
            while (count++ < index) {
                ++it;
            }
            return *it;
#endif
        }

        // Returns a copy of the key at the given sorted position.
        // Bounds checking (assert) is enabled for debug builds.
        // Performance is O(n), so be careful for performance critical code sections.
        TKey operator[](size_t index) const
        {
            assert_smaller_size(index);
#ifdef CPP17_AVAILABLE
            auto it = begin();
            std::advance(it, index);
            return *it;
#else
            auto count = 0;
            auto it = begin();
            while (count++ < index) {
                ++it;
            }
            return *it;
#endif
        }

        // Returns true if both instances have equal sizes and the corresponding elements (keys) are equal
        bool operator ==(const set<TKey, TCompare>& rhs) const
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

            auto it1 = begin();
            auto it2 = rhs.begin();
            while (it1 != end() && it2 != rhs.end()) {
                if (!(*it1 == *it2)) {
                    return false;
                }
                ++it1;
                ++it2;
            }

            return true;
#endif
        }

        // Returns false if either the sizes are not equal or at least one corresponding element (key) is not equal
        bool operator !=(const set<TKey, TCompare>& rhs) const
        {
            return !((*this) == rhs);
        }

    private:
        std::set<TKey, TCompare> m_set;

        void assert_smaller_size(const size_t index) const
        {
            assert(index < size());
        }

#ifdef CPP17_AVAILABLE
        template <typename Iterator, typename = std::enable_if_t<is_valid_iterator<Iterator>::value>>
        [[nodiscard]] auto zip_impl(const Iterator& set_begin, const Iterator& set_end) const ->
            set<std::pair<TKey, deref_type<Iterator>>>
        {
            using UKey = deref_type<Iterator>;
#else
        template <typename UKey>
        [[nodiscard]] set<std::pair<TKey, UKey>> zip_impl(const typename std::set<UKey>::const_iterator& set_begin,
                                                          const typename std::set<UKey>::const_iterator& set_end) const
        {
#endif
            const auto vec_size = std::distance(set_begin, set_end);
            assert(size() == vec_size);
            std::set<std::pair<TKey, UKey>> combined_set;
            auto it1 = begin();
            auto it2 = set_begin;
            for (; it1 != end() && it2 != set_end; ++it1, ++it2) {
                combined_set.insert({*it1, *it2});
            }
            return set<std::pair<TKey, UKey>>(combined_set);
        }
    };

    template <class TKey, class TCompare>
    [[nodiscard]] set<TKey, TCompare> lazy_set<TKey, TCompare>::get() const
    {
        std::set<TKey, TCompare> materialized(m_compare);
        m_operation([&materialized](const TKey& key) {
            materialized.insert(key);
        });
        return set<TKey, TCompare>(std::move(materialized));
    }
}
