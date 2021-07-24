set (LIBNAME functional_vector)

# Collect files without having to explicitly list each header and source file
file (GLOB HEADERS
	  "include/*.h"
	  "include/*.hpp")

file (GLOB SOURCES
      "*.cpp"
      "*.cc")

# Create named folders for the sources within the project
source_group("include" FILES ${HEADERS})
source_group("src" FILES ${SOURCES})

# Properties->C/C++->General->Additional Include Directories
include_directories ("./include")

# Set Properties->General->Configuration Type to Dynamic Library (.dll/.so/.dylib)
add_library(${LIBNAME} SHARED ${HEADERS} ${SOURCES})

add_compile_definitions()

set_target_properties(${LIBNAME} PROPERTIES 
	OUTPUT_NAME ${LIBNAME})

# Creates a folder "libraries" and adds target project under it
set_property(TARGET ${LIBNAME} PROPERTY FOLDER "lib")

# Adds logic to INSTALL.vcproj to copy math.dll to the destination directory
install (TARGETS ${LIBNAME}
		 RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin
		 LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/bin)
