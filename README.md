# MasterPassword-Gtk
A Gtk (more specifically gtkmm) GUI implementation of the MasterPassword algorithm. Note that this project does not aim to replace the existing implementations, as this was more a test project for myself to play with C++/Gtk+/gtkmm. 

Parts of the code (more specifically /src/mpw-algorithm/) are taken from the original MasterPassword Github repository (https://github.com/Lyndir/MasterPassword). Some minor changes were made to provide compatibility to the rest of the code.

## How to use

As already pointed out, this is a test project. If you really want to use it, I strongly recommend to build it yourself (I've got some weird problems when using the same build for multiple platforms). But if you're brave enough, I sometimes publish some pre-build binaries in the [Releases Section](https://github.com/0xdelta/MasterPassword-Gtk/releases).


## Release Notes

Current version: 1.2

Note, that the master branch is now used as a kind of "stable" branch. Development takes place in a separate branch called dev.

#### v1.0
First version. Basic functionality.

#### v1.1
* Improve error handling
* Add Manage Accounts window

#### v1.2
* Improve memory management
* Provide package support for Debian (.deb) installer

## Build
CMake is required to build this project.

The following libraries are required:
* libscrypt (https://github.com/technion/libscrypt)
* libcrypto (https://www.openssl.org/docs/manmaster/crypto/crypto.html)
* libconfig (http://www.hyperrealm.com/libconfig/)
* gtkmm (http://www.gtkmm.org/)

### Dependencies

Most of the dependencies listed above are available under common GNU/Linux distributions.

#### Debian/Ubuntu

    apt-get update
    apt-get install build-essential cmake libscrypt-dev libssl-dev libconfig++-dev libgtkmm-3.0-dev

#### Arch Linux

    pacman -Syu
    pacman -S base-devel cmake openssl libconfig gtkmm3

libscrypt is not available in the main package repositories of Arch, so you may need to compile it yourself.

### Build Masterpassword Gtk

Download the sources (download the tarball or use git clone).

OPTIONAL: Create a build directory, e.g.:

    mkdir build && cd build

Initialize CMake build files:

    cmake /path/to/CMakeLists.txt

Build & install using make:

    make
    make install
