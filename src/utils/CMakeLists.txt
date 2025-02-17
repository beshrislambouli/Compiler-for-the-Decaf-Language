add_library(utils STATIC)

# Automatically include all `.cpp` files in this directory
file(GLOB_RECURSE UTILS_SRC "*.cpp")

# Add the source files to the library
target_sources(utils PRIVATE ${UTILS_SRC})

# Ensure that the include directories for utils are added
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
