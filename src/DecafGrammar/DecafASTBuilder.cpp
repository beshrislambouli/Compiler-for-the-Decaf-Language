#include "DecafASTBuilder.h"

template<typename T>
std::unique_ptr<T> any_to_unique_ptr(antlrcpp::Any obj) {
    if (obj.isNull()) {
        return nullptr;
    }
    return std::unique_ptr<T>(obj.as<T*>());
}

template<typename T>
std::unique_ptr<T> make(antlr4::ParserRuleContext* ctx) {
    return std::make_unique<T>(ctx->getStart()->getLine(), ctx->getStart()->getCharPositionInLine());
}

antlrcpp::Any DecafASTBuilder::visitProgram(DecafParser::ProgramContext *ctx) {
    std::cout << "visitProgram" << std::endl;
    auto program = make <AST::Program>(ctx);

    for (auto import_decl_ctx : ctx -> import_decl() ) {
        auto import_decl = any_to_unique_ptr<AST::Import_Decl>(visitImport_decl(import_decl_ctx));
        program -> import_decls .push_back (std::move(import_decl));
    }

    for (auto field_decl_ctx : ctx -> field_decl()) {
        auto field_decl = any_to_unique_ptr<AST::Field_Decl>(visitField_decl(field_decl_ctx));
        program -> field_decls .push_back (std::move(field_decl));
    }

    for (auto method_decl_ctx : ctx -> method_decl()) {
        auto method_decl = any_to_unique_ptr<AST::Method_Decl>(visitMethod_decl(method_decl_ctx));
        program -> method_decls .push_back (std::move(method_decl));
    }

    return program.release();
}

antlrcpp::Any DecafASTBuilder::visitImport_decl(DecafParser::Import_declContext *ctx) {
    std::cout << "visitImport_decl" << std::endl;
    auto import_decl = make <AST::Import_Decl>(ctx);

    return import_decl.release();

}

antlrcpp::Any DecafASTBuilder::visitField_decl(DecafParser::Field_declContext *ctx) {
    std::cout << "visitField_decl" << std::endl;
    auto field_decl = make <AST::Field_Decl>(ctx);

    return field_decl.release();
    
}

antlrcpp::Any DecafASTBuilder::visitId_Field_Decl(DecafParser::Id_Field_DeclContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitArray_Field_Decl(DecafParser::Array_Field_DeclContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitMethod_decl(DecafParser::Method_declContext *ctx) {
    std::cout << "visitMethod_decl" << std::endl;
    auto method_decl = make <AST::Method_Decl>(ctx);

    return method_decl.release();
}

antlrcpp::Any DecafASTBuilder::visitParameter(DecafParser::ParameterContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitBlock(DecafParser::BlockContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitField_type(DecafParser::Field_typeContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitMethod_type(DecafParser::Method_typeContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitLocation_Assign_Op(DecafParser::Location_Assign_OpContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitLocation_Incr(DecafParser::Location_IncrContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitMethod_Call_Stmt(DecafParser::Method_Call_StmtContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitIf_Else_Stmt(DecafParser::If_Else_StmtContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitFor_Stmt(DecafParser::For_StmtContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitWhile_Stmt(DecafParser::While_StmtContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitReturn_Stmt(DecafParser::Return_StmtContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitBreak_Stmt(DecafParser::Break_StmtContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitContinue_Stmt(DecafParser::Continue_StmtContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitFor_Upd_Assign_Op(DecafParser::For_Upd_Assign_OpContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitFor_Upd_Incr(DecafParser::For_Upd_IncrContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitAssign_op(DecafParser::Assign_opContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitIncrement(DecafParser::IncrementContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitLoc_Var(DecafParser::Loc_VarContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitLoc_Array(DecafParser::Loc_ArrayContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitLen_Expr(DecafParser::Len_ExprContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitAdd_Op_Expr(DecafParser::Add_Op_ExprContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitParen_Expr(DecafParser::Paren_ExprContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitINT_Expr(DecafParser::INT_ExprContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitLoc_Expr(DecafParser::Loc_ExprContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitOr_Op_Expr(DecafParser::Or_Op_ExprContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitLONG_Expr(DecafParser::LONG_ExprContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitMethod_Call_Expr(DecafParser::Method_Call_ExprContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitMul_Op_Expr(DecafParser::Mul_Op_ExprContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitEq_Op_Expr(DecafParser::Eq_Op_ExprContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitMinus_Expr(DecafParser::Minus_ExprContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitAnd_Op_Expr(DecafParser::And_Op_ExprContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitNot_Expr(DecafParser::Not_ExprContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitRel_Op_Expr(DecafParser::Rel_Op_ExprContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitLiteral_Expr(DecafParser::Literal_ExprContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitExpr_Arg(DecafParser::Expr_ArgContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitString_Arg(DecafParser::String_ArgContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitBin_mul_op(DecafParser::Bin_mul_opContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitBin_add_op(DecafParser::Bin_add_opContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitRel_op(DecafParser::Rel_opContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitEq_op(DecafParser::Eq_opContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitInt_Lit(DecafParser::Int_LitContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitLong_Lit(DecafParser::Long_LitContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitChar_Lit(DecafParser::Char_LitContext *ctx) {
    
}

antlrcpp::Any DecafASTBuilder::visitBool_Lit(DecafParser::Bool_LitContext *ctx) {
    
}
