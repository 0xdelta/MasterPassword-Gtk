# Libcrypto is used for sha265/hmac-sha265
pkg_check_modules(CRYPTO libcrypto)

# Headers & Library locations
include_directories(${CRYPTO_INCLUDE_DIRS})
link_directories(${CRYPTO_LIBRARY_DIRS})

# Add other flags to the compiler
add_definitions(${CRYPTO_CFLAGS_OTHER})

# Create the target
file(GLOB MPW_SOURCES
        "*.c"
        "*.h")
add_library(mpw-algorithm ${MPW_SOURCES})

# Link the required libraries to the target
target_link_libraries(mpw-algorithm scrypt ${CRYPTO_LIBRARIES})