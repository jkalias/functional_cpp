// MIT License
//
// Created by iamOgunyinka on 08 Sep 2021 (@iamOgunyinka, https://github.com/iamOgunyinka)
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
#include "compatibility.h"

namespace fcpp {

#ifdef CPP17_AVAILABLE
#include <optional>
template<typename T>
using optional_t = std::optional<T>;
#else
#include <cstddef>
#include <utility>

// A replacement for std::optional when C++17 is not available
template<typename T>
class optional {
public:
    optional()
    : _value{nullptr}
    {
    }
    
    ~optional()
    {
        reset();
    }

    optional& operator=(optional const& other)
    {
        _value = nullptr;
        if (other.has_value()) {
            _value = new T{ other.value() };
        }
        return *this;
    }

    optional(const optional& other)
    {
        _value = nullptr;
        if (other.has_value()) {
            _value = new T{ other.value() };
        }
    }
    
    optional(T const& val)
    : _value(new T{val})
    {
    }
    
    bool has_value() const
    {
        return _value != nullptr;
    }
    
    T* operator->() const
    {
        assert(has_value());
        return _value;
    }
    
    T& operator*() const
    {
        assert(has_value());
        return *_value;
    }
    
    const T& value() const
    {
        assert(has_value());
        return *_value;
    }
    
    optional<T>& operator= (T const& value)
    {
        reset();
        _value = new T(value);
        return *this;
    }
    
private:
    void reset()
    {
        if (_value)
        {
            delete _value;
            _value = nullptr;
        }
    }
    
    T* _value;
};

template<typename T>
using optional_t = optional<T>;

#endif

}
