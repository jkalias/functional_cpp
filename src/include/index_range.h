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

#ifndef INDEX_RANGE_H
#define INDEX_RANGE_H
#include <stddef.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #ifdef functional_vector_EXPORTS
        #define FunctionalVectorExport __declspec( dllexport )
    #else
        #define FunctionalVectorExport __declspec( dllimport )
    #endif
#else
    #define FunctionalVectorExport __attribute__ ((__visibility__("default")))
#endif

struct FunctionalVectorExport index_range
{
public:
    static index_range invalid;
    static index_range from_start_and_count(size_t start, size_t count);
    static index_range from_start_and_end(size_t start, size_t end);
    
    bool operator == (const index_range& rhs) const;
    bool operator != (const index_range& rhs) const;
    
    size_t start;
    size_t count;
    bool is_valid;
    
private:
    index_range(size_t start, size_t count);
};

#endif /* INDEX_RANGE_H */
