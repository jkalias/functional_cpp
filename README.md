[![CMake Build Matrix](https://github.com/jkalias/functional_cpp/actions/workflows/cmake.yml/badge.svg)](https://github.com/jkalias/functional_cpp/actions/workflows/cmake.yml)
[![GitHub license](https://img.shields.io/github/license/jkalias/functional_cpp)](https://github.com/jkalias/functional_cpp/blob/main/LICENSE)
# Say hello to functional C++
A wrapper for C++ std::vector and std::set geared towards functional programming and fluent APIs. This project is heavily influenced and inspired by C# and Swift.

The primary focus of this library is
* readability at the call site ("make it work, make it right, make it fast")
* surfacing existing algorithms from the standard library, and lowering the barrier for their extended usage
* elimination of vector index operations
* encapsulation of the iterator madness
* removal of manual for-loops

## Compilation (Cmake)
### Dependencies
* CMake >= 3.14

### Minimum C++ version
* C++11

An out-of-source build strategy is used. All following examples assume an output build folder named ```build```. If no additional argument is passed to CMake, C++11 is used. Otherwise, you can pass ```-DCMAKE_CXX_STANDARD=17``` and it will use C++17 for example.
### macOS (Xcode)
```console
cd functional_vector
cmake -S . -B build -G Xcode
```
Then open the generated ```functional_vector.xcodeproj``` in the ```build``` folder.

### macOS (Makefiles/clang)
```console
cd functional_vector
cmake -S . -B build
cmake --build build
build/tests/unit_tests
```

### macOS (Makefiles/g++)
Assuming you have installed Homebrew, you can then use the gcc and g++ compilers by doing the following (this example uses version gcc 11)
```console
cd functional_vector
cmake \
    -S . \
    -B build \
    -DCMAKE_C_COMPILER=/opt/homebrew/Cellar/gcc/11.2.0/bin/gcc-11 \
    -DCMAKE_CXX_COMPILER=/opt/homebrew/Cellar/gcc/11.2.0/bin/g++-11
cmake --build build
build/tests/unit_tests
```

### Linux (Makefiles)
```console
cd functional_vector
cmake -S . -B build
cmake --build build
build/tests/unit_tests
```

### Windows (Visual Studio)
```console
cd functional_vector
cmake -S . -B build
```
Then open the generated ```functional_vector.sln``` in the ```build``` folder.

## Usage (functional_vector)
### zip, map, filter, sort
```c++
#include "functional_vector.h" // instead of <vector>

struct person {
    person(int age, std::string name)
    : age(age), name(std::move(name))
    {}
    int age;
    std::string name;
};

// ...

// the employees' ages
const functional_vector<int> ages({32, 45, 37, 23});

// the employees' names
const functional_vector<std::string> names({"Jake", "Anna", "Kate", "Bob"});

const auto employees_below_40 = ages
    // zip two vectors for simultaneous processing
    .zip(names)

    // apply the functional map algorithm (transform from one type to another)
    .map<person>([](const auto& pair) {                     
        return person(pair.first, pair.second);
    })
    
    // filter the elements using a local function (lambda)
    .filter([](const auto& person) {
        return person.age < 40;
    })
    
    // sort according to custom predicate
    .sort([](const auto& person1, const auto& person2) {
        return person1.age < person2.age;
    });

/*
 prints the following:
 Bob is 23 years old.
 Jake is 32 years old.
 Kate is 37 years old.
 */
employees_below_40.for_each([](const auto& person) {
    std::cout << person.name << " is " << person.age << " years old." << std::endl;
});
```
### index search
```c++
#include "functional_vector.h" // instead of <vector>

const functional_vector numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});

const auto first_index_of_one = numbers.find_first_index(1);
// returns 0
first_index_of_one.value();

const auto last_index_of_one = numbers.find_last_index(1);
// returns 8
last_index_of_one.value();

// all_indices_of_one -> { 0, 6, 8 }
const auto all_indices_of_one = numbers.find_all_indices(1);

const auto index_of_nine = numbers.find_first_index(9);
// returns false
index_of_nine.has_value();
```

### remove, insert
```c++
#include "functional_vector.h" // instead of <vector>
#include "index_range.h"

functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});

// numbers -> functional_vector<int>({1, 4, 2, 5, 3, 1, 7, 1});
numbers.remove_at(4);

// numbers -> functional_vector<int>({4, 2, 5, 3, 1, 7, 1});
numbers.remove_front();

// numbers -> functional_vector<int>({4, 2, 5, 3, 1, 7});
numbers.remove_back();

// numbers -> functional_vector<int>({4, 2, 7});
numbers.remove_range(index_range::start_count(2, 3));

// numbers -> functional_vector<int>({4, 8, 2, 7});
numbers.insert_at(1, 8);

// numbers -> functional_vector<int>({-10, 4, 8, 2, 7});
numbers.insert_front(-10);

// numbers -> functional_vector<int>({-10, 4, 8, 2, 7, 9});
numbers.insert_back(9);

// numbers -> functional_vector<int>({-10, 4, 8, 3, -2, 5, 2, 7, 9});
numbers.insert_at(3, std::vector({3, -2, 5}));

// numbers -> functional_vector<int>({4, -6, 7, -10, 4, 8, 3, -2, 5, 2, 7, 9});
numbers.insert_front(functional_vector({4, -6, 7}));

// numbers -> functional_vector<int>({4, -6, 7, -10, 4, 8, 3, -2, 5, 2, 7, 9, 7, 3});
numbers.insert_back(std::initializer_list({7, 3}));
```

### size/capacity, reserve/resize
```c++
#include "functional_vector.h" // instead of <vector>

// numbers.capacity() = 9
// numbers.size() = 9
functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});

// numbers -> functional_vector<int>({1, 4, 2, 5, 8});
// numbers.capacity() = 9
// numbers.size() = 5
numbers.resize(5);

// numbers -> functional_vector<int>({1, 4, 2, 5, 8, 0, 0});
// numbers.capacity() = 9
// numbers.size() = 7
numbers.resize(7);

// empty_numbers.capacity() = 0
// empty_numbers.size() = 0
functional_vector<int> empty_numbers;

// empty_numbers.capacity() = 5
// empty_numbers.size() = 0
empty_numbers.reserve(5);
```

### all_of, any_of, none_of
```c++
#include "functional_vector.h" // instead of <vector>

functional_vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});

// returns true
numbers.all_of([](const auto &number) {
    return number < 10;
});

// returns false
numbers.all_of([](const auto &number) {
    return number > 2;
});

// returns true
numbers.any_of([](const auto &number) {
    return number < 5;
});

// returns false
numbers.any_of([](const auto &number) {
    return number > 9;
});

// returns true
numbers.none_of([](const auto &number) {
    return number < -2;
});

// returns false
numbers.none_of([](const auto &number) {
    return number > 7;
});
```

### Parallel algorithms
Since C++17 several STL algorithms can be executed in parallel.

clang on macOS does not yet fully support the parallel execution model, however on Windows and Linux, a `functional_vector` supports the following parallel algorithms
```c++
for_each_parallel
map_parallel
filter_parallel
sort_parallel
all_of_parallel
any_of_parallel
none_of_parallel
```
