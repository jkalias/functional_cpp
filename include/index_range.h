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
#include "export_def.h"

// A struct used for container safe access based on index
struct FunctionalCppExport index_range
{
    // Used for returning values of invalid operations
    static index_range invalid;
    
    // Create with start index and element count (end index is calculated)
    static index_range start_count(int start, int count);
    
    // Create with start and end index (count is calculated)
    static index_range start_end(int start, int end);
    
    // The start index of the index range
    // example:
    //     [0] [1] [2] [3] [4] [5] [6]
    //      5   3   9   1   8   3   2
    //              ^       ^
    //              |       |
    //            start    end
    //
    //      start = 2
    //      end = 4
    //      count = 3
    int start;
    
    // The end index of the index range
    // example:
    //     [0] [1] [2] [3] [4] [5] [6]
    //      5   3   9   1   8   3   2
    //              ^       ^
    //              |       |
    //            start    end
    //
    //      start = 2
    //      end = 4
    //      count = 3
    int end;
    
    // The total count of the elements in the index range
    // example:
    //     [0] [1] [2] [3] [4] [5] [6]
    //      5   3   9   1   8   3   2
    //              ^       ^
    //              |       |
    //            start    end
    //
    //      start = 2
    //      end = 4
    //      count = 3
    int count;
    
    // Returns true if it's safe to use its contents
    bool is_valid;
    
    bool operator == (const index_range& rhs) const;
    bool operator != (const index_range& rhs) const;
    
private:
    index_range(int start, int count);
};
