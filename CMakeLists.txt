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
        "${CMAKE_SOURCE_DIR}/ui" "$<TARGET_FILE_DIR:mpw-gtk>/ui"
        VERBATIM)

# Package
set(CMAKE_INSTALL_PREFIX /usr)

install(DIRECTORY ui/ DESTINATION share/mpw-gtk/ui)
install(FILES resources/icon_256x256.png DESTINATION share/mpw-gtk/)
install(FILES resources/mpw-gtk.desktop DESTINATION share/applications/)

## General
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "mpw-gtk")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A Gtk (more specifically gtkmm) GUI implementation of the MasterPassword algorithm.")
set(CPACK_PACKAGE_CONTACT "https://github.com/0xdelta/MasterPassword-Gtk")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

## Deb specific
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Lukas (0xdelta)") #required
set(CPACK_DEBIAN_PACKAGE_SECTION "utility")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libgtk-3-0 (>= 3.10), openssl (>= 1.0.1), libconfig9")

include(CPack)