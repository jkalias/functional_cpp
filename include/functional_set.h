// MIT License
//
// Copyright (c) 2022 Ioannis Kaliakatsos
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
#include <set>
#include <algorithm>
#include "optional.h"

template <typename T>
class functional_vector;

// A lightweight wrapper around std::set, enabling fluent and functional
// programming on the set itself, rather than using the more procedural style
// of the standard library algorithms.
//
// Member functions can be mutating (eg. my_set.insert()) or
// non-mutating (eg. my_vector.inserting()) enforcing thread safety if needed
template <class TKey, class TCompare = std::less<TKey>>
class functional_set
{
public:
    functional_set()
    : backing_set_()
    {
    }
    
    explicit functional_set(const std::set<TKey, TCompare>& set)
    : backing_set_(set)
    {
    }
    
    explicit functional_set(const std::vector<TKey>& vector)
    : backing_set_(vector.begin(), vector.end())
    {
    }
    
    explicit functional_set(const functional_vector<TKey>& vector)
    : backing_set_(vector.begin(), vector.end())
    {
    }
    
    explicit functional_set(const std::initializer_list<TKey>& list)
    : backing_set_(list.begin(), list.end())
    {
    }
    
    // Returns the set of elements which belong to the current set but not in the other set.
    // In Venn diagram notation, if A is the current set and B is the other set, then
    // the difference is the operation A – B = {x : x ∈ A and x ∉ B}
    //
    // example:
    //      const functional_set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
    //      const functional_set<int> set2(std::set<int>({2, 5, 7, 10, 15, 17}));
    //      const auto& diff = set1.difference(set2);
    //
    // outcome:
    //      diff -> functional_set<int>({1, 3, 8})
    functional_set difference_with(const functional_set<TKey, TCompare>& other) const {
        std::set<TKey, TCompare> diff;
        std::set_difference(begin(),
                            end(),
                            other.begin(),
                            other.end(),
                            std::inserter(diff, diff.begin()));
        return functional_set(diff);
    }
    
    functional_set difference_with(const std::set<TKey, TCompare>& other) const {
        return difference_with(functional_set(other));
    }
    
    // Returns the set of elements which belong either to the current or the other set.
    // In Venn diagram notation, if A is the current set and B is the other set, then
    // the union is the operation A ∪ B = {x : x ∈ A or x ∈ B}
    //
    // example:
    //      const functional_set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
    //      const functional_set<int> set2(std::set<int>({2, 5, 7, 10, 15, 17}));
    //      const auto& combined = set1.set_union(set2);
    //
    // outcome:
    //      combined -> functional_set<int>({1, 2, 3, 5, 7, 8, 10, 15, 17})
    functional_set union_with(const functional_set<TKey, TCompare>& other) const {
        std::set<TKey, TCompare> combined;
        std::set_union(begin(),
                       end(),
                       other.begin(),
                       other.end(),
                       std::inserter(combined, combined.begin()));
        return functional_set(combined);
    }
    
    functional_set union_with(const std::set<TKey, TCompare>& other) const {
        return union_with(functional_set(other));
    }
    
    // Returns the set of elements which belong to both the current and the other set.
    // In Venn diagram notation, if A is the current set and B is the other set, then
    // the intersection is the operation A ∩ B = {x : x ∈ A and x ∈ B}
    //
    // example:
    //      const functional_set<int> set1(std::set<int>({1, 2, 3, 5, 7, 8, 10}));
    //      const functional_set<int> set2(std::set<int>({2, 5, 7, 10, 15, 17}));
    //      const auto& combined = set1.set_union(set2);
    //
    // outcome:
    //      combined -> functional_set<int>({2, 5, 7, 10})
    functional_set intersect_with(const functional_set<TKey, TCompare>& other) const {
        std::set<TKey, TCompare> intersection;
        std::set_intersection(begin(),
                              end(),
                              other.begin(),
                              other.end(),
                              std::inserter(intersection, intersection.begin()));
        return functional_set(intersection);
    }
    
    functional_set intersect_with(const std::set<TKey, TCompare>& other) const {
        return intersect_with(functional_set(other));
    }
    
    // Returns the minimum key in the set, if it's not empty.
    //
    // example:
    //      const functional_set<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      auto minimum = numbers.min();
    //
    //      // an empty's set minimum value
    //      functional_set<int>().min().has_value() // false
    //
    // outcome:
    //      minimum.has_value() -> true
    //      minimum.value() -> 1
    optional_t<TKey> min() const {
        const auto& it = std::min_element(begin(), end());
        if (it != end()) {
            return *it;
        }
        return optional_t<TKey>();
    }
    
    // Returns the maximum key in the set, if it's not empty.
    //
    // example:
    //      const functional_set<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});
    //      auto maximum = numbers.max();
    //
    //      // an empty's set maximum value
    //      functional_set<int>().maxn().has_value() // false
    //
    // outcome:
    //      maximum.has_value() -> true
    //      maximum.value() -> 8
    optional_t<TKey> max() const {
        const auto& it = std::max_element(begin(), end());
        if (it != end()) {
            return *it;
        }
        return optional_t<TKey>();
    }
    
    // Performs the functional `map` algorithm, in which every element of the resulting set is the
    // output of applying the transform function on every element of this instance.
    //
    // example:
    //      const functional_vector<int> input_set({ 1, 3, -5 });
    //      const auto output_set = input_set.map<std::string>([](const int& element) {
    //          return std::to_string(element);
    //      });
    //
    // outcome:
    //      output_set -> functional_set<std::string>({ "-5", "1", "3" })
    //
    // is equivalent to:
    //      const functional_set<int> input_set({ 1, 3, -5 });
    //      functional_set<std::string> output_set;
    //      for (auto const& key: input_set) {
    //          output_set.insert(std::to_string(key));
    //      }
#ifdef CPP17_AVAILABLE
    template <class UKey, class UCompare = std::less<UKey>, typename Transform, typename = std::enable_if_t<std::is_invocable_r_v<UKey, Transform, TKey>>>
#else
    template <typename UKey, class UCompare = std::less<UKey>, typename Transform>
#endif
    functional_set<UKey, UCompare> map(Transform && transform) const
    {
        std::set<UKey, UCompare> transformed_set;
        for (const auto& key: backing_set_) {
            transformed_set.insert(transform(key));
        }
        return functional_set<UKey, UCompare>(transformed_set);
    }
    
    // all_of
    // any_of
    // none_of
    // filter
    // filtered
    // zip with functional_vector
    // zip with functional_set
    // zip with std::vector
    // zip with std::set
    
    // Executes the given operation for each key of the set. The operation must not
    // change the set's contents during execution.
#ifdef CPP17_AVAILABLE
    template <typename Callable, typename = std::enable_if_t<std::is_invocable_r_v<void, Callable, TKey const &>>>
#else
    template <typename Callable>
#endif
    const functional_set& for_each(Callable && operation) const
    {
        std::for_each(begin(),
                      end(),
                      std::forward<Callable>(operation));
        return *this;
    }
    
    // Removes an element from the set, if it exists, potentially changing the set's contents (mutating)
    //
    // example:
    //      functional_set<int> numbers({1, 4, 2});
    //      numbers.remove(4);
    //
    // outcome:
    //      numbers -> functional_set<int>({1, 2})
    functional_set& remove(const TKey& element)
    {
        backing_set_.erase(element);
        return *this;
    }
    
    // Returns a copy by removing an element from the set, if it exists (non-mutating)
    //
    // example:
    //      const functional_set<int> numbers({1, 4, 2});
    //      auto less_numbers = numbers.removing(4);
    //
    // outcome:
    //      less_numbers -> functional_set<int>({1, 2})
    //      numbers -> functional_set<int>({1, 2, 4})
    [[nodiscard]] functional_set removing(const TKey& element) const
    {
        auto copy(backing_set_);
        copy.erase(element);
        return functional_set(copy);
    }
    
    // Inserts an element in the set, if it does not already exist, potentially changing the set's contents (mutating)
    //
    // example:
    //      functional_set<int> numbers({1, 4, 2});
    //      numbers.insert(18);
    //
    // outcome:
    //      numbers -> functional_set<int>({1, 2, 4, 18})
    functional_set& insert(const TKey& element)
    {
        backing_set_.insert(element);
        return *this;
    }
    
    // Returns a copy by inserting an element in the set, if it does not already exist (non-mutating)
    //
    // example:
    //      const functional_set<int> numbers({1, 4, 2});
    //      auto augmented_numbers =  numbers.inserting(18);
    //
    // outcome:
    //      augmented_numbers -> functional_set<int>({1, 2, 4, 18})
    //      numbers -> functional_set<int>({1, 2, 4})
    [[nodiscard]] functional_set inserting(const TKey& element) const
    {
        auto copy(backing_set_);
        copy.insert(element);
        return functional_set(copy);
    }
    
    // Removes all keys from the set (mutating)
    //
    // example:
    //      functional_set<int> numbers({1, 4, 2});
    //      numbers.clear();
    //
    // outcome:
    //      numbers -> functional_set<int>({})
    functional_set& clear()
    {
        backing_set_.clear();
        return *this;
    }
    
    // Returns a new set by clearing all keys from the current set (non-mutating)
    //
    // example:
    //      const functional_set<int> numbers({1, 4, 2});
    //      auto cleared_numbers = numbers.clearing();
    //
    // outcome:
    //      cleared_numbers -> functional_set<int>({})
    //      numbers -> functional_set<int> numbers({1, 4, 2})
    [[nodiscard]] functional_set clearing() const
    {
        return functional_set();
    }
    
    // Returns true if the key is present in the set, otherwise false
    //
    // example:
    //      const functional_set<int> numbers({1, 4, 2});
    //      numbers.contains(1); // true
    //      numbers.contains(15); // false
    bool contains(const TKey& key) const
    {
        return backing_set_.count(key) != 0;
    }
    
    // Returns the size of the vector (how many elements it contains, it may be different from its capacity)
    size_t size() const
    {
        return backing_set_.size();
    }
    
    // clear
    // is_empty
    
    // Returns the begin iterator, useful for other standard library algorithms
    [[nodiscard]] typename std::set<TKey>::iterator begin()
    {
        return backing_set_.begin();
    }
    
    // Returns the const begin iterator, useful for other standard library algorithms
    [[nodiscard]] typename std::set<TKey>::const_iterator begin() const
    {
        return backing_set_.begin();
    }
    
    // Returns the end iterator, useful for other standard library algorithms
    [[nodiscard]] typename std::set<TKey>::iterator end()
    {
        return backing_set_.end();
    }
    
    // Returns the const end iterator, useful for other standard library algorithms
    [[nodiscard]] typename std::set<TKey>::const_iterator end() const
    {
        return backing_set_.end();
    }
    
    // Returns the given key in the current set, allowing subscripting.
    // Bounds checking (assert) is enabled for debug builds.
    // Performance is O(n), so be careful for performance critical code sections.
    TKey operator[](int index)
    {
        assert_smaller_size(index);
#ifdef CPP17_AVAILABLE
        auto it = std::advance(begin(), index);
        return *it;
#else
        auto count = 0;
        auto it = begin();
        while (count++ < index) {
            it++;
        }
        return *it;
#endif
    }
    
    // Returns the given key in the current constant set, allowing subscripting.
    // Bounds checking (assert) is enabled for debug builds.
    // Performance is O(n), so be careful for performance critical code sections.
    TKey operator[](int index) const
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
            it++;
        }
        return *it;
#endif
    }
    
    // Returns true if both instances have equal sizes and the corresponding elements (keys) are equal
    bool operator ==(const functional_set<TKey, TCompare>& rhs) const
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
    
    // Returns false if either the sizes are not equal or at least one corresponding element (key) is not equal
    bool operator !=(const functional_set<TKey, TCompare>& rhs) const
    {
        return !((*this) == rhs);
    }
    
private:
    std::set<TKey, TCompare> backing_set_;
    
    void assert_smaller_size(int index) const
    {
        assert(index < size() && index >= 0);
    }
};
