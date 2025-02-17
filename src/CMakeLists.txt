add_executable(DecafCompiler main.cpp)

add_subdirectory(utils)
add_subdirectory(scanner)
add_subdirectory(parser)


target_link_libraries(DecafCompiler PRIVATE utils scanner parser)
