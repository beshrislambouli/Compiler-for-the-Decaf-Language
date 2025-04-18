#pragma once 

#include <iostream>
#include <fstream>
#include "semantics.h"
#include "Linear.h"
#include "LinearBuilder.h"
#include "LinearPrinter.h"
#include "Assembler.h"
class CodeGenerator : Linear::Visitor {
    Assembler assembler;
public:
    int Generate(std::ifstream& fin, std::ofstream& fout);


    void visit(Linear::Program& program) override;
    void visit(Linear::Method& method) override;
    void visit(Linear::Operand& instr) override;
    void visit(Linear::Literal& instr) override;
    void visit(Linear::Location& instr) override;
    void visit(Linear::Var& instr) override;
    void visit(Linear::Arr& instr) override;
    void visit(Linear::Instr& instr) override;
    void visit(Linear::Statement& instr) override;
    void visit(Linear::Binary& instr) override;
    void visit(Linear::Unary& instr) override;
    void visit(Linear::Assign& instr) override;
    void visit(Linear::Helper& instr) override;
    void visit(Linear::Push_Scope& instr) override;
    void visit(Linear::Pop_Scope& instr) override;
    void visit(Linear::Declare& instr) override;
    void visit(Linear::Label& instr) override;
    void visit(Linear::Method_Call& instr) override;
    void visit(Linear::Return& instr) override;
    void visit(Linear::Jump& instr) override;
    void visit(Linear::J_Cond& instr) override;
    void visit(Linear::J_UnCond& instr) override;
};