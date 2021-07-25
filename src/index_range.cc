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

#include "index_range.h"

index_range index_range::invalid = from_start_and_count(-1, -1);

index_range::index_range(size_t start, size_t count)
: start(-1), count(-1)
{
    is_valid = start >= 0 && start != -1 && count > 0 && count != -1;
    if (is_valid) {
        this->start = start;
        this->count = count;
    }
}

index_range index_range::from_start_and_count(size_t start, size_t count)
{
    return index_range(start, count);
}

index_range index_range::from_start_and_end(size_t start, size_t end)
{
    return index_range(start, end - start + 1);
}

size_t index_range::end() const
{
    if (!is_valid) {
        return -1;
    }
    return start + count - 1;
}

bool index_range::operator == (const index_range& rhs) const
{
    return start == rhs.start && count == rhs.count;
}

bool index_range::operator != (const index_range& rhs) const
{
    return !(*this == rhs);
}
