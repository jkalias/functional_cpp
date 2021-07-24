set(EXENAME unit_tests)

# Enable test detection
enable_testing()
include(GoogleTest)

# Properties->C/C++->General->Additional Include Directories
include_directories ("${PROJECT_SOURCE_DIR}/src/include")

# Collect test sources into the variable TEST_SOURCES
file (GLOB TEST_SOURCES
      "*.cpp"
      "*.cc")

# Set Properties->General->Configuration Type to Application
add_executable (${EXENAME} ${TEST_SOURCES})

# Properties->Linker->Input->Additional Dependencies
target_link_libraries(${EXENAME} PUBLIC functional_vector gtest_main)

# Creates a folder "executables" and adds target project under it
set_property(TARGET ${EXENAME} PROPERTY FOLDER "executables")

# The official site of Google Test suggests the gtest_discover_tests
# command, but this does not work for Xcode on macOS, therefore we chose
# the legacy gtest_add_tests command
gtest_add_tests(TARGET ${EXENAME} TEST_PREFIX fp:)

# Adds logic to copy executable to destination directory
install (TARGETS ${EXENAME}
		 RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin
		 LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/bin)
