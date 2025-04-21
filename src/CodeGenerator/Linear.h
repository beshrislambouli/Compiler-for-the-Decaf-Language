
#pragma once 

#include <vector>
#include <string>
#include <memory>

namespace Linear {
    

class Program;
class Method;

class Operand;
    class Literal;
    class Location;
        class Var;
        class Arr;


class Instr;

    class Statement;
        class Binary;
        class Unary;
        class Assign;

    class Helper;
        class Push_Scope;
        class Pop_Scope;
        class Declare;
    
    class Label;
    class Method_Call;
    class Return;
    
    class Jump;
        class J_Cond;
        class J_UnCond;


class Visitor {
public:
    virtual ~Visitor() = default;

    virtual void visit(Program& program) = 0;
    virtual void visit(Method& method) = 0;
    virtual void visit(Operand& instr) = 0;
    virtual void visit(Literal& instr) = 0;
    virtual void visit(Location& instr) = 0;
    virtual void visit(Var& instr) = 0;
    virtual void visit(Arr& instr) = 0;
    virtual void visit(Instr& instr) = 0;
    virtual void visit(Statement& instr) = 0;
    virtual void visit(Binary& instr) = 0;
    virtual void visit(Unary& instr) = 0;
    virtual void visit(Assign& instr) = 0;
    virtual void visit(Helper& instr) = 0;
    virtual void visit(Push_Scope& instr) = 0;
    virtual void visit(Pop_Scope& instr) = 0;
    virtual void visit(Declare& instr) = 0;
    virtual void visit(Label& instr) = 0;
    virtual void visit(Method_Call& instr) = 0;
    virtual void visit(Return& instr) = 0;
    virtual void visit(Jump& instr) = 0;
    virtual void visit(J_Cond& instr) = 0;
    virtual void visit(J_UnCond& instr) = 0;
};

enum Type {
    Int,
    Long,
    Void
};

class Linear {
public:
    virtual ~Linear() = default;
    virtual void accept(Visitor& visitor) = 0 ;
};


class Program : public Linear {
public:
    std::vector<std::unique_ptr<Declare>>  globals;
    std::vector<std::unique_ptr<Method>>   methods;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Method : public Linear {
public:
    Type type;
    std::string id;
    std::vector<std::unique_ptr<Var>>   params;
    std::vector<std::unique_ptr<Instr>> instrs;
    
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};


class Operand : public Linear {
public:
    Type type;
    std::string id;

    Operand(){}
    Operand(Type type, std::string id) 
    : type(type)
    , id(id)
    {}
};


class Literal : public Operand {
public:
    bool is_string = false;

    Literal(){}
    Literal(Type type, std::string id) 
    : Operand(type,id)
    {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};



class Location : public Operand {
public:

    Location(){}
    Location(Type type, std::string id) 
    : Operand(type,id)
    {}
};

class Var : public Location {
public:
    bool is_array_var = false;
    
    Var(){}
    Var (Type type, std::string id) : Location(type,id){}
    Var (Type type, std::string id, bool is_array_var) : Location(type,id), is_array_var(is_array_var) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Arr : public Location {
public:
    std::unique_ptr<Operand> index;

    Arr (){}
    Arr (Type type, std::string id, std::unique_ptr<Operand>&& index) 
    : Location(type,id)
    , index(std::move(index))
    {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};


class Instr : public Linear {
public:
};

class Statement : public Instr {
public:
    std::unique_ptr<Location> dist;
    std::vector<std::unique_ptr<Operand>> operands;

    Statement(){}
    Statement(std::unique_ptr<Location>&& dist, std::unique_ptr<Operand>&& operand1)
    : dist(std::move(dist))
    {
        operands.push_back(std::move(operand1));
    }
    Statement(std::unique_ptr<Location>&& dist, std::unique_ptr<Operand>&& operand1, std::unique_ptr<Operand>&& operand2)
    : dist(std::move(dist))
    {
        operands.push_back(std::move(operand1));
        operands.push_back(std::move(operand2));
    }
};

class Binary : public Statement {
public:
    enum Op {
        Plus,
        Minus,
        Mul,
        Div,
        Mod,
        LT,
        GT,
        LE,
        GE,
        EQ,
        NEQ,
        OR,
        AND,
    };
    Op op;
    Binary(){}
    Binary(std::unique_ptr<Location>&& dist, std::unique_ptr<Operand>&& operand1, std::unique_ptr<Operand>&& operand2, Op op)
    : Statement(std::move(dist),std::move(operand1),std::move(operand2))
    , op(op)
    {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Unary : public Statement {
public:
    enum Op {
        Minus,
        Not,

    };
    Op op;

    Unary(std::unique_ptr<Location>&& dist, std::unique_ptr<Operand>&& operand1, Op op)
    : Statement(std::move(dist),std::move(operand1))
    , op(op)
    {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};


class Assign : public Statement {
public:
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};


class Helper : public Instr {};

class Push_Scope : public Helper {
public:
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Pop_Scope : public Helper {
public:
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
}; 

class Declare : public Helper {
public:
    std::unique_ptr<Location> location;
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
}; 

class Label : public Instr {
public:
    std::string label;

    Label(std::string label) : label(label) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Method_Call : public Instr {
public:
    std::string id;
    std::unique_ptr<Location> return_location; //OPTIONAL
    std::vector<std::unique_ptr<Operand>> args;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};


class Return : public Instr {
public:
    std::unique_ptr<Operand> return_value; // OPTIONAL
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Jump : public Instr {
public:
    std::string label;
    Jump(std::string label) : label(label) {}
};

class J_Cond : public Jump {
public:
    std::unique_ptr<Operand> condition;


    J_Cond(std::string label, std::unique_ptr<Operand>&& condition)
    : Jump (label)
    , condition(std::move(condition))
    {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class J_UnCond : public Jump {
public:

    J_UnCond(std::string label) : Jump(label) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};


}