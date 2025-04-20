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
    push_scope();
    for (auto& method : program.methods) {
        method -> accept(*this);
    }
    pop_scope();
}

void CodeGenerator::visit(Linear::Method& method) {
    for (auto& instr : method.instrs) {
        instr->accept(*this);
    }
}

void CodeGenerator::visit(Linear::Operand& instr) {
    assert(false);
}
void CodeGenerator::visit(Linear::Literal& instr) {
    ret = "$" + instr.id;
}
void CodeGenerator::visit(Linear::Location& instr) {
    assert(false);
}
void CodeGenerator::visit(Linear::Var& instr) {
    ret = get_loc(instr.id);
}
void CodeGenerator::visit(Linear::Arr& instr) {
    assert(false);
}
void CodeGenerator::visit(Linear::Instr& instr) {}
void CodeGenerator::visit(Linear::Statement& instr) {}
void CodeGenerator::visit(Linear::Binary& instr) {}
void CodeGenerator::visit(Linear::Unary& instr) {}
void CodeGenerator::visit(Linear::Assign& instr) {
    load (instr.operands[0], "%rax");
    store ("%rax", instr.dist);
}
void CodeGenerator::visit(Linear::Helper& instr) {}
void CodeGenerator::visit(Linear::Push_Scope& instr) {
    push_scope();
}
void CodeGenerator::visit(Linear::Pop_Scope& instr) {
    pop_scope();
}
void CodeGenerator::visit(Linear::Declare& instr) {
    put (instr.location->id, instr.location->type);
}
void CodeGenerator::visit(Linear::Label& instr) {}
void CodeGenerator::visit(Linear::Method_Call& instr) {}
void CodeGenerator::visit(Linear::Return& instr) {}
void CodeGenerator::visit(Linear::Jump& instr) {}
void CodeGenerator::visit(Linear::J_Cond& instr) {}
void CodeGenerator::visit(Linear::J_UnCond& instr) {}


std::string CodeGenerator::code() {
    std::string x86 = "";
    for (auto u : asm_code) {
        x86 += u + '\n';
    }
    return x86;
}
void CodeGenerator::add_instr(std::string instr) {
    asm_code.push_back(instr);
}
void CodeGenerator::add_comment(std::string instr) {
    asm_code.push_back("# " + instr);
}
void CodeGenerator::load (std::unique_ptr<Linear::Operand>& src_operand, std::string dist_reg) {
    auto type = src_operand->type;
    add_instr( instr_("mov",type) + query(src_operand) + ", " + reg_(dist_reg,type) );
}
void CodeGenerator::store(std::string src_reg, std::unique_ptr<Linear::Location>& dist_loc) {
    auto type = dist_loc->type;
    add_instr( instr_("mov",type) + reg_(src_reg,type) + ", " + query(dist_loc) );
}

void CodeGenerator::push_scope(){
    bool is_global = symbol_table_stack.size()==0;
    symbol_table_stack.push_back(Symbol_Table(is_global));
}
void CodeGenerator::pop_scope (){
    assert(symbol_table_stack.size()>0);
    symbol_table_stack.pop_back();
}
bool CodeGenerator::is_global (){
    assert(symbol_table_stack.size()>0);
    return symbol_table_stack.back().is_global;
}
Info CodeGenerator::get(std::string id){
    for (int i = symbol_table_stack.size () - 1 ; i >= 0 ; i -- ) {
        if (symbol_table_stack[i].exist(id)) {
            return symbol_table_stack[i].get(id);
        }
    }
    assert(false);
}
std::string CodeGenerator::get_loc(std::string id){
    Info info = get(id);
    if (info.is_global) {
        return info.id + "(%rip)";
    } else {
        return std::to_string(info.offset) + "(%rbp)";
    }
}
void CodeGenerator::put(std::string id, Linear::Type type){
    assert(symbol_table_stack.size()>0);
    symbol_table_stack.back().put(id,stack_alloc(type),type);
}

int CodeGenerator::type_size(Linear::Type type){
    switch (type) {
        case Linear::Type::Int: return 4;
        case Linear::Type::Long: return 8;
        default: assert(false);
    }
}
std::string CodeGenerator::type_suffix(Linear::Type type){
    switch (type) {
        case Linear::Type::Int: return "l";
        case Linear::Type::Long: return "q";
        default: assert(false);
    }
}
int CodeGenerator::stack_alloc(Linear::Type type) {
    int type_size_ = type_size(type);
    stack_offset -= type_size_;
    if (abs(stack_offset) % type_size_ != 0 ) {
        stack_offset -= (type_size_ - (abs(stack_offset) % type_size_));
    }
    return stack_offset;
}
std::string CodeGenerator::instr_ (std::string id,  Linear::Type type){
    return id + type_suffix(type) + " ";
}
std::string CodeGenerator::reg_   (std::string reg, Linear::Type type){
    std::string reg_64[16] = {
        "%rax", "%rbx", "%rcx", "%rdx",
        "%rsi", "%rdi", "%rbp", "%rsp",
        "%r8",  "%r9",  "%r10", "%r11",
        "%r12", "%r13", "%r14", "%r15"
    };
    std::string reg_32[16] = {
        "%eax", "%ebx", "%ecx", "%edx",
        "%esi", "%edi", "%ebp", "%esp",
        "%r8d", "%r9d", "%r10d", "%r11d",
        "%r12d", "%r13d", "%r14d", "%r15d"
    };

    for (int i = 0 ; i < 16 ; i ++ ) {
        if (reg_32 [i] == reg || reg_64[i] == reg) {
            switch (type) {
                case Linear::Type::Int:  return reg_32[i];
                case Linear::Type::Long: return reg_64[i];
                default: assert(false);
            }
        }
    }
    assert(false);
}

std::string CodeGenerator::query(std::unique_ptr<Linear::Operand>& operand) {
    operand->accept(*this);
    return ret;
}

std::string CodeGenerator::query(std::unique_ptr<Linear::Location>& operand) {
    operand->accept(*this);
    return ret;
}

bool Symbol_Table::exist(std::string id) {
    return table.find(id) != table.end();
}
Info Symbol_Table::get(std::string id){
    assert(exit(id));
    return table [id];
}
void Symbol_Table::put(std::string id, int offset, Linear::Type type){
    assert(!exit(id));
    table [id]= Info(
        offset,
        type,
        id,
        is_global
    );
}