cmake_minimum_required(VERSION 2.8)
project(mpw-gtk)

# Set some compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall")

# Add subdirectories for the project files
add_subdirectory(src/mpw-algorithm)
add_subdirectory(src/mpw-model)
add_subdirectory(src/mpw-gui)

# Create a custom target that copies the ui files to the binary folder
add_custom_target(copy-ui-files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/ui" "$<TARGET_FILE_DIR:mpw-gui>/ui"
        VERBATIM)