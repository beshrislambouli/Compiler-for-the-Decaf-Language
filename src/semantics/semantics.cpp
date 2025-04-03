#include "semantics.h"

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

    // AST::ASTPrinter printer(fout);
    // ast -> accept(printer);
    ast -> accept(*this);
    
    fout << "PASS" << std::endl;
    return 0;
}


void Semantics::visit(AST::Program& node) {
    for (auto& import_decl : node.import_decls ) {
        import_decl -> accept (*this);
    }

    for (auto& field_decl : node.field_decls ) {
        field_decl -> accept (*this);
    }

    for (auto& method_decl : node.method_decls ) {
        method_decl -> accept (*this);
    }
}

void Semantics::visit(AST::Import_Decl& node) {
    node.id -> accept (*this);
}

void Semantics::visit(AST::Field_Decl& node) {
    node.field_type -> accept (*this);

    for (auto& field : node.fields) {
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
    node.id -> accept(*this);
    for (auto& parameter: node.parameters) {
        parameter -> accept(*this);
    }
    node.block -> accept(*this);
}

void Semantics::visit(AST::Parameter& node) {
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
    node.block_then -> accept (*this);

    if (node.block_else) {
        node.block_else -> accept (*this);
    }
}

void Semantics::visit(AST::For_Stmt& node) {
    node.id -> accept(*this);
    node.expr_init -> accept(*this);
    node.expr_cond -> accept(*this);
    node.for_update -> accept(*this);
}

void Semantics::visit(AST::While_Stmt& node) {
    node.expr_cond -> accept (*this);
    node.block -> accept (*this);
}

void Semantics::visit(AST::Return_Stmt& node) {
    if (node.expr) {
        node.expr -> accept (*this);
    }
}

void Semantics::visit(AST::Break_Stmt& node) {

}

void Semantics::visit(AST::Continue_Stmt& node) {

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

}
