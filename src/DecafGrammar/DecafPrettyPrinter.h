#pragma once

#include "DecafAST.h"
#include <iostream>
#include <string>
#include <typeinfo>

namespace AST {

class ASTPrinter : public Visitor {
private:
    std::ostream& out;
    int indent_level = 0;
    
    // Helper methods for managing indentation
    void print_indent();
    void increase_indent();
    void decrease_indent();
    
    // Helper to print the node type and location
    template<typename T>
    void print_node_header(const T& node, const std::string& extra_info = "");

public:
    ASTPrinter(std::ostream& out);
    virtual ~ASTPrinter() = default;
    
    // Visit methods implementation for all AST nodes
    void visit(Program& node) override;
    void visit(Import_Decl& node) override;
    void visit(Field_Decl& node) override;
    void visit(Id_Field_Decl& node) override;
    void visit(Array_Field_Decl& node) override;
    void visit(Method_Decl& node) override;
    void visit(Parameter& node) override;
    void visit(Field_Type& node) override;
    void visit(Method_Type& node) override;
    void visit(Block& node) override;
    void visit(Location_Assign_Op& node) override;
    void visit(Location_Incr& node) override;
    void visit(Method_Call_Stmt& node) override;
    void visit(If_Else_Stmt& node) override;
    void visit(For_Stmt& node) override;
    void visit(While_Stmt& node) override;
    void visit(Return_Stmt& node) override;
    void visit(Break_Stmt& node) override;
    void visit(Continue_Stmt& node) override;
    void visit(For_Upd_Assign_Op& node) override;
    void visit(For_Upd_Incr& node) override;
    void visit(Loc_Var& node) override;
    void visit(Loc_Array& node) override;
    void visit(Minus_Expr& node) override;
    void visit(Not_Expr& node) override;
    void visit(INT_Expr& node) override;
    void visit(LONG_Expr& node) override;
    void visit(Paren_Expr& node) override;
    void visit(Mul_Op_Expr& node) override;
    void visit(Add_Op_Expr& node) override;
    void visit(Rel_Op_Expr& node) override;
    void visit(Eq_Op_Expr& node) override;
    void visit(Logic_Op_Expr& node) override;
    void visit(Loc_Expr& node) override;
    void visit(Method_Call_Expr& node) override;
    void visit(Literal_Expr& node) override;
    void visit(Len_Expr& node) override;
    void visit(Expr_Arg& node) override;
    void visit(String_Arg& node) override;
    void visit(Assign_Op& node) override;
    void visit(Increment& node) override;
    void visit(Mul_Op& node) override;
    void visit(Add_Op& node) override;
    void visit(Rel_Op& node) override;
    void visit(Eq_Op& node) override;
    void visit(Logic_Op& node) override;
    void visit(Literal& node) override;
    void visit(Int_Lit& node) override;
    void visit(Long_Lit& node) override;
    void visit(Char_Lit& node) override;
    void visit(Bool_Lit& node) override;
    void visit(Type& node) override;
    void visit(Id& node) override;
};

} // namespace AST