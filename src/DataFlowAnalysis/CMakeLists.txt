add_library(DataFlowAnalysis STATIC)

# Automatically include all `.cpp` files in this directory
file(GLOB_RECURSE DATAFLOWANALYSIS_SRC "*.cpp")

# Add the source files to the library
target_sources(DataFlowAnalysis PRIVATE ${DATAFLOWANALYSIS_SRC})

# Ensure that the include directories for CodeGenerator are added
target_include_directories(DataFlowAnalysis PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link against DecafGrammar to access DecafLexer.h and DecafParser.h
target_link_libraries(DataFlowAnalysis PUBLIC DecafGrammar)
target_link_libraries(DataFlowAnalysis PUBLIC semantics)
target_link_libraries(DataFlowAnalysis PUBLIC CodeGenerator)
# target_link_libraries(DataFlowAnalysis PUBLIC RegisterAllocator)