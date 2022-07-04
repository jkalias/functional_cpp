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
//#include <type_traits>
//#include "index_range.h"
//#include "optional.h"
//#ifdef PARALLEL_ALGORITHM_AVAILABLE
//#include <execution>
//#endif

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
//
//    explicit functional_set(std::set<T>&& set)
//    : backing_set_(std::move(vector))
//    {
//    }
//
//    explicit functional_set(std::initializer_list<T> list)
//    : backing_set_(std::move(list))
//    {
//    }
    
//    // Creates a new vector by repeating a given element.
//    //
//    // example:
//    //      const functional_vector<std::string> filled_vector(3, "John");
//    //
//    // outcome:
//    //      filled_vector -> functional_vector<std::string>({ "John", "John", "John" })
//    explicit functional_vector(size_t count, const T& element)
//    : backing_vector_(count, element)
//    {
//    }
//
//    // Performs the functional `map` algorithm, in which every element of the resulting vector is the
//    // output of applying the transform function on every element of this instance.
//    //
//    // example:
//    //      const functional_vector<int> input_vector({ 1, 3, -5 });
//    //      const auto output_vector = input_vector.map<std::string>([](const auto& element) {
//    //      	return std::to_string(element);
//    //      });
//    //
//    // outcome:
//    //      output_vector -> functional_vector<std::string>({ "1", "3", "-5" })
//    //
//    // is equivalent to:
//    //      const functional_vector<int> input_vector({ 1, 3, -5 });
//    //      functional_vector<std::string> output_vector;
//    //      for (auto i = 0; i < input_vector.size(); ++i) {
//    //      	output_vector.insert_back(std::to_string(input_vector[i]));
//    //      }
//#ifdef CPP17_AVAILABLE
//    template <typename U, typename Transform, typename = std::enable_if_t<std::is_invocable_r_v<U, Transform, T>>>
//#else
//    template <typename U, typename Transform>
//#endif
    
    // Returns the size of the vector (how many elements it contains, it may be different from its capacity)
    size_t size() const
    {
        return backing_set_.size();
    }
    
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

private:
    std::set<T> backing_set_;
};
