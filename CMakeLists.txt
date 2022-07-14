cmake_minimum_required(VERSION 3.14)
project(functional_cpp VERSION 1.0)

# GoogleTest requires at least C++11
if(NOT "${CMAKE_CXX_STANDARD}")
  set(CMAKE_CXX_STANDARD 11)
endif()
message("-- C++ standard version: ${CMAKE_CXX_STANDARD}")

# For Windows: Prevent overriding the parent project's compiler/linker settings
if(CMAKE_HOST_WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

# Caching googletest source and libs
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/8d51ffdfab10b3fba636ae69bc03da4b54f8c235.zip
)
FetchContent_MakeAvailable(googletest)

# Set compiler settings based on the current platform
if(CMAKE_HOST_UNIX)
    add_definitions(-w -fpermissive -Wshadow -Wunused -Woverloaded-virtual -Wnon-virtual-dtor -Werror)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -w -Wshadow -Wunused -Woverloaded-virtual -Wnon-virtual-dtor -Werror")
else()
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Ox /Ob2 /Oi /Ot /Oy /GT /GL /W4 /Zc:__cplusplus")
    
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG" )
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /INCREMENTAL:NO /LTCG" )    
    
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /INCREMENTAL:NO /LTCG" )
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /INCREMENTAL:NO /LTCG" )

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()


# Turn on the ability to create folders to organize projects
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Include the testing subdirectory
add_subdirectory(src)
add_subdirectory(tests)
