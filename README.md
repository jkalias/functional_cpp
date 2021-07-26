[![CMake Build Matrix](https://github.com/jkalias/functional_vector/actions/workflows/cmake.yml/badge.svg)](https://github.com/jkalias/functional_vector/actions/workflows/cmake.yml)
[![GitHub license](https://img.shields.io/github/license/jkalias/functional_vector)](https://github.com/jkalias/functional_vector/blob/main/LICENSE)
# Say hello to functional C++ vectors
A wrapper for C++ std::vector geared towards functional programming and fluent APIs.
The primary focus is readability at the call site (not performance) and eliminating manual management of vector indices.
This is heavily influenced and inspired by C# and Swift.

## Compilation (Cmake)
### Dependencies
* CMake >= 3.14

### Minimum C++ version
* C++17

An out-of-source build strategy is used. All following examples assume an output build folder named ```build```.
### macOS (Xcode)
```console
cd functional_vector
cmake -S . -B build -G Xcode
```
Then open the generated ```functional_vector.xcodeproj``` in the ```build``` folder.

### macOS and Linux (Makefiles)
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

## Usage
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
const auto ages = functional_vector<int>({32, 45, 37, 23});

// the employees' names
const auto names = functional_vector<std::string>({"Jake", "Anna", "Kate", "Bob"});

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
employees_sorted_by_age.for_each([](const auto& person) {
    std::cout << person.name << " is " << person.age << " years old." << std::endl;
});
```
