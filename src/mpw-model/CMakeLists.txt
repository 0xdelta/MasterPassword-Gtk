# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCFG libconfig++)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${LIBCFG_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/src/mpw-algorithm/)
link_directories(${LIBCFG_LIBRARY_DIRS})

# Add other flags to the compiler
add_definitions(${LIBCFG_CFLAGS_OTHER})

file(GLOB MPW_MODEL_SOURCES
        "*.cpp"
        "*.h")

# Create the target
add_library(mpw-model ${MPW_MODEL_SOURCES})

# The mpw algorithm is required for the model
target_link_libraries(mpw-model ${LIBCFG_LIBRARIES} mpw-algorithm)