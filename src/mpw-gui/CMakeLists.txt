# Gtkmm is used for the gui
pkg_check_modules(GTKMM gtkmm-3.0)

# Header & Library locations
include_directories(${GTKMM_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/src/mpw-algorithm/ ${CMAKE_SOURCE_DIR}/src/mpw-model/)
link_directories(${GTKMM_LIBRARY_DIRS})

# Other compiler flags
add_definitions(${GTKMM_CFLAGS_OTHER})

# Create the target
file(GLOB MPW_UI_SOURCES
        "*.cpp"
        "*.h")
add_executable(mpw-gui ${MPW_UI_SOURCES})

# Link the required libraries to the target
target_link_libraries(mpw-gui ${GTKMM_LIBRARIES} mpw-algorithm mpw-model)