# libconfig++ is used for the user config files
pkg_check_modules(LIBCFG libconfig++)

# Header & Library locations
include_directories(${LIBCFG_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/src/mpw-algorithm/)
link_directories(${LIBCFG_LIBRARY_DIRS})

# Other compiler flags
add_definitions(${LIBCFG_CFLAGS_OTHER})

# Create the target
file(GLOB MPW_MODEL_SOURCES
        "*.cpp"
        "*.h")
add_library(mpw-model ${MPW_MODEL_SOURCES})

# Link the required libraries to the target
target_link_libraries(mpw-model ${LIBCFG_LIBRARIES} mpw-algorithm)