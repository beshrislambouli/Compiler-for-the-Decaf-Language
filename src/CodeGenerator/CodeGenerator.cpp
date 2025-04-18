#include "CodeGenerator.h"


int CodeGenerator::Generate(std::ifstream& fin, std::ofstream& fout) {

    Semantics semantics;
    int valid_semantics = semantics.check(fin,fout);
    if (valid_semantics != 0 ) return valid_semantics;


    LinearBuilder linear_builder;
    std::unique_ptr<Linear::Program> linear_program = linear_builder.build (std::move(semantics.AST));

    Linear::PrettyPrinter printer;
    linear_program -> accept (printer); 

    linear_program -> accept (*this);
    return 0;
} 



void CodeGenerator::visit(Linear::Program& program) {}
void CodeGenerator::visit(Linear::Method& method) {}
void CodeGenerator::visit(Linear::Operand& instr) {}
void CodeGenerator::visit(Linear::Literal& instr) {}
void CodeGenerator::visit(Linear::Location& instr) {}
void CodeGenerator::visit(Linear::Var& instr) {}
void CodeGenerator::visit(Linear::Arr& instr) {}
void CodeGenerator::visit(Linear::Instr& instr) {}
void CodeGenerator::visit(Linear::Statement& instr) {}
void CodeGenerator::visit(Linear::Binary& instr) {}
void CodeGenerator::visit(Linear::Unary& instr) {}
void CodeGenerator::visit(Linear::Assign& instr) {}
void CodeGenerator::visit(Linear::Helper& instr) {}
void CodeGenerator::visit(Linear::Push_Scope& instr) {}
void CodeGenerator::visit(Linear::Pop_Scope& instr) {}
void CodeGenerator::visit(Linear::Declare& instr) {}
void CodeGenerator::visit(Linear::Label& instr) {}
void CodeGenerator::visit(Linear::Method_Call& instr) {}
void CodeGenerator::visit(Linear::Return& instr) {}
void CodeGenerator::visit(Linear::Jump& instr) {}
void CodeGenerator::visit(Linear::J_Cond& instr) {}
void CodeGenerator::visit(Linear::J_UnCond& instr) {}