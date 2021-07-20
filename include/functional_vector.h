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

#ifndef functional_vector_h
#define functional_vector_h
#include <vector>
#include <functional>
#include <algorithm>
#include <optional>

template <typename T>
class functional_vector {
public:
    functional_vector()
    : backing_vector_()
    {
    }
    
    functional_vector(const std::vector<T> &vector)
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
    
    functional_vector adding(T value) const
    {
        auto augmented_vector(backing_vector_);
        augmented_vector.push_back(value);
        return functional_vector(augmented_vector);
    }
    
    functional_vector& add_range(const std::vector<T>& vector)
    {
        backing_vector_.insert(backing_vector_.end(),
                               vector.begin(),
                               vector.end());
        return *this;
    }
    
    functional_vector adding_range(const std::vector<T>& vector) const
    {
        auto augmented_vector(backing_vector_);
        augmented_vector.reserve(augmented_vector.size() + vector.size());
        augmented_vector.insert(augmented_vector.end(),
                                vector.begin(),
                                vector.end());
        return functional_vector(augmented_vector);
    }
    
    functional_vector& add_range(const std::initializer_list<T>& list)
    {
        backing_vector_.insert(backing_vector_.end(),
                               list.begin(),
                               list.end());
        return *this;
    }
    
    functional_vector adding_range(const std::initializer_list<T>& list) const
    {
        auto augmented_vector(backing_vector_);
        augmented_vector.reserve(augmented_vector.size() + list.size());
        augmented_vector.insert(augmented_vector.end(),
                                list.begin(),
                                list.end());
        return functional_vector(augmented_vector);
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
    
    functional_vector reversed() const
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
    functional_vector<functional_vector_tuple<U>> zip(const std::vector<U>& vector) const
    {
        assert(backing_vector_.size() == vector.size());
        std::vector<functional_vector_tuple<U>> combined_vector;
        combined_vector.reserve(vector.size());
        for (auto i = 0; i < backing_vector_.size(); i++) {
            combined_vector.push_back({ backing_vector_[i], vector[i] });
        }
        return functional_vector<functional_vector_tuple<U>>(combined_vector);
    }
    
    template <typename U>
    functional_vector<functional_vector_tuple<U>> zip(const functional_vector<U>& vector) const
    {
        assert(backing_vector_.size() == vector.size());
        std::vector<functional_vector_tuple<U>> combined_vector;
        combined_vector.reserve(vector.size());
        for (auto i = 0; i < backing_vector_.size(); i++) {
            combined_vector.push_back({ backing_vector_[i], vector[i] });
        }
        return functional_vector<functional_vector_tuple<U>>(combined_vector);
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
    
    functional_vector sorted_ascending() const
    {
        return sorted(std::less_equal<T>());
    }
    
    functional_vector sorted_descending() const
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
    
    typename std::vector<T>::iterator begin()
    {
        return backing_vector_.begin();
    }
    
    typename std::vector<T>::const_iterator begin() const
    {
        return backing_vector_.begin();
    }
    
    typename std::vector<T>::iterator end()
    {
        return backing_vector_.end();
    }
    
    typename std::vector<T>::const_iterator end() const
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
    
    std::optional<size_t> first_index_of(const T& element) const
    {
        auto it = std::find(backing_vector_.begin(),
                            backing_vector_.end(),
                            element);
        if (it != backing_vector_.end()) {
            return std::distance(backing_vector_.begin(), it);
        }
        return std::nullopt;
    }
    
    std::optional<size_t> last_index_of(const T& element) const
    {
        auto it = std::find(backing_vector_.rbegin(),
                            backing_vector_.rend(),
                            element);
        if (it != backing_vector_.rend()) {
            return std::distance(it, backing_vector_.rend()) - 1;
        }
        return std::nullopt;
    }
    
    std::vector<size_t> all_indices_of(const T& element) const
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
    
    functional_vector removing_at(size_t index) const
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
    
    functional_vector removing_last() const
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
    
    functional_vector removing_first() const
    {
        if (size() == 0) {
            return *this;
        }
        return removing_at(0);
    }
    
    functional_vector& insert_at(size_t index, const T& element)
    {
        assert(index < size());
        backing_vector_.insert(begin() + index, element);
        return *this;
    }
    
    functional_vector inserting_at(size_t index, const T& element) const
    {
        assert(index < size());
        auto copy(backing_vector_);
        copy.insert(copy.begin() + index, element);
        return functional_vector(copy);
    }
    
private:
    std::vector<T> backing_vector_;
};

#endif /* functional_vector_h */
