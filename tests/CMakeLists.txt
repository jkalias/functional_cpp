set(EXENAME functional_vector_test)

# Enable test detection
enable_testing()
include(GoogleTest)

# Properties->C/C++->General->Additional Include Directories
include_directories ("${PROJECT_SOURCE_DIR}/include")

# Collect headers into the variable HEADERS
file (GLOB HEADERS "${PROJECT_SOURCE_DIR}/include/*.h")

# Collect test sources into the variable TEST_SOURCES
file (GLOB TEST_SOURCES "*.cc")

# Create named folders for the sources within the project
source_group("src" FILES ${TEST_SOURCES})
source_group("include" FILES ${HEADERS})

# Set Properties->General->Configuration Type to Application
# Adds sources to the Solution Explorer
add_executable (${EXENAME} ${TEST_SOURCES} ${HEADERS})

# Properties->Linker->Input->Additional Dependencies
target_link_libraries(${EXENAME} gtest_main)

# The official site of Google Test suggests the gtest_discover_tests
# command, but this does not work for Xcode on macOS, therefore we chose
# the legacy gtest_add_tests command
gtest_add_tests(TARGET ${EXENAME} TEST_PREFIX fp:)
