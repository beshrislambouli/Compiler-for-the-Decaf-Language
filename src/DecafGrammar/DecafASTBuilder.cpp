#include "DecafASTBuilder.h"

using namespace AST;

template<typename T>
std::unique_ptr<T> any_to_unique_ptr(antlrcpp::Any obj) {
    if (obj.isNull()) {
        std::cerr << "ERROR: any_to_unique_ptr" << std::endl;
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
    array_field_decl -> size = std::make_unique<Int_Lit>(ctx->INT_LITERAL()->getText(), ctx->INT_LITERAL()->getSymbol()->getLine(), ctx->INT_LITERAL()->getSymbol()->getCharPositionInLine());

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
        std::cerr << "ERROR DecafASTBuilder::visitMethod_type" << std::endl;
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
        std::cerr << "ERROR DecafASTBuilder::visitField_type" << std::endl;
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

    for_stmt -> block = get (Block, ctx->block() );

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

    if (ctx->expr()) {
        return_stmt -> expr = get (Expr, ctx->expr());
    }

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

    if (ctx->ASSIGN()) {
        assign_op -> type = Assign_Op::ASSIGN;
    } else if (ctx->PLUS_ASSIGN()) {
        assign_op -> type = Assign_Op::PLUS_ASSIGN;
    } else if (ctx->MINUS_ASSIGN()) {
        assign_op -> type = Assign_Op::MINUS_ASSIGN;
    } else if (ctx->MUL_ASSIGN()) {
        assign_op -> type = Assign_Op::MUL_ASSIGN;
    } else if (ctx->DIV_ASSIGN()) {
        assign_op -> type = Assign_Op::DIV_ASSIGN;
    } else if (ctx->MOD_ASSIGN()) {
        assign_op -> type = Assign_Op::MOD_ASSIGN;
    } else {
        std::cerr << "ERROR: DecafASTBuilder::visitAssign_op" << std::endl;
    }

    return assign_op.release();
}

antlrcpp::Any DecafASTBuilder::visitIncrement(DecafParser::IncrementContext *ctx) {
    auto increment = make_t (Increment);

    if (ctx->INCREMENT()) {
        increment -> type = Increment::INCREMENT;
    } else if (ctx->DECREMENT()) {
        increment -> type = Increment::DECREMENT;
    }

    return increment.release();
}

antlrcpp::Any DecafASTBuilder::visitLoc_Var(DecafParser::Loc_VarContext *ctx) {
    auto loc_var = make_t (Loc_Var);

    loc_var -> id = make_Id();

    return (Location*)loc_var.release();
}

antlrcpp::Any DecafASTBuilder::visitLoc_Array(DecafParser::Loc_ArrayContext *ctx) {
    auto loc_array = make_t (Loc_Array);

    loc_array -> id = make_Id();

    loc_array ->expr= get(Expr, ctx->expr());

    return (Location*)loc_array.release();
}

antlrcpp::Any DecafASTBuilder::visitLen_Expr(DecafParser::Len_ExprContext *ctx) {
    auto len_expr = make_t (Len_Expr);

    len_expr -> id = make_Id();

    return (Expr*)len_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitAdd_Op_Expr(DecafParser::Add_Op_ExprContext *ctx) {
    auto add_op_expr = make_t (Add_Op_Expr);

    add_op_expr -> expr_lhs = get (Expr, ctx->expr(0));

    add_op_expr -> bin_op   = get (Bin_Op, ctx->bin_add_op());

    add_op_expr -> expr_rhs = get (Expr, ctx->expr(1));

    return (Expr*)add_op_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitParen_Expr(DecafParser::Paren_ExprContext *ctx) {
    auto paren_expr = make_t (Paren_Expr);

    paren_expr -> expr = get (Expr,ctx->expr());

    return (Expr*)paren_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitINT_Expr(DecafParser::INT_ExprContext *ctx) {
    auto int_expr = make_t (INT_Expr);

    int_expr -> expr = get (Expr,ctx->expr());

    return (Expr*)int_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitLoc_Expr(DecafParser::Loc_ExprContext *ctx) {
    auto loc_expr = make_t (Loc_Expr);

    loc_expr -> location = get (Location, ctx->location());

    return (Expr*)loc_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitOr_Op_Expr(DecafParser::Or_Op_ExprContext *ctx) {
    auto logic_op_expr = make_t (Logic_Op_Expr);

    logic_op_expr -> expr_lhs = get (Expr, ctx->expr(0));

    logic_op_expr -> bin_op = std::make_unique<Logic_Op>(Logic_Op::OR, ctx->OR()->getSymbol()->getLine(), ctx->OR()->getSymbol()->getCharPositionInLine());

    logic_op_expr -> expr_rhs = get (Expr, ctx->expr(1));

    return (Expr*)logic_op_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitLONG_Expr(DecafParser::LONG_ExprContext *ctx) {
    auto long_expr = make_t (LONG_Expr);

    long_expr -> expr = get (Expr, ctx->expr());

    return (Expr*)long_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitMethod_Call_Expr(DecafParser::Method_Call_ExprContext *ctx) {
    auto method_call_expr = make_t (Method_Call_Expr);

    method_call_expr -> id = make_Id();

    for (auto extern_arg_ctx : ctx->extern_arg()) {
        method_call_expr -> extern_args .push_back (get(Extern_Arg, extern_arg_ctx));
    }

    return (Expr*)method_call_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitMul_Op_Expr(DecafParser::Mul_Op_ExprContext *ctx) {
    auto mul_op_expr = make_t (Mul_Op_Expr);

    mul_op_expr -> expr_lhs = get (Expr, ctx->expr(0));

    mul_op_expr -> bin_op   = get (Bin_Op, ctx->bin_mul_op());

    mul_op_expr -> expr_rhs = get (Expr, ctx->expr(1));

    return (Expr*)mul_op_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitEq_Op_Expr(DecafParser::Eq_Op_ExprContext *ctx) {
    auto eq_op_expr = make_t (Eq_Op_Expr);

    eq_op_expr -> expr_lhs = get (Expr, ctx->expr(0));

    eq_op_expr -> bin_op   = get (Bin_Op, ctx->eq_op());

    eq_op_expr -> expr_rhs = get (Expr, ctx->expr(1));

    return (Expr*)eq_op_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitMinus_Expr(DecafParser::Minus_ExprContext *ctx) {
    auto minus_expr = make_t (Minus_Expr);

    minus_expr -> expr = get (Expr, ctx->expr());

    return (Expr*)minus_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitAnd_Op_Expr(DecafParser::And_Op_ExprContext *ctx) {
    auto logic_op_expr = make_t (Logic_Op_Expr);

    logic_op_expr -> expr_lhs = get (Expr, ctx->expr(0));

    logic_op_expr -> bin_op = std::make_unique<Logic_Op>(Logic_Op::AND, ctx->AND()->getSymbol()->getLine(), ctx->AND()->getSymbol()->getCharPositionInLine());

    logic_op_expr -> expr_rhs = get (Expr, ctx->expr(1));

    return (Expr*)logic_op_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitNot_Expr(DecafParser::Not_ExprContext *ctx) {
    auto not_expr = make_t (Not_Expr);

    not_expr -> expr = get (Expr, ctx->expr());

    return (Expr*)not_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitRel_Op_Expr(DecafParser::Rel_Op_ExprContext *ctx) {
    auto rel_op_expr = make_t (Rel_Op_Expr);

    rel_op_expr -> expr_lhs = get (Expr, ctx->expr(0));

    rel_op_expr -> bin_op   = get (Bin_Op, ctx->rel_op());

    rel_op_expr -> expr_rhs = get (Expr, ctx->expr(1));

    return (Expr*)rel_op_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitLiteral_Expr(DecafParser::Literal_ExprContext *ctx) {
    auto literal_expr = make_t (Literal_Expr);

    literal_expr -> literal = get (Literal, ctx->literal());

    return (Expr*)literal_expr.release();
}

antlrcpp::Any DecafASTBuilder::visitExpr_Arg(DecafParser::Expr_ArgContext *ctx) {
    auto expr_arg = make_t (Expr_Arg);

    expr_arg -> expr  = get (Expr, ctx->expr());
    
    return (Extern_Arg*)expr_arg.release();
}

antlrcpp::Any DecafASTBuilder::visitString_Arg(DecafParser::String_ArgContext *ctx) {
    auto string_arg = make_t (String_Arg);

    string_arg -> string_arg = ctx->STRING_LITERAL()->getText();

    return (Extern_Arg*)string_arg.release();
}

antlrcpp::Any DecafASTBuilder::visitBin_mul_op(DecafParser::Bin_mul_opContext *ctx) {
    auto bin_mul_op = make_t (Mul_Op);

    if (ctx->STAR()) {
        bin_mul_op->type = Mul_Op::STAR;
    } else if (ctx->DIV()) {
        bin_mul_op->type = Mul_Op::DIV;
    } else if (ctx->MOD()) {
        bin_mul_op->type = Mul_Op::MOD;
    } else {
        std::cerr << "ERROR: DecafASTBuilder::visitBin_mul_op" << std::endl;
    }


    return (Bin_Op*)bin_mul_op.release();
}

antlrcpp::Any DecafASTBuilder::visitBin_add_op(DecafParser::Bin_add_opContext *ctx) {
    auto bin_add_op = make_t (Add_Op);

    if (ctx->PLUS()) {
        bin_add_op->type = Add_Op::PLUS;
    } else if (ctx->MINUS()) {
        bin_add_op->type = Add_Op::MINUS;
    } else {
        std::cerr << "ERROR: DecafASTBuilder::visitBin_add_op" << std::endl;
    }

    return (Bin_Op*)bin_add_op.release();
}

antlrcpp::Any DecafASTBuilder::visitRel_op(DecafParser::Rel_opContext *ctx) {
    auto rel_op = make_t (Rel_Op);

    if (ctx->LT()) {
        rel_op->type = Rel_Op::LT;
    } else if (ctx->GT()) {
        rel_op->type = Rel_Op::GT;
    } else if (ctx->LE()) {
        rel_op->type = Rel_Op::LE;
    } else if (ctx->GE()) {
        rel_op->type = Rel_Op::GE;
    } else {
        std::cerr << "ERROR: DecafASTBuilder::visitRel_op" << std::endl;
    }

    return (Bin_Op*)rel_op.release();
}

antlrcpp::Any DecafASTBuilder::visitEq_op(DecafParser::Eq_opContext *ctx) {
    auto eq_op = make_t (Eq_Op);

    if (ctx->EQ()) {
        eq_op->type = Eq_Op::EQ;
    } else if (ctx->NEQ()) {
        eq_op->type = Eq_Op::NEQ;
    } else {
        std::cerr << "ERROR: DecafASTBuilder::visitEq_op" << std::endl;
    }

    return (Bin_Op*)eq_op.release();
}

antlrcpp::Any DecafASTBuilder::visitInt_Lit(DecafParser::Int_LitContext *ctx) {
    auto int_lit = make_t (Int_Lit);

    int_lit->literal = ctx->INT_LITERAL()->getText();

    return (Literal*)int_lit.release();
}

antlrcpp::Any DecafASTBuilder::visitLong_Lit(DecafParser::Long_LitContext *ctx) {
    auto long_lit = make_t (Long_Lit);

    long_lit->literal = ctx->LONG_LITERAL()->getText();

    return (Literal*)long_lit.release();
}

antlrcpp::Any DecafASTBuilder::visitChar_Lit(DecafParser::Char_LitContext *ctx) {
    auto char_lit = make_t (Char_Lit);
    
    char_lit->literal = ctx->CHAR_LITERAL()->getText();

    return (Literal*)char_lit.release();
}

antlrcpp::Any DecafASTBuilder::visitBool_Lit(DecafParser::Bool_LitContext *ctx) {
    auto bool_lit = make_t (Bool_Lit);

    bool_lit->literal = ctx->BOOL_LITERAL()->getText();

    return (Literal*)bool_lit.release();
}
