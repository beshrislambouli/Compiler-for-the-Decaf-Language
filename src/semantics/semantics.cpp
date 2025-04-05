#include "semantics.h"


#define declare_method(id, type, is_import, parameters, AST_Node_Type) \
    error += scope_stack.declare_method(id, type, is_import, parameters, node.row, node.col, AST_Node_Type);

#define declare_var(id, type, AST_Node_Type) \
    error += scope_stack.declare_var(id, type, node.row, node.col, AST_Node_Type);

#define declare_arr(id, type, AST_Node_Type) \
    error += scope_stack.declare_arr(id, type, node.row, node.col, AST_Node_Type);



#define is_instance_of(uptr, Type) (dynamic_cast<Type*>((uptr).get()) != nullptr)


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

    // fout << "PASS" << std::endl;
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
    if (node.size->literal == "0") {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " array size must be positive" << std::endl;
        error += err.str();
    }
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

    if (node.assign_op->type != AST::Assign_Op::Type::ASSIGN) {

        if (node.location->type_t->type != T_t::Int && node.location->type_t->type != T_t::Long) {
            std::stringstream err;
            err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " assign operation must be a int or long." << std::endl;
            error += err.str();
        }
        if (node.expr->type_t->type != T_t::Int && node.expr->type_t->type != T_t::Long) {
            std::stringstream err;
            err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " assign operation must be a int or long." << std::endl;
            error += err.str();
        }

    }
    
    if (node.location->type_t->type != node.expr->type_t->type) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " assignment operation must have same type" << std::endl;
        error += err.str();
    }
}

void Semantics::visit(AST::Location_Incr& node) {
    if (is_instance_of(node.location,AST::Loc_Var) && !scope_stack.is_var(node.location->id->id)) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " " << node.location->id->id << " must be a scalar." << std::endl;
        error += err.str();
    }
    node.location  -> accept (*this);
    node.increment -> accept (*this);
    if (node.location->type_t->type != T_t::Int && node.location->type_t->type != T_t::Long) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " " << node.location->id->id << " must be a int or long." << std::endl;
        error += err.str();
    }
}

void Semantics::visit(AST::Method_Call_Stmt& node) {

    if ( !scope_stack.is_method(node.id->id) ) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " " << node.id->id << " used in method call but is not method" << std::endl;
        error += err.str();
    }

    node.id -> accept (*this);
    scope_stack.is_extern_arg_for_import_method = scope_stack.is_import(node.id->id);
    for (auto& extern_arg : node.extern_args) {
        extern_arg -> accept (*this);
    }
    scope_stack.is_extern_arg_for_import_method = false;

    if (scope_stack.is_method(node.id->id) && !scope_stack.is_import(node.id->id)) {
        std::vector<T_t> req_types = scope_stack.get_method_parameters(node.id->id);
        if (req_types.size () != node.extern_args.size () ) {
            std::stringstream err;
            err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " number of arguments don't match" << std::endl;
            error += err.str();
        } else {
            for (int i = 0 ; i < node.extern_args.size () ; i ++ ) {
                if (is_instance_of(node.extern_args[i],AST::Expr_Arg)) {
                    AST::Expr_Arg* expr_arg = dynamic_cast<AST::Expr_Arg*>(node.extern_args[i].get()); // TODO: recheck this or think about another way
                    
                    if (expr_arg->expr->type_t->type != req_types [i]) {
                        std::stringstream err;
                        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " argument #" << i + 1 << " has wrong type"<< std::endl;
                        error += err.str();
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

    if (node.id->type_t->type != node.expr_init->type_t->type) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " assignment operation must have same type" << std::endl;
        error += err.str();
    }


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
    auto current_method_return_type = scope_stack.get_type(current_method_id_opt.value()).value();

    if (node.expr) {
        node.expr -> accept (*this);

        if ( current_method_return_type == T_t::Void ) {
            std::stringstream err;
            err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " return value on void function" << std::endl;
            error += err.str();
        } else if (current_method_return_type != node.expr->type_t->type) {
            std::stringstream err;
            err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " return value of different type than the method." << std::endl;
            error += err.str();
        }

    } else {
        if ( current_method_return_type != T_t::Void ) {
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

    if (node.assign_Op->type != AST::Assign_Op::Type::ASSIGN) {

        if (node.location->type_t->type != T_t::Int && node.location->type_t->type != T_t::Long) {
            std::stringstream err;
            err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " assign operation must be a int or long." << std::endl;
            error += err.str();
        }
        if (node.expr->type_t->type != T_t::Int && node.expr->type_t->type != T_t::Long) {
            std::stringstream err;
            err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " assign operation must be a int or long." << std::endl;
            error += err.str();
        }

    }
    
    if (node.location->type_t->type != node.expr->type_t->type) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " assignment operation must have same type" << std::endl;
        error += err.str();
    }
}

void Semantics::visit(AST::For_Upd_Incr& node) {
    if (is_instance_of(node.location,AST::Loc_Var) && !scope_stack.is_var(node.location->id->id)) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " " << node.location->id->id << " must be a scalar." << std::endl;
        error += err.str();
    }
    node.location -> accept (*this);
    if (node.location->type_t->type != T_t::Int && node.location->type_t->type != T_t::Long) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " " << node.location->id->id << " must be a int or long." << std::endl;
        error += err.str();
    }
    node.increment-> accept (*this);
}

void Semantics::visit(AST::Loc_Var& node) {
    if (scope_stack.is_declared(node.id->id) && !scope_stack.is_var(node.id->id) && !(scope_stack.is_extern_arg_for_import_method && scope_stack.is_array(node.id->id)) ) {
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

    if (is_instance_of(node.expr,AST::Literal_Expr)) {
        AST::Literal_Expr* literal_expr = dynamic_cast<AST::Literal_Expr*>(node.expr.get());
        if (literal_expr) {
            literal_expr -> literal -> minus = true;
        }
    }


    node.expr -> accept (*this);
    if (node.expr->type_t->type != T_t::Long && node.expr->type_t->type != T_t::Int ) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " unary minus should be used on int or long " << std::endl;
        error += err.str();
    }
    node.assign_type(node.expr->type_t->type);
}

void Semantics::visit(AST::Not_Expr& node) {
    node.expr -> accept (*this);
    if (node.expr->type_t->type != T_t::Bool ) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " Not operation should be used on bool " << std::endl;
        error += err.str();
    }
    node.assign_type(T_t::Bool);
}

void Semantics::visit(AST::INT_Expr& node) {
    node.expr -> accept (*this);

    if (node.expr->type_t->type != T_t::Long && node.expr->type_t->type != T_t::Int ) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " int() operation should be used on int or long " << std::endl;
        error += err.str();
    }

    node.assign_type(T_t::Int);
}

void Semantics::visit(AST::LONG_Expr& node) {
    node.expr -> accept (*this);

    if (node.expr->type_t->type != T_t::Long && node.expr->type_t->type != T_t::Int ) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " long() operation should be used on int or long " << std::endl;
        error += err.str();
    }

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

    if (node.expr_lhs->type_t->type != T_t::Long && node.expr_lhs->type_t->type != T_t::Int ) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " arithmetic operations should be used on int or long " << std::endl;
        error += err.str();
    }


    if (node.expr_rhs->type_t->type != T_t::Long && node.expr_rhs->type_t->type != T_t::Int ) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " arithmetic operations should be used on int or long " << std::endl;
        error += err.str();
    }

    if (node.expr_lhs->type_t->type != node.expr_rhs->type_t->type) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " arithmetic operations operands should have the same type" << std::endl;
        error += err.str();
    }

    node.assign_type(node.expr_lhs->type_t->type);
}

void Semantics::visit(AST::Add_Op_Expr& node) {
    node.expr_lhs -> accept (*this);
    node.bin_op   -> accept (*this);
    node.expr_rhs -> accept (*this);

    if (node.expr_lhs->type_t->type != T_t::Long && node.expr_lhs->type_t->type != T_t::Int ) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " arithmetic operations should be used on int or long " << std::endl;
        error += err.str();
    }


    if (node.expr_rhs->type_t->type != T_t::Long && node.expr_rhs->type_t->type != T_t::Int ) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " arithmetic operations should be used on int or long " << std::endl;
        error += err.str();
    }

    if (node.expr_lhs->type_t->type != node.expr_rhs->type_t->type) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " arithmetic operations operands should have the same type" << std::endl;
        error += err.str();
    }

    node.assign_type(node.expr_lhs->type_t->type);
}

void Semantics::visit(AST::Rel_Op_Expr& node) {
    node.expr_lhs -> accept (*this);
    node.bin_op   -> accept (*this);
    node.expr_rhs -> accept (*this);

    if (node.expr_lhs->type_t->type != T_t::Long && node.expr_lhs->type_t->type != T_t::Int ) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " relational operators should be used on int or long " << std::endl;
        error += err.str();
    }


    if (node.expr_rhs->type_t->type != T_t::Long && node.expr_rhs->type_t->type != T_t::Int ) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " relational operators should be used on int or long " << std::endl;
        error += err.str();
    }

    node.assign_type(T_t::Bool);
}

void Semantics::visit(AST::Eq_Op_Expr& node) {
    node.expr_lhs -> accept (*this);
    node.bin_op   -> accept (*this);
    node.expr_rhs -> accept (*this);

    if (node.expr_lhs->type_t->type != T_t::Long && node.expr_lhs->type_t->type != T_t::Int && node.expr_lhs->type_t->type != T_t::Bool) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " Equal operations should be used on int, long, or bool " << std::endl;
        error += err.str();
    }

    if (node.expr_rhs->type_t->type != T_t::Long && node.expr_rhs->type_t->type != T_t::Int && node.expr_rhs->type_t->type != T_t::Bool) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " Equal operations should be used on int, long, or bool " << std::endl;
        error += err.str();
    }

    if (node.expr_lhs->type_t->type != node.expr_rhs->type_t->type) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " Equal operations operands should have the same type" << std::endl;
        error += err.str();
    }


    node.assign_type(T_t::Bool);
}

void Semantics::visit(AST::Logic_Op_Expr& node) {
    node.expr_lhs -> accept (*this);
    node.bin_op   -> accept (*this);
    node.expr_rhs -> accept (*this);

    if (node.expr_lhs->type_t->type != T_t::Bool) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " logical operations should be used on bool " << std::endl;
        error += err.str();
    }

    if (node.expr_rhs->type_t->type != T_t::Bool) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " logical operations should be used on bool " << std::endl;
        error += err.str();
    }

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
    node.assign_type(node.id->type_t->type);

    scope_stack.is_extern_arg_for_import_method = scope_stack.is_import(node.id->id);
    for (auto& extern_arg : node.extern_args) {
        extern_arg -> accept (*this);
    }
    scope_stack.is_extern_arg_for_import_method = false;

    if (scope_stack.is_method(node.id->id) && !scope_stack.is_import(node.id->id)) {
        std::vector<T_t> req_types = scope_stack.get_method_parameters(node.id->id);
        if (req_types.size () != node.extern_args.size () ) {
            std::stringstream err;
            err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " number of arguments don't match" << std::endl;
            error += err.str();
        } else {
            for (int i = 0 ; i < node.extern_args.size () ; i ++ ) {
                if (is_instance_of(node.extern_args[i],AST::Expr_Arg)) {
                    AST::Expr_Arg* expr_arg = dynamic_cast<AST::Expr_Arg*>(node.extern_args[i].get()); // TODO: recheck this or think about another way
                    
                    if (expr_arg->expr->type_t->type != req_types [i]) {
                        std::stringstream err;
                        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " argument #" << i + 1 << " has wrong type"<< std::endl;
                        error += err.str();
                    }
                    
                }
            }
        }
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
    if (!scope_stack.is_extern_arg_for_import_method) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " string literal may not be used as parameters to non-import methods" << std::endl;
        error += err.str();
    }
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

    try {
        std::string num = node.literal;

        int base = 10;
        if (num.size() > 2 && (num[0] == '0') && num[1] == 'x') {
            base = 16;
        }

        std::string full = (node.minus ? "-" : "") + num;

        auto val = std::stoll(full, nullptr, base);

        if (! (val >= std::numeric_limits<int32_t>::min() && val <= std::numeric_limits<int32_t>::max()) ) {
            std::stringstream err;
            err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " int literal out of bound" << std::endl;
            error += err.str();
        }
    } catch (const std::out_of_range&) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " int literal out of bound" << std::endl;
        error += err.str();
    } catch (const std::invalid_argument&) {
    }
}

void Semantics::visit(AST::Long_Lit& node) {
    node.assign_type(T_t::Long);

    try {
        std::string num = node.literal;
        num.pop_back();


        int base = 10;
        if (num.size() > 2 && (num[0] == '0') && num[1] == 'x') {
            base = 16;
        }

        std::string full = (node.minus ? "-" : "") + num;

        auto val = std::stoll(full, nullptr, base);

        if (! (val >= std::numeric_limits<int64_t>::min() && val <= std::numeric_limits<int64_t>::max()) ) {
            std::stringstream err;
            err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " long literal out of bound" << std::endl;
            error += err.str();
        }
    } catch (const std::out_of_range&) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " long literal out of bound" << std::endl;
        error += err.str();
    } catch (const std::invalid_argument&) {
    }
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

        node.assign_type(T_t::Null_Type);

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
