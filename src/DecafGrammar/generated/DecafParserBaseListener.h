
// Generated from DecafParser.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"
#include "DecafParserListener.h"


/**
 * This class provides an empty implementation of DecafParserListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  DecafParserBaseListener : public DecafParserListener {
public:

  virtual void enterProgram(DecafParser::ProgramContext * /*ctx*/) override { }
  virtual void exitProgram(DecafParser::ProgramContext * /*ctx*/) override { }

  virtual void enterImport_decl(DecafParser::Import_declContext * /*ctx*/) override { }
  virtual void exitImport_decl(DecafParser::Import_declContext * /*ctx*/) override { }

  virtual void enterField_decl(DecafParser::Field_declContext * /*ctx*/) override { }
  virtual void exitField_decl(DecafParser::Field_declContext * /*ctx*/) override { }

  virtual void enterArray_field_decl(DecafParser::Array_field_declContext * /*ctx*/) override { }
  virtual void exitArray_field_decl(DecafParser::Array_field_declContext * /*ctx*/) override { }

  virtual void enterMethod_decl(DecafParser::Method_declContext * /*ctx*/) override { }
  virtual void exitMethod_decl(DecafParser::Method_declContext * /*ctx*/) override { }

  virtual void enterBlock(DecafParser::BlockContext * /*ctx*/) override { }
  virtual void exitBlock(DecafParser::BlockContext * /*ctx*/) override { }

  virtual void enterType(DecafParser::TypeContext * /*ctx*/) override { }
  virtual void exitType(DecafParser::TypeContext * /*ctx*/) override { }

  virtual void enterStatement(DecafParser::StatementContext * /*ctx*/) override { }
  virtual void exitStatement(DecafParser::StatementContext * /*ctx*/) override { }

  virtual void enterFor_update(DecafParser::For_updateContext * /*ctx*/) override { }
  virtual void exitFor_update(DecafParser::For_updateContext * /*ctx*/) override { }

  virtual void enterAssign_expr(DecafParser::Assign_exprContext * /*ctx*/) override { }
  virtual void exitAssign_expr(DecafParser::Assign_exprContext * /*ctx*/) override { }

  virtual void enterAssign_op(DecafParser::Assign_opContext * /*ctx*/) override { }
  virtual void exitAssign_op(DecafParser::Assign_opContext * /*ctx*/) override { }

  virtual void enterIncrement(DecafParser::IncrementContext * /*ctx*/) override { }
  virtual void exitIncrement(DecafParser::IncrementContext * /*ctx*/) override { }

  virtual void enterMethod_call(DecafParser::Method_callContext * /*ctx*/) override { }
  virtual void exitMethod_call(DecafParser::Method_callContext * /*ctx*/) override { }

  virtual void enterMethod_name(DecafParser::Method_nameContext * /*ctx*/) override { }
  virtual void exitMethod_name(DecafParser::Method_nameContext * /*ctx*/) override { }

  virtual void enterLocation(DecafParser::LocationContext * /*ctx*/) override { }
  virtual void exitLocation(DecafParser::LocationContext * /*ctx*/) override { }

  virtual void enterExpr(DecafParser::ExprContext * /*ctx*/) override { }
  virtual void exitExpr(DecafParser::ExprContext * /*ctx*/) override { }

  virtual void enterExtern_arg(DecafParser::Extern_argContext * /*ctx*/) override { }
  virtual void exitExtern_arg(DecafParser::Extern_argContext * /*ctx*/) override { }

  virtual void enterBin_op(DecafParser::Bin_opContext * /*ctx*/) override { }
  virtual void exitBin_op(DecafParser::Bin_opContext * /*ctx*/) override { }

  virtual void enterArith_op(DecafParser::Arith_opContext * /*ctx*/) override { }
  virtual void exitArith_op(DecafParser::Arith_opContext * /*ctx*/) override { }

  virtual void enterRel_op(DecafParser::Rel_opContext * /*ctx*/) override { }
  virtual void exitRel_op(DecafParser::Rel_opContext * /*ctx*/) override { }

  virtual void enterEq_op(DecafParser::Eq_opContext * /*ctx*/) override { }
  virtual void exitEq_op(DecafParser::Eq_opContext * /*ctx*/) override { }

  virtual void enterCond_op(DecafParser::Cond_opContext * /*ctx*/) override { }
  virtual void exitCond_op(DecafParser::Cond_opContext * /*ctx*/) override { }

  virtual void enterLiteral(DecafParser::LiteralContext * /*ctx*/) override { }
  virtual void exitLiteral(DecafParser::LiteralContext * /*ctx*/) override { }

  virtual void enterAlpha_num(DecafParser::Alpha_numContext * /*ctx*/) override { }
  virtual void exitAlpha_num(DecafParser::Alpha_numContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

