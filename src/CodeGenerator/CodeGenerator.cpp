#include "CodeGenerator.h"

#define is_instance_of(uptr, Type) (dynamic_cast<Type*>((uptr).get()) != nullptr)


int CodeGenerator::Generate(std::ifstream& fin, std::ofstream& fout) {

    Semantics semantics;
    int valid_semantics = semantics.check(fin,fout);
    if (valid_semantics != 0 ) return valid_semantics;


    LinearBuilder linear_builder;
    std::unique_ptr<Linear::Program> linear_program = linear_builder.build (std::move(semantics.AST));

    Linear::PrettyPrinter printer;
    linear_program -> accept (printer); 

    linear_program -> accept (*this);
    fout << code();
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


std::string CodeGenerator::code() {}
void CodeGenerator::add_instr(std::string instr) {}
void CodeGenerator::add_comment(std::string instr) {}
void CodeGenerator::load (std::unique_ptr<Linear::Operand> src_operand, std::string dst_reg) {}
void CodeGenerator::store(std::string src_reg, std::unique_ptr<Linear::Location> dst_loc) {}

void CodeGenerator::push_scope(){}
void CodeGenerator::pop_scope (){}
bool CodeGenerator::is_global (){}
Info CodeGenerator::get(std::string id){}
void CodeGenerator::put(std::string id, Linear::Type type){}

int CodeGenerator::type_size(Linear::Type type){}
std::string CodeGenerator::instr_ (std::string id,  Linear::Type type){}
std::string CodeGenerator::reg_   (std::string reg, Linear::Type type){}



bool Symbol_Table::exist(std::string id) {}
Info Symbol_Table::get(std::string id){}
void Symbol_Table::put(std::string id, int offset, Linear::Type type){}