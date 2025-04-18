#include "CodeGenerator.h"


std::string suf(Linear::Type type) {
    switch (type) {
        case Linear::Int: return "l";
        case Linear::Long: return "q";
        default: return "z"; // Default to quadword
    }
}

std::string reg(int reg_num, Linear::Type type) {
    std::string ret = "%r";
    if ( reg_num == 0 ){
        ret += "10";
    } else {
        ret += "11";
    }
    if (type==Linear::Int) {
        ret += "d";
    }
    return ret;
}

int CodeGenerator::Generate(std::ifstream& fin, std::ofstream& fout) {

    Semantics semantics;
    int valid_semantics = semantics.check(fin,fout);
    if (valid_semantics != 0 ) return valid_semantics;


    LinearBuilder linear_builder;
    std::unique_ptr<Linear::Program> linear_program = linear_builder.build (std::move(semantics.AST));

    Linear::PrettyPrinter printer;
    linear_program -> accept (printer); 

    linear_program -> accept (*this);
    std::cout << assembler.asm_code;
    return 0;
} 



void CodeGenerator::visit(Linear::Program& program) {
    for (auto& method : program.methods) {
        method -> accept(*this);
    }
}

void CodeGenerator::visit(Linear::Method& method) {
    for (auto& instr : method.instrs) {
        instr->accept(*this);
    }
}
void CodeGenerator::visit(Linear::Operand& instr) {}
void CodeGenerator::visit(Linear::Literal& instr) {
    assembler.ret = "$" + instr.id;
}
void CodeGenerator::visit(Linear::Location& instr) {}
void CodeGenerator::visit(Linear::Var& instr) {
    assembler.ret = assembler.get(instr.id);
}
void CodeGenerator::visit(Linear::Arr& instr) {}
void CodeGenerator::visit(Linear::Instr& instr) {}
void CodeGenerator::visit(Linear::Statement& instr) {}
void CodeGenerator::visit(Linear::Binary& instr) {}
void CodeGenerator::visit(Linear::Unary& instr) {}
void CodeGenerator::visit(Linear::Assign& instr) {
    std::string code = "";
    Linear::Type _T = instr.dist->type;

    instr.operands[0]->accept(*this);
    std::string operand = assembler.ret;
    assembler.add_instr("mov" + suf(_T) + " " + operand + ", " + reg(0,_T));

    instr.dist->accept(*this);
    std::string dist = assembler.ret;
    assembler.add_instr("mov" + suf(_T) + " " + reg(0,_T) + ", " + dist);
}
void CodeGenerator::visit(Linear::Helper& instr) {}
void CodeGenerator::visit(Linear::Push_Scope& instr) {
    assembler.push_scope();
}
void CodeGenerator::visit(Linear::Pop_Scope& instr) {
    assembler.pop_scope();
}
void CodeGenerator::visit(Linear::Declare& instr) {
    assembler.declare(instr.location->id,instr.location->type);
}
void CodeGenerator::visit(Linear::Label& instr) {}
void CodeGenerator::visit(Linear::Method_Call& instr) {}
void CodeGenerator::visit(Linear::Return& instr) {}
void CodeGenerator::visit(Linear::Jump& instr) {}
void CodeGenerator::visit(Linear::J_Cond& instr) {}
void CodeGenerator::visit(Linear::J_UnCond& instr) {

}