#pragma once

#include "DecafParserBaseVisitor.h"
#include "DecafAST.h"
#include <memory>

class DecafASTBuilder : public DecafParserBaseVisitor {
public:

  virtual antlrcpp::Any visitProgram(DecafParser::ProgramContext *ctx) override ;

  virtual antlrcpp::Any visitImport_decl(DecafParser::Import_declContext *ctx) override ;

  virtual antlrcpp::Any visitField_decl(DecafParser::Field_declContext *ctx) override ;

  virtual antlrcpp::Any visitId_Field_Decl(DecafParser::Id_Field_DeclContext *ctx) override ;

  virtual antlrcpp::Any visitArray_Field_Decl(DecafParser::Array_Field_DeclContext *ctx) override ;

  virtual antlrcpp::Any visitMethod_decl(DecafParser::Method_declContext *ctx) override ;

  virtual antlrcpp::Any visitParameter(DecafParser::ParameterContext *ctx) override ;

  virtual antlrcpp::Any visitBlock(DecafParser::BlockContext *ctx) override ;

  virtual antlrcpp::Any visitField_type(DecafParser::Field_typeContext *ctx) override ;

  virtual antlrcpp::Any visitMethod_type(DecafParser::Method_typeContext *ctx) override ;

  virtual antlrcpp::Any visitLocation_Assign_Op(DecafParser::Location_Assign_OpContext *ctx) override ;

  virtual antlrcpp::Any visitLocation_Incr(DecafParser::Location_IncrContext *ctx) override ;

  virtual antlrcpp::Any visitMethod_Call_Stmt(DecafParser::Method_Call_StmtContext *ctx) override ;

  virtual antlrcpp::Any visitIf_Else_Stmt(DecafParser::If_Else_StmtContext *ctx) override ;

  virtual antlrcpp::Any visitFor_Stmt(DecafParser::For_StmtContext *ctx) override ;

  virtual antlrcpp::Any visitWhile_Stmt(DecafParser::While_StmtContext *ctx) override ;

  virtual antlrcpp::Any visitReturn_Stmt(DecafParser::Return_StmtContext *ctx) override ;

  virtual antlrcpp::Any visitBreak_Stmt(DecafParser::Break_StmtContext *ctx) override ;

  virtual antlrcpp::Any visitContinue_Stmt(DecafParser::Continue_StmtContext *ctx) override ;

  virtual antlrcpp::Any visitFor_Upd_Assign_Op(DecafParser::For_Upd_Assign_OpContext *ctx) override ;

  virtual antlrcpp::Any visitFor_Upd_Incr(DecafParser::For_Upd_IncrContext *ctx) override ;

  virtual antlrcpp::Any visitAssign_op(DecafParser::Assign_opContext *ctx) override ;

  virtual antlrcpp::Any visitIncrement(DecafParser::IncrementContext *ctx) override ;

  virtual antlrcpp::Any visitLoc_Var(DecafParser::Loc_VarContext *ctx) override ;

  virtual antlrcpp::Any visitLoc_Array(DecafParser::Loc_ArrayContext *ctx) override ;

  virtual antlrcpp::Any visitLen_Expr(DecafParser::Len_ExprContext *ctx) override ;

  virtual antlrcpp::Any visitAdd_Op_Expr(DecafParser::Add_Op_ExprContext *ctx) override ;

  virtual antlrcpp::Any visitParen_Expr(DecafParser::Paren_ExprContext *ctx) override ;

  virtual antlrcpp::Any visitINT_Expr(DecafParser::INT_ExprContext *ctx) override ;

  virtual antlrcpp::Any visitLoc_Expr(DecafParser::Loc_ExprContext *ctx) override ;

  virtual antlrcpp::Any visitOr_Op_Expr(DecafParser::Or_Op_ExprContext *ctx) override ;

  virtual antlrcpp::Any visitLONG_Expr(DecafParser::LONG_ExprContext *ctx) override ;

  virtual antlrcpp::Any visitMethod_Call_Expr(DecafParser::Method_Call_ExprContext *ctx) override ;

  virtual antlrcpp::Any visitMul_Op_Expr(DecafParser::Mul_Op_ExprContext *ctx) override ;

  virtual antlrcpp::Any visitEq_Op_Expr(DecafParser::Eq_Op_ExprContext *ctx) override ;

  virtual antlrcpp::Any visitMinus_Expr(DecafParser::Minus_ExprContext *ctx) override ;

  virtual antlrcpp::Any visitAnd_Op_Expr(DecafParser::And_Op_ExprContext *ctx) override ;

  virtual antlrcpp::Any visitNot_Expr(DecafParser::Not_ExprContext *ctx) override ;

  virtual antlrcpp::Any visitRel_Op_Expr(DecafParser::Rel_Op_ExprContext *ctx) override ;

  virtual antlrcpp::Any visitLiteral_Expr(DecafParser::Literal_ExprContext *ctx) override ;

  virtual antlrcpp::Any visitExpr_Arg(DecafParser::Expr_ArgContext *ctx) override ;

  virtual antlrcpp::Any visitString_Arg(DecafParser::String_ArgContext *ctx) override ;

  virtual antlrcpp::Any visitBin_mul_op(DecafParser::Bin_mul_opContext *ctx) override ;

  virtual antlrcpp::Any visitBin_add_op(DecafParser::Bin_add_opContext *ctx) override ;

  virtual antlrcpp::Any visitRel_op(DecafParser::Rel_opContext *ctx) override ;

  virtual antlrcpp::Any visitEq_op(DecafParser::Eq_opContext *ctx) override ;

  virtual antlrcpp::Any visitInt_Lit(DecafParser::Int_LitContext *ctx) override ;

  virtual antlrcpp::Any visitLong_Lit(DecafParser::Long_LitContext *ctx) override ;

  virtual antlrcpp::Any visitChar_Lit(DecafParser::Char_LitContext *ctx) override ;

  virtual antlrcpp::Any visitBool_Lit(DecafParser::Bool_LitContext *ctx) override ;


};

