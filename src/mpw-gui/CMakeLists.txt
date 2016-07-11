# Gtkmm is used for the gui
pkg_check_modules(GTKMM gtkmm-3.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

# Header & Library locations
include_directories(${GTKMM_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/src/mpw-algorithm/ ${CMAKE_SOURCE_DIR}/src/mpw-model/)
link_directories(${GTKMM_LIBRARY_DIRS})

# Other compiler flags
add_definitions(${GTKMM_CFLAGS_OTHER})

# Create the target
file(GLOB MPW_GUI_SOURCES
        "*.cpp"
        "*.h")
add_executable(mpw-gtk ${MPW_GUI_SOURCES})

# Link the required libraries to the target
target_link_libraries(mpw-gtk ${GTKMM_LIBRARIES} mpw-algorithm mpw-model)

set(CMAKE_INSTALL_PREFIX /usr)
install(TARGETS mpw-gtk DESTINATION bin)