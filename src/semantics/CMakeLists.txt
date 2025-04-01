add_library(semantics STATIC)

# Automatically include all `.cpp` files in this directory
file(GLOB_RECURSE SEMANTICS_SRC "*.cpp")

# Add the source files to the library
target_sources(semantics PRIVATE ${SEMANTICS_SRC})

# Ensure that the include directories for semantics are added
target_include_directories(semantics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link against DecafGrammar to access DecafLexer.h and DecafParser.h
target_link_libraries(semantics PUBLIC DecafGrammar)