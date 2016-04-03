# MasterPassword-Gtk
A Gtk (more specifically gtkmm) GUI implementation of the MasterPassword algorithm. Note that this project does not aim to replace the existing implementations, as this was more a test project for myself to play with C++/Gtk+/gtkmm. 

Parts of the code (more specifically /src/mpw-algorithm/) are taken from the original MasterPassword Github repository (https://github.com/Lyndir/MasterPassword). Some minor changes were made to provide compatibility to the rest of the code.

## Project State
Current version: 1.1

Note, that the master branch is now used as a kind of "stable" branch. Development takes place in a separate branch called dev.

## Release Notes

### v1.0
First version. Basic functionality.

### v1.1
* Improved error handling
* Manage Accounts window

## Build
CMake is required to build this project.

The following libraries are required:
* libscrypt (https://github.com/technion/libscrypt)
* libcrypto (https://www.openssl.org/docs/manmaster/crypto/crypto.html)
* libconfig (http://www.hyperrealm.com/libconfig/)
* gtkmm (http://www.gtkmm.org/)

To compile the project, download the dependencies and perform the following steps:

Create a build directory, e.g.:

    mkdir build && cd build

Initialize CMake build files:

    cmake /path/to/project/root

Build using CMake:

    cmake --build /path/to/build/directory

The mpw-gui executable is now located in:

    build/src/mpw-gui/
