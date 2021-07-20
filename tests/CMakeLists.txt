set (EXENAME functional_vector_test)

enable_testing()

# Properties->C/C++->General->Additional Include Directories
include_directories ("${PROJECT_SOURCE_DIR}/include")
include_directories (..)

# Collect headers into the variable HEADERS
file (GLOB HEADERS "${PROJECT_SOURCE_DIR}/include/*.h")

# Collect sources into the variable SOURCES
file (GLOB SOURCES "*.cc")

# Create named folders for the sources within the project
source_group("src" FILES ${SOURCES})
source_group("include" FILES ${HEADERS})

# Set Properties->General->Configuration Type to Application(.exe)
# Adds sources to the Solution Explorer
add_executable (${EXENAME} ${SOURCES} ${HEADERS})

# Properties->Linker->Input->Additional Dependencies
target_link_libraries(${EXENAME} gtest_main)

include(GoogleTest)
gtest_discover_tests(${EXENAME})
