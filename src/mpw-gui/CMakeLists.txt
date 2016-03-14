# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${GTKMM_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/src/mpw-algorithm/ ${CMAKE_SOURCE_DIR}/src/mpw-model/)
link_directories(${GTKMM_LIBRARY_DIRS})

# Add other flags to the compiler
add_definitions(${GTKMM_CFLAGS_OTHER})

file(GLOB MPW_UI_SOURCES
        "*.cpp"
        "*.h")

# Create the target
add_executable(mpw-gui ${MPW_UI_SOURCES})

# The mpw algorithm is required to to run mpw-ui
target_link_libraries(mpw-gui ${GTKMM_LIBRARIES} mpw-algorithm mpw-model)