
#include <vector>
#include <string>
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
        class Add_Scope;
        class Pop_Scope;
        class Declare;
    
    class Label;
    class Method_Call;
    class Return;
    
    class Jump;
        class J_Cond;
        class J_Uncond;


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
    virtual void visit(Add_Scope& instr) = 0;
    virtual void visit(Pop_Scope& instr) = 0;
    virtual void visit(Declare& instr) = 0;
    virtual void visit(Label& instr) = 0;
    virtual void visit(Method_Call& instr) = 0;
    virtual void visit(Return& instr) = 0;
    virtual void visit(Jump& instr) = 0;
    virtual void visit(J_Cond& instr) = 0;
    virtual void visit(J_Uncond& instr) = 0;
};

enum Type {
    Int,
    Long,
    Bool,
    Void
};

class Linear {
public:
    virtual ~Linear() = default;
    virtual void accept(Visitor& visitor) = 0 ;
};


class Program : public Linear {
public:
    std::vector<Location> globals;
    std::vector<Method> methods;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Method : public Linear {
public:
    Type type;
    std::string id;
    std::vector<Location> params;
    std::vector<Instr> instrs;
    
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};


class Operand : public Linear {
public:
    Type type;
    std::string id;
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Literal : public Operand {
public:
    bool is_string = false;
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};



class Location : public Operand {};

class Var : public Location {
public:
    bool is_array_var = false;
    
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Arr : public Location {
public:
    Operand index;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};


class Instr : public Linear {};

class Statement : public Instr {
public:
    Operand dist;
    std::vector<Operand> operands;
};

class Binary : public Statement {
public:
    enum Op {

    };
    Op op;
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Unary : public Statement {
public:
    enum Op {
        
    };
    Op op;
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

class Add_Scope : public Helper {
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
    Type type;
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
}; 

class Label : public Instr {
public:
    std::string label;
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Method_call : public Instr {
public:
    std::string id;
    Location return_location;
    std::vector<Operand> args;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};


class Return : public Instr {
public:
    Operand return_value;
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class Jump : public Instr {
public:
    std::string label;
};

class J_Cond : public Jump {
public:
    Operand condition;
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

class J_UnCond : public Jump {
public:
    Operand condition;
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};


}