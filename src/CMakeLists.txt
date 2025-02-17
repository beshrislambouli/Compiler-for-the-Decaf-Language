add_executable(DecafCompiler main.cpp)

add_subdirectory(utils)

target_link_libraries(DecafCompiler PRIVATE utils)
