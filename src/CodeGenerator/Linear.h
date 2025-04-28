
#pragma once 

#include <vector>
#include <string>
#include <memory>
#include <cassert>
#include <map>

#define is_instance_of(uptr, Type) (dynamic_cast<Type*>((uptr).get()) != nullptr)

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
    
    virtual std::string get_dist() = 0;
    virtual std::vector<std::string> get_operands() = 0;
};


class Program : public Linear {
public:
    std::vector<std::unique_ptr<Declare>>  globals;
    std::vector<std::unique_ptr<Method>>   methods;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }

    std::string get_dist() override {}
    std::vector<std::string> get_operands() override {}
};

class Method : public Linear {
public:
    Type type;
    std::string id;
    std::vector<std::unique_ptr<Var>>   params;
    std::vector<std::unique_ptr<Instr>> instrs;
    std::map <std::string, int> var_to_color;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }

    std::string get_dist() override {}
    std::vector<std::string> get_operands() override {}
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

    virtual std::unique_ptr<Operand> get_copy() = 0;
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

    std::unique_ptr<Operand> get_copy() override {
        auto ret = std::make_unique<Literal>(type,id);
        ret -> is_string = is_string;
        return ret;
    }

    std::string get_dist() override {
        return "";
    }
    std::vector<std::string> get_operands() override {
        std::vector<std::string> ret;
        return ret;
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

    std::unique_ptr<Operand> get_copy() override {
        return std::make_unique<Var>(type,id,is_array_var);
    }

    std::string get_dist() override {
        return "";
    }
    std::vector<std::string> get_operands() override {
        std::vector<std::string> ret;
        ret .push_back (id);
        return ret;
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

    std::unique_ptr<Operand> get_copy() override {
        return std::make_unique<Arr>(type,id,std::move(index->get_copy()));
    }

    std::string get_dist() override {
        return "";
    }
    std::vector<std::string> get_operands() override {
        std::vector<std::string> ret;

        ret.push_back (id);
        
        std::vector<std::string> tmp = index->get_operands();
        for (auto u : tmp) ret .push_back (u);
    
        return ret;
    }
};


class Instr : public Linear {
public:
};

class Statement : public Instr {
public:
    std::unique_ptr<Operand> dist;
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

    std::string get_dist() override {
        return dist->id;
    }
    std::vector<std::string> get_operands() override {
        std::vector<std::string> ret;

        for (auto& operand : operands) {
            std::vector<std::string> tmp = operand->get_operands();
            for (auto u : tmp) ret .push_back (u);
        }

        // NOTE: you also need to get b,c but not a in a [b[c]] = 2;
        std::vector<std::string> tmp = dist->get_operands();
        for (auto u : tmp) {
            if ( u == dist->id) continue; // don't add a
            ret .push_back (u);
        }

        return ret;
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

    std::string hash () {
        std::string ret = "";
        ret += operands[0]->id + "_";
        switch (op) {
            // Add cases when Op enum is populated
            case Binary::Plus:
                ret +="+";
                break;
            case Binary::Minus:
                ret +="-";
                break;
            case Binary::Mul:
                ret +="*";
                break;
            case Binary::Div:
                ret +="/";
                break;
            case Binary::Mod:
                ret +="%";
                break;
            case Binary::LT:
                ret +="<";
                break;
            case Binary::GT:
                ret +=">";
                break;
            case Binary::LE:
                ret +="<=";
                break;
            case Binary::GE:
                ret +=">=";
                break;
            case Binary::EQ:
                ret +="==";
                break;
            case Binary::NEQ:
                ret +="!=";
                break;
            case Binary::OR:
                ret +="||";
                break;
            case Binary::AND:
                ret +="&&";
                break;
            default: assert(false);
        }

        ret += "_" + operands[1]->id;

        return ret;
    }
};

class Unary : public Statement {
public:
    enum Op {
        Minus,
        Not,
        LONG_CAST,
    };
    Op op;

    Unary(std::unique_ptr<Location>&& dist, std::unique_ptr<Operand>&& operand1, Op op)
    : Statement(std::move(dist),std::move(operand1))
    , op(op)
    {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }

    std::string get_dist() override {
        return dist->id;
    }
};


class Assign : public Statement {
public:
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }

};


class Helper : public Instr { // should not need to use them
public:
    std::string get_dist() override {
        return "";
    }
    std::vector<std::string> get_operands() override {
        std::vector<std::string> ret;
        return ret;
    }
};

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
    std::unique_ptr<Operand> location;
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

    std::string get_dist() override {
        return "";
    }
    std::vector<std::string> get_operands() override {
        std::vector<std::string> ret;
        return ret;
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

    std::string get_dist() override {
        if ( return_location ) {
            return return_location->id;
        } else {
            return "";
        }
    }
    std::vector<std::string> get_operands() override {
        std::vector<std::string> ret;
        for (auto& operand : args) {
            std::vector<std::string> tmp = operand->get_operands();
            for (auto u : tmp) ret .push_back (u);
        }
        return ret;
    }
};


class Return : public Instr {
public:
    std::unique_ptr<Operand> return_value; // OPTIONAL
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }

    std::string get_dist() override {
        return "";
    }
    std::vector<std::string> get_operands() override {
        std::vector<std::string> ret;
        if ( return_value ) {
            std::vector<std::string> tmp = return_value->get_operands();
            for (auto u : tmp) ret .push_back (u);
        }
        return ret;
    }
};

class Jump : public Instr {
public:
    std::string label;
    Jump(std::string label) : label(label) {}
};

class J_Cond : public Jump {
public:
    std::string jump_on = ""; // "0" or "1"
    std::unique_ptr<Operand> condition;


    J_Cond(std::string label, std::string jump_on, std::unique_ptr<Operand>&& condition)
    : Jump (label)
    , jump_on(jump_on)
    , condition(std::move(condition))
    {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }

    std::string get_dist() override {
        return "";
    }
    std::vector<std::string> get_operands() override {
        std::vector<std::string> ret;

        std::vector<std::string> tmp = condition->get_operands();
        for (auto u : tmp) ret .push_back (u);

        return ret;
    }
};

class J_UnCond : public Jump {
public:

    J_UnCond(std::string label) : Jump(label) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }

    std::string get_dist() override {
        return "";
    }
    std::vector<std::string> get_operands() override {
        std::vector<std::string> ret;
        return ret;
    }
};


}