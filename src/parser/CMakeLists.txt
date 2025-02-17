add_library(parser STATIC)

# Automatically include all `.cpp` files in this directory
file(GLOB_RECURSE PARSER_SRC "*.cpp")

# Add the source files to the library
target_sources(parser PRIVATE ${PARSER_SRC})

# Ensure that the include directories for parser are added
target_include_directories(parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
