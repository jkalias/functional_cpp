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
#include <string>

struct child
{
    child()
    : age(0)
    {
    }
    
    child(int age)
    : age(age)
    {
    }
    
    int age;
};

struct person
{
    person()
    : age(0), name("")
    {
    }
    
    person(int age, std::string name)
    : age(age), name(std::move(name))
    {
    }
    
    int age;
    std::string name;
    
    std::size_t hash() const {
        std::size_t h1 = std::hash<std::string>{}(name);
        std::size_t h2 = std::hash<int>{}(age);
        return h1 ^ (h2 << 1);
    }
    
    bool operator == (const person& other) const {
        return age == other.age && name == other.name;
    }
    
    bool operator< (const person& other) const {
        return hash() < other.hash();
    }
};

struct person_comparator {
    bool operator() (const person& a, const person& b) const {
        return a < b;
    }
};
