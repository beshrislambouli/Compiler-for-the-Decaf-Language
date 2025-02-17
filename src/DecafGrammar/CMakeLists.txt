# Generate the grammar CPP files in the 'generated' directory
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_LIST_DIR}/generated/DecafLexer.cpp
    COMMAND antlr4 -Dlanguage=Cpp -o generated DecafLexer.g4
    COMMAND sed -i 's/u8//g' generated/DecafLexer.cpp
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    DEPENDS DecafLexer.g4
)

add_library(DecafGrammar STATIC generated/DecafLexer.cpp)
target_link_libraries(DecafGrammar PUBLIC /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}/libantlr4-runtime.a)
target_include_directories(DecafGrammar PUBLIC /usr/include/antlr4-runtime ${CMAKE_CURRENT_LIST_DIR}/generated)