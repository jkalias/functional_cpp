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
cd functional_cpp
cmake -S . -B build -G Xcode
```
Then open the generated ```functional_cpp.xcodeproj``` in the ```build``` folder.

### macOS (Makefiles/clang)
```console
cd functional_cpp
cmake -S . -B build
cmake --build build
build/tests/unit_tests
```

### macOS (Makefiles/g++)
Assuming you have installed Homebrew, you can then use the gcc and g++ compilers by doing the following (this example uses version gcc 11)
```console
cd functional_cpp
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
cd functional_cpp
cmake -S . -B build
cmake --build build
build/tests/unit_tests
```

### Windows (Visual Studio)
```console
cd functional_cpp
cmake -S . -B build
```
Then open the generated ```functional_cpp.sln``` in the ```build``` folder.

## Functional vector usage (fcpp::vector)
### extract unique (distinct) elements
```c++
#include "vector.h" // instead of <vector>

const fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});

// contains only 1, 2, 3, 4, 5, 7, 8
const auto& unique_numbers = numbers.distinct();
```

### zip, map, filter, sort
```c++
#include "vector.h" // instead of <vector>

struct person {
    person(int age, std::string name)
    : age(age), name(std::move(name))
    {}
    int age;
    std::string name;
    
    std::size_t hash() const {
        // a clever implementation of hash    
        // ...
    }
    
    bool operator< (const person& other) const {
        return hash() < other.hash();
    }
};

// ...

// the employees' ages
const fcpp::vector<int> ages({32, 45, 37, 23});

// the employees' names
const fcpp::vector<std::string> names({"Jake", "Anna", "Kate", "Bob"});

const auto employees_below_40 = ages
    // zip two vectors for simultaneous processing
    .zip(names)

    // apply the functional map algorithm (transform from one type to another)
    .map<person>([](const std::pair<int, std::string>& pair) {                     
        return person(pair.first, pair.second);
    })
    
    // filter the elements using a local function (lambda)
    .filter([](const person& p) {
        return p.age < 40;
    })
    
    // sort according to custom predicate
    .sort([](const person& person1, const person& person2) {
        return person1.age < person2.age;
    });

/*
 prints the following:
 Bob is 23 years old.
 Jake is 32 years old.
 Kate is 37 years old.
 */
employees_below_40.for_each([](const person& p) {
    std::cout << p.name << " is " << p.age << " years old." << std::endl;
});
```
### index search
```c++
#include "vector.h" // instead of <vector>

const fcpp::vector numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});

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
#include "vector.h" // instead of <vector>
#include "index_range.h"

fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});

// numbers -> fcpp::vector<int>({1, 4, 2, 5, 3, 1, 7, 1});
numbers.remove_at(4);

// numbers -> fcpp::vector<int>({4, 2, 5, 3, 1, 7, 1});
numbers.remove_front();

// numbers -> fcpp::vector<int>({4, 2, 5, 3, 1, 7});
numbers.remove_back();

// numbers -> fcpp::vector<int>({4, 2, 7});
numbers.remove_range(index_range::start_count(2, 3));

// numbers -> fcpp::vector<int>({4, 8, 2, 7});
numbers.insert_at(1, 8);

// numbers -> fcpp::vector<int>({-10, 4, 8, 2, 7});
numbers.insert_front(-10);

// numbers -> fcpp::vector<int>({-10, 4, 8, 2, 7, 9});
numbers.insert_back(9);

// numbers -> fcpp::vector<int>({-10, 4, 8, 3, -2, 5, 2, 7, 9});
numbers.insert_at(3, std::vector({3, -2, 5}));

// numbers -> fcpp::vector<int>({4, -6, 7, -10, 4, 8, 3, -2, 5, 2, 7, 9});
numbers.insert_front(fcpp::vector({4, -6, 7}));

// numbers -> fcpp::vector<int>({4, -6, 7, -10, 4, 8, 3, -2, 5, 2, 7, 9, 7, 3});
numbers.insert_back(std::initializer_list({7, 3}));
```

### size, capacity, reserve, resize
```c++
#include "vector.h" // instead of <vector>

// numbers.capacity() = 9
// numbers.size() = 9
fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});

// numbers -> fcpp::vector<int>({1, 4, 2, 5, 8});
// numbers.capacity() = 9
// numbers.size() = 5
numbers.resize(5);

// numbers -> fcpp::vector<int>({1, 4, 2, 5, 8, 0, 0});
// numbers.capacity() = 9
// numbers.size() = 7
numbers.resize(7);

// empty_numbers.capacity() = 0
// empty_numbers.size() = 0
fcpp::vector<int> empty_numbers;

// empty_numbers.capacity() = 5
// empty_numbers.size() = 0
empty_numbers.reserve(5);
```

### all_of, any_of, none_of
```c++
#include "vector.h" // instead of <vector>

fcpp::vector<int> numbers({1, 4, 2, 5, 8, 3, 1, 7, 1});

// returns true
numbers.all_of([](const int& number) {
    return number < 10;
});

// returns false
numbers.all_of([](const int& number) {
    return number > 2;
});

// returns true
numbers.any_of([](const int& number) {
    return number < 5;
});

// returns false
numbers.any_of([](const int& number) {
    return number > 9;
});

// returns true
numbers.none_of([](const int& number) {
    return number < -2;
});

// returns false
numbers.none_of([](const int& number) {
    return number > 7;
});
```

### Parallel algorithms
Since C++17 several STL algorithms can be executed in parallel.

clang on macOS does not yet fully support the parallel execution model, however on Windows and Linux, an `fcpp::vector` supports the following parallel algorithms
```c++
for_each_parallel
map_parallel
filter_parallel
sort_parallel
all_of_parallel
any_of_parallel
none_of_parallel
```

## Functional set usage (fcpp::set)
### difference, union, intersection (works with fcpp::set and std::set)
```c++
#include "set.h" // instead of <set>

// struct person as defined previously
struct person_comparator {
    bool operator() (const person& a, const person& b) const {
        return a < b;
    }
};

// ...

// a set containing all colleagues
const fcpp::set<person, person_comparator> colleagues({
    person(51, "George"),
    person(15, "Jake"),
    person(18, "Jannet"),
    person(41, "Jackie"),
    person(25, "Kate")
});

// a set containing all friends
const fcpp::set<person, person_comparator> friends({
    person(51, "George"),
    person(41, "Jackie"),
    person(42, "Crystal"),
});

// find which colleagues are not friends
// contains person(15, "Jake"), person(18, "Jannet") and person(25, "Kate")
const auto colleagues_but_not_friends = colleagues.difference_with(friends);

// find which friends are colleagues
// same as colleagues.intersect_with(friends)
// contains person(51, "George"), person(41, "Jackie")
const auto good_colleagues = friends.intersection_with(colleagues);

// a set of close family members
const fcpp::set<person, person_comparator> family({
    person(51, "Paul"),
    person(81, "Barbara"),
});

// all of our friends and family for the next party invitation
// contains person(51, "George"), person(41, "Jackie"), person(42, "Crystal"), person(51, "Paul"), person(81, "Barbara") 
const auto friends_and_family = friends.union_with(family);
```

### zip, map, filter
```c++
#include "set.h" // instead of <set>

// the employees' ages
const fcpp::set<int> ages({ 25, 45, 30, 63 });

// the employees' names
const fcpp::set<std::string> names({ "Jake", "Bob", "Michael", "Philipp" });

const auto employees_below_40 = ages
    // zip two vectors for simultaneous processing
    .zip(names)

    // apply the functional map algorithm (transform from one type to another)
    .map<person>([](const std::pair<int, std::string>& pair) {                     
        return person(pair.first, pair.second);
    })
    
    // filter the elements using a local function (lambda)
    .filter([](const person& p) {
        return p.age < 40;
    });
    
/*
 prints the following:
 Jake is 30 years old.
 Bob is 25 years old.
 */
employees_below_40.for_each([](const person& p) {
    std::cout << p.name << " is " << p.age << " years old." << std::endl;
});
```

### all_of, any_of, none_of
```c++
#include "set.h" // instead of <set>

fcpp::set<int> numbers({1, 4, 2, 5, 8, 3, 7});

// returns true
numbers.all_of([](const int& number) {
    return number < 10;
});

// returns false
numbers.all_of([](const int& number) {
    return number > 2;
});

// returns true
numbers.any_of([](const int& number) {
    return number < 5;
});

// returns false
numbers.any_of([](const int& number) {
    return number > 9;
});

// returns true
numbers.none_of([](const int& number) {
    return number < -2;
});

// returns false
numbers.none_of([](const int& number) {
    return number > 7;
});
```

### remove, insert, contains, size, clear
```c++
#include "set.h" // instead of <set>

fcpp::set<int> numbers({1, 2, 3, 4, 5, 7, 8});

// numbers -> fcpp::set<int> numbers({1, 2, 3, 5, 7, 8});
numbers.remove(4);

// numbers -> fcpp::set<int> numbers({1, 2, 3, 5, 7, 8, 10});
numbers.insert(10);

// returns true
numbers.contains(10);

// returns false
numbers.contains(25);

// returns 7
numbers.size();

// removes all keys
numbers.clear();
```
