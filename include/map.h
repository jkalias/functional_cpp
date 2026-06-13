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
#include <functional>
#include <initializer_list>
#include <map>
#include <memory>
#include <type_traits>
#include <utility>
#include "vector.h"

namespace fcpp {

template <class TKey, class TValue, class TCompare>
class map;

// A lightweight wrapper representing a deferred map pipeline, enabling fluent and functional
// programming while avoiding intermediate map materialization.
//
// Member functions are non-mutating and keep extending the pipeline. Terminal functions such as
// `get` and `reduce` execute the stored operations.
template <class TKey, class TValue, class TCompare = std::less<TKey>>
class lazy_map
{
public:
    using value_type = std::pair<const TKey, TValue>;

    lazy_map()
        : m_compare()
        , m_operation([](const std::function<void(const value_type&)>&) {})
    {
    }

    // Creates a lazy map by copying the provided std::map as an owned source.
    explicit lazy_map(const std::map<TKey, TValue, TCompare>& map)
        : m_compare(map.key_comp())
    {
        auto source = std::make_shared<std::map<TKey, TValue, TCompare>>(map);
        m_operation = [source](const std::function<void(const value_type&)>& consumer) {
            std::for_each(source->begin(), source->end(), consumer);
        };
    }

    // Creates a lazy map by moving the provided std::map as an owned source.
    explicit lazy_map(std::map<TKey, TValue, TCompare>&& map)
        : m_compare(map.key_comp())
    {
        auto source = std::make_shared<std::map<TKey, TValue, TCompare>>(std::move(map));
        m_operation = [source](const std::function<void(const value_type&)>& consumer) {
            std::for_each(source->begin(), source->end(), consumer);
        };
    }

    // Creates a lazy map by directly providing the deferred operation.
    // This constructor is mostly useful for composing lazy_map instances.
    explicit lazy_map(std::function<void(const std::function<void(const value_type&)>&)> operation)
        : m_compare()
        , m_operation(std::move(operation))
    {
    }

    // Creates a lazy map by directly providing the deferred operation and comparator.
    // This constructor is mostly useful for preserving comparator state while composing
    // lazy_map instances.
    lazy_map(std::function<void(const std::function<void(const value_type&)>&)> operation,
             const TCompare& compare)
        : m_compare(compare)
        , m_operation(std::move(operation))
    {
    }

    // Performs the functional `map_to` algorithm lazily. The transform is not applied until
    // a terminal operation, such as `get` or `reduce`, is called.
    //
    // example:
    //      const fcpp::map<std::string, int> ages({{"jake", 32}, {"mary", 26}, {"david", 40}});
    //      const auto labels_by_initial = ages
    //          .lazy()
    //          .map_to<char, std::string>([](const auto& element) {
    //              return std::make_pair(element.first[0], std::to_string(element.second) + " years");
    //          })
    //          .get();
    //
    // outcome:
    //      labels_by_initial -> fcpp::map<char, std::string>({
    //          {'d', "40 years"}, {'j', "32 years"}, {'m', "26 years"}
    //      })
#ifdef CPP17_AVAILABLE
    template <typename UKey, typename UValue, typename Transform, typename = std::enable_if_t<std::is_invocable_r_v<std::pair<UKey, UValue>, Transform, value_type>>>
#else
    template <typename UKey, typename UValue, typename Transform>
#endif
    [[nodiscard]] lazy_map<UKey, UValue> map_to(Transform&& transform) const
    {
        const auto previous = m_operation;
        typename std::decay<Transform>::type transform_copy(std::forward<Transform>(transform));
        return lazy_map<UKey, UValue>(
            [previous, transform_copy](const std::function<void(const typename lazy_map<UKey, UValue>::value_type&)>& consumer) mutable {
                previous([&consumer, &transform_copy](const value_type& element) {
                    const auto transformed = transform_copy(element);
                    const typename lazy_map<UKey, UValue>::value_type transformed_element(transformed.first, transformed.second);
                    consumer(transformed_element);
                });
            });
    }

    // Performs the functional `filter` algorithm lazily, in which all key/value pairs which match
    // the given predicate are kept. The predicate is not applied until a terminal operation,
    // such as `get` or `reduce`, is called.
    //
    // example:
    //      const fcpp::map<std::string, int> ages({{"jake", 32}, {"mary", 26}, {"david", 40}});
    //      const auto adults = ages
    //          .lazy()
    //          .filter([](const auto& element) {
    //              return element.second >= 32;
    //          })
    //          .get();
    //
    // outcome:
    //      adults -> fcpp::map<std::string, int>({{"david", 40}, {"jake", 32}})
#ifdef CPP17_AVAILABLE
    template <typename Filter, typename = std::enable_if_t<std::is_invocable_r_v<bool, Filter, value_type>>>
#else
    template <typename Filter>
#endif
    [[nodiscard]] lazy_map filter(Filter&& predicate_to_keep) const
    {
        const auto previous = m_operation;
        typename std::decay<Filter>::type predicate_copy(std::forward<Filter>(predicate_to_keep));
        return lazy_map(
            [previous, predicate_copy](const std::function<void(const value_type&)>& consumer) mutable {
                previous([&consumer, &predicate_copy](const value_type& element) {
                    if (predicate_copy(element)) {
                        consumer(element);
                    }
                });
            },
            m_compare);
    }

    // Performs the functional `filter` algorithm lazily.
    // See also `filter` for more documentation.
#ifdef CPP17_AVAILABLE
    template <typename Filter, typename = std::enable_if_t<std::is_invocable_r_v<bool, Filter, value_type>>>
#else
    template <typename Filter>
#endif
    [[nodiscard]] lazy_map filtered(Filter&& predicate_to_keep) const
    {
        return filter(std::forward<Filter>(predicate_to_keep));
    }

    // Performs the functional `reduce` (fold/accumulate) algorithm, by returning the result of
    // accumulating all key/value pairs in this lazy map to an initial value.
    //
    // example:
    //      const fcpp::map<std::string, int> ages({{"jake", 32}, {"mary", 26}, {"david", 40}});
    //      const auto total_age = ages
    //          .lazy()
    //          .filter([](const auto& element) {
    //              return element.second >= 32;
    //          })
    //          .reduce(0, [](const int& partial_sum, const auto& element) {
    //              return partial_sum + element.second;
    //          });
    //
    // outcome:
    //      total_age -> 72
#ifdef CPP17_AVAILABLE
    template <typename U, typename Reduce, typename = std::enable_if_t<std::is_invocable_r_v<U, Reduce, U, value_type>>>
#else
    template <typename U, typename Reduce>
#endif
    U reduce(const U& initial, Reduce&& reduction) const
    {
        auto result = initial;
        m_operation([&result, &reduction](const value_type& element) {
            result = reduction(result, element);
        });
        return result;
    }

    // Materializes this lazy map to a functional map, executing all stored operations.
    [[nodiscard]] map<TKey, TValue, TCompare> get() const;

private:
    TCompare m_compare;
    std::function<void(const std::function<void(const value_type&)>&)> m_operation;
};

// A lightweight wrapper around std::map, enabling fluent and functional
// programming on the map itself, rather than using the more procedural style
// of the standard library algorithms.
//
// Member functions can be mutating (eg. my_map.remove("name")) or
// non-mutating (eg. my_map.removing("name")) enforcing thread safety if needed
template <class TKey, class TValue, class TCompare = std::less<TKey>>
class map
{
public:
    using value_type = std::pair<const TKey, TValue>;

    map()
    : m_map()
    {
    }

    explicit map(std::map<TKey, TValue, TCompare> map)
    : m_map(std::move(map))
    {
    }

    explicit map(const std::initializer_list<std::pair<TKey, TValue>>& list)
    : m_map(list.begin(), list.end())
    {
    }

    // Performs the functional `map` algorithm from one map to another map, in which every key/value
    // pair of the resulting map is the output of applying the transform function on every key/value
    // pair of this instance. If two source elements produce equivalent result keys, the first
    // pair encountered during map iteration is kept, following std::map::insert semantics.
    //
    // example:
    //      const fcpp::map<std::string, int> ages({{"jake", 32}, {"mary", 26}, {"david", 40}});
    //      const auto labels_by_initial = ages.map_to<char, std::string>([](const auto& element) {
    //          return std::make_pair(element.first[0], std::to_string(element.second) + " years");
    //      });
    //
    // outcome:
    //      labels_by_initial -> fcpp::map<char, std::string>({
    //          {'d', "40 years"}, {'j', "32 years"}, {'m', "26 years"}
    //      })
    //
    // is equivalent to:
    //      const fcpp::map<std::string, int> ages({{"jake", 32}, {"mary", 26}, {"david", 40}});
    //      fcpp::map<char, std::string> labels_by_initial;
    //      for (const auto& element : ages) {
    //          labels_by_initial.insert(element.first[0], std::to_string(element.second) + " years");
    //      }
#ifdef CPP17_AVAILABLE
    template <typename UKey, typename UValue, typename Transform, typename = std::enable_if_t<std::is_invocable_r_v<std::pair<UKey, UValue>, Transform, value_type>>>
#else
    template <typename UKey, typename UValue, typename Transform>
#endif
    [[nodiscard]] fcpp::map<UKey, UValue> map_to(Transform && transform) const
    {
        fcpp::map<UKey, UValue> transformed;
        for (const auto& element : m_map) {
            const auto transformed_element = transform(element);
            transformed.insert(transformed_element.first, transformed_element.second);
        }
        return transformed;
    }

    // Returns true if all key/value pairs match the predicate (return true)
    //
    // example:
    //      const fcpp::map<std::string, int> ages({{"jake", 32}, {"mary", 26}, {"david", 40}});
    //
    //      // returns true
    //      ages.all_of([](const auto& element) {
    //          return element.second > 20;
    //      });
    //
    //      // returns false
    //      ages.all_of([](const auto& element) {
    //          return element.second < 35;
    //      });
#ifdef CPP17_AVAILABLE
    template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<bool, Callable, value_type>>>
#else
    template <typename Callable>
#endif
    bool all_of(Callable && unary_predicate) const
    {
        return std::all_of(begin(),
                           end(),
                           std::forward<Callable>(unary_predicate));
    }

    // Returns true if at least one key/value pair matches the predicate (returns true)
    //
    // example:
    //      const fcpp::map<std::string, int> ages({{"jake", 32}, {"mary", 26}, {"david", 40}});
    //
    //      // returns true
    //      ages.any_of([](const auto& element) {
    //          return element.second == 40;
    //      });
    //
    //      // returns false
    //      ages.any_of([](const auto& element) {
    //          return element.second > 50;
    //      });
#ifdef CPP17_AVAILABLE
    template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<bool, Callable, value_type>>>
#else
    template <typename Callable>
#endif
    bool any_of(Callable && unary_predicate) const
    {
        return std::any_of(begin(),
                           end(),
                           std::forward<Callable>(unary_predicate));
    }

    // Returns true if no key/value pair matches the predicate (all return false)
    //
    // example:
    //      const fcpp::map<std::string, int> ages({{"jake", 32}, {"mary", 26}, {"david", 40}});
    //
    //      // returns true
    //      ages.none_of([](const auto& element) {
    //          return element.second < 18;
    //      });
    //
    //      // returns false
    //      ages.none_of([](const auto& element) {
    //          return element.second == 26;
    //      });
#ifdef CPP17_AVAILABLE
    template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<bool, Callable, value_type>>>
#else
    template <typename Callable>
#endif
    bool none_of(Callable && unary_predicate) const
    {
        return std::none_of(begin(),
                            end(),
                            std::forward<Callable>(unary_predicate));
    }

    // Performs the functional `reduce` (fold/accumulate) algorithm, by returning the result of
    // accumulating all key/value pairs in the map to an initial value. (non-mutating)
    //
    // example:
    //      const fcpp::map<std::string, int> ages({{"jake", 32}, {"mary", 26}, {"david", 40}});
    //      const auto total_age = ages.reduce(0, [](const int& partial_sum, const auto& element) {
    //          return partial_sum + element.second;
    //      });
    //
    // outcome:
    //      total_age -> 98
#ifdef CPP17_AVAILABLE
    template <typename U, typename Reduce, typename = std::enable_if_t<std::is_invocable_r_v<U, Reduce, U, value_type>>>
#else
    template <typename U, typename Reduce>
#endif
    U reduce(const U& initial, Reduce&& reduction) const
    {
        auto result = initial;
        for (const auto& x : m_map)
        {
            result = reduction(result, x);
        }
        return result;
    }

    // Performs the functional `filter` algorithm, in which all key/value pairs of this instance
    // which match the given predicate are kept (mutating)
    //
    // example:
    //      fcpp::map<std::string, int> ages({{"jake", 32}, {"mary", 26}, {"david", 40}});
    //      ages.filter([](const auto& element) {
    //          return element.second >= 32;
    //      });
    //
    // outcome:
    //      ages -> fcpp::map<std::string, int>({{"david", 40}, {"jake", 32}})
    //
    // is equivalent to:
    //      fcpp::map<std::string, int> ages({{"jake", 32}, {"mary", 26}, {"david", 40}});
    //      auto it = ages.begin();
    //      while (it != ages.end()) {
    //          if (it->second >= 32) {
    //              ++it;
    //          } else {
    //              const auto key = it->first;
    //              ++it;
    //              ages.remove(key);
    //          }
    //      }
#ifdef CPP17_AVAILABLE
    template <typename Filter, typename = std::enable_if_t<std::is_invocable_r_v<bool, Filter, value_type>>>
#else
    template <typename Filter>
#endif
    map& filter(Filter && predicate_to_keep)
    {
        auto it = begin();
        while (it != end()) {
            if (!predicate_to_keep(*it)) {
                it = m_map.erase(it);
            } else {
                ++it;
            }
        }
        return *this;
    }

    // Performs the functional `filter` algorithm in a copy of this instance, in which all key/value
    // pairs of the copy which match the given predicate are kept (non-mutating)
    //
    // example:
    //      const fcpp::map<std::string, int> ages({{"jake", 32}, {"mary", 26}, {"david", 40}});
    //      const auto adults = ages.filtered([](const auto& element) {
    //          return element.second >= 32;
    //      });
    //
    // outcome:
    //      ages -> fcpp::map<std::string, int>({{"david", 40}, {"jake", 32}, {"mary", 26}})
    //      adults -> fcpp::map<std::string, int>({{"david", 40}, {"jake", 32}})
#ifdef CPP17_AVAILABLE
    template <typename Filter, typename = std::enable_if_t<std::is_invocable_r_v<bool, Filter, value_type>>>
#else
    template <typename Filter>
#endif
    [[nodiscard]] map filtered(Filter && predicate_to_keep) const
    {
        auto copy(m_map);
        auto it = copy.begin();
        while (it != copy.end()) {
            if (!predicate_to_keep(*it)) {
                it = copy.erase(it);
            } else {
                ++it;
            }
        }
        return map(copy);
    }

    // Executes the given operation for each key/value pair in the map. The operation must not
    // change the map's contents during execution.
    //
    // example:
    //      const fcpp::map<std::string, int> ages({{"jake", 32}, {"mary", 26}, {"david", 40}});
    //      int total_age = 0;
    //      ages.for_each([&total_age](const auto& element) {
    //          total_age += element.second;
    //      });
    //
    // outcome:
    //      total_age -> 98
#ifdef CPP17_AVAILABLE
    template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<void, Callable, value_type>>>
#else
    template <typename Callable>
#endif
    const map& for_each(Callable && operation) const
    {
        std::for_each(begin(),
                      end(),
                      std::forward<Callable>(operation));
        return *this;
    }

    // Returns a vector whose elements are the map's keys in sorted key order.
    //
    // example:
    //      const fcpp::map<std::string, int> ages({{"jake", 32}, {"mary", 26}, {"david", 40}});
    //      const auto names = ages.keys();
    //
    // outcome:
    //      names -> fcpp::vector<std::string>({"david", "jake", "mary"})
    vector<TKey> keys() const
    {
        vector<TKey> vec;
        vec.reserve(size());
        for_each([&vec](const value_type& element) {
            vec.insert_back(element.first);
        });
        return vec;
    }

    // Returns a vector whose elements are the map's values in sorted key order.
    //
    // example:
    //      const fcpp::map<std::string, int> ages({{"jake", 32}, {"mary", 26}, {"david", 40}});
    //      const auto years = ages.values();
    //
    // outcome:
    //      years -> fcpp::vector<int>({40, 32, 26})
    vector<TValue> values() const
    {
        vector<TValue> vec;
        vec.reserve(size());
        for_each([&vec](const value_type& element) {
            vec.insert_back(element.second);
        });
        return vec;
    }

    // Removes the element with the given key in place (mutating)
    //
    // example:
    //      fcpp::map<std::string, int> ages({{"jake", 32}, {"mary", 26}, {"david", 40}});
    //      ages.remove("mary");
    //
    // outcome:
    //      ages -> fcpp::map<std::string, int>({{"david", 40}, {"jake", 32}})
    map& remove(const TKey& key)
    {
        m_map.erase(key);
        return *this;
    }

    // Returns a copy of this instance without the element with the given key (non-mutating)
    //
    // example:
    //      const fcpp::map<std::string, int> ages({{"jake", 32}, {"mary", 26}, {"david", 40}});
    //      const auto adults = ages.removing("mary");
    //
    // outcome:
    //      ages -> fcpp::map<std::string, int>({{"david", 40}, {"jake", 32}, {"mary", 26}})
    //      adults -> fcpp::map<std::string, int>({{"david", 40}, {"jake", 32}})
    [[nodiscard]] map removing(const TKey& key) const
    {
        auto copy(m_map);
        copy.erase(key);
        return map(copy);
    }

    // Inserts a key/value pair in place (mutating). If an equivalent key already exists, the
    // existing mapped value is left unchanged, following std::map::insert semantics.
    //
    // example:
    //      fcpp::map<std::string, int> ages({{"jake", 32}, {"mary", 26}});
    //      ages.insert("david", 40);
    //
    // outcome:
    //      ages -> fcpp::map<std::string, int>({{"david", 40}, {"jake", 32}, {"mary", 26}})
    map& insert(const TKey& key, const TValue& value)
    {
        m_map.insert(std::make_pair(key, value));
        return *this;
    }

    // Inserts a key/value pair in place (mutating). If an equivalent key already exists, the
    // existing mapped value is left unchanged, following std::map::insert semantics.
    map& insert(const value_type& element)
    {
        m_map.insert(element);
        return *this;
    }

    // Returns a copy of this instance with the key/value pair inserted (non-mutating). If an
    // equivalent key already exists, the existing mapped value is left unchanged in the returned copy.
    //
    // example:
    //      const fcpp::map<std::string, int> ages({{"jake", 32}, {"mary", 26}});
    //      const auto more_ages = ages.inserting("david", 40);
    //
    // outcome:
    //      ages -> fcpp::map<std::string, int>({{"jake", 32}, {"mary", 26}})
    //      more_ages -> fcpp::map<std::string, int>({{"david", 40}, {"jake", 32}, {"mary", 26}})
    [[nodiscard]] map inserting(const TKey& key, const TValue& value) const
    {
        auto copy(m_map);
        copy.insert(std::make_pair(key, value));
        return map(copy);
    }

    // Returns a copy of this instance with the key/value pair inserted (non-mutating). If an
    // equivalent key already exists, the existing mapped value is left unchanged in the returned copy.
    [[nodiscard]] map inserting(const value_type& element) const
    {
        auto copy(m_map);
        copy.insert(element);
        return map(copy);
    }

    // Removes all key/value pairs from the map (mutating)
    map& clear()
    {
        m_map.clear();
        return *this;
    }

    // Returns true if the map is empty
    [[nodiscard]] bool is_empty() const
    {
        return m_map.empty();
    }

    // Returns true if the key is present in the map, otherwise false
    [[nodiscard]] bool contains(const TKey& key) const
    {
        return m_map.find(key) != end();
    }

    // Returns the number of elements in the map
    [[nodiscard]] size_t size() const
    {
        return m_map.size();
    }

    // Starts a lazy pipeline. The returned lazy map defers following map_to/filter transformations
    // until a terminal operation, such as get() or reduce(), is called.
    [[nodiscard]] lazy_map<TKey, TValue, TCompare> lazy() const
    {
        return lazy_map<TKey, TValue, TCompare>(m_map);
    }

    // Returns the begin iterator, useful for other standard library algorithms
    [[nodiscard]] typename std::map<TKey, TValue, TCompare>::iterator begin()
    {
        return m_map.begin();
    }

    // Returns the const begin iterator, useful for other standard library algorithms
    [[nodiscard]] typename std::map<TKey, TValue, TCompare>::const_iterator begin() const
    {
        return m_map.begin();
    }

    // Returns the end iterator, useful for other standard library algorithms
    [[nodiscard]] typename std::map<TKey, TValue, TCompare>::iterator end()
    {
        return m_map.end();
    }

    // Returns the const end iterator, useful for other standard library algorithms
    [[nodiscard]] typename std::map<TKey, TValue, TCompare>::const_iterator end() const
    {
        return m_map.end();
    }

    // Returns a reference to the value that is mapped to a key,
    // assuming that such key already exists.
    const TValue& operator[](const TKey& key) const
    {
        const auto it = m_map.find(key);
        FCPP_PRECONDITION(it != end());
        return (*it).second;
    }

    // Returns a reference to the value that is mapped to a key,
    // performing an insertion if such key does not already exist.
    TValue& operator[](const TKey& key)
    {
        return m_map[key];
    }

    bool operator ==(const map<TKey, TValue, TCompare>& rhs) const
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
            it1++;
            it2++;
        }

        return true;
#endif
    }

    bool operator !=(const map<TKey, TValue, TCompare>& rhs) const
    {
        return !((*this) == rhs);
    }

private:
    std::map<TKey, TValue, TCompare> m_map;
};

template <class TKey, class TValue, class TCompare>
[[nodiscard]] map<TKey, TValue, TCompare> lazy_map<TKey, TValue, TCompare>::get() const
{
    std::map<TKey, TValue, TCompare> materialized(m_compare);
    m_operation([&materialized](const value_type& element) {
        materialized.insert(element);
    });
    return map<TKey, TValue, TCompare>(std::move(materialized));
}

}
