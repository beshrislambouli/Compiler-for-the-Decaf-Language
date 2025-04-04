#include "semantics.h"


#define declare_method(id, type, is_import, parameters, AST_Node_Type) \
    error += scope_stack.declare_method(id, type, is_import, parameters, node.row, node.col, AST_Node_Type);

#define declare_var(id, type, AST_Node_Type) \
    error += scope_stack.declare_var(id, type, node.row, node.col, AST_Node_Type);

#define declare_arr(id, type, AST_Node_Type) \
    error += scope_stack.declare_arr(id, type, node.row, node.col, AST_Node_Type);



#define is_instance_of(uptr, Type) (dynamic_cast<Type*>((uptr).get()))


int Semantics::check (std::ifstream& fin, std::ofstream& fout) {
    antlr4::ANTLRInputStream input(fin);
    DecafLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();

    DecafParser parser(&tokens);
    DecafParser::ProgramContext* tree = parser.program(); 
    if (parser.getNumberOfSyntaxErrors() > 0) {
        fout << "Parsing ERROR" << std::endl;
        return 1;
    }
    DecafASTBuilder builder;
    std::unique_ptr<AST::Program> ast = std::unique_ptr<AST::Program>(builder.visitProgram(tree).as<AST::Program*>());


    ast -> accept(*this);
    

    if (error .size () > 0 ) {
        fout << error ;
        return 1;
    }

    fout << "PASS" << std::endl;
    return 0;
}


void Semantics::visit(AST::Program& node) {
    scope_stack .push_new_scope();

    for (auto& import_decl : node.import_decls ) {
        import_decl -> accept (*this);
    }

    for (auto& field_decl : node.field_decls ) {
        field_decl -> accept (*this);
    }

    for (auto& method_decl : node.method_decls ) {
        method_decl -> accept (*this);
    }

    // making sure there is a main function of type void with no parameters
    if  ( ! ( scope_stack.is_declared("main") && scope_stack.is_method("main") && !scope_stack.is_import("main") && scope_stack.get_type("main") == T_t::Void && scope_stack.get_method_parameters("main").size () == 0 ) ){
        error += "Error: main function is not correct\n";
    }

    scope_stack .pop ();
}

void Semantics::visit(AST::Import_Decl& node) {
    // declaring the imported methods
    declare_method (node.id->id, T_t::Int, true, {}, "Import_Decl");
    node.id -> accept (*this);
}

void Semantics::visit(AST::Field_Decl& node) {
    node.field_type -> accept (*this);

    for (auto& field : node.fields) {

        // declaring the fields
        if (is_instance_of(field,AST::Array_Field_Decl)) {
            declare_arr (field->id->id, node.field_type->type->type, "Field_Decl")
        } else {
            declare_var (field->id->id, node.field_type->type->type, "Field_Decl")
        }
        
        field -> accept (*this);
    }
}

void Semantics::visit(AST::Id_Field_Decl& node) {
    node.id -> accept(*this);
}

void Semantics::visit(AST::Array_Field_Decl& node) {
    node.id -> accept(*this);
    node.size -> accept(*this);
}

void Semantics::visit(AST::Method_Decl& node) {
    node.method_type -> accept(*this);

    // note that i declare the method in the global scope and the function scope

    std::vector <T_t> parameters_types ;
    for (auto& parameter: node.parameters) {
        parameters_types .push_back (parameter->field_type->type->type);
    }

    declare_method (node.id->id, node.method_type->type->type, false, parameters_types, "Method_Decl");

    scope_stack.push_method_scope(node.id->id);

    declare_method (node.id->id, node.method_type->type->type, false, parameters_types, "Method_Decl");


    node.id -> accept(*this);
    for (auto& parameter: node.parameters) {
        parameter -> accept(*this);
    }

    node.block -> accept(*this);

    scope_stack.pop();
}

void Semantics::visit(AST::Parameter& node) {
    declare_var (node.id->id, node.field_type->type->type, "Parameter");
    node.field_type -> accept(*this);
    node.id -> accept(*this);
}

void Semantics::visit(AST::Method_Type& node) {
    node.type -> accept(*this);
}

void Semantics::visit(AST::Field_Type& node) {
    node.type -> accept(*this);
}

void Semantics::visit(AST::Block& node) {
    for (auto& field_decl : node.field_decls) {
        field_decl -> accept (*this);
    }
    for (auto& statement : node.statements) {
        statement -> accept (*this);
    }
}

void Semantics::visit(AST::Location_Assign_Op& node) {
    if (is_instance_of(node.location,AST::Loc_Var) && !scope_stack.is_var(node.location->id->id)) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " " << node.location->id->id << " must be a scalar." << std::endl;
        error += err.str();
    }

    node.location -> accept (*this);
    node.assign_op-> accept (*this);
    node.expr -> accept (*this);
}

void Semantics::visit(AST::Location_Incr& node) {
    if (is_instance_of(node.location,AST::Loc_Var) && !scope_stack.is_var(node.location->id->id)) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " " << node.location->id->id << " must be a scalar." << std::endl;
        error += err.str();
    }
    node.location  -> accept (*this);
    node.increment -> accept (*this);
}

void Semantics::visit(AST::Method_Call_Stmt& node) {

    if ( !scope_stack.is_method(node.id->id) ) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " " << node.id->id << " used in method call but is not method" << std::endl;
        error += err.str();
    }

    node.id -> accept (*this);

    for (auto& extern_arg : node.extern_args) {
        extern_arg -> accept (*this);
    }

    if (scope_stack.is_method(node.id->id) && !scope_stack.is_import(node.id->id)) {
        for (auto& extern_arg : node.extern_args) {
            if (is_instance_of(extern_arg,AST::String_Arg)) {
                std::stringstream err;
                err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " string literal may not be used as parameters to non-import methods" << std::endl;
                error += err.str();
            }

            // TODO: there has to be a better way
            if (is_instance_of(extern_arg,AST::Expr_Arg)) {
                
                AST::Expr_Arg* expr_arg = dynamic_cast<AST::Expr_Arg*>(extern_arg.get());

                if (is_instance_of(expr_arg->expr,AST::Loc_Expr)) {

                    AST::Loc_Expr* loc_expr = dynamic_cast<AST::Loc_Expr*>(expr_arg->expr.get());

                    if (is_instance_of(loc_expr->location, AST::Loc_Var)) {

                        AST::Loc_Var* loc_var = dynamic_cast<AST::Loc_Var*>(loc_expr->location.get());

                            if ( scope_stack.is_array(loc_var->id->id) ) {
                                std::stringstream err;
                                err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " arrays may not be used as parameters to non-import methods" << std::endl;
                                error += err.str();
                            }

                    }
                }
            }

        }
    }
}

void Semantics::visit(AST::If_Else_Stmt& node) {
    node.expr_if -> accept (*this);
    if (node.expr_if->type_t->type != T_t::Bool) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " if condition must have type bool" << std::endl;
        error += err.str();
    }

    scope_stack.push_new_scope();
    node.block_then -> accept (*this);
    scope_stack.pop();

    if (node.block_else) {
        scope_stack.push_new_scope();
        node.block_else -> accept (*this);
        scope_stack.pop();
    }
}

void Semantics::visit(AST::For_Stmt& node) {
    node.id -> accept(*this);
    node.expr_init -> accept(*this);
    node.expr_cond -> accept(*this);
    if (node.expr_cond->type_t->type != T_t::Bool) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " for condition must have type bool" << std::endl;
        error += err.str();
    }
    node.for_update -> accept(*this);

    scope_stack.push_loop_scope();
    node.block -> accept(*this);
    scope_stack.pop();
}

void Semantics::visit(AST::While_Stmt& node) {
    node.expr_cond -> accept (*this);
    if (node.expr_cond->type_t->type != T_t::Bool) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " while condition must have type bool" << std::endl;
        error += err.str();
    }

    scope_stack.push_loop_scope();
    node.block -> accept (*this);
    scope_stack.pop();
}

void Semantics::visit(AST::Return_Stmt& node) {

    auto current_method_id_opt = scope_stack.get_current_method();
    if (! current_method_id_opt.has_value() || !scope_stack.is_method(current_method_id_opt.value()) ) {
        std::cout << "ERROR: return in non method\n"; // (should not happen in first place but just checking) 
    }

    if (node.expr) {

        if ( scope_stack.get_type(current_method_id_opt.value()).value() == T_t::Void ) {
            std::stringstream err;
            err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " return value on void function" << std::endl;
            error += err.str();
        }

        node.expr -> accept (*this);
    } else {
        if ( scope_stack.get_type(current_method_id_opt.value()).value() != T_t::Void ) {
            std::stringstream err;
            err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " doesn't return value on a non-void function" << std::endl;
            error += err.str();
        }
    }
}

void Semantics::visit(AST::Break_Stmt& node) {
    if (!scope_stack.is_loop()) {
        error += "break in non loop\n";
    }
}

void Semantics::visit(AST::Continue_Stmt& node) {
    if (!scope_stack.is_loop()) {
        error += "continue in non loop\n";
    }
}

void Semantics::visit(AST::For_Upd_Assign_Op& node) {
    if (is_instance_of(node.location,AST::Loc_Var) && !scope_stack.is_var(node.location->id->id)) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " " << node.location->id->id << " must be a scalar." << std::endl;
        error += err.str();
    }
    node.location -> accept (*this);
    node.assign_Op-> accept (*this);
    node.expr -> accept (*this);
}

void Semantics::visit(AST::For_Upd_Incr& node) {
    if (is_instance_of(node.location,AST::Loc_Var) && !scope_stack.is_var(node.location->id->id)) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " " << node.location->id->id << " must be a scalar." << std::endl;
        error += err.str();
    }
    node.location -> accept (*this);
    node.increment-> accept (*this);
}

void Semantics::visit(AST::Loc_Var& node) {
    if (scope_stack.is_declared(node.id->id) && !scope_stack.is_var(node.id->id) && !scope_stack.is_array(node.id->id)) { // note that in f (arr) arr is a loc_var so it can be an array
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " " << node.id->id << " used as location var but not a var " << std::endl;
        error += err.str();
    }
    node.id -> accept (*this);
    node.assign_type(node.id->type_t->type);
}

void Semantics::visit(AST::Loc_Array& node) {
    if (!scope_stack.is_array(node.id->id)) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " " << node.id->id << " used as location array but not an array " << std::endl;
        error += err.str();
    }

    node.id  -> accept (*this);
    node.expr-> accept (*this);

    if (node.expr->type_t->type != T_t::Int) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " index must be of type int " << std::endl;
        error += err.str();
    }

    node.assign_type(node.id->type_t->type);
}

void Semantics::visit(AST::Minus_Expr& node) {
    node.expr -> accept (*this);
    node.assign_type(node.expr->type_t->type);
}

void Semantics::visit(AST::Not_Expr& node) {
    node.expr -> accept (*this);
    node.assign_type(T_t::Bool);
}

void Semantics::visit(AST::INT_Expr& node) {
    node.expr -> accept (*this);
    node.assign_type(T_t::Int);
}

void Semantics::visit(AST::LONG_Expr& node) {
    node.expr -> accept (*this);
    node.assign_type(T_t::Long);
}

void Semantics::visit(AST::Paren_Expr& node) {
    node.expr -> accept (*this);
    node.assign_type(node.expr->type_t->type);
}

void Semantics::visit(AST::Mul_Op_Expr& node) {
    node.expr_lhs -> accept (*this);
    node.bin_op   -> accept (*this);
    node.expr_rhs -> accept (*this);

    node.assign_type(node.expr_lhs->type_t->type);
}

void Semantics::visit(AST::Add_Op_Expr& node) {
    node.expr_lhs -> accept (*this);
    node.bin_op   -> accept (*this);
    node.expr_rhs -> accept (*this);

    node.assign_type(node.expr_lhs->type_t->type);
}

void Semantics::visit(AST::Rel_Op_Expr& node) {
    node.expr_lhs -> accept (*this);
    node.bin_op   -> accept (*this);
    node.expr_rhs -> accept (*this);

    node.assign_type(T_t::Bool);
}

void Semantics::visit(AST::Eq_Op_Expr& node) {
    node.expr_lhs -> accept (*this);
    node.bin_op   -> accept (*this);
    node.expr_rhs -> accept (*this);

    node.assign_type(T_t::Bool);
}

void Semantics::visit(AST::Logic_Op_Expr& node) {
    node.expr_lhs -> accept (*this);
    node.bin_op   -> accept (*this);
    node.expr_rhs -> accept (*this);

    node.assign_type(T_t::Bool);
}

void Semantics::visit(AST::Loc_Expr& node) {
    node.location -> accept (*this);
    node.assign_type(node.location->type_t->type);
}

void Semantics::visit(AST::Method_Call_Expr& node) {
    if ( !scope_stack.is_method(node.id->id) ) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " " << node.id->id << " used in method call but is not method" << std::endl;
        error += err.str();
    } else {
        auto t_op = scope_stack.get_type(node.id->id);
        if (t_op.has_value()) {
            node.assign_type(t_op.value());
        }
    }

    if ( scope_stack.is_method(node.id->id) && scope_stack.get_type (node.id->id).value() == T_t::Void ) { // since we know if is a method it is safe to .value() it
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " method " << node.id->id << " used as expr but it does not return a result" << std::endl;
        error += err.str();
    }


    node.id -> accept (*this);
    for (auto& extern_arg : node.extern_args) {
        extern_arg -> accept (*this);
    }
}

void Semantics::visit(AST::Literal_Expr& node) {
    node.literal -> accept (*this);
    node.assign_type(node.literal->type_t->type);
}

void Semantics::visit(AST::Len_Expr& node) {
    if (!scope_stack.is_array(node.id->id)) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " len operator must be on an array variable." << std::endl;
        error += err.str();
    }
    node.assign_type(T_t::Int);
    node.id -> accept (*this);
}

void Semantics::visit(AST::Expr_Arg& node) {
    node.expr -> accept (*this);
}

void Semantics::visit(AST::String_Arg& node) {

}

void Semantics::visit(AST::Assign_Op& node) {
    
}

void Semantics::visit(AST::Increment& node) {
    
}

void Semantics::visit(AST::Mul_Op& node) {

}

void Semantics::visit(AST::Add_Op& node) {

}

void Semantics::visit(AST::Rel_Op& node) {

}

void Semantics::visit(AST::Eq_Op& node) {

}

void Semantics::visit(AST::Logic_Op& node) {

}

void Semantics::visit(AST::Literal& node) {

}

void Semantics::visit(AST::Int_Lit& node) {
    node.assign_type(T_t::Int);
}

void Semantics::visit(AST::Long_Lit& node) {
    node.assign_type(T_t::Long);
}

void Semantics::visit(AST::Char_Lit& node) {
    node.assign_type(T_t::Int);
}

void Semantics::visit(AST::Bool_Lit& node) {
    node.assign_type(T_t::Bool);
}

void Semantics::visit(AST::Type& node) {

}

void Semantics::visit(AST::Id& node) {
    if (! scope_stack.is_declared (node.id) ) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " " << node.id << " not defined" << std::endl;
        error += err.str();
    } else {
        auto t = scope_stack.get_type(node.id);
        if (t.has_value()) {
            node.assign_type(t.value());
        }
    }
}
