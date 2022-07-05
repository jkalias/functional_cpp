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

template <typename T>
class functional_vector;

// A lightweight wrapper around std::set, enabling fluent and functional
// programming on the set itself, rather than using the more procedural style
// of the standard library algorithms.
//
// Member functions can be mutating (eg. my_set.insert()) or
// non-mutating (eg. my_vector.inserting()) enforcing thread safety if needed
template <typename T>
class functional_set
{
public:
    functional_set()
    : backing_set_()
    {
    }
    
    explicit functional_set(const std::set<T>& set)
    : backing_set_(set)
    {
    }
    
    explicit functional_set(const std::vector<T>& vector)
    : backing_set_(vector.begin(), vector.end())
    {
    }
    
    explicit functional_set(const functional_vector<T>& vector)
    : backing_set_(vector.cbegin(), vector.cend())
    {
    }
    
    explicit functional_set(const std::initializer_list<T>& list)
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
    functional_set difference(const functional_set<T>& other) const {
        std::set<T> diff;
        std::set_difference(cbegin(),
                            cend(),
                            other.cbegin(),
                            other.cend(),
                            std::inserter(diff, diff.begin()));
        return functional_set(diff);
    }
    
    functional_set difference(const std::set<T>& other) const {
        return difference(functional_set(other));
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
    functional_set union_with(const functional_set<T>& other) const {
        std::set<T> combined;
        std::set_union(cbegin(),
                       cend(),
                       other.cbegin(),
                       other.cend(),
                       std::inserter(combined, combined.begin()));
        return functional_set(combined);
    }
    
    functional_set union_with(const std::set<T>& other) const {
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
    functional_set intersect_with(const functional_set<T>& other) const {
        std::set<T> intersection;
        std::set_intersection(cbegin(),
                              cend(),
                              other.cbegin(),
                              other.cend(),
                              std::inserter(intersection, intersection.begin()));
        return functional_set(intersection);
    }
    
    functional_set intersect_with(const std::set<T>& other) const {
        return intersect_with(functional_set(other));
    }
    
    // min
    // max
    // map algorithm
    // map parallel algorithm
    // all_of
    // all_of_parallel
    // any_of
    // any_of_parallel
    // none_of
    // none_of_parallel
    // filter
    // filter_parallel
    // filtered
    // filtered_parallel
    // zip with functional_vector
    // zip with functional_set
    // zip with std::vector
    // zip with std::set
    // for_each -> test
    // for_each_parallel
    // remove
    // removing
    // insert
    // inserting
    // cleared
    // contains
    // find
    
    // Returns the size of the vector (how many elements it contains, it may be different from its capacity)
    size_t size() const
    {
        return backing_set_.size();
    }
    
    // clear
    // is_empty
    // capacity
    // reserve
    
    // Returns the begin iterator, useful for other standard library algorithms
    [[nodiscard]] typename std::set<T>::iterator begin()
    {
        return backing_set_.begin();
    }
    
    // Returns the const begin iterator, useful for other standard library algorithms
    [[nodiscard]] typename std::set<T>::const_iterator cbegin() const
    {
        return backing_set_.begin();
    }
    
    // Returns the end iterator, useful for other standard library algorithms
    [[nodiscard]] typename std::set<T>::iterator end()
    {
        return backing_set_.end();
    }
    
    // Returns the const end iterator, useful for other standard library algorithms
    [[nodiscard]] typename std::set<T>::const_iterator cend() const
    {
        return backing_set_.end();
    }
    
    // Returns the given key in the current set, allowing subscripting.
    // Bounds checking (assert) is enabled for debug builds.
    // Performance is O(n), so be careful for performance critical code sections.
    T operator[](int index)
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
    T operator[](int index) const
    {
        assert_smaller_size(index);
#ifdef CPP17_AVAILABLE
        auto it = std::advance(begin(), index);
        return *it;
#else
        auto count = 0;
        auto it = cbegin();
        while (count++ < index) {
            it++;
        }
        return *it;
#endif
    }
    
    // Returns true if both instances have equal sizes and the corresponding elements (keys) are equal
    bool operator ==(const functional_set<T>& rhs) const
    {
#ifdef CPP17_AVAILABLE
        return std::equal(cbegin(),
                          cend(),
                          rhs.cbegin(),
                          rhs.cend());
#else
        if (size() != rhs.size()) {
            return false;
        }
        
        auto it1 = cbegin();
        auto it2 = rhs.cbegin();
        while (it1 != cend() && it2 != rhs.cend()) {
            if (*it1 != *it2) {
                return false;
            }
            it1++;
            it2++;
        }
        
        return true;
#endif
    }
    
    // Returns false if either the sizes are not equal or at least one corresponding element (key) is not equal
    bool operator !=(const functional_set<T>& rhs) const
    {
        return !((*this) == rhs);
    }
    
private:
    std::set<T> backing_set_;
    
    void assert_smaller_size(int index) const
    {
        assert(index < size() && index >= 0);
    }
};
