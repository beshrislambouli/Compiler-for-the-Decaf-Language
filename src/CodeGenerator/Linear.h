
namespace Linear {
    

class Program;
class method;

class Instr;

    class Statement;
        class Binary;
        class Unary;
        class Assign;

    class Branch;
    class Jump;
    class Method_Call;
    class Return;


    class Operand;

        class Literal;
            class Bool_Literal;
            class Int_Literal ;
            class Long_Literal;
            class String_Literal;
        
        class Location;
            class Var;
            class Arr;

    
    class Helper;
        class Add_Scope;
        class Pop_Scope;
        class Declare;
        class Label;


class Visitor {
public:
    virtual ~Visitor() = default;

    virtual void visit(Program& program) = 0;
    virtual void visit(method& method) = 0;
    virtual void visit(Binary& instr) = 0;
    virtual void visit(Unary& instr) = 0;
    virtual void visit(Assign& instr) = 0;
    virtual void visit(Branch& instr) = 0;
    virtual void visit(Jump& instr) = 0;
    virtual void visit(Method_Call& instr) = 0;
    virtual void visit(Return& instr) = 0;
    virtual void visit(Bool_Literal& instr) = 0;
    virtual void visit(Int_Literal& instr) = 0;
    virtual void visit(Long_Literal& instr) = 0;
    virtual void visit(String_Literal& instr) = 0;
    virtual void visit(Var& instr) = 0;
    virtual void visit(Arr& instr) = 0;
    virtual void visit(Add_Scope& instr) = 0;
    virtual void visit(Pop_Scope& instr) = 0;
    virtual void visit(Declare& instr) = 0;
    virtual void visit(Label& instr) = 0;
};

class Linear {
public:
    virtual ~Linear() = default;
    virtual void accept(Visitor& visitor) = 0 ;
};

}