#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <limits>
#include <stdexcept>
#include <cstdint>
#include "DecafLexer.h"
#include "DecafParser.h"
#include "DecafAST.h"
#include "DecafPrettyPrinter.h"
#include "DecafASTBuilder.h"
#include "antlr4-runtime.h"
#include "Scope.h"

using T_t = AST::Type::Type_t;

class Semantics : AST::Visitor {
    Scope_Stack scope_stack;
    std::string error="";
public:
    int check (std::ifstream& fin, std::ofstream& fout);

    void visit(AST::Program& node) override ;
    void visit(AST::Import_Decl& node) override ;
    void visit(AST::Field_Decl& node) override ;
    void visit(AST::Id_Field_Decl& node) override ;
    void visit(AST::Array_Field_Decl& node) override ;
    void visit(AST::Method_Decl& node) override ;
    void visit(AST::Parameter& node) override ;
    void visit(AST::Block& node) override ;
    void visit(AST::Field_Type& node) override ;
    void visit(AST::Method_Type& node) override ;
    void visit(AST::Location_Assign_Op& node) override ;
    void visit(AST::Location_Incr& node) override ;
    void visit(AST::Method_Call_Stmt& node) override ;
    void visit(AST::If_Else_Stmt& node) override ;
    void visit(AST::For_Stmt& node) override ;
    void visit(AST::While_Stmt& node) override ;
    void visit(AST::Return_Stmt& node) override ;
    void visit(AST::Break_Stmt& node) override ;
    void visit(AST::Continue_Stmt& node) override ;
    void visit(AST::For_Upd_Assign_Op& node) override ;
    void visit(AST::For_Upd_Incr& node) override ;
    void visit(AST::Loc_Var& node) override ;
    void visit(AST::Loc_Array& node) override ;
    void visit(AST::Minus_Expr& node) override ;
    void visit(AST::Not_Expr& node) override ;
    void visit(AST::INT_Expr& node) override ;
    void visit(AST::LONG_Expr& node) override ;
    void visit(AST::Mul_Op_Expr& node) override ;
    void visit(AST::Add_Op_Expr& node) override ;
    void visit(AST::Rel_Op_Expr& node) override ;
    void visit(AST::Eq_Op_Expr& node) override ;
    void visit(AST::Logic_Op_Expr& node) override ;
    void visit(AST::Loc_Expr& node) override ;
    void visit(AST::Method_Call_Expr& node) override ;
    void visit(AST::Literal_Expr& node) override ;
    void visit(AST::Len_Expr& node) override ;
    void visit(AST::Paren_Expr& node) override ;
    void visit(AST::Expr_Arg& node) override ;
    void visit(AST::String_Arg& node) override ;
    void visit(AST::Assign_Op& node) override ;
    void visit(AST::Increment& node) override ;
    void visit(AST::Mul_Op& node) override ;
    void visit(AST::Add_Op& node) override ;
    void visit(AST::Rel_Op& node) override ;
    void visit(AST::Eq_Op& node) override ;
    void visit(AST::Logic_Op& node) override ;
    void visit(AST::Literal& node) override ;
    void visit(AST::Int_Lit& node) override ;
    void visit(AST::Long_Lit& node) override ;
    void visit(AST::Char_Lit& node) override ;
    void visit(AST::Bool_Lit& node) override ;
    void visit(AST::Type& node) override ;
    void visit(AST::Id& node) override ;
};