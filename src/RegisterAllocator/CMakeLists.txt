add_library(RegisterAllocator STATIC)

# Automatically include all `.cpp` files in this directory
file(GLOB_RECURSE REGISTERALLOCATOR_SRC "*.cpp")

# Add the source files to the library
target_sources(RegisterAllocator PRIVATE ${REGISTERALLOCATOR_SRC})

# Ensure that the include directories for CodeGenerator are added
target_include_directories(RegisterAllocator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link against DecafGrammar to access DecafLexer.h and DecafParser.h
target_link_libraries(RegisterAllocator PUBLIC DecafGrammar)
target_link_libraries(RegisterAllocator PUBLIC semantics)
target_link_libraries(RegisterAllocator PUBLIC CodeGenerator)
target_link_libraries(RegisterAllocator PUBLIC DataFlowAnalysis)

