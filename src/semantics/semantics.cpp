#include "semantics.h"

int Semantics::check (std::ifstream& fin, std::ofstream& fout) {
    antlr4::ANTLRInputStream input(fin);
    DecafLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();

    DecafParser parser(&tokens);
    if (parser.getNumberOfSyntaxErrors() > 0) {
        fout << "Parsing ERROR" << std::endl;
        return 1;
    }
    DecafParser::ProgramContext* tree = parser.program(); 
    DecafASTBuilder builder;
    std::unique_ptr<AST::Program> ast = std::unique_ptr<AST::Program>(builder.visitProgram(tree).as<AST::Program*>());

    ast->accept(*this);
    fout << "PASS" << std::endl;
    return 0;
}


void Semantics::visit(AST::Program& node) {
    for (auto& u: node.import_decls) {
        u->accept(*this);
    }
    for (auto& u : node.field_decls) {
        u->accept(*this);
    }
    for (auto& u : node.method_decls) {
        u->accept(*this);
    }
}

void Semantics::visit(AST::Import_Decl& node) {
    node.id -> accept(*this);
}

void Semantics::visit(AST::Field_Decl& node) {
    node.field_type -> accept(*this);
    for (auto& u : node.fields) {
        u->accept(*this);
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
    for (auto& u: node.parameters) {
        u -> accept(*this);
    }
    node.block -> accept(*this);
}

void Semantics::visit(AST::Parameter& node) {
    node.field_type -> accept(*this);
    node.id -> accept(*this);
}

void Semantics::visit(AST::Block& node) {
    for (auto& u : node.field_decls) {
        u -> accept (*this);
    }
    for (auto& u : node.statements) {
        u -> accept (*this);
    }
}

void Semantics::visit(AST::Field_Type& node) {
    node.type -> accept(*this);
}

void Semantics::visit(AST::Method_Type& node) {
    node.type -> accept(*this);
}

void Semantics::visit(AST::Location_Assign_Op& node) {}

void Semantics::visit(AST::Location_Incr& node) {}

void Semantics::visit(AST::Method_Call_Stmt& node) {}

void Semantics::visit(AST::If_Else_Stmt& node) {}

void Semantics::visit(AST::For_Stmt& node) {}

void Semantics::visit(AST::While_Stmt& node) {}

void Semantics::visit(AST::Return_Stmt& node) {}

void Semantics::visit(AST::Break_Stmt& node) {}

void Semantics::visit(AST::Continue_Stmt& node) {}

void Semantics::visit(AST::For_Upd_Assign_Op& node) {}

void Semantics::visit(AST::For_Upd_Incr& node) {}

void Semantics::visit(AST::Loc_Var& node) {}

void Semantics::visit(AST::Loc_Array& node) {}

void Semantics::visit(AST::Minus_Expr& node) {}

void Semantics::visit(AST::Not_Expr& node) {}

void Semantics::visit(AST::INT_Expr& node) {}

void Semantics::visit(AST::LONG_Expr& node) {}

void Semantics::visit(AST::Mul_Op_Expr& node) {}

void Semantics::visit(AST::Add_Op_Expr& node) {}

void Semantics::visit(AST::Rel_Op_Expr& node) {}

void Semantics::visit(AST::Eq_Op_Expr& node) {}

void Semantics::visit(AST::Logic_Op_Expr& node) {}

void Semantics::visit(AST::Loc_Expr& node) {}

void Semantics::visit(AST::Method_Call_Expr& node) {}

void Semantics::visit(AST::Literal_Expr& node) {}

void Semantics::visit(AST::Len_Expr& node) {}

void Semantics::visit(AST::Paren_Expr& node) {}

void Semantics::visit(AST::Expr_Arg& node) {}

void Semantics::visit(AST::String_Arg& node) {}

void Semantics::visit(AST::Assign_Op& node) {}

void Semantics::visit(AST::Increment& node) {}

void Semantics::visit(AST::Mul_Op& node) {}

void Semantics::visit(AST::Add_Op& node) {}

void Semantics::visit(AST::Rel_Op& node) {}

void Semantics::visit(AST::Eq_Op& node) {}

void Semantics::visit(AST::Logic_Op& node) {}

void Semantics::visit(AST::Literal& node) {}

void Semantics::visit(AST::Int_Lit& node) {}

void Semantics::visit(AST::Long_Lit& node) {}

void Semantics::visit(AST::Char_Lit& node) {}

void Semantics::visit(AST::Bool_Lit& node) {}

void Semantics::visit(AST::Type& node) {}

void Semantics::visit(AST::Id& node) {}
