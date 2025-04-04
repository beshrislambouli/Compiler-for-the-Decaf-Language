#include "semantics.h"


#define declare(id, t, s) \
    error += scope_stack.Declare(id, t, node.row, node.col, s);

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

    fout << "PASS" << std::endl;
    return 0;
}


void Semantics::visit(AST::Program& node) {
    scope_stack .add_new_scope();

    for (auto& import_decl : node.import_decls ) {
        import_decl -> accept (*this);
    }

    for (auto& field_decl : node.field_decls ) {
        field_decl -> accept (*this);
    }

    bool correct_main = false;
    for (auto& method_decl : node.method_decls ) {
        method_decl -> accept (*this);

        if (method_decl->id->id == "main" && method_decl->parameters.size () == 0 && method_decl->method_type->type->type == T_t::Void ) {
            correct_main = 1;
        }
    }

    // making sure there is a main function of type void with no parameters
    if ( !correct_main ) {
        error += "Error: main function is not correct\n";
    }

    scope_stack .pop ();
}

void Semantics::visit(AST::Import_Decl& node) {
    // declaring the imported methods
    declare (node.id->id, T_t::Int,"Import_Decl");
    node.id -> accept (*this);
}

void Semantics::visit(AST::Field_Decl& node) {
    node.field_type -> accept (*this);

    for (auto& field : node.fields) {

        // declaring the fields
        if (is_instance_of(field,AST::Array_Field_Decl)) {
            declare (field->id->id, node.field_type->type->type_Arr(), "Field_Decl")
        } else {
            declare (field->id->id, node.field_type->type->type, "Field_Decl")
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

    // note that i delcate the method in the global scope and the function scope

    declare (node.id->id, node.method_type->type->type, "Method_Decl");

    scope_stack.add_new_scope();

    declare (node.id->id, node.method_type->type->type, "Method_Decl");
    node.id -> accept(*this);


    for (auto& parameter: node.parameters) {
        parameter -> accept(*this);
    }


    node.block -> accept(*this);

    scope_stack.pop();
}

void Semantics::visit(AST::Parameter& node) {
    declare (node.id->id, node.field_type->type->type, "Parameter");
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
    // because of method_decl parameters, the scope of block is created before the call of block

    for (auto& field_decl : node.field_decls) {
        field_decl -> accept (*this);
    }
    for (auto& statement : node.statements) {
        statement -> accept (*this);
    }
}

void Semantics::visit(AST::Location_Assign_Op& node) {
    node.location -> accept (*this);
    node.assign_op-> accept (*this);
    node.expr -> accept (*this);
}

void Semantics::visit(AST::Location_Incr& node) {
    node.location  -> accept (*this);
    node.increment -> accept (*this);
}

void Semantics::visit(AST::Method_Call_Stmt& node) {
    node.id -> accept (*this);

    for (auto& extern_arg : node.extern_args) {
        extern_arg -> accept (*this);
    }
}

void Semantics::visit(AST::If_Else_Stmt& node) {
    node.expr_if -> accept (*this);

    scope_stack.add_new_scope();
    node.block_then -> accept (*this);
    scope_stack.pop();

    if (node.block_else) {
        scope_stack.add_new_scope();
        node.block_else -> accept (*this);
        scope_stack.pop();
    }
}

void Semantics::visit(AST::For_Stmt& node) {
    node.id -> accept(*this);
    node.expr_init -> accept(*this);
    node.expr_cond -> accept(*this);
    node.for_update -> accept(*this);

    scope_stack.add_new_scope(true);
    node.block -> accept(*this);
    scope_stack.pop();
}

void Semantics::visit(AST::While_Stmt& node) {
    node.expr_cond -> accept (*this);

    scope_stack.add_new_scope(true);
    node.block -> accept (*this);
    scope_stack.pop();
}

void Semantics::visit(AST::Return_Stmt& node) {
    if (node.expr) {
        node.expr -> accept (*this);
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
    node.location -> accept (*this);
    node.assign_Op-> accept (*this);
    node.expr -> accept (*this);
}

void Semantics::visit(AST::For_Upd_Incr& node) {
    node.location -> accept (*this);
    node.increment-> accept (*this);
}

void Semantics::visit(AST::Loc_Var& node) {
    node.id -> accept (*this);
}

void Semantics::visit(AST::Loc_Array& node) {
    node.id  -> accept (*this);
    node.expr-> accept (*this);
}

void Semantics::visit(AST::Minus_Expr& node) {
    node.expr -> accept (*this);
}

void Semantics::visit(AST::Not_Expr& node) {
    node.expr -> accept (*this);
}

void Semantics::visit(AST::INT_Expr& node) {
    node.expr -> accept (*this);
}

void Semantics::visit(AST::LONG_Expr& node) {
    node.expr -> accept (*this);
}

void Semantics::visit(AST::Paren_Expr& node) {
    node.expr -> accept (*this);
}

void Semantics::visit(AST::Mul_Op_Expr& node) {
    node.expr_lhs -> accept (*this);
    node.bin_op   -> accept (*this);
    node.expr_rhs -> accept (*this);
}

void Semantics::visit(AST::Add_Op_Expr& node) {
    node.expr_lhs -> accept (*this);
    node.bin_op   -> accept (*this);
    node.expr_rhs -> accept (*this);
}

void Semantics::visit(AST::Rel_Op_Expr& node) {
    node.expr_lhs -> accept (*this);
    node.bin_op   -> accept (*this);
    node.expr_rhs -> accept (*this);
}

void Semantics::visit(AST::Eq_Op_Expr& node) {
    node.expr_lhs -> accept (*this);
    node.bin_op   -> accept (*this);
    node.expr_rhs -> accept (*this);
}

void Semantics::visit(AST::Logic_Op_Expr& node) {
    node.expr_lhs -> accept (*this);
    node.bin_op   -> accept (*this);
    node.expr_rhs -> accept (*this);
}

void Semantics::visit(AST::Loc_Expr& node) {
    node.location -> accept (*this);
}

void Semantics::visit(AST::Method_Call_Expr& node) {
    if ( scope_stack.get (node.id->id) == T_t::Void ) {
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
}

void Semantics::visit(AST::Len_Expr& node) {
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

}

void Semantics::visit(AST::Long_Lit& node) {

}

void Semantics::visit(AST::Char_Lit& node) {

}

void Semantics::visit(AST::Bool_Lit& node) {

}

void Semantics::visit(AST::Type& node) {

}

void Semantics::visit(AST::Id& node) {
    if (! scope_stack.declared (node.id) ) {
        std::stringstream err;
        err << "Error: " << "Line: " << node.row << " " << "Col: " << node.col << " " << node.id << " not defined" << std::endl;
        error += err.str();
    }
}
