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
        block -> statements .push_back (get(Statement,statement_ctx));
    }

    return block.release();
}

antlrcpp::Any DecafASTBuilder::visitLocation_Assign_Op(DecafParser::Location_Assign_OpContext *ctx) {
    auto location_assign_op = make_t (Location_Assign_Op);

    location_assign_op -> location = get(Location, ctx->location());

    location_assign_op -> assign_op= get(Assign_Op, ctx->assign_op());

    location_assign_op -> expr = get (Expr, ctx->expr()) ;


    return (Statement*)location_assign_op.release() ;
}

antlrcpp::Any DecafASTBuilder::visitLocation_Incr(DecafParser::Location_IncrContext *ctx) {
    auto location_incr = make_t (Location_Incr);

    location_incr -> location = get(Location,ctx->location());

    location_incr -> increment = get(Increment,ctx->increment());

    return (Statement*)location_incr.release() ;
}

antlrcpp::Any DecafASTBuilder::visitMethod_Call_Stmt(DecafParser::Method_Call_StmtContext *ctx) {
    auto method_call_stmt = make_t (Method_Call_Stmt);

    method_call_stmt -> id = make_Id();

    for (auto extern_arg_ctx : ctx -> extern_arg()) {
        method_call_stmt -> extern_args .push_back (get(Extern_Arg,extern_arg_ctx));
    }

    return (Statement*)method_call_stmt.release() ;
}

antlrcpp::Any DecafASTBuilder::visitIf_Else_Stmt(DecafParser::If_Else_StmtContext *ctx) {
    auto if_else_stmt = make_t (If_Else_Stmt);

    if_else_stmt -> expr_if = get(Expr,ctx->expr());

    if_else_stmt -> block_then = get (Block,ctx->block(0));

    if (ctx->ELSE()) {
        if_else_stmt -> block_else = get (Block,ctx->block(1));
    }

    return (Statement*)if_else_stmt.release();
}

antlrcpp::Any DecafASTBuilder::visitFor_Stmt(DecafParser::For_StmtContext *ctx) {
    auto for_stmt = make_t (For_Stmt);

    for_stmt -> id = make_Id();

    for_stmt -> expr_init = get (Expr,ctx->expr(0));

    for_stmt -> expr_cond = get (Expr,ctx->expr(1));

    for_stmt -> for_update= get (For_Update,ctx->for_update());

    return (Statement*)for_stmt.release ();
}

antlrcpp::Any DecafASTBuilder::visitWhile_Stmt(DecafParser::While_StmtContext *ctx) {
    auto while_stmt = make_t (While_Stmt);

    while_stmt -> expr_cond = get (Expr,ctx->expr());

    while_stmt -> block = get (Block,ctx->block());

    return (Statement*)while_stmt.release();
}

antlrcpp::Any DecafASTBuilder::visitReturn_Stmt(DecafParser::Return_StmtContext *ctx) {
    auto return_stmt = make_t (Return_Stmt);

    return_stmt -> expr = get (Expr, ctx->expr());

    return (Statement*)return_stmt.release();
}

antlrcpp::Any DecafASTBuilder::visitBreak_Stmt(DecafParser::Break_StmtContext *ctx) {
    auto break_stmt = make_t (Break_Stmt);
    return (Statement*)break_stmt.release();
}

antlrcpp::Any DecafASTBuilder::visitContinue_Stmt(DecafParser::Continue_StmtContext *ctx) {
    auto continue_stmt = make_t (Continue_Stmt);
    return (Statement*)continue_stmt.release();
}

antlrcpp::Any DecafASTBuilder::visitFor_Upd_Assign_Op(DecafParser::For_Upd_Assign_OpContext *ctx) {
    auto for_upd_assign_op = make_t (For_Upd_Assign_Op);

    for_upd_assign_op -> location = get(Location, ctx->location());

    for_upd_assign_op -> assign_Op = get(Assign_Op, ctx->assign_op());

    for_upd_assign_op -> expr = get(Expr, ctx->expr());

    return (For_Update*)for_upd_assign_op.release();
}

antlrcpp::Any DecafASTBuilder::visitFor_Upd_Incr(DecafParser::For_Upd_IncrContext *ctx) {
    auto for_upd_incr = make_t (For_Upd_Incr);

    for_upd_incr -> location = get(Location, ctx->location());

    for_upd_incr -> increment= get(Increment, ctx->increment());

    return (For_Update*)for_upd_incr.release();
}

antlrcpp::Any DecafASTBuilder::visitAssign_op(DecafParser::Assign_opContext *ctx) {
    auto assign_op = make_t (Assign_Op);
    return assign_op.release();
}

antlrcpp::Any DecafASTBuilder::visitIncrement(DecafParser::IncrementContext *ctx) {
    auto increment = make_t (Increment);
    return increment.release();
}

antlrcpp::Any DecafASTBuilder::visitLoc_Var(DecafParser::Loc_VarContext *ctx) {
    auto loc_var = make_t (Loc_Var);
    return (Location*)loc_var.release();
}

antlrcpp::Any DecafASTBuilder::visitLoc_Array(DecafParser::Loc_ArrayContext *ctx) {
    auto loc_array = make_t (Loc_Array);
    return (Location*)loc_array.release();
}

antlrcpp::Any DecafASTBuilder::visitLen_Expr(DecafParser::Len_ExprContext *ctx) {
    auto len_expr = make_t (Len_Expr);
    return (Expr*)len_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitAdd_Op_Expr(DecafParser::Add_Op_ExprContext *ctx) {
    auto add_op_expr = make_t (Add_Op_Expr);
    return (Expr*)add_op_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitParen_Expr(DecafParser::Paren_ExprContext *ctx) {
    auto paren_expr = make_t (Paren_Expr);
    return (Expr*)paren_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitINT_Expr(DecafParser::INT_ExprContext *ctx) {
    auto int_expr = make_t (INT_Expr);
    return (Expr*)int_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitLoc_Expr(DecafParser::Loc_ExprContext *ctx) {
    auto loc_expr = make_t (Loc_Expr);
    return (Expr*)loc_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitOr_Op_Expr(DecafParser::Or_Op_ExprContext *ctx) {
    auto loc_expr = make_t (Loc_Expr);
    return (Expr*)loc_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitLONG_Expr(DecafParser::LONG_ExprContext *ctx) {
    auto long_expr = make_t (LONG_Expr);
    return (Expr*)long_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitMethod_Call_Expr(DecafParser::Method_Call_ExprContext *ctx) {
    auto method_call_expr = make_t (Method_Call_Expr);
    return (Expr*)method_call_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitMul_Op_Expr(DecafParser::Mul_Op_ExprContext *ctx) {
    auto mul_op_expr = make_t (Mul_Op_Expr);
    return (Expr*)mul_op_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitEq_Op_Expr(DecafParser::Eq_Op_ExprContext *ctx) {
    auto eq_op_expr = make_t (Eq_Op_Expr);
    return (Expr*)eq_op_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitMinus_Expr(DecafParser::Minus_ExprContext *ctx) {
    auto minus_expr = make_t (Minus_Expr);
    return (Expr*)minus_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitAnd_Op_Expr(DecafParser::And_Op_ExprContext *ctx) {
    auto logic_op_expr = make_t (Logic_Op_Expr);
    return (Expr*)logic_op_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitNot_Expr(DecafParser::Not_ExprContext *ctx) {
    auto not_expr = make_t (Not_Expr);
    return (Expr*)not_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitRel_Op_Expr(DecafParser::Rel_Op_ExprContext *ctx) {
    auto rel_op_expr = make_t (Rel_Op_Expr);
    return (Expr*)rel_op_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitLiteral_Expr(DecafParser::Literal_ExprContext *ctx) {
    auto literal_expr = make_t (Literal_Expr);
    return (Expr*)literal_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitExpr_Arg(DecafParser::Expr_ArgContext *ctx) {
    auto expr_arg = make_t (Expr_Arg);
    return (Extern_Arg*)expr_arg.release();
}

antlrcpp::Any DecafASTBuilder::visitString_Arg(DecafParser::String_ArgContext *ctx) {
    auto string_arg = make_t (String_Arg);
    return (Extern_Arg*)string_arg.release();
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
