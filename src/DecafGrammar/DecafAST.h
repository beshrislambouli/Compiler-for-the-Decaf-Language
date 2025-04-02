#pragma once

#include <vector>
#include <memory>
#include <string>

namespace AST {
// Forward Declaration

class Program;
class Import_Decl;
class Field_Decl;

class Field;
    class Id_Field_Decl;
    class Array_Field_Decl;

class Method_Decl;
class Parameter;
class Field_Type;
class Method_Type;
class Block;

class Statement;
    class Location_Assign_Op;
    class Location_Incr;
    class Method_Call_Stmt;
    class If_Else_Stmt;
    class For_Stmt;
    class While_Stmt;
    class Return_Stmt;
    class Break_Stmt;
    class Continue_Stmt;

class For_Update;
    class For_Upd_Assign_Op;
    class For_Upd_Incr;

class Location;
    class Loc_Var;
    class Loc_Array;

class Expr;

    class Unary_Expr;
        class Minus_Expr;
        class Not_Expr;
        class INT_Expr;
        class LONG_Expr;
        class Paren_Expr;

    class Binary_Expr;
        class Mul_Op_Expr;
        class Add_Op_Expr;
        class Rel_Op_Expr;
        class Eq_Op_Expr;
        class Logic_Op_Expr;

    
    class Loc_Expr;
    class Method_Call_Expr;
    class Literal_Expr;
    class Len_Expr;
    

class Extern_Arg;
    class Expr_Arg;
    class String_Arg;

class Assign_Op;
class Increment;

class Bin_Op;
    class Mul_Op;
    class Add_Op;
    class Rel_Op;
    class Eq_Op;
    class Logic_Op;

class Literal;
    class Int_Lit;
    class Long_Lit;
    class Char_Lit;
    class Bool_Lit;

class Type;
class Id;


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



class AST_Node {
public:
    int row;
    int col;
    AST_Node(int row, int col) : row(row), col(col) {}

    virtual ~AST_Node() = default;
    virtual void accept(Visitor& visitor) = 0;
};

class Program : AST_Node {
public:
    std::vector<std::unique_ptr<Import_Decl>> import_decls;
    std::vector<std::unique_ptr<Field_Decl>>  field_decls;
    std::vector<std::unique_ptr<Method_Decl>> method_decls;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Import_Decl : AST_Node {
public:
    std::unique_ptr<Id> id;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Field_Decl : AST_Node {
    // TODO: thing again about this, 
    // but I am planning to add multiaple Field_Decl nodes if they are all delcared on the same line 
public:
    std::unique_ptr<Field_Type> field_type;
    std::unique_ptr<Field> field;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Field : AST_Node {};

class Id_Field_Decl : Field {
public:
    std::unique_ptr<Id> id;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Array_Field_Decl : Field {
public:
    std::unique_ptr<Id> id;
    std::unique_ptr<Int_Lit> size;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Method_Decl : AST_Node {
public:
    std::unique_ptr<Method_Type> method_type;
    std::unique_ptr<Id> id;
    std::vector<std::unique_ptr<Parameter>> parameters;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Parameter : AST_Node {
public:
    std::unique_ptr<Field_Type> field_type;
    std::unique_ptr<Id> id;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Method_Type : AST_Node {
public:
    std::unique_ptr<Type> type;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Field_Type : AST_Node {
public:
    std::unique_ptr<Type> type;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Block : AST_Node {
public:
    std::vector<std::unique_ptr<Field_Decl>> field_decls;
    std::vector<std::unique_ptr<Statement>> statements;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Statement : AST_Node {};

class Location_Assign_Op : Statement {
public:
    std::unique_ptr<Location> location;
    std::unique_ptr<Assign_Op> assign_op;
    std::unique_ptr<Expr> expr;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }    
};

class Location_Incr : Statement {
public:
    std::unique_ptr<Location> location;
    std::unique_ptr<Increment> increment;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }    
};

class Method_Call_Stmt : Statement {
public:
    std::unique_ptr<Id> id;
    std::vector<Extern_Arg> extern_args;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }   
};

class If_Else_Stmt : Statement {
public:
    std::unique_ptr<Expr> expr_if;
    std::unique_ptr<Block> block_then;
    std::unique_ptr<Block> block_else; // OPTIONAL

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class For_Stmt : Statement {
public:
    std::unique_ptr<Id> id;
    std::unique_ptr<Expr> expr_init;
    std::unique_ptr<Expr> expr_cond;
    std::unique_ptr<For_Update> for_update;

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class While_Stmt : Statement {
public:
    std::unique_ptr<Expr> expr_cond;
    std::unique_ptr<Block> block;

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Return_Stmt : Statement {
public:
    std::unique_ptr<Expr> expr; // OPTIONAL

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Break_Stmt : Statement {
public:
    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Continue_Stmt : Statement {
public:
    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class For_Update : AST_Node {};

class For_Upd_Assign_Op : For_Update {
public:
    std::unique_ptr<Location> location;
    std::unique_ptr<Assign_Op> assign_Op;
    std::unique_ptr<Expr> expr;

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class For_Upd_Incr : For_Update {
public:
    std::unique_ptr<Location> location;
    std::unique_ptr<Increment> increment;
    
    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Location : AST_Node {};

class Loc_Var : Location {
public:
    std::unique_ptr<Id> id;

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Loc_Array : Location {
public:
    std::unique_ptr<Id> id;
    std::unique_ptr<Expr> expr;

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Expr : AST_Node {}; // TODO: TYPE??

class Unary_Expr : Expr {
public:
    std::unique_ptr<Expr> expr;
}; 

class Minus_Expr : Unary_Expr {
public:

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Not_Expr : Unary_Expr {
public:

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};


class INT_Expr : Unary_Expr {
public:

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};


class LONG_Expr : Unary_Expr {
public:

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Paren_Expr : Unary_Expr {
public:

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};


class Bin_Expr : Expr {
public:
    std::unique_ptr<Expr> expr_lhs;
    std::unique_ptr<Bin_Op> bin_op;
    std::unique_ptr<Expr> expr_rhs;
};

class Mul_Op_Expr : Bin_Expr {
public:

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Add_Op_Expr : Bin_Expr {
public:

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Rel_Op_Expr : Bin_Expr {
public:

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Eq_Op_Expr : Bin_Expr {
public:

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Logic_Op_Expr : Bin_Expr {
public:

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Loc_Expr : Expr  {
public:
    std::unique_ptr<Location> location;

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Method_Call_Expr : Expr {
public:
    std::unique_ptr<Id> id;
    std::unique_ptr<Extern_Arg> exter_args;

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Literal_Expr : Expr {
public:
    std::unique_ptr<Literal> literal;
    
    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Len_Expr : Expr {
public:
    std::unique_ptr<Id> id;

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Extern_Arg : AST_Node {};

class Expr_Arg : Extern_Arg {
public:
    std::unique_ptr<Expr> expr;

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class String_Arg : Extern_Arg {
public:
    std::string string_arg;

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Assign_Op : AST_Node {
public:
    enum Type {
        ASSIGN,
        PLUS_ASSIGN,
        MINUS_ASSIGN,
        MUL_ASSIGN,
        DIV_ASSIGN,
        MOD_ASSIGN
    };

    Type type;

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Increment : AST_Node {
public:
    enum Type {
        INCREMENT,
        DECREMENT
    };
    Type type;

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Bin_Op : AST_Node {};

class Mul_Op : Bin_Op {
public:
    enum Type {
        STAR,
        DIV,
        MOD
    };
    Type type;

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Add_Op : Bin_Op {
public:
    enum Type {
        PLUS,
        MINUS,
    };
    Type type;

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Rel_Op : Bin_Op {
public:
    enum Type {
        LT,
        GT,
        LE,
        GE,
    };
    Type type;

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Eq_Op : Bin_Op {
public:
    enum Type {
        EQ,
        NEQ
    };
    Type type;

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Logic_Op : Bin_Op {
public:
    enum Type {
        OR,
        AND
    };
    Type type;

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Literal : AST_Node {
    // TODO: Don't forget the hex numbers
public:
    std::string literal;
};

class Int_Lit : Literal {
public:
    bool minus;

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Long_Lit : Literal {
public:

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Char_Lit : Literal {
public:

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Bool_Lit : Literal {
public:

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Type : AST_Node {
    enum Type_t {
        Int,
        Long,
        Char,
        Bool
    };
    Type_t type;
    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Id : AST_Node {
    std::string id;

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

}