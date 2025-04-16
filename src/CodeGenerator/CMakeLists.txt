add_library(CodeGenerator STATIC)

# Automatically include all `.cpp` files in this directory
file(GLOB_RECURSE CODEGENERATOR_SRC "*.cpp")

# Add the source files to the library
target_sources(CodeGenerator PRIVATE ${CODEGENERATOR_SRC})

# Ensure that the include directories for CodeGenerator are added
target_include_directories(CodeGenerator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link against DecafGrammar to access DecafLexer.h and DecafParser.h
target_link_libraries(CodeGenerator PUBLIC DecafGrammar)
target_link_libraries(CodeGenerator PUBLIC semantics)