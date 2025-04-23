#include "Linear.h"
#include <map>

class Scope_Info {
public:
    std::map <std::string, std::string> new_name;

    bool exist (std::string id) {
        return new_name.find (id) != new_name.end();
    }

    void declare (std::string from, std::string to) {
       assert (!exist(from));
       new_name [from] = to;
    }

    std::string rename (std::string from) {
        assert (exist(from));
        return new_name [from];
    }
};
class Program_Info {
public:
    std::vector <Scope_Info> scope_stack;
    std::map <std::string, int> name_num;

    void declare (std::string from) {
        assert (scope_stack.size() > 0 );
        std::string to = from;
        // already have this , change the name 
        if (name_num.find (from) != name_num.end ()) {
            to += "_renamed_" + std::to_string (name_num[from] ++) ;
        } else {
            name_num [from] = 1;
        }

        scope_stack .back().declare (from,to);
    }

    std::string rename (std::string from) {
        for (int i = scope_stack.size () - 1 ; i >= 0 ; i -- ) {
            if (scope_stack [i].exist (from)) {
                return scope_stack [i].rename (from);
            }
        }
        assert (false);
    }

    void push_scope () {
        scope_stack .push_back (Scope_Info());
    }
    void pop_scope () {
        scope_stack .pop_back();
    }
};
class Preprocess : public Linear::Visitor {
    Program_Info program_info;
public:
    
    void visit(Linear::Program& program) {
        program_info .push_scope();

        for (auto& declare : program.globals ) {
            declare ->accept (*this);
        }

        for (auto& method : program.methods ) {
            method ->accept (*this);
        }

        program_info .pop_scope();
    }
    void visit(Linear::Method& method) {

        // program_info.declare (method.id);
        // method.id = program_info.rename (method.id);

        for (auto& var : method.params ) {
            program_info.declare (var->id);
            var->id = program_info.rename (var->id);
        }

        for (auto& instr : method.instrs ) {
            instr -> accept (*this);
        }
    }
    void visit(Linear::Operand& instr) {}
    void visit(Linear::Literal& instr) {}
    void visit(Linear::Location& instr) {}
    void visit(Linear::Var& instr) {
        instr.id = program_info.rename (instr.id);
    }
    void visit(Linear::Arr& instr) {
        instr.id = program_info.rename (instr.id);
        instr.index -> accept(*this);
    }
    void visit(Linear::Instr& instr) {}
    void visit(Linear::Statement& instr) {}
    void visit(Linear::Binary& instr) {
        instr.dist -> accept (*this);
        for (auto& operand : instr.operands) {
            operand -> accept(*this);
        }
    }
    void visit(Linear::Unary& instr) {
        instr.dist -> accept (*this);
        for (auto& operand : instr.operands) {
            operand -> accept(*this);
        }
    }
    void visit(Linear::Assign& instr) {
        instr.dist -> accept (*this);
        for (auto& operand : instr.operands) {
            operand -> accept(*this);
        }
    }
    void visit(Linear::Helper& instr) {}
    void visit(Linear::Push_Scope& instr) {
        program_info.push_scope();
    }
    void visit(Linear::Pop_Scope& instr) {
        program_info.pop_scope ();
    }
    void visit(Linear::Declare& instr) {
        program_info.declare (instr.location->id);
        instr.location ->accept (*this);
    }
    void visit(Linear::Label& instr) {}
    void visit(Linear::Method_Call& instr) {
        // instr.id = program_info.rename (instr.id);

        if (instr.return_location) {
            instr.return_location -> accept(*this);
        }
        for (auto& arg : instr.args ) {
            arg->accept(*this);
        }
    }
    void visit(Linear::Return& instr) {
        if (instr.return_value) {
            instr.return_value -> accept(*this);
        }
    }
    void visit(Linear::Jump& instr) {}
    void visit(Linear::J_Cond& instr) {
        instr.condition->accept(*this);
    }
    void visit(Linear::J_UnCond& instr) {}
};

