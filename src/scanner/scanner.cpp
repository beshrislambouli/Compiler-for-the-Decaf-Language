#include "scanner.h"

int scanner () {

    std::string filename = "src/scanner/in.dcf";
    std::ifstream file;
  file.open(filename);

  if (!file.is_open()) {
    std::cout << "Failed to open file: " << filename << "\n";
    return -1;
  }

  // Create lexer
  antlr4::ANTLRInputStream input(file);
  DecafLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  tokens.fill();

  int t = 0 ;
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
            default: type = "";
        }
        if ( token->getText() == "<EOF>" ) {break;}

        std::cout << line << " ";
        if (type != "") std::cout << type << " ";
        std::cout << token->getText() << std::endl ;
    }

    return 0;
}