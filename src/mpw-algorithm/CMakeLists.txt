file(GLOB MPW_SOURCES
        "*.c"
        "*.h")

add_library(mpw-algorithm ${MPW_SOURCES})

target_link_libraries(mpw-algorithm scrypt crypto)