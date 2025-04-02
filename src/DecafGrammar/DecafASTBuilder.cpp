#include "DecafASTBuilder.h"

using namespace AST;

template<typename T>
std::unique_ptr<T> any_to_unique_ptr(antlrcpp::Any obj) {
    if (obj.isNull()) {
        std::cout << "ERROR: any_to_unique_ptr" << std::endl;
        return nullptr;
    }
    return std::unique_ptr<T>(obj.as<T*>());
}


#define make_t(T) std::make_unique<T>(ctx->getStart()->getLine(), ctx->getStart()->getCharPositionInLine())
#define make_Id() std::make_unique<Id>((ctx)->ID()->getText(), (ctx)->ID()->getSymbol()->getLine(), (ctx)->ID()->getSymbol()->getCharPositionInLine())
#define make_Type(T) std::make_unique<Type>(T, ctx->getStart()->getLine(), ctx->getStart()->getCharPositionInLine())
#define get(T, ctx) any_to_unique_ptr<T>(visit(ctx))


antlrcpp::Any DecafASTBuilder::visitProgram(DecafParser::ProgramContext *ctx) {
    auto program = make_t (Program);

    for (auto import_decl_ctx : ctx -> import_decl() ) {
        auto import_decl = get(Import_Decl,import_decl_ctx);
        program -> import_decls .push_back (std::move(import_decl));
    }

    for (auto field_decl_ctx : ctx -> field_decl()) {
        auto field_decl = get(Field_Decl,field_decl_ctx);
        program -> field_decls .push_back (std::move(field_decl));
    }

    for (auto method_decl_ctx : ctx -> method_decl()) {
        auto method_decl = get(Method_Decl,method_decl_ctx);
        program -> method_decls .push_back (std::move(method_decl));
    }

    return program.release();
}

antlrcpp::Any DecafASTBuilder::visitImport_decl(DecafParser::Import_declContext *ctx) {
    auto import_decl = make_t (Import_Decl);

    import_decl -> id = make_Id();

    return import_decl.release();

}

antlrcpp::Any DecafASTBuilder::visitField_decl(DecafParser::Field_declContext *ctx) {
    auto field_decl = make_t (Field_Decl);

    field_decl -> field_type = get(Field_Type,ctx->field_type());

    for (auto field_ctx : ctx -> field() ) {
        field_decl -> fields.push_back (get(Field,field_ctx));
    }

    return field_decl.release();
}

antlrcpp::Any DecafASTBuilder::visitId_Field_Decl(DecafParser::Id_Field_DeclContext *ctx) {
    auto id_field_decl = make_t (Id_Field_Decl);

    id_field_decl -> id = make_Id();

    return (Field*)id_field_decl.release();
}

antlrcpp::Any DecafASTBuilder::visitArray_Field_Decl(DecafParser::Array_Field_DeclContext *ctx) {
    auto array_field_decl = make_t (Array_Field_Decl);

    array_field_decl -> id = make_Id();
    array_field_decl -> size = std::make_unique<Int_Lit>(false, ctx->INT_LITERAL()->getText(), ctx->INT_LITERAL()->getSymbol()->getLine(), ctx->INT_LITERAL()->getSymbol()->getCharPositionInLine());

    return (Field*)array_field_decl.release();
}

antlrcpp::Any DecafASTBuilder::visitMethod_decl(DecafParser::Method_declContext *ctx) {
    auto method_decl = make_t (Method_Decl);

    method_decl -> method_type = get(Method_Type,ctx->method_type());
    
    method_decl -> id = make_Id();

    for (auto parameter_ctx : ctx -> parameter() ) {
        method_decl -> parameters. push_back (get(Parameter,parameter_ctx));
    }

    method_decl -> block = get(Block,ctx->block());

    return method_decl.release();
}

antlrcpp::Any DecafASTBuilder::visitParameter(DecafParser::ParameterContext *ctx) {
    auto parameter = make_t (Parameter);

    parameter -> field_type = get(Field_Type,ctx->field_type());

    parameter -> id = make_Id();

    return parameter.release();
}

antlrcpp::Any DecafASTBuilder::visitMethod_type(DecafParser::Method_typeContext *ctx) {
    auto method_type = make_t (Method_Type);

    if (ctx->INT()) {
        method_type -> type = make_Type (Type::Int);
    } else if (ctx->LONG()) {
        method_type -> type = make_Type (Type::Long);
    } else if (ctx->BOOL()) {
        method_type -> type = make_Type (Type::Bool);
    } else if (ctx->VOID()) {
        method_type -> type = make_Type (Type::Void);
    } else {
        std::cout << "ERROR DecafASTBuilder::visitMethod_type" << std::endl;
    }
    
    return method_type.release();
}

antlrcpp::Any DecafASTBuilder::visitField_type(DecafParser::Field_typeContext *ctx) {
    auto field_type = make_t (Field_Type);

    if (ctx->INT()) {
        field_type -> type = make_Type (Type::Int);
    } else if (ctx->LONG()) {
        field_type -> type = make_Type (Type::Long);
    } else if (ctx->BOOL()) {
        field_type -> type = make_Type (Type::Bool);
    } else {
        std::cout << "ERROR DecafASTBuilder::visitField_type" << std::endl;
    }

    return field_type.release();
}


antlrcpp::Any DecafASTBuilder::visitBlock(DecafParser::BlockContext *ctx) {
    auto block = make_t (Block);

    for (auto field_decl_ctx : ctx->field_decl() ) {
        block -> field_decls .push_back (get(Field_Decl,field_decl_ctx));
    }

    for (auto statement_ctx : ctx->statement() ) {
        // block -> statements .push_back (get(Statement,statement_ctx));
    }

    return block.release();
}

antlrcpp::Any DecafASTBuilder::visitLocation_Assign_Op(DecafParser::Location_Assign_OpContext *ctx) {
    auto location_assign_op = make_t (Location_Assign_Op);
    return location_assign_op.release() ;
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
