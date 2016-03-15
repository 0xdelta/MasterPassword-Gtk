cmake_minimum_required(VERSION 2.8)
project(mpw-gtk)

# Set some general compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall")

# Use pkg-config to resolve libraries
find_package(PkgConfig REQUIRED)

# Subdirectories containing the source files
add_subdirectory(src/mpw-algorithm)
add_subdirectory(src/mpw-model)
add_subdirectory(src/mpw-gui)

# A custom target that copies the ui files to the binary folder
add_custom_target(copy-ui-files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/ui" "$<TARGET_FILE_DIR:mpw-gui>/ui"
        VERBATIM)