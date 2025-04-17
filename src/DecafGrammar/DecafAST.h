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

class Type : public AST_Node {
public:
    enum Type_t {
        Int,
        Long,
        Bool,
        Void,
        Null_Type,
    };
    Type_t type = Null_Type;
    
    Type(int row, int col) : AST_Node(row, col) {}

    Type(Type_t type, int row, int col) : type(type), AST_Node(row,col) {}
    
    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Program : public AST_Node {
public:
    std::vector<std::unique_ptr<Import_Decl>> import_decls;
    std::vector<std::unique_ptr<Field_Decl>>  field_decls;
    std::vector<std::unique_ptr<Method_Decl>> method_decls;

    Program(int row, int col) : AST_Node(row,col) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Import_Decl : public AST_Node {
public:
    std::unique_ptr<Id> id;
    
    Import_Decl(int row, int col) : AST_Node(row, col) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Field_Decl : public AST_Node {
public:
    std::unique_ptr<Field_Type> field_type;
    std::vector<std::unique_ptr<Field>> fields;
    
    Field_Decl(int row, int col) : AST_Node(row, col) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Field : public AST_Node {
public:
    std::unique_ptr<Id> id;
    Field(int row, int col) : AST_Node(row, col) {}
};

class Id_Field_Decl : public Field {
public:
    
    Id_Field_Decl(int row, int col) : Field(row, col) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Array_Field_Decl : public Field {
public:
    std::unique_ptr<Int_Lit> size;
    
    Array_Field_Decl(int row, int col) : Field(row, col) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Method_Decl : public AST_Node {
public:
    std::unique_ptr<Method_Type> method_type;
    std::unique_ptr<Id> id;
    std::vector<std::unique_ptr<Parameter>> parameters;
    std::unique_ptr<Block> block;
    
    Method_Decl(int row, int col) : AST_Node(row, col) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Parameter : public AST_Node {
public:
    std::unique_ptr<Field_Type> field_type;
    std::unique_ptr<Id> id;
    
    Parameter(int row, int col) : AST_Node(row, col) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Method_Type : public AST_Node {
public:
    std::unique_ptr<Type> type;
    
    Method_Type(int row, int col) : AST_Node(row, col) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Field_Type : public AST_Node {
public:
    std::unique_ptr<Type> type;
    
    Field_Type(int row, int col) : AST_Node(row, col) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Block : public AST_Node {
public:
    std::vector<std::unique_ptr<Field_Decl>> field_decls;
    std::vector<std::unique_ptr<Statement>> statements;
    
    Block(int row, int col) : AST_Node(row, col) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Statement : public AST_Node {
public:
    Statement(int row, int col) : AST_Node(row, col) {}
};

class Location_Assign_Op : public Statement {
public:
    std::unique_ptr<Location> location;
    std::unique_ptr<Assign_Op> assign_op;
    std::unique_ptr<Expr> expr;
    
    Location_Assign_Op(int row, int col) : Statement(row, col) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }    
};

class Location_Incr : public Statement {
public:
    std::unique_ptr<Location> location;
    std::unique_ptr<Increment> increment;
    
    Location_Incr(int row, int col) : Statement(row, col) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }    
};

class Method_Call_Stmt : public Statement {
public:
    std::unique_ptr<Id> id;
    std::vector<std::unique_ptr<Extern_Arg>> extern_args;
    
    Method_Call_Stmt(int row, int col) : Statement(row, col) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }   
};

class If_Else_Stmt : public Statement {
public:
    std::unique_ptr<Expr> expr_if;
    std::unique_ptr<Block> block_then;
    std::unique_ptr<Block> block_else; // OPTIONAL
    
    If_Else_Stmt(int row, int col) : Statement(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class For_Stmt : public Statement {
public:
    std::unique_ptr<Id> id;
    std::unique_ptr<Expr> expr_init;
    std::unique_ptr<Expr> expr_cond;
    std::unique_ptr<For_Update> for_update;
    std::unique_ptr<Block> block;
    
    For_Stmt(int row, int col) : Statement(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class While_Stmt : public Statement {
public:
    std::unique_ptr<Expr> expr_cond;
    std::unique_ptr<Block> block;
    
    While_Stmt(int row, int col) : Statement(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Return_Stmt : public Statement {
public:
    std::unique_ptr<Expr> expr; // OPTIONAL
    
    Return_Stmt(int row, int col) : Statement(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Break_Stmt : public Statement {
public:
    Break_Stmt(int row, int col) : Statement(row, col) {}
    
    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Continue_Stmt : public Statement {
public:
    Continue_Stmt(int row, int col) : Statement(row, col) {}
    
    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class For_Update : public AST_Node {
public:
    For_Update(int row, int col) : AST_Node(row, col) {}
};

class For_Upd_Assign_Op : public For_Update {
public:
    std::unique_ptr<Location> location;
    std::unique_ptr<Assign_Op> assign_Op;
    std::unique_ptr<Expr> expr;
    
    For_Upd_Assign_Op(int row, int col) : For_Update(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class For_Upd_Incr : public For_Update {
public:
    std::unique_ptr<Location> location;
    std::unique_ptr<Increment> increment;
    
    For_Upd_Incr(int row, int col) : For_Update(row, col) {}
    
    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Location : public AST_Node {
public:
    std::unique_ptr<Id> id;

    std::unique_ptr<Type> type_t;

    Location(int row, int col) : AST_Node(row, col) {}

    void assign_type(Type::Type_t t) {
        type_t = std::make_unique<Type>(t, -1,-1);
    }
};

class Loc_Var : public Location {
public:
    Loc_Var(int row, int col) : Location(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Loc_Array : public Location {
public:
    std::unique_ptr<Expr> expr;
    
    Loc_Array(int row, int col) : Location(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Expr : public AST_Node {
public:
    std::unique_ptr<Type> type_t;
    
    Expr(int row, int col) : AST_Node(row, col) {}

    void assign_type(Type::Type_t t) {
        type_t = std::make_unique<Type>(t, -1,-1);
    }
};

class Unary_Expr : public Expr {
public:
    std::unique_ptr<Expr> expr;
    
    Unary_Expr(int row, int col) : Expr(row, col) {}
}; 

class Minus_Expr : public Unary_Expr {
public:
    Minus_Expr(int row, int col) : Unary_Expr(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Not_Expr : public Unary_Expr {
public:
    Not_Expr(int row, int col) : Unary_Expr(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};


class INT_Expr : public Unary_Expr {
public:
    INT_Expr(int row, int col) : Unary_Expr(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};


class LONG_Expr : public Unary_Expr {
public:
    LONG_Expr(int row, int col) : Unary_Expr(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Paren_Expr : public Unary_Expr {
public:
    Paren_Expr(int row, int col) : Unary_Expr(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};


class Bin_Expr : public Expr {
public:
    std::unique_ptr<Expr> expr_lhs;
    std::unique_ptr<Bin_Op> bin_op;
    std::unique_ptr<Expr> expr_rhs;
    
    Bin_Expr(int row, int col) : Expr(row, col) {}
};

class Mul_Op_Expr : public Bin_Expr {
public:
    Mul_Op_Expr(int row, int col) : Bin_Expr(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Add_Op_Expr : public Bin_Expr {
public:
    Add_Op_Expr(int row, int col) : Bin_Expr(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Rel_Op_Expr : public Bin_Expr {
public:
    Rel_Op_Expr(int row, int col) : Bin_Expr(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Eq_Op_Expr : public Bin_Expr {
public:
    Eq_Op_Expr(int row, int col) : Bin_Expr(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Logic_Op_Expr : public Bin_Expr {
public:
    Logic_Op_Expr(int row, int col) : Bin_Expr(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Loc_Expr : public Expr  {
public:
    std::unique_ptr<Location> location;
    
    Loc_Expr(int row, int col) : Expr(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Method_Call_Expr : public Expr {
public:
    std::unique_ptr<Id> id;
    std::vector<std::unique_ptr<Extern_Arg>> extern_args;
    
    Method_Call_Expr(int row, int col) : Expr(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Literal_Expr : public Expr {
public:
    std::unique_ptr<Literal> literal;
    
    Literal_Expr(int row, int col) : Expr(row, col) {}
    
    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Len_Expr : public Expr {
public:
    std::unique_ptr<Id> id;
    
    Len_Expr(int row, int col) : Expr(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Extern_Arg : public AST_Node {
public:
    Extern_Arg(int row, int col) : AST_Node(row, col) {}
};

class Expr_Arg : public Extern_Arg {
public:
    std::unique_ptr<Expr> expr;
    
    Expr_Arg(int row, int col) : Extern_Arg(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class String_Arg : public Extern_Arg {
public:
    std::string string_arg;
    
    String_Arg(int row, int col) : Extern_Arg(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Assign_Op : public AST_Node {
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
    
    Assign_Op(int row, int col) : AST_Node(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Increment : public AST_Node {
public:
    enum Type {
        INCREMENT,
        DECREMENT
    };
    Type type;
    
    Increment(int row, int col) : AST_Node(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Bin_Op : public AST_Node {
public:
    enum Type {
        STAR,
        DIV,
        MOD,
        PLUS,
        MINUS,
        LT,
        GT,
        LE,
        GE,
        EQ,
        NEQ,
        OR,
        AND,
    };
    Type type;
    Bin_Op(int row, int col) : AST_Node(row, col) {}
    Bin_Op(Type type, int row, int col) : type(type), AST_Node(row, col) {}
};

class Mul_Op : public Bin_Op {
public:
    Mul_Op(int row, int col) : Bin_Op(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Add_Op : public Bin_Op {
public:
    Add_Op(int row, int col) : Bin_Op(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Rel_Op : public Bin_Op {
public:
    Rel_Op(int row, int col) : Bin_Op(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Eq_Op : public Bin_Op {
public:
    Eq_Op(int row, int col) : Bin_Op(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Logic_Op : public Bin_Op {
public:    
    Logic_Op(int row, int col) : Bin_Op(row, col) {}

    Logic_Op(Type type, int row, int col) : Bin_Op(type,row,col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Literal : public AST_Node {
    // TODO: Don't forget the hex numbers
public:
    bool minus = false;
    std::string literal="";
    std::unique_ptr<Type> type_t;
    
    Literal(int row, int col) : AST_Node(row, col) {}

    Literal(std::string literal, int row, int col) : literal(literal), AST_Node(row, col) {}

    void assign_type(Type::Type_t t) {
        type_t = std::make_unique<Type>(t, -1,-1);
    }
};

class Int_Lit : public Literal {
public:    
    Int_Lit(int row, int col) : Literal(row, col) {}

    Int_Lit(std::string literal, int row, int col)
    : Literal(literal, row, col)
    {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Long_Lit : public Literal {
public:
    Long_Lit(int row, int col) : Literal(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Char_Lit : public Literal {
public:
    Char_Lit(int row, int col) : Literal(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Bool_Lit : public Literal {
public:
    Bool_Lit(int row, int col) : Literal(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }
};


class Id : public AST_Node {
public:
    std::string id="";
    
    std::unique_ptr<Type> type_t;
    
    Id(int row, int col) : AST_Node(row, col) {}

    Id(std::string id, int row, int col) : id(id), AST_Node(row, col) {}

    void accept (Visitor& visitor) override {
        visitor.visit(*this);
    }

    void assign_type(Type::Type_t t) {
        type_t = std::make_unique<Type>(t, -1,-1);
    }
};

}