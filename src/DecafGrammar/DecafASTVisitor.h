#pragma once

#include "DecafAST.h"

namespace AST {

class Visitor {
public:
    virtual ~Visitor() = default;

    virtual void visit(Program& node) = 0;
    virtual void visit(Import_Decl& node) = 0;
    virtual void visit(Field_Decl& node) = 0;
    // virtual void visit(Field& node) = 0;
    virtual void visit(Id_Field_Decl& node) = 0;
    virtual void visit(Array_Field_Decl& node) = 0;
    virtual void visit(Method_Decl& node) = 0;
    virtual void visit(Parameter& node) = 0;
    virtual void visit(Block& node) = 0;
    virtual void visit(Field_Type& node) = 0;
    virtual void visit(Method_Type& node) = 0;
    // virtual void visit(Statement& node) = 0;
    virtual void visit(Location_Assign_Op& node) = 0;
    virtual void visit(Location_Incr& node) = 0;
    virtual void visit(Method_Call_Stmt& node) = 0;
    virtual void visit(If_Else_Stmt& node) = 0;
    virtual void visit(For_Stmt& node) = 0;
    virtual void visit(While_Stmt& node) = 0;
    virtual void visit(Return_Stmt& node) = 0;
    virtual void visit(Break_Stmt& node) = 0;
    virtual void visit(Continue_Stmt& node) = 0;
    // virtual void visit(For_Update& node) = 0;
    virtual void visit(For_Upd_Assign_Op& node) = 0;
    virtual void visit(For_Upd_Incr& node) = 0;
    // virtual void visit(Location& node) = 0;
    virtual void visit(Loc_Var& node) = 0;
    virtual void visit(Loc_Array& node) = 0;
    virtual void visit(Minus_Expr& node) = 0;
    virtual void visit(Not_Expr& node) = 0;
    virtual void visit(INT_Expr& node) = 0;
    virtual void visit(LONG_Expr& node) = 0;
    virtual void visit(Mul_Op_Expr& node) = 0;
    virtual void visit(Add_Op_Expr& node) = 0;
    virtual void visit(Rel_Op_Expr& node) = 0;
    virtual void visit(Eq_Op_Expr& node) = 0;
    virtual void visit(Logic_Op_Expr& node) = 0;
    virtual void visit(Loc_Expr& node) = 0;
    virtual void visit(Method_Call_Expr& node) = 0;
    virtual void visit(Literal_Expr& node) = 0;
    virtual void visit(Len_Expr& node) = 0;
    virtual void visit(Paren_Expr& node) = 0;
    // virtual void visit(Extern_Arg& node) = 0;
    virtual void visit(Expr_Arg& node) = 0;
    virtual void visit(String_Arg& node) = 0;
    virtual void visit(Assign_Op& node) = 0;
    virtual void visit(Increment& node) = 0;
    virtual void visit(Mul_Op& node) = 0;
    virtual void visit(Add_Op& node) = 0;
    virtual void visit(Rel_Op& node) = 0;
    virtual void visit(Eq_Op& node) = 0;
    virtual void visit(Logic_Op& node) = 0;
    virtual void visit(Literal& node) = 0;
    virtual void visit(Int_Lit& node) = 0;
    virtual void visit(Long_Lit& node) = 0;
    virtual void visit(Char_Lit& node) = 0;
    virtual void visit(Bool_Lit& node) = 0;
    virtual void visit(Type& node) = 0;
    virtual void visit(Id& node) = 0;
};

}