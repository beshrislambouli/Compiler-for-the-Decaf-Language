#include "scanner.h"

int Scanner::scan (std::ifstream& fin, std::ofstream& fout) {

    antlr4::ANTLRInputStream input(fin);
    DecafLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();


    for (auto token : tokens.getTokens()) {
        int line = token->getLine();
        std::string type;
        switch (token->getType()) {
            case DecafLexer::CHAR_LITERAL: type = "CHARLITERAL"; break;
            case DecafLexer::INT_LITERAL: type = "INTLITERAL"; break;
            case DecafLexer::LONG_LITERAL: type = "LONGLITERAL"; break;
            case DecafLexer::BOOL_LITERAL: type = "BOOLEANLITERAL"; break;
            case DecafLexer::STRING_LITERAL: type = "STRINGLITERAL"; break;
            case DecafLexer::ID: type = "IDENTIFIER"; break;
            case DecafLexer::ERROR: type = "ERROR"; break;
            default: type = "";
        }

        if ( token->getText() == "<EOF>" ) {break;}
        if (type == "ERROR") {
            fout << "ERROR" << std::endl;
            return 1;
        }

        fout << line << " ";
        if (type != "") fout << type << " ";
        fout << token->getText() << std::endl ;
    }

    return 0;
}