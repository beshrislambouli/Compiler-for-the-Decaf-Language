#include "semantics.h"

int Semantics::check (std::ifstream& fin, std::ofstream& fout) {
    antlr4::ANTLRInputStream input(fin);
    DecafLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();

    DecafParser parser(&tokens);
    antlr4::tree::ParseTree* tree = parser.program(); 
    if (parser.getNumberOfSyntaxErrors() > 0) {
        fout << "Parsing ERROR" << std::endl;
        return 1;
    }
    fout << "PASS" << std::endl;
    return 0;
}


void Semantics::visit(AST::Program& node) {}
void Semantics::visit(AST::Import_Decl& node) {}
void Semantics::visit(AST::Field_Decl& node) {}
void Semantics::visit(AST::Id_Field_Decl& node) {}
void Semantics::visit(AST::Array_Field_Decl& node) {}
void Semantics::visit(AST::Method_Decl& node) {}
void Semantics::visit(AST::Parameter& node) {}
void Semantics::visit(AST::Block& node) {}
void Semantics::visit(AST::Field_Type& node) {}
void Semantics::visit(AST::Method_Type& node) {}
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