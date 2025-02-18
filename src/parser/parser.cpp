#include "parser.h"

int Parser::parse (std::ifstream& fin, std::ofstream& fout) {

    antlr4::ANTLRInputStream input(fin);
    DecafLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();

    DecafParser parser(&tokens);
    antlr4::tree::ParseTree* tree = parser.program(); 
    if (parser.getNumberOfSyntaxErrors() > 0) {
        return 1; 
    }
    return 0;
}