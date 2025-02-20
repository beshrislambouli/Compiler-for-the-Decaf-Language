
// Generated from DecafParser.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"




class  DecafParser : public antlr4::Parser {
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
    LINE_COMMENT = 54, ERROR = 55, ALPHA = 56, DIGIT = 57
  };

  enum {
    RuleProgram = 0, RuleImport_decl = 1, RuleField_decl = 2, RuleArray_field_decl = 3, 
    RuleMethod_decl = 4, RuleBlock = 5, RuleType = 6, RuleStatement = 7, 
    RuleFor_update = 8, RuleAssign_expr = 9, RuleAssign_op = 10, RuleIncrement = 11, 
    RuleMethod_call = 12, RuleMethod_name = 13, RuleLocation = 14, RuleExpr = 15, 
    RuleExtern_arg = 16, RuleBin_op = 17, RuleArith_op = 18, RuleRel_op = 19, 
    RuleEq_op = 20, RuleCond_op = 21, RuleLiteral = 22, RuleAlpha_num = 23
  };

  DecafParser(antlr4::TokenStream *input);
  ~DecafParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class ProgramContext;
  class Import_declContext;
  class Field_declContext;
  class Array_field_declContext;
  class Method_declContext;
  class BlockContext;
  class TypeContext;
  class StatementContext;
  class For_updateContext;
  class Assign_exprContext;
  class Assign_opContext;
  class IncrementContext;
  class Method_callContext;
  class Method_nameContext;
  class LocationContext;
  class ExprContext;
  class Extern_argContext;
  class Bin_opContext;
  class Arith_opContext;
  class Rel_opContext;
  class Eq_opContext;
  class Cond_opContext;
  class LiteralContext;
  class Alpha_numContext; 

  class  ProgramContext : public antlr4::ParserRuleContext {
  public:
    ProgramContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<Import_declContext *> import_decl();
    Import_declContext* import_decl(size_t i);
    std::vector<Field_declContext *> field_decl();
    Field_declContext* field_decl(size_t i);
    std::vector<Method_declContext *> method_decl();
    Method_declContext* method_decl(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ProgramContext* program();

  class  Import_declContext : public antlr4::ParserRuleContext {
  public:
    Import_declContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IMPORT();
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Import_declContext* import_decl();

  class  Field_declContext : public antlr4::ParserRuleContext {
  public:
    Field_declContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeContext *type();
    antlr4::tree::TerminalNode *SEMI();
    std::vector<antlr4::tree::TerminalNode *> ID();
    antlr4::tree::TerminalNode* ID(size_t i);
    std::vector<Array_field_declContext *> array_field_decl();
    Array_field_declContext* array_field_decl(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Field_declContext* field_decl();

  class  Array_field_declContext : public antlr4::ParserRuleContext {
  public:
    Array_field_declContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *SOP();
    antlr4::tree::TerminalNode *INT_LITERAL();
    antlr4::tree::TerminalNode *SCL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Array_field_declContext* array_field_decl();

  class  Method_declContext : public antlr4::ParserRuleContext {
  public:
    Method_declContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> ID();
    antlr4::tree::TerminalNode* ID(size_t i);
    antlr4::tree::TerminalNode *POP();
    antlr4::tree::TerminalNode *PCL();
    BlockContext *block();
    std::vector<TypeContext *> type();
    TypeContext* type(size_t i);
    antlr4::tree::TerminalNode *VOID();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Method_declContext* method_decl();

  class  BlockContext : public antlr4::ParserRuleContext {
  public:
    BlockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *COP();
    antlr4::tree::TerminalNode *CCL();
    std::vector<Field_declContext *> field_decl();
    Field_declContext* field_decl(size_t i);
    std::vector<StatementContext *> statement();
    StatementContext* statement(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  BlockContext* block();

  class  TypeContext : public antlr4::ParserRuleContext {
  public:
    TypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INT();
    antlr4::tree::TerminalNode *LONG();
    antlr4::tree::TerminalNode *BOOL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TypeContext* type();

  class  StatementContext : public antlr4::ParserRuleContext {
  public:
    StatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LocationContext *location();
    Assign_exprContext *assign_expr();
    std::vector<antlr4::tree::TerminalNode *> SEMI();
    antlr4::tree::TerminalNode* SEMI(size_t i);
    Method_callContext *method_call();
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *POP();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *PCL();
    std::vector<BlockContext *> block();
    BlockContext* block(size_t i);
    antlr4::tree::TerminalNode *ELSE();
    antlr4::tree::TerminalNode *FOR();
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *ASSIGN();
    For_updateContext *for_update();
    antlr4::tree::TerminalNode *WHILE();
    antlr4::tree::TerminalNode *RETURN();
    antlr4::tree::TerminalNode *BREAK();
    antlr4::tree::TerminalNode *CONTINUE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  StatementContext* statement();

  class  For_updateContext : public antlr4::ParserRuleContext {
  public:
    For_updateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LocationContext *location();
    Assign_exprContext *assign_expr();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  For_updateContext* for_update();

  class  Assign_exprContext : public antlr4::ParserRuleContext {
  public:
    Assign_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Assign_opContext *assign_op();
    ExprContext *expr();
    IncrementContext *increment();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Assign_exprContext* assign_expr();

  class  Assign_opContext : public antlr4::ParserRuleContext {
  public:
    Assign_opContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ASSIGN();
    antlr4::tree::TerminalNode *PLUS_ASSIGN();
    antlr4::tree::TerminalNode *MINUS_ASSIGN();
    antlr4::tree::TerminalNode *MUL_ASSIGN();
    antlr4::tree::TerminalNode *DIV_ASSIGN();
    antlr4::tree::TerminalNode *MOD_ASSIGN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Assign_opContext* assign_op();

  class  IncrementContext : public antlr4::ParserRuleContext {
  public:
    IncrementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INCREMENT();
    antlr4::tree::TerminalNode *DECREMENT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  IncrementContext* increment();

  class  Method_callContext : public antlr4::ParserRuleContext {
  public:
    Method_callContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Method_nameContext *method_name();
    antlr4::tree::TerminalNode *POP();
    antlr4::tree::TerminalNode *PCL();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    std::vector<Extern_argContext *> extern_arg();
    Extern_argContext* extern_arg(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Method_callContext* method_call();

  class  Method_nameContext : public antlr4::ParserRuleContext {
  public:
    Method_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Method_nameContext* method_name();

  class  LocationContext : public antlr4::ParserRuleContext {
  public:
    LocationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *SOP();
    ExprContext *expr();
    antlr4::tree::TerminalNode *SCL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  LocationContext* location();

  class  ExprContext : public antlr4::ParserRuleContext {
  public:
    ExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LocationContext *location();
    Method_callContext *method_call();
    LiteralContext *literal();
    antlr4::tree::TerminalNode *INT();
    antlr4::tree::TerminalNode *POP();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *PCL();
    antlr4::tree::TerminalNode *LONG();
    antlr4::tree::TerminalNode *LEN();
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *MINUS();
    antlr4::tree::TerminalNode *NOT();
    Bin_opContext *bin_op();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ExprContext* expr();
  ExprContext* expr(int precedence);
  class  Extern_argContext : public antlr4::ParserRuleContext {
  public:
    Extern_argContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExprContext *expr();
    antlr4::tree::TerminalNode *STRING_LITERAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Extern_argContext* extern_arg();

  class  Bin_opContext : public antlr4::ParserRuleContext {
  public:
    Bin_opContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Arith_opContext *arith_op();
    Rel_opContext *rel_op();
    Eq_opContext *eq_op();
    Cond_opContext *cond_op();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Bin_opContext* bin_op();

  class  Arith_opContext : public antlr4::ParserRuleContext {
  public:
    Arith_opContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PLUS();
    antlr4::tree::TerminalNode *MINUS();
    antlr4::tree::TerminalNode *STAR();
    antlr4::tree::TerminalNode *DIV();
    antlr4::tree::TerminalNode *MOD();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Arith_opContext* arith_op();

  class  Rel_opContext : public antlr4::ParserRuleContext {
  public:
    Rel_opContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LT();
    antlr4::tree::TerminalNode *GT();
    antlr4::tree::TerminalNode *LE();
    antlr4::tree::TerminalNode *GE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Rel_opContext* rel_op();

  class  Eq_opContext : public antlr4::ParserRuleContext {
  public:
    Eq_opContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EQ();
    antlr4::tree::TerminalNode *NEQ();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Eq_opContext* eq_op();

  class  Cond_opContext : public antlr4::ParserRuleContext {
  public:
    Cond_opContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AND();
    antlr4::tree::TerminalNode *OR();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Cond_opContext* cond_op();

  class  LiteralContext : public antlr4::ParserRuleContext {
  public:
    LiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INT_LITERAL();
    antlr4::tree::TerminalNode *MINUS();
    antlr4::tree::TerminalNode *LONG_LITERAL();
    antlr4::tree::TerminalNode *CHAR_LITERAL();
    antlr4::tree::TerminalNode *BOOL_LITERAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  LiteralContext* literal();

  class  Alpha_numContext : public antlr4::ParserRuleContext {
  public:
    Alpha_numContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ALPHA();
    antlr4::tree::TerminalNode *DIGIT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Alpha_numContext* alpha_num();


  virtual bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;
  bool exprSempred(ExprContext *_localctx, size_t predicateIndex);

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

