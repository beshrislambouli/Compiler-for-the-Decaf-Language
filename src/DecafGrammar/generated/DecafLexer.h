
// Generated from DecafLexer.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"




class  DecafLexer : public antlr4::Lexer {
public:
  enum {
    IMPORT = 1, SEMI = 2, COMMA = 3, SOP = 4, SCL = 5, VOID = 6, POP = 7, 
    PCL = 8, COP = 9, CCL = 10, INT = 11, LONG = 12, BOOL = 13, IF = 14, 
    ELSE = 15, FOR = 16, ASSIGN = 17, WHILE = 18, RETURN = 19, BREAK = 20, 
    CONTINUE = 21, PLUS_ASSIGN = 22, MINUS_ASSIGN = 23, MUL_ASSIGN = 24, 
    DIV_ASSIGN = 25, MOD_ASSIGN = 26, INCREMENT = 27, DECREMENT = 28, MINUS = 29, 
    NOT = 30, LEN = 31, PLUS = 32, STAR = 33, DIV = 34, MOD = 35, LT = 36, 
    GT = 37, LE = 38, GE = 39, EQ = 40, NEQ = 41, AND = 42, OR = 43, CHAR_LITERAL = 44, 
    STRING_LITERAL = 45, BOOL_LITERAL = 46, INT_LITERAL = 47, LONG_LITERAL = 48, 
    ID = 49, DECIMAL_LITERAL = 50, HEX_LITERAL = 51, WS = 52, COMMENT = 53, 
    LINE_COMMENT = 54, ERROR = 55
  };

  DecafLexer(antlr4::CharStream *input);
  ~DecafLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

