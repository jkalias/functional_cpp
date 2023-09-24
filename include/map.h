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
#include <map>
#include "optional.h"

namespace fcpp {

template <typename T>
class vector;

// A lightweight wrapper around std::map, enabling fluent and functional
// programming on the map itself, rather than using the more procedural style
// of the standard library algorithms.
template <class TKey, class TValue, class TCompare = std::less<TKey>>
class map
{
public:
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
    
//    // Returns the set of elements which belong to the current set but not in the other set.
//    // In Venn diagram notation, if A is the current set and B is the other set, then
//    // the difference is the operation A – B = {x : x ∈ A and x ∉ B}
//    //
//    // example:
//    //      const fcpp::set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
//    //      const fcpp::set<int> set2(std::set<int>({2, 5, 7, 10, 15, 17}));
//    //      const auto& diff = set1.difference(set2);
//    //
//    // outcome:
//    //      diff -> fcpp::set<int>({1, 3, 8})
//    [[nodiscard]] set difference_with(const set<TKey, TCompare>& other) const {
//        std::set<TKey, TCompare> diff;
//        std::set_difference(begin(),
//                            end(),
//                            other.begin(),
//                            other.end(),
//                            std::inserter(diff, diff.begin()));
//        return set(diff);
//    }
//
//    [[nodiscard]] set difference_with(const std::set<TKey, TCompare>& other) const {
//        return difference_with(set(other));
//    }
//    
//    // Returns the set of elements which belong either to the current or the other set.
//    // In Venn diagram notation, if A is the current set and B is the other set, then
//    // the union is the operation A ∪ B = {x : x ∈ A or x ∈ B}
//    //
//    // example:
//    //      const fcpp::set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
//    //      const fcpp::set<int> set2(std::set<int>({2, 5, 7, 10, 15, 17}));
//    //      const auto& combined = set1.set_union(set2);
//    //
//    // outcome:
//    //      combined -> fcpp::set<int>({1, 2, 3, 5, 7, 8, 10, 15, 17})
//    [[nodiscard]] set union_with(const set<TKey, TCompare>& other) const {
//        std::set<TKey, TCompare> combined;
//        std::set_union(begin(),
//                       end(),
//                       other.begin(),
//                       other.end(),
//                       std::inserter(combined, combined.begin()));
//        return set(combined);
//    }
//
//    [[nodiscard]] set union_with(const std::set<TKey, TCompare>& other) const {
//        return union_with(set(other));
//    }
//    
//    // Returns the set of elements which belong to both the current and the other set.
//    // In Venn diagram notation, if A is the current set and B is the other set, then
//    // the intersection is the operation A ∩ B = {x : x ∈ A and x ∈ B}
//    //
//    // example:
//    //      const fcpp::set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
//    //      const fcpp::set<int> set2(std::set<int>({2, 5, 7, 10, 15, 17}));
//    //      const auto& combined = set1.set_union(set2);
//    //
//    // outcome:
//    //      combined -> fcpp::set<int>({2, 5, 7, 10})
//    [[nodiscard]] set intersect_with(const set<TKey, TCompare>& other) const {
//        std::set<TKey, TCompare> intersection;
//        std::set_intersection(begin(),
//                              end(),
//                              other.begin(),
//                              other.end(),
//                              std::inserter(intersection, intersection.begin()));
//        return set(intersection);
//    }
//
//    [[nodiscard]] set intersect_with(const std::set<TKey, TCompare>& other) const {
//        return intersect_with(set(other));
//    }
//    
//    // Returns the minimum key in the set, if it's not empty.
//    //
//    // example:
//    //      const fcpp::set<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
//    //      auto minimum = numbers.min();
//    //
//    //      // an empty's set minimum value
//    //      fcpp::set<int>().min().has_value() // false
//    //
//    // outcome:
//    //      minimum.has_value() -> true
//    //      minimum.value() -> 1
//    [[nodiscard]] fcpp::optional_t<TKey> min() const {
//        const auto& it = std::min_element(begin(), end());
//        if (it != end()) {
//            return *it;
//        }
//        return fcpp::optional_t<TKey>();
//    }
//    
//    // Returns the maximum key in the set, if it's not empty.
//    //
//    // example:
//    //      const fcpp::set<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
//    //      auto maximum = numbers.max();
//    //
//    //      // an empty's set maximum value
//    //      fcpp::set<int>().max().has_value() // false
//    //
//    // outcome:
//    //      maximum.has_value() -> true
//    //      maximum.value() -> 8
//    [[nodiscard]] fcpp::optional_t<TKey> max() const {
//        const auto& it = std::max_element(begin(), end());
//        if (it != end()) {
//            return *it;
//        }
//        return fcpp::optional_t<TKey>();
//    }
//    
//    // Performs the functional `map` algorithm, in which every element of the resulting set is the
//    // output of applying the transform function on every element of this instance.
//    //
//    // example:
//    //      const fcpp::vector<int> input_set({ 1, 3, -5 });
//    //      const auto output_set = input_set.map<std::string>([](const int& element) {
//    //          return std::to_string(element);
//    //      });
//    //
//    // outcome:
//    //      output_set -> fcpp::set<std::string>({ "-5", "1", "3" })
//    //
//    // is equivalent to:
//    //      const fcpp::set<int> input_set({ 1, 3, -5 });
//    //      fcpp::set<std::string> output_set;
//    //      for (auto const& key: input_set) {
//    //          output_set.insert(std::to_string(key));
//    //      }
//#ifdef CPP17_AVAILABLE
//    template <class UKey, class UCompare = std::less<UKey>, typename Transform, typename = std::enable_if_t<std::is_invocable_r_v<UKey, Transform, TKey>>>
//#else
//    template <typename UKey, class UCompare = std::less<UKey>, typename Transform>
//#endif
//    set<UKey, UCompare> map(Transform && transform) const
//    {
//        std::set<UKey, UCompare> transformed_set;
//        for (const auto& key: m_map) {
//            transformed_set.insert(transform(key));
//        }
//        return set<UKey, UCompare>(transformed_set);
//    }
//    
//    // Returns true if all keys match the predicate (return true)
//    //
//    // example:
//    //      const fcpp::set<int> numbers({1, 4, 2, 5, 8, 3});
//    //
//    //      // returns true
//    //      numbers.all_of([](const int &number) {
//    //          return number < 10;
//    //      });
//    //
//    //      // returns false
//    //      numbers.all_of([](const int &number) {
//    //          return number > 2;
//    //      });
//#ifdef CPP17_AVAILABLE
//    template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<bool, Callable, TKey>>>
//#else
//    template <typename Callable>
//#endif
//    bool all_of(Callable && unary_predicate) const
//    {
//        return std::all_of(begin(),
//                           end(),
//                           std::forward<Callable>(unary_predicate));
//    }
//    
//    // Returns true if at least one key match the predicate (returns true)
//    //
//    // example:
//    //      const fcpp::set<int> numbers({1, 4, 2, 5, 8, 3});
//    //
//    //      // returns true
//    //      numbers.any_of([](const int &number) {
//    //          return number < 5;
//    //      });
//    //
//    //      // returns false
//    //      numbers.any_of([](const int &number) {
//    //          return number > 10;
//    //      });
//#ifdef CPP17_AVAILABLE
//    template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<bool, Callable, TKey>>>
//#else
//    template <typename Callable>
//#endif
//    bool any_of(Callable && unary_predicate) const
//    {
//        return std::any_of(begin(),
//                           end(),
//                           std::forward<Callable>(unary_predicate));
//    }
//    
//    // Returns true if none of the keys match the predicate (all return false)
//    //
//    // example:
//    //      const fcpp::set<int> numbers({1, 4, 2, 5, 8, 3});
//    //
//    //      // returns true
//    //      numbers.none_of([](const int &number) {
//    //          return number > 10;
//    //      });
//    //
//    //      // returns false
//    //      numbers.none_of([](const int &number) {
//    //          return number < 6;
//    //      });
//#ifdef CPP17_AVAILABLE
//    template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<bool, Callable, TKey>>>
//#else
//    template <typename Callable>
//#endif
//    bool none_of(Callable && unary_predicate) const
//    {
//        return std::none_of(begin(),
//                            end(),
//                            std::forward<Callable>(unary_predicate));
//    }
//
//    // Performs the functional `reduce` (fold/accumulate) algorithm, by returning the result of
//    // accumulating all the values in the vector to an initial value. (non-mutating)
//    //
//    // example:
//    //      const fcpp::set<std::string> tokens({ "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "brown", "dog" });
//    //      const auto sentence = tokens.reduce<std::string>("", [](const std::string& partial, const std::string& token) {
//    //          return partial.length() != 0
//    //              ? partial + " " + token
//    //              : token;
//    //      });
//    //
//    // outcome: (a set does not allow multiple entries, and its elements are internally managed, order can vary)
//    //
//    //      sentence -> std::string("brown dog fox jumps lazy over quick the");
//#ifdef CPP17_AVAILABLE
//    template <typename U, typename Reduce, typename = std::enable_if_t<std::is_invocable_r_v<U, Reduce, U, TKey>>>
//#else
//    template <typename U, typename Reduce>
//#endif
//    U reduce(const U& initial, Reduce&& reduction) const
//    {
//        auto result = initial;
//        for (const auto& x : m_map)
//        {
//            result = reduction(result, x);
//        }
//        return result;
//    }
//
//    // Performs the functional `filter` algorithm, in which all keys of this instance
//    // which match the given predicate are kept (mutating)
//    //
//    // example:
//    //      fcpp::set<int> numbers({ 1, 3, -5, 2, -1, 9, -4 });
//    //      numbers.filter([](const int& element) {
//    //          return element >= 1.5;
//    //      });
//    //
//    // outcome:
//    //      numbers -> fcpp::set<int>({ 2, 3, 9 });
//    //
//    // is equivalent to:
//    //      fcpp::set<int> numbers({ 1, 3, -5, 2, -1, 9, -4 });
//    //      for (auto i = 0; i < numbers.size(); ++i) {
//    //          if (numbers[i] >= 1.5) {
//    //              continue;
//    //          }
//    //          numbers.remove(i);
//    //          i--;
//    //      }
//#ifdef CPP17_AVAILABLE
//    template <typename Filter, typename = std::enable_if_t<std::is_invocable_r_v<bool, Filter, TKey>>>
//#else
//    template <typename Filter>
//#endif
//    set& filter(Filter && predicate_to_keep)
//    {
//        std::set<TKey, TCompare> copy;
//        auto it = begin();
//        for (; it != end(); it++) {
//            if (predicate_to_keep(*it)) {
//                copy.insert(*it);
//            }
//        }
//        m_map = std::move(copy);
//        return *this;
//    }
//    
//    // Performs the functional `filter` algorithm in a copy of this instance, in which all keys
//    // of the copy which match the given predicate are kept (non-mutating)
//    //
//    // example:
//    //      const fcpp::set<int> numbers({ 1, 3, -5, 2, -1, 9, -4 });
//    //      auto filtered_numbers = numbers.filtered([](const int& element) {
//    //          return element >= 1.5;
//    //      });
//    //
//    // outcome:
//    //      filtered_numbers -> fcpp::set<int>({ 2, 3, 9 });
//    //      numbers -> fcpp::set<int>({ 1, 3, -5, 2, -1, 9, -4 });
//#ifdef CPP17_AVAILABLE
//    template <typename Filter, typename = std::enable_if_t<std::is_invocable_r_v<bool, Filter, TKey>>>
//#else
//    template <typename Filter>
//#endif
//    set filtered(Filter && predicate_to_keep) const
//    {
//        std::set<TKey, TCompare> copy;
//        auto it = begin();
//        for (; it != end(); it++) {
//            if (predicate_to_keep(*it)) {
//                copy.insert(*it);
//            }
//        }
//        return set(copy);
//    }
//    
//#ifdef CPP17_AVAILABLE
//    template<typename Iterator>
//    using deref_type = typename std::iterator_traits<Iterator>::value_type;
//    
//    template<typename Iterator>
//    struct is_valid_iterator {
//        static bool const value = std::is_constructible_v<deref_type<Iterator>>;
//    };
//#endif
//    
//    // Performs the functional `zip` algorithm, in which every key of the resulting set is a
//    // tuple of this instance's key (first) and the second set's key (second).
//    // The sizes of the two sets must be equal.
//    //
//    // example:
//    //      const fcpp::set<int> ages({ 25, 45, 30, 63 });
//    //      const fcpp::set<std::string> persons({ "Jake", "Bob", "Michael", "Philipp" });
//    //      const auto zipped = ages.zip(persons);
//    //
//    // outcome:
//    //      zipped -> fcpp::set<std::pair<int, std::string>>({
//    //                          std::pair<int, std::string>(25, "Bob"),
//    //                          std::pair<int, std::string>(30, "Jake"),
//    //                          std::pair<int, std::string>(45, "Michael"),
//    //                          std::pair<int, std::string>(63, "Philipp"),
//    //                       })
//    template <typename UKey>
//    [[nodiscard]] set<std::pair<TKey, UKey>> zip(const set<UKey>& set) const
//    {
//#ifdef CPP17_AVAILABLE
//        return zip_impl(set.begin(), set.end());
//#else
//        return zip_impl<UKey>(set.begin(), set.end());
//#endif
//    }
//    
//    // Performs the functional `zip` algorithm.
//    // The number of keys must match the set's size.
//    // For more details, see the zip function which accepts a fcpp::set as input.
//    template <typename UKey>
//    [[nodiscard]] set<std::pair<TKey, UKey>> zip(const std::set<UKey>& set) const
//    {
//        return zip(fcpp::set<UKey>(set));
//    }
//    
//    // Performs the functional `zip` algorithm by using the unique values of the vector.
//    // The number of uniques vector values must match the set's size.
//    // For more details, see the zip function which accepts a fcpp::set as input.
//    template <typename UKey>
//    [[nodiscard]] set<std::pair<TKey, UKey>> zip(const vector<UKey>& vector) const
//    {
//        const auto distinct_values = vector.distinct();
//        return zip(distinct_values);
//    }
//    
//    // Performs the functional `zip` algorithm by using the unique values of the vector.
//    // The number of uniques vector values must match the set's size.
//    // For more details, see the zip function which accepts a fcpp::set as input.
//    template <typename UKey>
//    [[nodiscard]] set<std::pair<TKey, UKey>> zip(const std::vector<UKey>& vector) const
//    {
//        return zip(fcpp::vector<UKey>(vector));
//    }
//    
//    // Executes the given operation for each key of the set.
//    // The operation must not change the set's contents during execution.
//#ifdef CPP17_AVAILABLE
//    template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<void, Callable, TKey const &>>>
//#else
//    template <typename Callable>
//#endif
//    const set& for_each(Callable && operation) const
//    {
//        std::for_each(begin(),
//                      end(),
//                      std::forward<Callable>(operation));
//        return *this;
//    }
//    
//    vector<TKey> keys() const {
//        vector<TKey> vec;
//        vec.reserve(size());
//        for_each([&vec](const TKey& key) {
//            vec.insert_back(key);
//        });
//        return std::move(vec);
//    }
//    
//    // Removes an element from the set, if it exists, potentially changing the set's contents (mutating)
//    //
//    // example:
//    //      fcpp::set<int> numbers({1, 4, 2});
//    //      numbers.remove(4);
//    //
//    // outcome:
//    //      numbers -> fcpp::set<int>({1, 2})
//    set& remove(const TKey& element)
//    {
//        m_map.erase(element);
//        return *this;
//    }
//    
//    // Returns a copy by removing an element from the set, if it exists (non-mutating)
//    //
//    // example:
//    //      const fcpp::set<int> numbers({1, 4, 2});
//    //      auto less_numbers = numbers.removing(4);
//    //
//    // outcome:
//    //      less_numbers -> fcpp::set<int>({1, 2})
//    //      numbers -> fcpp::set<int>({1, 2, 4})
//    [[nodiscard]] set removing(const TKey& element) const
//    {
//        auto copy(m_map);
//        copy.erase(element);
//        return set(copy);
//    }
//    
//    // Inserts an element in the set, if it does not already exist, potentially changing the set's contents (mutating)
//    //
//    // example:
//    //      fcpp::set<int> numbers({1, 4, 2});
//    //      numbers.insert(18);
//    //
//    // outcome:
//    //      numbers -> fcpp::set<int>({1, 2, 4, 18})
//    set& insert(const TKey& element)
//    {
//        m_map.insert(element);
//        return *this;
//    }
//    
//    // Returns a copy by inserting an element in the set, if it does not already exist (non-mutating)
//    //
//    // example:
//    //      const fcpp::set<int> numbers({1, 4, 2});
//    //      auto augmented_numbers =  numbers.inserting(18);
//    //
//    // outcome:
//    //      augmented_numbers -> fcpp::set<int>({1, 2, 4, 18})
//    //      numbers -> fcpp::set<int>({1, 2, 4})
//    [[nodiscard]] set inserting(const TKey& element) const
//    {
//        auto copy(m_map);
//        copy.insert(element);
//        return set(copy);
//    }
//    
    // Removes all key/value pairs from the map (mutating)
    //
    // example:
    //      map<std::string, int> persons({{"jake", 32}, {"mary", 26}, {"david", 40}});
    //      persons.clear();
    //
    // outcome:
    //      persons -> fcpp::map<std::string, int>{{}}
    map& clear()
    {
        m_map.clear();
        return *this;
    }
    
    // Returns true if the map is empty
    //
    // example:
    //      const fcpp::map<int, std::string> data({{1, "one"}, {2, "two"}, {3, "three"}});
    //
    //      // returns false
    //      data.is_empty();
    //
    //      // returns true
    //      fcpp::map<int, std::string>().is_empty();
    [[nodiscard]] bool is_empty() const
    {
        return m_map.empty();
    }
//    
//    
//    // Returns true if the key is present in the set, otherwise false
//    //
//    // example:
//    //      const fcpp::set<int> numbers({1, 4, 2});
//    //      numbers.contains(1); // true
//    //      numbers.contains(15); // false
//    [[nodiscard]] bool contains(const TKey& key) const
//    {
//        return m_map.count(key) != 0;
//    }
//    
    // Returns the size of the map (how many elements it contains, it may be different from its capacity)
    [[nodiscard]] size_t size() const
    {
        return m_map.size();
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
        auto it = m_map.find(key);
        assert(it != end());
        return (*it).second;
    }
    
    // Returns a reference to the value that is mapped to a key,
    // performing an insertion if such key does not already exist.
    TValue& operator[](const TKey& key)
    {
        return m_map[key];
    }
//
//    // Returns true if both instances have equal sizes and the corresponding elements (keys) are equal
//    bool operator ==(const set<TKey, TCompare>& rhs) const
//    {
//#ifdef CPP17_AVAILABLE
//        return std::equal(begin(),
//                          end(),
//                          rhs.begin(),
//                          rhs.end());
//#else
//        if (size() != rhs.size()) {
//            return false;
//        }
//        
//        auto it1 = begin();
//        auto it2 = rhs.begin();
//        while (it1 != end() && it2 != rhs.end()) {
//            if (!(*it1 == *it2)) {
//                return false;
//            }
//            it1++;
//            it2++;
//        }
//        
//        return true;
//#endif
//    }
//    
//    // Returns false if either the sizes are not equal or at least one corresponding element (key) is not equal
//    bool operator !=(const set<TKey, TCompare>& rhs) const
//    {
//        return !((*this) == rhs);
//    }
//    
private:
    std::map<TKey, TValue, TCompare> m_map;
//    
//    void assert_smaller_size(const size_t index) const
//    {
//        assert(index < size());
//    }
//    
//#ifdef CPP17_AVAILABLE
//    template<typename Iterator, typename = std::enable_if_t<is_valid_iterator<Iterator>::value>>
//    [[nodiscard]] auto zip_impl( const Iterator& set_begin, const Iterator& set_end) const ->
//    set<std::pair<TKey, deref_type<Iterator>>>
//    {
//        using UKey = deref_type<Iterator>;
//#else
//    template <typename UKey>
//    [[nodiscard]] set<std::pair<TKey, UKey>> zip_impl(const typename std::set<UKey>::const_iterator& set_begin,
//                                                      const typename std::set<UKey>::const_iterator& set_end) const
//    {
//#endif
//        const auto vec_size = std::distance(set_begin, set_end);
//        assert(size() == vec_size);
//        std::set<std::pair<TKey, UKey>> combined_set;
//        auto it1 = begin();
//        auto it2 = set_begin;
//        for (; it1 != end() && it2 != set_end; it1++, it2++) {
//            combined_set.insert({*it1, *it2});
//        }
//        return set<std::pair<TKey, UKey>>(combined_set);
//    }
};

}
