
// Generated from DecafParser.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"
#include "DecafParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by DecafParser.
 */
class  DecafParserListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterProgram(DecafParser::ProgramContext *ctx) = 0;
  virtual void exitProgram(DecafParser::ProgramContext *ctx) = 0;

  virtual void enterImport_decl(DecafParser::Import_declContext *ctx) = 0;
  virtual void exitImport_decl(DecafParser::Import_declContext *ctx) = 0;

  virtual void enterField_decl(DecafParser::Field_declContext *ctx) = 0;
  virtual void exitField_decl(DecafParser::Field_declContext *ctx) = 0;

  virtual void enterArray_field_decl(DecafParser::Array_field_declContext *ctx) = 0;
  virtual void exitArray_field_decl(DecafParser::Array_field_declContext *ctx) = 0;

  virtual void enterMethod_decl(DecafParser::Method_declContext *ctx) = 0;
  virtual void exitMethod_decl(DecafParser::Method_declContext *ctx) = 0;

  virtual void enterBlock(DecafParser::BlockContext *ctx) = 0;
  virtual void exitBlock(DecafParser::BlockContext *ctx) = 0;

  virtual void enterType(DecafParser::TypeContext *ctx) = 0;
  virtual void exitType(DecafParser::TypeContext *ctx) = 0;

  virtual void enterStatement(DecafParser::StatementContext *ctx) = 0;
  virtual void exitStatement(DecafParser::StatementContext *ctx) = 0;

  virtual void enterFor_update(DecafParser::For_updateContext *ctx) = 0;
  virtual void exitFor_update(DecafParser::For_updateContext *ctx) = 0;

  virtual void enterAssign_expr(DecafParser::Assign_exprContext *ctx) = 0;
  virtual void exitAssign_expr(DecafParser::Assign_exprContext *ctx) = 0;

  virtual void enterAssign_op(DecafParser::Assign_opContext *ctx) = 0;
  virtual void exitAssign_op(DecafParser::Assign_opContext *ctx) = 0;

  virtual void enterIncrement(DecafParser::IncrementContext *ctx) = 0;
  virtual void exitIncrement(DecafParser::IncrementContext *ctx) = 0;

  virtual void enterMethod_call(DecafParser::Method_callContext *ctx) = 0;
  virtual void exitMethod_call(DecafParser::Method_callContext *ctx) = 0;

  virtual void enterMethod_name(DecafParser::Method_nameContext *ctx) = 0;
  virtual void exitMethod_name(DecafParser::Method_nameContext *ctx) = 0;

  virtual void enterLocation(DecafParser::LocationContext *ctx) = 0;
  virtual void exitLocation(DecafParser::LocationContext *ctx) = 0;

  virtual void enterExpr(DecafParser::ExprContext *ctx) = 0;
  virtual void exitExpr(DecafParser::ExprContext *ctx) = 0;

  virtual void enterExtern_arg(DecafParser::Extern_argContext *ctx) = 0;
  virtual void exitExtern_arg(DecafParser::Extern_argContext *ctx) = 0;

  virtual void enterBin_op(DecafParser::Bin_opContext *ctx) = 0;
  virtual void exitBin_op(DecafParser::Bin_opContext *ctx) = 0;

  virtual void enterArith_op(DecafParser::Arith_opContext *ctx) = 0;
  virtual void exitArith_op(DecafParser::Arith_opContext *ctx) = 0;

  virtual void enterRel_op(DecafParser::Rel_opContext *ctx) = 0;
  virtual void exitRel_op(DecafParser::Rel_opContext *ctx) = 0;

  virtual void enterEq_op(DecafParser::Eq_opContext *ctx) = 0;
  virtual void exitEq_op(DecafParser::Eq_opContext *ctx) = 0;

  virtual void enterCond_op(DecafParser::Cond_opContext *ctx) = 0;
  virtual void exitCond_op(DecafParser::Cond_opContext *ctx) = 0;

  virtual void enterLiteral(DecafParser::LiteralContext *ctx) = 0;
  virtual void exitLiteral(DecafParser::LiteralContext *ctx) = 0;

  virtual void enterAlpha_num(DecafParser::Alpha_numContext *ctx) = 0;
  virtual void exitAlpha_num(DecafParser::Alpha_numContext *ctx) = 0;


};

