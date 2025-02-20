
// Generated from DecafParser.g4 by ANTLR 4.7.2


#include "DecafParserListener.h"

#include "DecafParser.h"


using namespace antlrcpp;
using namespace antlr4;

DecafParser::DecafParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

DecafParser::~DecafParser() {
  delete _interpreter;
}

std::string DecafParser::getGrammarFileName() const {
  return "DecafParser.g4";
}

const std::vector<std::string>& DecafParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& DecafParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- ProgramContext ------------------------------------------------------------------

DecafParser::ProgramContext::ProgramContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* DecafParser::ProgramContext::EOF() {
  return getToken(DecafParser::EOF, 0);
}

std::vector<DecafParser::Import_declContext *> DecafParser::ProgramContext::import_decl() {
  return getRuleContexts<DecafParser::Import_declContext>();
}

DecafParser::Import_declContext* DecafParser::ProgramContext::import_decl(size_t i) {
  return getRuleContext<DecafParser::Import_declContext>(i);
}

std::vector<DecafParser::Field_declContext *> DecafParser::ProgramContext::field_decl() {
  return getRuleContexts<DecafParser::Field_declContext>();
}

DecafParser::Field_declContext* DecafParser::ProgramContext::field_decl(size_t i) {
  return getRuleContext<DecafParser::Field_declContext>(i);
}

std::vector<DecafParser::Method_declContext *> DecafParser::ProgramContext::method_decl() {
  return getRuleContexts<DecafParser::Method_declContext>();
}

DecafParser::Method_declContext* DecafParser::ProgramContext::method_decl(size_t i) {
  return getRuleContext<DecafParser::Method_declContext>(i);
}


size_t DecafParser::ProgramContext::getRuleIndex() const {
  return DecafParser::RuleProgram;
}

void DecafParser::ProgramContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterProgram(this);
}

void DecafParser::ProgramContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitProgram(this);
}

DecafParser::ProgramContext* DecafParser::program() {
  ProgramContext *_localctx = _tracker.createInstance<ProgramContext>(_ctx, getState());
  enterRule(_localctx, 0, DecafParser::RuleProgram);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(51);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == DecafParser::IMPORT) {
      setState(48);
      import_decl();
      setState(53);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(57);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(54);
        field_decl(); 
      }
      setState(59);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx);
    }
    setState(63);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << DecafParser::VOID)
      | (1ULL << DecafParser::INT)
      | (1ULL << DecafParser::LONG)
      | (1ULL << DecafParser::BOOL))) != 0)) {
      setState(60);
      method_decl();
      setState(65);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(66);
    match(DecafParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Import_declContext ------------------------------------------------------------------

DecafParser::Import_declContext::Import_declContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* DecafParser::Import_declContext::IMPORT() {
  return getToken(DecafParser::IMPORT, 0);
}

tree::TerminalNode* DecafParser::Import_declContext::ID() {
  return getToken(DecafParser::ID, 0);
}

tree::TerminalNode* DecafParser::Import_declContext::SEMI() {
  return getToken(DecafParser::SEMI, 0);
}


size_t DecafParser::Import_declContext::getRuleIndex() const {
  return DecafParser::RuleImport_decl;
}

void DecafParser::Import_declContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterImport_decl(this);
}

void DecafParser::Import_declContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitImport_decl(this);
}

DecafParser::Import_declContext* DecafParser::import_decl() {
  Import_declContext *_localctx = _tracker.createInstance<Import_declContext>(_ctx, getState());
  enterRule(_localctx, 2, DecafParser::RuleImport_decl);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(68);
    match(DecafParser::IMPORT);
    setState(69);
    match(DecafParser::ID);
    setState(70);
    match(DecafParser::SEMI);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Field_declContext ------------------------------------------------------------------

DecafParser::Field_declContext::Field_declContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

DecafParser::TypeContext* DecafParser::Field_declContext::type() {
  return getRuleContext<DecafParser::TypeContext>(0);
}

tree::TerminalNode* DecafParser::Field_declContext::SEMI() {
  return getToken(DecafParser::SEMI, 0);
}

std::vector<tree::TerminalNode *> DecafParser::Field_declContext::ID() {
  return getTokens(DecafParser::ID);
}

tree::TerminalNode* DecafParser::Field_declContext::ID(size_t i) {
  return getToken(DecafParser::ID, i);
}

std::vector<DecafParser::Array_field_declContext *> DecafParser::Field_declContext::array_field_decl() {
  return getRuleContexts<DecafParser::Array_field_declContext>();
}

DecafParser::Array_field_declContext* DecafParser::Field_declContext::array_field_decl(size_t i) {
  return getRuleContext<DecafParser::Array_field_declContext>(i);
}

std::vector<tree::TerminalNode *> DecafParser::Field_declContext::COMMA() {
  return getTokens(DecafParser::COMMA);
}

tree::TerminalNode* DecafParser::Field_declContext::COMMA(size_t i) {
  return getToken(DecafParser::COMMA, i);
}


size_t DecafParser::Field_declContext::getRuleIndex() const {
  return DecafParser::RuleField_decl;
}

void DecafParser::Field_declContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterField_decl(this);
}

void DecafParser::Field_declContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitField_decl(this);
}

DecafParser::Field_declContext* DecafParser::field_decl() {
  Field_declContext *_localctx = _tracker.createInstance<Field_declContext>(_ctx, getState());
  enterRule(_localctx, 4, DecafParser::RuleField_decl);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(72);
    type();
    setState(75);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
    case 1: {
      setState(73);
      match(DecafParser::ID);
      break;
    }

    case 2: {
      setState(74);
      array_field_decl();
      break;
    }

    }
    setState(84);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == DecafParser::COMMA) {
      setState(77);
      match(DecafParser::COMMA);
      setState(80);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
      case 1: {
        setState(78);
        match(DecafParser::ID);
        break;
      }

      case 2: {
        setState(79);
        array_field_decl();
        break;
      }

      }
      setState(86);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(87);
    match(DecafParser::SEMI);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Array_field_declContext ------------------------------------------------------------------

DecafParser::Array_field_declContext::Array_field_declContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* DecafParser::Array_field_declContext::ID() {
  return getToken(DecafParser::ID, 0);
}

tree::TerminalNode* DecafParser::Array_field_declContext::SOP() {
  return getToken(DecafParser::SOP, 0);
}

tree::TerminalNode* DecafParser::Array_field_declContext::INT_LITERAL() {
  return getToken(DecafParser::INT_LITERAL, 0);
}

tree::TerminalNode* DecafParser::Array_field_declContext::SCL() {
  return getToken(DecafParser::SCL, 0);
}


size_t DecafParser::Array_field_declContext::getRuleIndex() const {
  return DecafParser::RuleArray_field_decl;
}

void DecafParser::Array_field_declContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArray_field_decl(this);
}

void DecafParser::Array_field_declContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArray_field_decl(this);
}

DecafParser::Array_field_declContext* DecafParser::array_field_decl() {
  Array_field_declContext *_localctx = _tracker.createInstance<Array_field_declContext>(_ctx, getState());
  enterRule(_localctx, 6, DecafParser::RuleArray_field_decl);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(89);
    match(DecafParser::ID);
    setState(90);
    match(DecafParser::SOP);
    setState(91);
    match(DecafParser::INT_LITERAL);
    setState(92);
    match(DecafParser::SCL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Method_declContext ------------------------------------------------------------------

DecafParser::Method_declContext::Method_declContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> DecafParser::Method_declContext::ID() {
  return getTokens(DecafParser::ID);
}

tree::TerminalNode* DecafParser::Method_declContext::ID(size_t i) {
  return getToken(DecafParser::ID, i);
}

tree::TerminalNode* DecafParser::Method_declContext::POP() {
  return getToken(DecafParser::POP, 0);
}

tree::TerminalNode* DecafParser::Method_declContext::PCL() {
  return getToken(DecafParser::PCL, 0);
}

DecafParser::BlockContext* DecafParser::Method_declContext::block() {
  return getRuleContext<DecafParser::BlockContext>(0);
}

std::vector<DecafParser::TypeContext *> DecafParser::Method_declContext::type() {
  return getRuleContexts<DecafParser::TypeContext>();
}

DecafParser::TypeContext* DecafParser::Method_declContext::type(size_t i) {
  return getRuleContext<DecafParser::TypeContext>(i);
}

tree::TerminalNode* DecafParser::Method_declContext::VOID() {
  return getToken(DecafParser::VOID, 0);
}

std::vector<tree::TerminalNode *> DecafParser::Method_declContext::COMMA() {
  return getTokens(DecafParser::COMMA);
}

tree::TerminalNode* DecafParser::Method_declContext::COMMA(size_t i) {
  return getToken(DecafParser::COMMA, i);
}


size_t DecafParser::Method_declContext::getRuleIndex() const {
  return DecafParser::RuleMethod_decl;
}

void DecafParser::Method_declContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMethod_decl(this);
}

void DecafParser::Method_declContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMethod_decl(this);
}

DecafParser::Method_declContext* DecafParser::method_decl() {
  Method_declContext *_localctx = _tracker.createInstance<Method_declContext>(_ctx, getState());
  enterRule(_localctx, 8, DecafParser::RuleMethod_decl);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(96);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case DecafParser::INT:
      case DecafParser::LONG:
      case DecafParser::BOOL: {
        setState(94);
        type();
        break;
      }

      case DecafParser::VOID: {
        setState(95);
        match(DecafParser::VOID);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(98);
    match(DecafParser::ID);
    setState(99);
    match(DecafParser::POP);
    setState(112);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << DecafParser::INT)
      | (1ULL << DecafParser::LONG)
      | (1ULL << DecafParser::BOOL))) != 0)) {
      setState(100);
      type();
      setState(101);
      match(DecafParser::ID);
      setState(109);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == DecafParser::COMMA) {
        setState(103);
        match(DecafParser::COMMA);
        setState(104);
        type();
        setState(105);
        match(DecafParser::ID);
        setState(111);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(114);
    match(DecafParser::PCL);
    setState(115);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockContext ------------------------------------------------------------------

DecafParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* DecafParser::BlockContext::COP() {
  return getToken(DecafParser::COP, 0);
}

tree::TerminalNode* DecafParser::BlockContext::CCL() {
  return getToken(DecafParser::CCL, 0);
}

std::vector<DecafParser::Field_declContext *> DecafParser::BlockContext::field_decl() {
  return getRuleContexts<DecafParser::Field_declContext>();
}

DecafParser::Field_declContext* DecafParser::BlockContext::field_decl(size_t i) {
  return getRuleContext<DecafParser::Field_declContext>(i);
}

std::vector<DecafParser::StatementContext *> DecafParser::BlockContext::statement() {
  return getRuleContexts<DecafParser::StatementContext>();
}

DecafParser::StatementContext* DecafParser::BlockContext::statement(size_t i) {
  return getRuleContext<DecafParser::StatementContext>(i);
}


size_t DecafParser::BlockContext::getRuleIndex() const {
  return DecafParser::RuleBlock;
}

void DecafParser::BlockContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBlock(this);
}

void DecafParser::BlockContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBlock(this);
}

DecafParser::BlockContext* DecafParser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 10, DecafParser::RuleBlock);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(117);
    match(DecafParser::COP);
    setState(121);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << DecafParser::INT)
      | (1ULL << DecafParser::LONG)
      | (1ULL << DecafParser::BOOL))) != 0)) {
      setState(118);
      field_decl();
      setState(123);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(127);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << DecafParser::IF)
      | (1ULL << DecafParser::FOR)
      | (1ULL << DecafParser::WHILE)
      | (1ULL << DecafParser::RETURN)
      | (1ULL << DecafParser::BREAK)
      | (1ULL << DecafParser::CONTINUE)
      | (1ULL << DecafParser::ID))) != 0)) {
      setState(124);
      statement();
      setState(129);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(130);
    match(DecafParser::CCL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeContext ------------------------------------------------------------------

DecafParser::TypeContext::TypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* DecafParser::TypeContext::INT() {
  return getToken(DecafParser::INT, 0);
}

tree::TerminalNode* DecafParser::TypeContext::LONG() {
  return getToken(DecafParser::LONG, 0);
}

tree::TerminalNode* DecafParser::TypeContext::BOOL() {
  return getToken(DecafParser::BOOL, 0);
}


size_t DecafParser::TypeContext::getRuleIndex() const {
  return DecafParser::RuleType;
}

void DecafParser::TypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterType(this);
}

void DecafParser::TypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitType(this);
}

DecafParser::TypeContext* DecafParser::type() {
  TypeContext *_localctx = _tracker.createInstance<TypeContext>(_ctx, getState());
  enterRule(_localctx, 12, DecafParser::RuleType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(132);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << DecafParser::INT)
      | (1ULL << DecafParser::LONG)
      | (1ULL << DecafParser::BOOL))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

DecafParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

DecafParser::LocationContext* DecafParser::StatementContext::location() {
  return getRuleContext<DecafParser::LocationContext>(0);
}

DecafParser::Assign_exprContext* DecafParser::StatementContext::assign_expr() {
  return getRuleContext<DecafParser::Assign_exprContext>(0);
}

std::vector<tree::TerminalNode *> DecafParser::StatementContext::SEMI() {
  return getTokens(DecafParser::SEMI);
}

tree::TerminalNode* DecafParser::StatementContext::SEMI(size_t i) {
  return getToken(DecafParser::SEMI, i);
}

DecafParser::Method_callContext* DecafParser::StatementContext::method_call() {
  return getRuleContext<DecafParser::Method_callContext>(0);
}

tree::TerminalNode* DecafParser::StatementContext::IF() {
  return getToken(DecafParser::IF, 0);
}

tree::TerminalNode* DecafParser::StatementContext::POP() {
  return getToken(DecafParser::POP, 0);
}

std::vector<DecafParser::ExprContext *> DecafParser::StatementContext::expr() {
  return getRuleContexts<DecafParser::ExprContext>();
}

DecafParser::ExprContext* DecafParser::StatementContext::expr(size_t i) {
  return getRuleContext<DecafParser::ExprContext>(i);
}

tree::TerminalNode* DecafParser::StatementContext::PCL() {
  return getToken(DecafParser::PCL, 0);
}

std::vector<DecafParser::BlockContext *> DecafParser::StatementContext::block() {
  return getRuleContexts<DecafParser::BlockContext>();
}

DecafParser::BlockContext* DecafParser::StatementContext::block(size_t i) {
  return getRuleContext<DecafParser::BlockContext>(i);
}

tree::TerminalNode* DecafParser::StatementContext::ELSE() {
  return getToken(DecafParser::ELSE, 0);
}

tree::TerminalNode* DecafParser::StatementContext::FOR() {
  return getToken(DecafParser::FOR, 0);
}

tree::TerminalNode* DecafParser::StatementContext::ID() {
  return getToken(DecafParser::ID, 0);
}

tree::TerminalNode* DecafParser::StatementContext::ASSIGN() {
  return getToken(DecafParser::ASSIGN, 0);
}

DecafParser::For_updateContext* DecafParser::StatementContext::for_update() {
  return getRuleContext<DecafParser::For_updateContext>(0);
}

tree::TerminalNode* DecafParser::StatementContext::WHILE() {
  return getToken(DecafParser::WHILE, 0);
}

tree::TerminalNode* DecafParser::StatementContext::RETURN() {
  return getToken(DecafParser::RETURN, 0);
}

tree::TerminalNode* DecafParser::StatementContext::BREAK() {
  return getToken(DecafParser::BREAK, 0);
}

tree::TerminalNode* DecafParser::StatementContext::CONTINUE() {
  return getToken(DecafParser::CONTINUE, 0);
}


size_t DecafParser::StatementContext::getRuleIndex() const {
  return DecafParser::RuleStatement;
}

void DecafParser::StatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStatement(this);
}

void DecafParser::StatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStatement(this);
}

DecafParser::StatementContext* DecafParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 14, DecafParser::RuleStatement);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(177);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(134);
      location();
      setState(135);
      assign_expr();
      setState(136);
      match(DecafParser::SEMI);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(138);
      method_call();
      setState(139);
      match(DecafParser::SEMI);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(141);
      match(DecafParser::IF);
      setState(142);
      match(DecafParser::POP);
      setState(143);
      expr(0);
      setState(144);
      match(DecafParser::PCL);
      setState(145);
      block();
      setState(148);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == DecafParser::ELSE) {
        setState(146);
        match(DecafParser::ELSE);
        setState(147);
        block();
      }
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(150);
      match(DecafParser::FOR);
      setState(151);
      match(DecafParser::POP);
      setState(152);
      match(DecafParser::ID);
      setState(153);
      match(DecafParser::ASSIGN);
      setState(154);
      expr(0);
      setState(155);
      match(DecafParser::SEMI);
      setState(156);
      expr(0);
      setState(157);
      match(DecafParser::SEMI);
      setState(158);
      for_update();
      setState(159);
      match(DecafParser::PCL);
      setState(160);
      block();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(162);
      match(DecafParser::WHILE);
      setState(163);
      match(DecafParser::POP);
      setState(164);
      expr(0);
      setState(165);
      match(DecafParser::PCL);
      setState(166);
      block();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(168);
      match(DecafParser::RETURN);
      setState(170);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << DecafParser::POP)
        | (1ULL << DecafParser::INT)
        | (1ULL << DecafParser::LONG)
        | (1ULL << DecafParser::MINUS)
        | (1ULL << DecafParser::NOT)
        | (1ULL << DecafParser::LEN)
        | (1ULL << DecafParser::CHAR_LITERAL)
        | (1ULL << DecafParser::BOOL_LITERAL)
        | (1ULL << DecafParser::INT_LITERAL)
        | (1ULL << DecafParser::LONG_LITERAL)
        | (1ULL << DecafParser::ID))) != 0)) {
        setState(169);
        expr(0);
      }
      setState(172);
      match(DecafParser::SEMI);
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(173);
      match(DecafParser::BREAK);
      setState(174);
      match(DecafParser::SEMI);
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(175);
      match(DecafParser::CONTINUE);
      setState(176);
      match(DecafParser::SEMI);
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- For_updateContext ------------------------------------------------------------------

DecafParser::For_updateContext::For_updateContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

DecafParser::LocationContext* DecafParser::For_updateContext::location() {
  return getRuleContext<DecafParser::LocationContext>(0);
}

DecafParser::Assign_exprContext* DecafParser::For_updateContext::assign_expr() {
  return getRuleContext<DecafParser::Assign_exprContext>(0);
}


size_t DecafParser::For_updateContext::getRuleIndex() const {
  return DecafParser::RuleFor_update;
}

void DecafParser::For_updateContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFor_update(this);
}

void DecafParser::For_updateContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFor_update(this);
}

DecafParser::For_updateContext* DecafParser::for_update() {
  For_updateContext *_localctx = _tracker.createInstance<For_updateContext>(_ctx, getState());
  enterRule(_localctx, 16, DecafParser::RuleFor_update);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(179);
    location();
    setState(180);
    assign_expr();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Assign_exprContext ------------------------------------------------------------------

DecafParser::Assign_exprContext::Assign_exprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

DecafParser::Assign_opContext* DecafParser::Assign_exprContext::assign_op() {
  return getRuleContext<DecafParser::Assign_opContext>(0);
}

DecafParser::ExprContext* DecafParser::Assign_exprContext::expr() {
  return getRuleContext<DecafParser::ExprContext>(0);
}

DecafParser::IncrementContext* DecafParser::Assign_exprContext::increment() {
  return getRuleContext<DecafParser::IncrementContext>(0);
}


size_t DecafParser::Assign_exprContext::getRuleIndex() const {
  return DecafParser::RuleAssign_expr;
}

void DecafParser::Assign_exprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssign_expr(this);
}

void DecafParser::Assign_exprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssign_expr(this);
}

DecafParser::Assign_exprContext* DecafParser::assign_expr() {
  Assign_exprContext *_localctx = _tracker.createInstance<Assign_exprContext>(_ctx, getState());
  enterRule(_localctx, 18, DecafParser::RuleAssign_expr);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(186);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case DecafParser::ASSIGN:
      case DecafParser::PLUS_ASSIGN:
      case DecafParser::MINUS_ASSIGN:
      case DecafParser::MUL_ASSIGN:
      case DecafParser::DIV_ASSIGN:
      case DecafParser::MOD_ASSIGN: {
        enterOuterAlt(_localctx, 1);
        setState(182);
        assign_op();
        setState(183);
        expr(0);
        break;
      }

      case DecafParser::INCREMENT:
      case DecafParser::DECREMENT: {
        enterOuterAlt(_localctx, 2);
        setState(185);
        increment();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Assign_opContext ------------------------------------------------------------------

DecafParser::Assign_opContext::Assign_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* DecafParser::Assign_opContext::ASSIGN() {
  return getToken(DecafParser::ASSIGN, 0);
}

tree::TerminalNode* DecafParser::Assign_opContext::PLUS_ASSIGN() {
  return getToken(DecafParser::PLUS_ASSIGN, 0);
}

tree::TerminalNode* DecafParser::Assign_opContext::MINUS_ASSIGN() {
  return getToken(DecafParser::MINUS_ASSIGN, 0);
}

tree::TerminalNode* DecafParser::Assign_opContext::MUL_ASSIGN() {
  return getToken(DecafParser::MUL_ASSIGN, 0);
}

tree::TerminalNode* DecafParser::Assign_opContext::DIV_ASSIGN() {
  return getToken(DecafParser::DIV_ASSIGN, 0);
}

tree::TerminalNode* DecafParser::Assign_opContext::MOD_ASSIGN() {
  return getToken(DecafParser::MOD_ASSIGN, 0);
}


size_t DecafParser::Assign_opContext::getRuleIndex() const {
  return DecafParser::RuleAssign_op;
}

void DecafParser::Assign_opContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssign_op(this);
}

void DecafParser::Assign_opContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssign_op(this);
}

DecafParser::Assign_opContext* DecafParser::assign_op() {
  Assign_opContext *_localctx = _tracker.createInstance<Assign_opContext>(_ctx, getState());
  enterRule(_localctx, 20, DecafParser::RuleAssign_op);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(188);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << DecafParser::ASSIGN)
      | (1ULL << DecafParser::PLUS_ASSIGN)
      | (1ULL << DecafParser::MINUS_ASSIGN)
      | (1ULL << DecafParser::MUL_ASSIGN)
      | (1ULL << DecafParser::DIV_ASSIGN)
      | (1ULL << DecafParser::MOD_ASSIGN))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IncrementContext ------------------------------------------------------------------

DecafParser::IncrementContext::IncrementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* DecafParser::IncrementContext::INCREMENT() {
  return getToken(DecafParser::INCREMENT, 0);
}

tree::TerminalNode* DecafParser::IncrementContext::DECREMENT() {
  return getToken(DecafParser::DECREMENT, 0);
}


size_t DecafParser::IncrementContext::getRuleIndex() const {
  return DecafParser::RuleIncrement;
}

void DecafParser::IncrementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIncrement(this);
}

void DecafParser::IncrementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIncrement(this);
}

DecafParser::IncrementContext* DecafParser::increment() {
  IncrementContext *_localctx = _tracker.createInstance<IncrementContext>(_ctx, getState());
  enterRule(_localctx, 22, DecafParser::RuleIncrement);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(190);
    _la = _input->LA(1);
    if (!(_la == DecafParser::INCREMENT

    || _la == DecafParser::DECREMENT)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Method_callContext ------------------------------------------------------------------

DecafParser::Method_callContext::Method_callContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

DecafParser::Method_nameContext* DecafParser::Method_callContext::method_name() {
  return getRuleContext<DecafParser::Method_nameContext>(0);
}

tree::TerminalNode* DecafParser::Method_callContext::POP() {
  return getToken(DecafParser::POP, 0);
}

tree::TerminalNode* DecafParser::Method_callContext::PCL() {
  return getToken(DecafParser::PCL, 0);
}

std::vector<DecafParser::ExprContext *> DecafParser::Method_callContext::expr() {
  return getRuleContexts<DecafParser::ExprContext>();
}

DecafParser::ExprContext* DecafParser::Method_callContext::expr(size_t i) {
  return getRuleContext<DecafParser::ExprContext>(i);
}

std::vector<tree::TerminalNode *> DecafParser::Method_callContext::COMMA() {
  return getTokens(DecafParser::COMMA);
}

tree::TerminalNode* DecafParser::Method_callContext::COMMA(size_t i) {
  return getToken(DecafParser::COMMA, i);
}

std::vector<DecafParser::Extern_argContext *> DecafParser::Method_callContext::extern_arg() {
  return getRuleContexts<DecafParser::Extern_argContext>();
}

DecafParser::Extern_argContext* DecafParser::Method_callContext::extern_arg(size_t i) {
  return getRuleContext<DecafParser::Extern_argContext>(i);
}


size_t DecafParser::Method_callContext::getRuleIndex() const {
  return DecafParser::RuleMethod_call;
}

void DecafParser::Method_callContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMethod_call(this);
}

void DecafParser::Method_callContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMethod_call(this);
}

DecafParser::Method_callContext* DecafParser::method_call() {
  Method_callContext *_localctx = _tracker.createInstance<Method_callContext>(_ctx, getState());
  enterRule(_localctx, 24, DecafParser::RuleMethod_call);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(220);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(192);
      method_name();
      setState(193);
      match(DecafParser::POP);
      setState(202);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << DecafParser::POP)
        | (1ULL << DecafParser::INT)
        | (1ULL << DecafParser::LONG)
        | (1ULL << DecafParser::MINUS)
        | (1ULL << DecafParser::NOT)
        | (1ULL << DecafParser::LEN)
        | (1ULL << DecafParser::CHAR_LITERAL)
        | (1ULL << DecafParser::BOOL_LITERAL)
        | (1ULL << DecafParser::INT_LITERAL)
        | (1ULL << DecafParser::LONG_LITERAL)
        | (1ULL << DecafParser::ID))) != 0)) {
        setState(194);
        expr(0);
        setState(199);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == DecafParser::COMMA) {
          setState(195);
          match(DecafParser::COMMA);
          setState(196);
          expr(0);
          setState(201);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(204);
      match(DecafParser::PCL);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(206);
      method_name();
      setState(207);
      match(DecafParser::POP);
      setState(216);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << DecafParser::POP)
        | (1ULL << DecafParser::INT)
        | (1ULL << DecafParser::LONG)
        | (1ULL << DecafParser::MINUS)
        | (1ULL << DecafParser::NOT)
        | (1ULL << DecafParser::LEN)
        | (1ULL << DecafParser::CHAR_LITERAL)
        | (1ULL << DecafParser::STRING_LITERAL)
        | (1ULL << DecafParser::BOOL_LITERAL)
        | (1ULL << DecafParser::INT_LITERAL)
        | (1ULL << DecafParser::LONG_LITERAL)
        | (1ULL << DecafParser::ID))) != 0)) {
        setState(208);
        extern_arg();
        setState(213);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == DecafParser::COMMA) {
          setState(209);
          match(DecafParser::COMMA);
          setState(210);
          extern_arg();
          setState(215);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(218);
      match(DecafParser::PCL);
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Method_nameContext ------------------------------------------------------------------

DecafParser::Method_nameContext::Method_nameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* DecafParser::Method_nameContext::ID() {
  return getToken(DecafParser::ID, 0);
}


size_t DecafParser::Method_nameContext::getRuleIndex() const {
  return DecafParser::RuleMethod_name;
}

void DecafParser::Method_nameContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMethod_name(this);
}

void DecafParser::Method_nameContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMethod_name(this);
}

DecafParser::Method_nameContext* DecafParser::method_name() {
  Method_nameContext *_localctx = _tracker.createInstance<Method_nameContext>(_ctx, getState());
  enterRule(_localctx, 26, DecafParser::RuleMethod_name);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(222);
    match(DecafParser::ID);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LocationContext ------------------------------------------------------------------

DecafParser::LocationContext::LocationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* DecafParser::LocationContext::ID() {
  return getToken(DecafParser::ID, 0);
}

tree::TerminalNode* DecafParser::LocationContext::SOP() {
  return getToken(DecafParser::SOP, 0);
}

DecafParser::ExprContext* DecafParser::LocationContext::expr() {
  return getRuleContext<DecafParser::ExprContext>(0);
}

tree::TerminalNode* DecafParser::LocationContext::SCL() {
  return getToken(DecafParser::SCL, 0);
}


size_t DecafParser::LocationContext::getRuleIndex() const {
  return DecafParser::RuleLocation;
}

void DecafParser::LocationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLocation(this);
}

void DecafParser::LocationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLocation(this);
}

DecafParser::LocationContext* DecafParser::location() {
  LocationContext *_localctx = _tracker.createInstance<LocationContext>(_ctx, getState());
  enterRule(_localctx, 28, DecafParser::RuleLocation);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(230);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 20, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(224);
      match(DecafParser::ID);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(225);
      match(DecafParser::ID);
      setState(226);
      match(DecafParser::SOP);
      setState(227);
      expr(0);
      setState(228);
      match(DecafParser::SCL);
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

DecafParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

DecafParser::LocationContext* DecafParser::ExprContext::location() {
  return getRuleContext<DecafParser::LocationContext>(0);
}

DecafParser::Method_callContext* DecafParser::ExprContext::method_call() {
  return getRuleContext<DecafParser::Method_callContext>(0);
}

DecafParser::LiteralContext* DecafParser::ExprContext::literal() {
  return getRuleContext<DecafParser::LiteralContext>(0);
}

tree::TerminalNode* DecafParser::ExprContext::INT() {
  return getToken(DecafParser::INT, 0);
}

tree::TerminalNode* DecafParser::ExprContext::POP() {
  return getToken(DecafParser::POP, 0);
}

std::vector<DecafParser::ExprContext *> DecafParser::ExprContext::expr() {
  return getRuleContexts<DecafParser::ExprContext>();
}

DecafParser::ExprContext* DecafParser::ExprContext::expr(size_t i) {
  return getRuleContext<DecafParser::ExprContext>(i);
}

tree::TerminalNode* DecafParser::ExprContext::PCL() {
  return getToken(DecafParser::PCL, 0);
}

tree::TerminalNode* DecafParser::ExprContext::LONG() {
  return getToken(DecafParser::LONG, 0);
}

tree::TerminalNode* DecafParser::ExprContext::LEN() {
  return getToken(DecafParser::LEN, 0);
}

tree::TerminalNode* DecafParser::ExprContext::ID() {
  return getToken(DecafParser::ID, 0);
}

tree::TerminalNode* DecafParser::ExprContext::MINUS() {
  return getToken(DecafParser::MINUS, 0);
}

tree::TerminalNode* DecafParser::ExprContext::NOT() {
  return getToken(DecafParser::NOT, 0);
}

DecafParser::Bin_opContext* DecafParser::ExprContext::bin_op() {
  return getRuleContext<DecafParser::Bin_opContext>(0);
}


size_t DecafParser::ExprContext::getRuleIndex() const {
  return DecafParser::RuleExpr;
}

void DecafParser::ExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpr(this);
}

void DecafParser::ExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpr(this);
}


DecafParser::ExprContext* DecafParser::expr() {
   return expr(0);
}

DecafParser::ExprContext* DecafParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  DecafParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  DecafParser::ExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 30;
  enterRecursionRule(_localctx, 30, DecafParser::RuleExpr, precedence);

    

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(258);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 21, _ctx)) {
    case 1: {
      setState(233);
      location();
      break;
    }

    case 2: {
      setState(234);
      method_call();
      break;
    }

    case 3: {
      setState(235);
      literal();
      break;
    }

    case 4: {
      setState(236);
      match(DecafParser::INT);
      setState(237);
      match(DecafParser::POP);
      setState(238);
      expr(0);
      setState(239);
      match(DecafParser::PCL);
      break;
    }

    case 5: {
      setState(241);
      match(DecafParser::LONG);
      setState(242);
      match(DecafParser::POP);
      setState(243);
      expr(0);
      setState(244);
      match(DecafParser::PCL);
      break;
    }

    case 6: {
      setState(246);
      match(DecafParser::LEN);
      setState(247);
      match(DecafParser::POP);
      setState(248);
      match(DecafParser::ID);
      setState(249);
      match(DecafParser::PCL);
      break;
    }

    case 7: {
      setState(250);
      match(DecafParser::MINUS);
      setState(251);
      expr(3);
      break;
    }

    case 8: {
      setState(252);
      match(DecafParser::NOT);
      setState(253);
      expr(2);
      break;
    }

    case 9: {
      setState(254);
      match(DecafParser::POP);
      setState(255);
      expr(0);
      setState(256);
      match(DecafParser::PCL);
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(266);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<ExprContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleExpr);
        setState(260);

        if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
        setState(261);
        bin_op();
        setState(262);
        expr(5); 
      }
      setState(268);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- Extern_argContext ------------------------------------------------------------------

DecafParser::Extern_argContext::Extern_argContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

DecafParser::ExprContext* DecafParser::Extern_argContext::expr() {
  return getRuleContext<DecafParser::ExprContext>(0);
}

tree::TerminalNode* DecafParser::Extern_argContext::STRING_LITERAL() {
  return getToken(DecafParser::STRING_LITERAL, 0);
}


size_t DecafParser::Extern_argContext::getRuleIndex() const {
  return DecafParser::RuleExtern_arg;
}

void DecafParser::Extern_argContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExtern_arg(this);
}

void DecafParser::Extern_argContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExtern_arg(this);
}

DecafParser::Extern_argContext* DecafParser::extern_arg() {
  Extern_argContext *_localctx = _tracker.createInstance<Extern_argContext>(_ctx, getState());
  enterRule(_localctx, 32, DecafParser::RuleExtern_arg);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(271);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case DecafParser::POP:
      case DecafParser::INT:
      case DecafParser::LONG:
      case DecafParser::MINUS:
      case DecafParser::NOT:
      case DecafParser::LEN:
      case DecafParser::CHAR_LITERAL:
      case DecafParser::BOOL_LITERAL:
      case DecafParser::INT_LITERAL:
      case DecafParser::LONG_LITERAL:
      case DecafParser::ID: {
        enterOuterAlt(_localctx, 1);
        setState(269);
        expr(0);
        break;
      }

      case DecafParser::STRING_LITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(270);
        match(DecafParser::STRING_LITERAL);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Bin_opContext ------------------------------------------------------------------

DecafParser::Bin_opContext::Bin_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

DecafParser::Arith_opContext* DecafParser::Bin_opContext::arith_op() {
  return getRuleContext<DecafParser::Arith_opContext>(0);
}

DecafParser::Rel_opContext* DecafParser::Bin_opContext::rel_op() {
  return getRuleContext<DecafParser::Rel_opContext>(0);
}

DecafParser::Eq_opContext* DecafParser::Bin_opContext::eq_op() {
  return getRuleContext<DecafParser::Eq_opContext>(0);
}

DecafParser::Cond_opContext* DecafParser::Bin_opContext::cond_op() {
  return getRuleContext<DecafParser::Cond_opContext>(0);
}


size_t DecafParser::Bin_opContext::getRuleIndex() const {
  return DecafParser::RuleBin_op;
}

void DecafParser::Bin_opContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBin_op(this);
}

void DecafParser::Bin_opContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBin_op(this);
}

DecafParser::Bin_opContext* DecafParser::bin_op() {
  Bin_opContext *_localctx = _tracker.createInstance<Bin_opContext>(_ctx, getState());
  enterRule(_localctx, 34, DecafParser::RuleBin_op);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(277);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case DecafParser::MINUS:
      case DecafParser::PLUS:
      case DecafParser::STAR:
      case DecafParser::DIV:
      case DecafParser::MOD: {
        enterOuterAlt(_localctx, 1);
        setState(273);
        arith_op();
        break;
      }

      case DecafParser::LT:
      case DecafParser::GT:
      case DecafParser::LE:
      case DecafParser::GE: {
        enterOuterAlt(_localctx, 2);
        setState(274);
        rel_op();
        break;
      }

      case DecafParser::EQ:
      case DecafParser::NEQ: {
        enterOuterAlt(_localctx, 3);
        setState(275);
        eq_op();
        break;
      }

      case DecafParser::AND:
      case DecafParser::OR: {
        enterOuterAlt(_localctx, 4);
        setState(276);
        cond_op();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Arith_opContext ------------------------------------------------------------------

DecafParser::Arith_opContext::Arith_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* DecafParser::Arith_opContext::PLUS() {
  return getToken(DecafParser::PLUS, 0);
}

tree::TerminalNode* DecafParser::Arith_opContext::MINUS() {
  return getToken(DecafParser::MINUS, 0);
}

tree::TerminalNode* DecafParser::Arith_opContext::STAR() {
  return getToken(DecafParser::STAR, 0);
}

tree::TerminalNode* DecafParser::Arith_opContext::DIV() {
  return getToken(DecafParser::DIV, 0);
}

tree::TerminalNode* DecafParser::Arith_opContext::MOD() {
  return getToken(DecafParser::MOD, 0);
}


size_t DecafParser::Arith_opContext::getRuleIndex() const {
  return DecafParser::RuleArith_op;
}

void DecafParser::Arith_opContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArith_op(this);
}

void DecafParser::Arith_opContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArith_op(this);
}

DecafParser::Arith_opContext* DecafParser::arith_op() {
  Arith_opContext *_localctx = _tracker.createInstance<Arith_opContext>(_ctx, getState());
  enterRule(_localctx, 36, DecafParser::RuleArith_op);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(279);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << DecafParser::MINUS)
      | (1ULL << DecafParser::PLUS)
      | (1ULL << DecafParser::STAR)
      | (1ULL << DecafParser::DIV)
      | (1ULL << DecafParser::MOD))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Rel_opContext ------------------------------------------------------------------

DecafParser::Rel_opContext::Rel_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* DecafParser::Rel_opContext::LT() {
  return getToken(DecafParser::LT, 0);
}

tree::TerminalNode* DecafParser::Rel_opContext::GT() {
  return getToken(DecafParser::GT, 0);
}

tree::TerminalNode* DecafParser::Rel_opContext::LE() {
  return getToken(DecafParser::LE, 0);
}

tree::TerminalNode* DecafParser::Rel_opContext::GE() {
  return getToken(DecafParser::GE, 0);
}


size_t DecafParser::Rel_opContext::getRuleIndex() const {
  return DecafParser::RuleRel_op;
}

void DecafParser::Rel_opContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRel_op(this);
}

void DecafParser::Rel_opContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRel_op(this);
}

DecafParser::Rel_opContext* DecafParser::rel_op() {
  Rel_opContext *_localctx = _tracker.createInstance<Rel_opContext>(_ctx, getState());
  enterRule(_localctx, 38, DecafParser::RuleRel_op);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(281);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << DecafParser::LT)
      | (1ULL << DecafParser::GT)
      | (1ULL << DecafParser::LE)
      | (1ULL << DecafParser::GE))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Eq_opContext ------------------------------------------------------------------

DecafParser::Eq_opContext::Eq_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* DecafParser::Eq_opContext::EQ() {
  return getToken(DecafParser::EQ, 0);
}

tree::TerminalNode* DecafParser::Eq_opContext::NEQ() {
  return getToken(DecafParser::NEQ, 0);
}


size_t DecafParser::Eq_opContext::getRuleIndex() const {
  return DecafParser::RuleEq_op;
}

void DecafParser::Eq_opContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEq_op(this);
}

void DecafParser::Eq_opContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEq_op(this);
}

DecafParser::Eq_opContext* DecafParser::eq_op() {
  Eq_opContext *_localctx = _tracker.createInstance<Eq_opContext>(_ctx, getState());
  enterRule(_localctx, 40, DecafParser::RuleEq_op);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(283);
    _la = _input->LA(1);
    if (!(_la == DecafParser::EQ

    || _la == DecafParser::NEQ)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Cond_opContext ------------------------------------------------------------------

DecafParser::Cond_opContext::Cond_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* DecafParser::Cond_opContext::AND() {
  return getToken(DecafParser::AND, 0);
}

tree::TerminalNode* DecafParser::Cond_opContext::OR() {
  return getToken(DecafParser::OR, 0);
}


size_t DecafParser::Cond_opContext::getRuleIndex() const {
  return DecafParser::RuleCond_op;
}

void DecafParser::Cond_opContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCond_op(this);
}

void DecafParser::Cond_opContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCond_op(this);
}

DecafParser::Cond_opContext* DecafParser::cond_op() {
  Cond_opContext *_localctx = _tracker.createInstance<Cond_opContext>(_ctx, getState());
  enterRule(_localctx, 42, DecafParser::RuleCond_op);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(285);
    _la = _input->LA(1);
    if (!(_la == DecafParser::AND

    || _la == DecafParser::OR)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LiteralContext ------------------------------------------------------------------

DecafParser::LiteralContext::LiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* DecafParser::LiteralContext::INT_LITERAL() {
  return getToken(DecafParser::INT_LITERAL, 0);
}

tree::TerminalNode* DecafParser::LiteralContext::MINUS() {
  return getToken(DecafParser::MINUS, 0);
}

tree::TerminalNode* DecafParser::LiteralContext::LONG_LITERAL() {
  return getToken(DecafParser::LONG_LITERAL, 0);
}

tree::TerminalNode* DecafParser::LiteralContext::CHAR_LITERAL() {
  return getToken(DecafParser::CHAR_LITERAL, 0);
}

tree::TerminalNode* DecafParser::LiteralContext::BOOL_LITERAL() {
  return getToken(DecafParser::BOOL_LITERAL, 0);
}


size_t DecafParser::LiteralContext::getRuleIndex() const {
  return DecafParser::RuleLiteral;
}

void DecafParser::LiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLiteral(this);
}

void DecafParser::LiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLiteral(this);
}

DecafParser::LiteralContext* DecafParser::literal() {
  LiteralContext *_localctx = _tracker.createInstance<LiteralContext>(_ctx, getState());
  enterRule(_localctx, 44, DecafParser::RuleLiteral);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(294);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case DecafParser::MINUS:
      case DecafParser::INT_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(288);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == DecafParser::MINUS) {
          setState(287);
          match(DecafParser::MINUS);
        }
        setState(290);
        match(DecafParser::INT_LITERAL);
        break;
      }

      case DecafParser::LONG_LITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(291);
        match(DecafParser::LONG_LITERAL);
        break;
      }

      case DecafParser::CHAR_LITERAL: {
        enterOuterAlt(_localctx, 3);
        setState(292);
        match(DecafParser::CHAR_LITERAL);
        break;
      }

      case DecafParser::BOOL_LITERAL: {
        enterOuterAlt(_localctx, 4);
        setState(293);
        match(DecafParser::BOOL_LITERAL);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Alpha_numContext ------------------------------------------------------------------

DecafParser::Alpha_numContext::Alpha_numContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* DecafParser::Alpha_numContext::ALPHA() {
  return getToken(DecafParser::ALPHA, 0);
}

tree::TerminalNode* DecafParser::Alpha_numContext::DIGIT() {
  return getToken(DecafParser::DIGIT, 0);
}


size_t DecafParser::Alpha_numContext::getRuleIndex() const {
  return DecafParser::RuleAlpha_num;
}

void DecafParser::Alpha_numContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAlpha_num(this);
}

void DecafParser::Alpha_numContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DecafParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAlpha_num(this);
}

DecafParser::Alpha_numContext* DecafParser::alpha_num() {
  Alpha_numContext *_localctx = _tracker.createInstance<Alpha_numContext>(_ctx, getState());
  enterRule(_localctx, 46, DecafParser::RuleAlpha_num);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(296);
    _la = _input->LA(1);
    if (!(_la == DecafParser::ALPHA

    || _la == DecafParser::DIGIT)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool DecafParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 15: return exprSempred(dynamic_cast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool DecafParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 4);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> DecafParser::_decisionToDFA;
atn::PredictionContextCache DecafParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN DecafParser::_atn;
std::vector<uint16_t> DecafParser::_serializedATN;

std::vector<std::string> DecafParser::_ruleNames = {
  "program", "import_decl", "field_decl", "array_field_decl", "method_decl", 
  "block", "type", "statement", "for_update", "assign_expr", "assign_op", 
  "increment", "method_call", "method_name", "location", "expr", "extern_arg", 
  "bin_op", "arith_op", "rel_op", "eq_op", "cond_op", "literal", "alpha_num"
};

std::vector<std::string> DecafParser::_literalNames = {
  "", "'import'", "';'", "','", "'['", "']'", "'void'", "'('", "')'", "'{'", 
  "'}'", "'int'", "'long'", "'bool'", "'if'", "'else'", "'for'", "'='", 
  "'while'", "'return'", "'break'", "'continue'", "'+='", "'-='", "'*='", 
  "'/='", "'%='", "'++'", "'--'", "'-'", "'!'", "'len'", "'+'", "'*'", "'/'", 
  "'%'", "'<'", "'>'", "'<='", "'>='", "'=='", "'!='", "'&&'", "'||'"
};

std::vector<std::string> DecafParser::_symbolicNames = {
  "", "IMPORT", "SEMI", "COMMA", "SOP", "SCL", "VOID", "POP", "PCL", "COP", 
  "CCL", "INT", "LONG", "BOOL", "IF", "ELSE", "FOR", "ASSIGN", "WHILE", 
  "RETURN", "BREAK", "CONTINUE", "PLUS_ASSIGN", "MINUS_ASSIGN", "MUL_ASSIGN", 
  "DIV_ASSIGN", "MOD_ASSIGN", "INCREMENT", "DECREMENT", "MINUS", "NOT", 
  "LEN", "PLUS", "STAR", "DIV", "MOD", "LT", "GT", "LE", "GE", "EQ", "NEQ", 
  "AND", "OR", "CHAR_LITERAL", "STRING_LITERAL", "BOOL_LITERAL", "INT_LITERAL", 
  "LONG_LITERAL", "ID", "DECIMAL_LITERAL", "HEX_LITERAL", "WS", "COMMENT", 
  "LINE_COMMENT", "ERROR", "ALPHA", "DIGIT"
};

dfa::Vocabulary DecafParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> DecafParser::_tokenNames;

DecafParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x3b, 0x12d, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
    0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 
    0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 
    0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 0xe, 
    0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 0x4, 
    0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 0x9, 0x14, 0x4, 0x15, 
    0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x4, 0x17, 0x9, 0x17, 0x4, 0x18, 0x9, 
    0x18, 0x4, 0x19, 0x9, 0x19, 0x3, 0x2, 0x7, 0x2, 0x34, 0xa, 0x2, 0xc, 
    0x2, 0xe, 0x2, 0x37, 0xb, 0x2, 0x3, 0x2, 0x7, 0x2, 0x3a, 0xa, 0x2, 0xc, 
    0x2, 0xe, 0x2, 0x3d, 0xb, 0x2, 0x3, 0x2, 0x7, 0x2, 0x40, 0xa, 0x2, 0xc, 
    0x2, 0xe, 0x2, 0x43, 0xb, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x4e, 0xa, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x53, 0xa, 0x4, 0x7, 0x4, 
    0x55, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x58, 0xb, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 
    0x5, 0x6, 0x63, 0xa, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x7, 0x6, 0x6e, 0xa, 0x6, 
    0xc, 0x6, 0xe, 0x6, 0x71, 0xb, 0x6, 0x5, 0x6, 0x73, 0xa, 0x6, 0x3, 0x6, 
    0x3, 0x6, 0x3, 0x6, 0x3, 0x7, 0x3, 0x7, 0x7, 0x7, 0x7a, 0xa, 0x7, 0xc, 
    0x7, 0xe, 0x7, 0x7d, 0xb, 0x7, 0x3, 0x7, 0x7, 0x7, 0x80, 0xa, 0x7, 0xc, 
    0x7, 0xe, 0x7, 0x83, 0xb, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x8, 0x3, 0x8, 
    0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 
    0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 
    0x5, 0x9, 0x97, 0xa, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 
    0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 
    0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 
    0x9, 0x3, 0x9, 0x5, 0x9, 0xad, 0xa, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 
    0x3, 0x9, 0x3, 0x9, 0x5, 0x9, 0xb4, 0xa, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 
    0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x5, 0xb, 0xbd, 0xa, 0xb, 
    0x3, 0xc, 0x3, 0xc, 0x3, 0xd, 0x3, 0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 
    0x3, 0xe, 0x3, 0xe, 0x7, 0xe, 0xc8, 0xa, 0xe, 0xc, 0xe, 0xe, 0xe, 0xcb, 
    0xb, 0xe, 0x5, 0xe, 0xcd, 0xa, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 
    0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x7, 0xe, 0xd6, 0xa, 0xe, 0xc, 0xe, 
    0xe, 0xe, 0xd9, 0xb, 0xe, 0x5, 0xe, 0xdb, 0xa, 0xe, 0x3, 0xe, 0x3, 0xe, 
    0x5, 0xe, 0xdf, 0xa, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 0x10, 0x3, 0x10, 
    0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0xe9, 0xa, 0x10, 
    0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 
    0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 
    0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 
    0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 
    0x5, 0x11, 0x105, 0xa, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 
    0x7, 0x11, 0x10b, 0xa, 0x11, 0xc, 0x11, 0xe, 0x11, 0x10e, 0xb, 0x11, 
    0x3, 0x12, 0x3, 0x12, 0x5, 0x12, 0x112, 0xa, 0x12, 0x3, 0x13, 0x3, 0x13, 
    0x3, 0x13, 0x3, 0x13, 0x5, 0x13, 0x118, 0xa, 0x13, 0x3, 0x14, 0x3, 0x14, 
    0x3, 0x15, 0x3, 0x15, 0x3, 0x16, 0x3, 0x16, 0x3, 0x17, 0x3, 0x17, 0x3, 
    0x18, 0x5, 0x18, 0x123, 0xa, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 
    0x3, 0x18, 0x5, 0x18, 0x129, 0xa, 0x18, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 
    0x2, 0x3, 0x20, 0x1a, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 
    0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 
    0x2c, 0x2e, 0x30, 0x2, 0xa, 0x3, 0x2, 0xd, 0xf, 0x4, 0x2, 0x13, 0x13, 
    0x18, 0x1c, 0x3, 0x2, 0x1d, 0x1e, 0x4, 0x2, 0x1f, 0x1f, 0x22, 0x25, 
    0x3, 0x2, 0x26, 0x29, 0x3, 0x2, 0x2a, 0x2b, 0x3, 0x2, 0x2c, 0x2d, 0x3, 
    0x2, 0x3a, 0x3b, 0x2, 0x140, 0x2, 0x35, 0x3, 0x2, 0x2, 0x2, 0x4, 0x46, 
    0x3, 0x2, 0x2, 0x2, 0x6, 0x4a, 0x3, 0x2, 0x2, 0x2, 0x8, 0x5b, 0x3, 0x2, 
    0x2, 0x2, 0xa, 0x62, 0x3, 0x2, 0x2, 0x2, 0xc, 0x77, 0x3, 0x2, 0x2, 0x2, 
    0xe, 0x86, 0x3, 0x2, 0x2, 0x2, 0x10, 0xb3, 0x3, 0x2, 0x2, 0x2, 0x12, 
    0xb5, 0x3, 0x2, 0x2, 0x2, 0x14, 0xbc, 0x3, 0x2, 0x2, 0x2, 0x16, 0xbe, 
    0x3, 0x2, 0x2, 0x2, 0x18, 0xc0, 0x3, 0x2, 0x2, 0x2, 0x1a, 0xde, 0x3, 
    0x2, 0x2, 0x2, 0x1c, 0xe0, 0x3, 0x2, 0x2, 0x2, 0x1e, 0xe8, 0x3, 0x2, 
    0x2, 0x2, 0x20, 0x104, 0x3, 0x2, 0x2, 0x2, 0x22, 0x111, 0x3, 0x2, 0x2, 
    0x2, 0x24, 0x117, 0x3, 0x2, 0x2, 0x2, 0x26, 0x119, 0x3, 0x2, 0x2, 0x2, 
    0x28, 0x11b, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x11d, 0x3, 0x2, 0x2, 0x2, 0x2c, 
    0x11f, 0x3, 0x2, 0x2, 0x2, 0x2e, 0x128, 0x3, 0x2, 0x2, 0x2, 0x30, 0x12a, 
    0x3, 0x2, 0x2, 0x2, 0x32, 0x34, 0x5, 0x4, 0x3, 0x2, 0x33, 0x32, 0x3, 
    0x2, 0x2, 0x2, 0x34, 0x37, 0x3, 0x2, 0x2, 0x2, 0x35, 0x33, 0x3, 0x2, 
    0x2, 0x2, 0x35, 0x36, 0x3, 0x2, 0x2, 0x2, 0x36, 0x3b, 0x3, 0x2, 0x2, 
    0x2, 0x37, 0x35, 0x3, 0x2, 0x2, 0x2, 0x38, 0x3a, 0x5, 0x6, 0x4, 0x2, 
    0x39, 0x38, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x3b, 
    0x39, 0x3, 0x2, 0x2, 0x2, 0x3b, 0x3c, 0x3, 0x2, 0x2, 0x2, 0x3c, 0x41, 
    0x3, 0x2, 0x2, 0x2, 0x3d, 0x3b, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x40, 0x5, 
    0xa, 0x6, 0x2, 0x3f, 0x3e, 0x3, 0x2, 0x2, 0x2, 0x40, 0x43, 0x3, 0x2, 
    0x2, 0x2, 0x41, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x41, 0x42, 0x3, 0x2, 0x2, 
    0x2, 0x42, 0x44, 0x3, 0x2, 0x2, 0x2, 0x43, 0x41, 0x3, 0x2, 0x2, 0x2, 
    0x44, 0x45, 0x7, 0x2, 0x2, 0x3, 0x45, 0x3, 0x3, 0x2, 0x2, 0x2, 0x46, 
    0x47, 0x7, 0x3, 0x2, 0x2, 0x47, 0x48, 0x7, 0x33, 0x2, 0x2, 0x48, 0x49, 
    0x7, 0x4, 0x2, 0x2, 0x49, 0x5, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x4d, 0x5, 
    0xe, 0x8, 0x2, 0x4b, 0x4e, 0x7, 0x33, 0x2, 0x2, 0x4c, 0x4e, 0x5, 0x8, 
    0x5, 0x2, 0x4d, 0x4b, 0x3, 0x2, 0x2, 0x2, 0x4d, 0x4c, 0x3, 0x2, 0x2, 
    0x2, 0x4e, 0x56, 0x3, 0x2, 0x2, 0x2, 0x4f, 0x52, 0x7, 0x5, 0x2, 0x2, 
    0x50, 0x53, 0x7, 0x33, 0x2, 0x2, 0x51, 0x53, 0x5, 0x8, 0x5, 0x2, 0x52, 
    0x50, 0x3, 0x2, 0x2, 0x2, 0x52, 0x51, 0x3, 0x2, 0x2, 0x2, 0x53, 0x55, 
    0x3, 0x2, 0x2, 0x2, 0x54, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x55, 0x58, 0x3, 
    0x2, 0x2, 0x2, 0x56, 0x54, 0x3, 0x2, 0x2, 0x2, 0x56, 0x57, 0x3, 0x2, 
    0x2, 0x2, 0x57, 0x59, 0x3, 0x2, 0x2, 0x2, 0x58, 0x56, 0x3, 0x2, 0x2, 
    0x2, 0x59, 0x5a, 0x7, 0x4, 0x2, 0x2, 0x5a, 0x7, 0x3, 0x2, 0x2, 0x2, 
    0x5b, 0x5c, 0x7, 0x33, 0x2, 0x2, 0x5c, 0x5d, 0x7, 0x6, 0x2, 0x2, 0x5d, 
    0x5e, 0x7, 0x31, 0x2, 0x2, 0x5e, 0x5f, 0x7, 0x7, 0x2, 0x2, 0x5f, 0x9, 
    0x3, 0x2, 0x2, 0x2, 0x60, 0x63, 0x5, 0xe, 0x8, 0x2, 0x61, 0x63, 0x7, 
    0x8, 0x2, 0x2, 0x62, 0x60, 0x3, 0x2, 0x2, 0x2, 0x62, 0x61, 0x3, 0x2, 
    0x2, 0x2, 0x63, 0x64, 0x3, 0x2, 0x2, 0x2, 0x64, 0x65, 0x7, 0x33, 0x2, 
    0x2, 0x65, 0x72, 0x7, 0x9, 0x2, 0x2, 0x66, 0x67, 0x5, 0xe, 0x8, 0x2, 
    0x67, 0x68, 0x7, 0x33, 0x2, 0x2, 0x68, 0x6f, 0x3, 0x2, 0x2, 0x2, 0x69, 
    0x6a, 0x7, 0x5, 0x2, 0x2, 0x6a, 0x6b, 0x5, 0xe, 0x8, 0x2, 0x6b, 0x6c, 
    0x7, 0x33, 0x2, 0x2, 0x6c, 0x6e, 0x3, 0x2, 0x2, 0x2, 0x6d, 0x69, 0x3, 
    0x2, 0x2, 0x2, 0x6e, 0x71, 0x3, 0x2, 0x2, 0x2, 0x6f, 0x6d, 0x3, 0x2, 
    0x2, 0x2, 0x6f, 0x70, 0x3, 0x2, 0x2, 0x2, 0x70, 0x73, 0x3, 0x2, 0x2, 
    0x2, 0x71, 0x6f, 0x3, 0x2, 0x2, 0x2, 0x72, 0x66, 0x3, 0x2, 0x2, 0x2, 
    0x72, 0x73, 0x3, 0x2, 0x2, 0x2, 0x73, 0x74, 0x3, 0x2, 0x2, 0x2, 0x74, 
    0x75, 0x7, 0xa, 0x2, 0x2, 0x75, 0x76, 0x5, 0xc, 0x7, 0x2, 0x76, 0xb, 
    0x3, 0x2, 0x2, 0x2, 0x77, 0x7b, 0x7, 0xb, 0x2, 0x2, 0x78, 0x7a, 0x5, 
    0x6, 0x4, 0x2, 0x79, 0x78, 0x3, 0x2, 0x2, 0x2, 0x7a, 0x7d, 0x3, 0x2, 
    0x2, 0x2, 0x7b, 0x79, 0x3, 0x2, 0x2, 0x2, 0x7b, 0x7c, 0x3, 0x2, 0x2, 
    0x2, 0x7c, 0x81, 0x3, 0x2, 0x2, 0x2, 0x7d, 0x7b, 0x3, 0x2, 0x2, 0x2, 
    0x7e, 0x80, 0x5, 0x10, 0x9, 0x2, 0x7f, 0x7e, 0x3, 0x2, 0x2, 0x2, 0x80, 
    0x83, 0x3, 0x2, 0x2, 0x2, 0x81, 0x7f, 0x3, 0x2, 0x2, 0x2, 0x81, 0x82, 
    0x3, 0x2, 0x2, 0x2, 0x82, 0x84, 0x3, 0x2, 0x2, 0x2, 0x83, 0x81, 0x3, 
    0x2, 0x2, 0x2, 0x84, 0x85, 0x7, 0xc, 0x2, 0x2, 0x85, 0xd, 0x3, 0x2, 
    0x2, 0x2, 0x86, 0x87, 0x9, 0x2, 0x2, 0x2, 0x87, 0xf, 0x3, 0x2, 0x2, 
    0x2, 0x88, 0x89, 0x5, 0x1e, 0x10, 0x2, 0x89, 0x8a, 0x5, 0x14, 0xb, 0x2, 
    0x8a, 0x8b, 0x7, 0x4, 0x2, 0x2, 0x8b, 0xb4, 0x3, 0x2, 0x2, 0x2, 0x8c, 
    0x8d, 0x5, 0x1a, 0xe, 0x2, 0x8d, 0x8e, 0x7, 0x4, 0x2, 0x2, 0x8e, 0xb4, 
    0x3, 0x2, 0x2, 0x2, 0x8f, 0x90, 0x7, 0x10, 0x2, 0x2, 0x90, 0x91, 0x7, 
    0x9, 0x2, 0x2, 0x91, 0x92, 0x5, 0x20, 0x11, 0x2, 0x92, 0x93, 0x7, 0xa, 
    0x2, 0x2, 0x93, 0x96, 0x5, 0xc, 0x7, 0x2, 0x94, 0x95, 0x7, 0x11, 0x2, 
    0x2, 0x95, 0x97, 0x5, 0xc, 0x7, 0x2, 0x96, 0x94, 0x3, 0x2, 0x2, 0x2, 
    0x96, 0x97, 0x3, 0x2, 0x2, 0x2, 0x97, 0xb4, 0x3, 0x2, 0x2, 0x2, 0x98, 
    0x99, 0x7, 0x12, 0x2, 0x2, 0x99, 0x9a, 0x7, 0x9, 0x2, 0x2, 0x9a, 0x9b, 
    0x7, 0x33, 0x2, 0x2, 0x9b, 0x9c, 0x7, 0x13, 0x2, 0x2, 0x9c, 0x9d, 0x5, 
    0x20, 0x11, 0x2, 0x9d, 0x9e, 0x7, 0x4, 0x2, 0x2, 0x9e, 0x9f, 0x5, 0x20, 
    0x11, 0x2, 0x9f, 0xa0, 0x7, 0x4, 0x2, 0x2, 0xa0, 0xa1, 0x5, 0x12, 0xa, 
    0x2, 0xa1, 0xa2, 0x7, 0xa, 0x2, 0x2, 0xa2, 0xa3, 0x5, 0xc, 0x7, 0x2, 
    0xa3, 0xb4, 0x3, 0x2, 0x2, 0x2, 0xa4, 0xa5, 0x7, 0x14, 0x2, 0x2, 0xa5, 
    0xa6, 0x7, 0x9, 0x2, 0x2, 0xa6, 0xa7, 0x5, 0x20, 0x11, 0x2, 0xa7, 0xa8, 
    0x7, 0xa, 0x2, 0x2, 0xa8, 0xa9, 0x5, 0xc, 0x7, 0x2, 0xa9, 0xb4, 0x3, 
    0x2, 0x2, 0x2, 0xaa, 0xac, 0x7, 0x15, 0x2, 0x2, 0xab, 0xad, 0x5, 0x20, 
    0x11, 0x2, 0xac, 0xab, 0x3, 0x2, 0x2, 0x2, 0xac, 0xad, 0x3, 0x2, 0x2, 
    0x2, 0xad, 0xae, 0x3, 0x2, 0x2, 0x2, 0xae, 0xb4, 0x7, 0x4, 0x2, 0x2, 
    0xaf, 0xb0, 0x7, 0x16, 0x2, 0x2, 0xb0, 0xb4, 0x7, 0x4, 0x2, 0x2, 0xb1, 
    0xb2, 0x7, 0x17, 0x2, 0x2, 0xb2, 0xb4, 0x7, 0x4, 0x2, 0x2, 0xb3, 0x88, 
    0x3, 0x2, 0x2, 0x2, 0xb3, 0x8c, 0x3, 0x2, 0x2, 0x2, 0xb3, 0x8f, 0x3, 
    0x2, 0x2, 0x2, 0xb3, 0x98, 0x3, 0x2, 0x2, 0x2, 0xb3, 0xa4, 0x3, 0x2, 
    0x2, 0x2, 0xb3, 0xaa, 0x3, 0x2, 0x2, 0x2, 0xb3, 0xaf, 0x3, 0x2, 0x2, 
    0x2, 0xb3, 0xb1, 0x3, 0x2, 0x2, 0x2, 0xb4, 0x11, 0x3, 0x2, 0x2, 0x2, 
    0xb5, 0xb6, 0x5, 0x1e, 0x10, 0x2, 0xb6, 0xb7, 0x5, 0x14, 0xb, 0x2, 0xb7, 
    0x13, 0x3, 0x2, 0x2, 0x2, 0xb8, 0xb9, 0x5, 0x16, 0xc, 0x2, 0xb9, 0xba, 
    0x5, 0x20, 0x11, 0x2, 0xba, 0xbd, 0x3, 0x2, 0x2, 0x2, 0xbb, 0xbd, 0x5, 
    0x18, 0xd, 0x2, 0xbc, 0xb8, 0x3, 0x2, 0x2, 0x2, 0xbc, 0xbb, 0x3, 0x2, 
    0x2, 0x2, 0xbd, 0x15, 0x3, 0x2, 0x2, 0x2, 0xbe, 0xbf, 0x9, 0x3, 0x2, 
    0x2, 0xbf, 0x17, 0x3, 0x2, 0x2, 0x2, 0xc0, 0xc1, 0x9, 0x4, 0x2, 0x2, 
    0xc1, 0x19, 0x3, 0x2, 0x2, 0x2, 0xc2, 0xc3, 0x5, 0x1c, 0xf, 0x2, 0xc3, 
    0xcc, 0x7, 0x9, 0x2, 0x2, 0xc4, 0xc9, 0x5, 0x20, 0x11, 0x2, 0xc5, 0xc6, 
    0x7, 0x5, 0x2, 0x2, 0xc6, 0xc8, 0x5, 0x20, 0x11, 0x2, 0xc7, 0xc5, 0x3, 
    0x2, 0x2, 0x2, 0xc8, 0xcb, 0x3, 0x2, 0x2, 0x2, 0xc9, 0xc7, 0x3, 0x2, 
    0x2, 0x2, 0xc9, 0xca, 0x3, 0x2, 0x2, 0x2, 0xca, 0xcd, 0x3, 0x2, 0x2, 
    0x2, 0xcb, 0xc9, 0x3, 0x2, 0x2, 0x2, 0xcc, 0xc4, 0x3, 0x2, 0x2, 0x2, 
    0xcc, 0xcd, 0x3, 0x2, 0x2, 0x2, 0xcd, 0xce, 0x3, 0x2, 0x2, 0x2, 0xce, 
    0xcf, 0x7, 0xa, 0x2, 0x2, 0xcf, 0xdf, 0x3, 0x2, 0x2, 0x2, 0xd0, 0xd1, 
    0x5, 0x1c, 0xf, 0x2, 0xd1, 0xda, 0x7, 0x9, 0x2, 0x2, 0xd2, 0xd7, 0x5, 
    0x22, 0x12, 0x2, 0xd3, 0xd4, 0x7, 0x5, 0x2, 0x2, 0xd4, 0xd6, 0x5, 0x22, 
    0x12, 0x2, 0xd5, 0xd3, 0x3, 0x2, 0x2, 0x2, 0xd6, 0xd9, 0x3, 0x2, 0x2, 
    0x2, 0xd7, 0xd5, 0x3, 0x2, 0x2, 0x2, 0xd7, 0xd8, 0x3, 0x2, 0x2, 0x2, 
    0xd8, 0xdb, 0x3, 0x2, 0x2, 0x2, 0xd9, 0xd7, 0x3, 0x2, 0x2, 0x2, 0xda, 
    0xd2, 0x3, 0x2, 0x2, 0x2, 0xda, 0xdb, 0x3, 0x2, 0x2, 0x2, 0xdb, 0xdc, 
    0x3, 0x2, 0x2, 0x2, 0xdc, 0xdd, 0x7, 0xa, 0x2, 0x2, 0xdd, 0xdf, 0x3, 
    0x2, 0x2, 0x2, 0xde, 0xc2, 0x3, 0x2, 0x2, 0x2, 0xde, 0xd0, 0x3, 0x2, 
    0x2, 0x2, 0xdf, 0x1b, 0x3, 0x2, 0x2, 0x2, 0xe0, 0xe1, 0x7, 0x33, 0x2, 
    0x2, 0xe1, 0x1d, 0x3, 0x2, 0x2, 0x2, 0xe2, 0xe9, 0x7, 0x33, 0x2, 0x2, 
    0xe3, 0xe4, 0x7, 0x33, 0x2, 0x2, 0xe4, 0xe5, 0x7, 0x6, 0x2, 0x2, 0xe5, 
    0xe6, 0x5, 0x20, 0x11, 0x2, 0xe6, 0xe7, 0x7, 0x7, 0x2, 0x2, 0xe7, 0xe9, 
    0x3, 0x2, 0x2, 0x2, 0xe8, 0xe2, 0x3, 0x2, 0x2, 0x2, 0xe8, 0xe3, 0x3, 
    0x2, 0x2, 0x2, 0xe9, 0x1f, 0x3, 0x2, 0x2, 0x2, 0xea, 0xeb, 0x8, 0x11, 
    0x1, 0x2, 0xeb, 0x105, 0x5, 0x1e, 0x10, 0x2, 0xec, 0x105, 0x5, 0x1a, 
    0xe, 0x2, 0xed, 0x105, 0x5, 0x2e, 0x18, 0x2, 0xee, 0xef, 0x7, 0xd, 0x2, 
    0x2, 0xef, 0xf0, 0x7, 0x9, 0x2, 0x2, 0xf0, 0xf1, 0x5, 0x20, 0x11, 0x2, 
    0xf1, 0xf2, 0x7, 0xa, 0x2, 0x2, 0xf2, 0x105, 0x3, 0x2, 0x2, 0x2, 0xf3, 
    0xf4, 0x7, 0xe, 0x2, 0x2, 0xf4, 0xf5, 0x7, 0x9, 0x2, 0x2, 0xf5, 0xf6, 
    0x5, 0x20, 0x11, 0x2, 0xf6, 0xf7, 0x7, 0xa, 0x2, 0x2, 0xf7, 0x105, 0x3, 
    0x2, 0x2, 0x2, 0xf8, 0xf9, 0x7, 0x21, 0x2, 0x2, 0xf9, 0xfa, 0x7, 0x9, 
    0x2, 0x2, 0xfa, 0xfb, 0x7, 0x33, 0x2, 0x2, 0xfb, 0x105, 0x7, 0xa, 0x2, 
    0x2, 0xfc, 0xfd, 0x7, 0x1f, 0x2, 0x2, 0xfd, 0x105, 0x5, 0x20, 0x11, 
    0x5, 0xfe, 0xff, 0x7, 0x20, 0x2, 0x2, 0xff, 0x105, 0x5, 0x20, 0x11, 
    0x4, 0x100, 0x101, 0x7, 0x9, 0x2, 0x2, 0x101, 0x102, 0x5, 0x20, 0x11, 
    0x2, 0x102, 0x103, 0x7, 0xa, 0x2, 0x2, 0x103, 0x105, 0x3, 0x2, 0x2, 
    0x2, 0x104, 0xea, 0x3, 0x2, 0x2, 0x2, 0x104, 0xec, 0x3, 0x2, 0x2, 0x2, 
    0x104, 0xed, 0x3, 0x2, 0x2, 0x2, 0x104, 0xee, 0x3, 0x2, 0x2, 0x2, 0x104, 
    0xf3, 0x3, 0x2, 0x2, 0x2, 0x104, 0xf8, 0x3, 0x2, 0x2, 0x2, 0x104, 0xfc, 
    0x3, 0x2, 0x2, 0x2, 0x104, 0xfe, 0x3, 0x2, 0x2, 0x2, 0x104, 0x100, 0x3, 
    0x2, 0x2, 0x2, 0x105, 0x10c, 0x3, 0x2, 0x2, 0x2, 0x106, 0x107, 0xc, 
    0x6, 0x2, 0x2, 0x107, 0x108, 0x5, 0x24, 0x13, 0x2, 0x108, 0x109, 0x5, 
    0x20, 0x11, 0x7, 0x109, 0x10b, 0x3, 0x2, 0x2, 0x2, 0x10a, 0x106, 0x3, 
    0x2, 0x2, 0x2, 0x10b, 0x10e, 0x3, 0x2, 0x2, 0x2, 0x10c, 0x10a, 0x3, 
    0x2, 0x2, 0x2, 0x10c, 0x10d, 0x3, 0x2, 0x2, 0x2, 0x10d, 0x21, 0x3, 0x2, 
    0x2, 0x2, 0x10e, 0x10c, 0x3, 0x2, 0x2, 0x2, 0x10f, 0x112, 0x5, 0x20, 
    0x11, 0x2, 0x110, 0x112, 0x7, 0x2f, 0x2, 0x2, 0x111, 0x10f, 0x3, 0x2, 
    0x2, 0x2, 0x111, 0x110, 0x3, 0x2, 0x2, 0x2, 0x112, 0x23, 0x3, 0x2, 0x2, 
    0x2, 0x113, 0x118, 0x5, 0x26, 0x14, 0x2, 0x114, 0x118, 0x5, 0x28, 0x15, 
    0x2, 0x115, 0x118, 0x5, 0x2a, 0x16, 0x2, 0x116, 0x118, 0x5, 0x2c, 0x17, 
    0x2, 0x117, 0x113, 0x3, 0x2, 0x2, 0x2, 0x117, 0x114, 0x3, 0x2, 0x2, 
    0x2, 0x117, 0x115, 0x3, 0x2, 0x2, 0x2, 0x117, 0x116, 0x3, 0x2, 0x2, 
    0x2, 0x118, 0x25, 0x3, 0x2, 0x2, 0x2, 0x119, 0x11a, 0x9, 0x5, 0x2, 0x2, 
    0x11a, 0x27, 0x3, 0x2, 0x2, 0x2, 0x11b, 0x11c, 0x9, 0x6, 0x2, 0x2, 0x11c, 
    0x29, 0x3, 0x2, 0x2, 0x2, 0x11d, 0x11e, 0x9, 0x7, 0x2, 0x2, 0x11e, 0x2b, 
    0x3, 0x2, 0x2, 0x2, 0x11f, 0x120, 0x9, 0x8, 0x2, 0x2, 0x120, 0x2d, 0x3, 
    0x2, 0x2, 0x2, 0x121, 0x123, 0x7, 0x1f, 0x2, 0x2, 0x122, 0x121, 0x3, 
    0x2, 0x2, 0x2, 0x122, 0x123, 0x3, 0x2, 0x2, 0x2, 0x123, 0x124, 0x3, 
    0x2, 0x2, 0x2, 0x124, 0x129, 0x7, 0x31, 0x2, 0x2, 0x125, 0x129, 0x7, 
    0x32, 0x2, 0x2, 0x126, 0x129, 0x7, 0x2e, 0x2, 0x2, 0x127, 0x129, 0x7, 
    0x30, 0x2, 0x2, 0x128, 0x122, 0x3, 0x2, 0x2, 0x2, 0x128, 0x125, 0x3, 
    0x2, 0x2, 0x2, 0x128, 0x126, 0x3, 0x2, 0x2, 0x2, 0x128, 0x127, 0x3, 
    0x2, 0x2, 0x2, 0x129, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x12a, 0x12b, 0x9, 0x9, 
    0x2, 0x2, 0x12b, 0x31, 0x3, 0x2, 0x2, 0x2, 0x1d, 0x35, 0x3b, 0x41, 0x4d, 
    0x52, 0x56, 0x62, 0x6f, 0x72, 0x7b, 0x81, 0x96, 0xac, 0xb3, 0xbc, 0xc9, 
    0xcc, 0xd7, 0xda, 0xde, 0xe8, 0x104, 0x10c, 0x111, 0x117, 0x122, 0x128, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

DecafParser::Initializer DecafParser::_init;
