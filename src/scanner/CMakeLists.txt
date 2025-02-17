add_library(scanner STATIC)

# Automatically include all `.cpp` files in this directory
file(GLOB_RECURSE SCANNER_SRC "*.cpp")

# Add the source files to the library
target_sources(scanner PRIVATE ${SCANNER_SRC})

# Ensure that the include directories for scanner are added
target_include_directories(scanner PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link against DecafGrammar to access DecafLexer.h
target_link_libraries(scanner PUBLIC DecafGrammar)