add_executable(DecafCompiler main.cpp)

add_subdirectory(utils)
add_subdirectory(DecafGrammar)
add_subdirectory(scanner)
add_subdirectory(parser)
add_subdirectory(semantics)
add_subdirectory(CodeGenerator)
add_subdirectory(RegisterAllocator)



target_link_libraries(DecafCompiler PRIVATE utils DecafGrammar scanner parser semantics CodeGenerator RegisterAllocator)
