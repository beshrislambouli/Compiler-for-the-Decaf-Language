#include "Linear.h"

class Preprocess : public Linear::Visitor {
public:

    void visit(Linear::Program& program) {}
    void visit(Linear::Method& method) {}
    void visit(Linear::Operand& instr) {}
    void visit(Linear::Literal& instr) {}
    void visit(Linear::Location& instr) {}
    void visit(Linear::Var& instr) {}
    void visit(Linear::Arr& instr) {}
    void visit(Linear::Instr& instr) {}
    void visit(Linear::Statement& instr) {}
    void visit(Linear::Binary& instr) {}
    void visit(Linear::Unary& instr) {}
    void visit(Linear::Assign& instr) {}
    void visit(Linear::Helper& instr) {}
    void visit(Linear::Push_Scope& instr) {}
    void visit(Linear::Pop_Scope& instr) {}
    void visit(Linear::Declare& instr) {}
    void visit(Linear::Label& instr) {}
    void visit(Linear::Method_Call& instr) {}
    void visit(Linear::Return& instr) {}
    void visit(Linear::Jump& instr) {}
    void visit(Linear::J_Cond& instr) {}
    void visit(Linear::J_UnCond& instr) {}
};

