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
    
    method_name = method.id;
    stack_offset = 0;

    add_instr(method.id + ":");
    add_instr("pushq %rbp");
    add_instr("movq %rsp, %rbp");

    int place_holder_index = asm_code.size();
    add_comment("no_need_to_alloc");
    
    // to push a scope
    method.instrs[0] -> accept(*this);

    std::string param_reg [] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    for (int i = 0 ; i < method.params.size () ; i ++ ) {
        std::unique_ptr<Linear::Var>& param = method.params[i];
        
        // declare 
        put (param->id, param->type);

        // get location on stack
        std::string loc = get_loc(param->id);

        //move
        auto type = param->type;
        if (i < 6) {
            add_instr( instr_("mov",type) + reg_(param_reg[i],type) + ", " + loc);
        } else {
            std::string src = std::to_string (16 + 8 * (i-6)) + "(%rbp)";
            add_instr( instr_("mov",type) + src + ", " + reg_("%r10",type) );
            add_instr( instr_("mov",type) + reg_("%r10",type) + ", " + loc );
        }
        
    }

    // already pushed the scope
    for (int i = 1 ; i < method.instrs.size () ; i ++ ) {
        method.instrs[i] -> accept(*this);
    }

    stack_offset = abs(stack_offset);
    if ( stack_offset > 0 ) {
        if ( stack_offset % 16 != 0 ){
            stack_offset += (16 - (stack_offset % 16) );
        }
        asm_code [place_holder_index] = "subq $" + std::to_string(stack_offset) + ", %rsp";
    }
    

    add_instr(".L_" + method_name + "_epilogue:");
    add_instr("movq %rbp, %rsp");
    add_instr("popq %rbp");
    add_instr("ret");
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
void CodeGenerator::visit(Linear::Instr& instr) {
    assert(false);
}
void CodeGenerator::visit(Linear::Statement& instr) {
    assert(false);
}
void CodeGenerator::visit(Linear::Binary& instr) {
    auto type = instr.dist ->type;
    std::string rax = reg_("%rax",type);
    std::string rbx = reg_("%rbx",type);

    load (instr.operands[0], rax);
    load (instr.operands[1], rbx);

    bool is_compare = false;
    switch (instr.op)
    {
    case Linear::Binary::Plus: 
        add_instr( instr_ ("add",type) + rbx + ", " + rax);
        break;
    
    case Linear::Binary::Minus: 
        add_instr( instr_ ("sub",type) + rbx + ", " + rax);
        break;
    
    case Linear::Binary::Mul: 
        add_instr( instr_ ("imul",type) + rbx + ", " + rax);
        break;

    case Linear::Binary::Div: 
        if (type == Linear::Type::Long) add_instr("cqto");
        else                            add_instr("cltd");
        add_instr( instr_ ("idiv",type) + rbx);
        break;

    case Linear::Binary::Mod: 
        if (type == Linear::Type::Long) add_instr("cqto");
        else                            add_instr("cltd");
        add_instr( instr_ ("idiv",type) + rbx);
        add_instr( instr_ ("mov",type) + reg_("%rdx",type) + ", " + rax);
        break;
    
    case Linear::Binary::EQ: 
        add_instr( instr_ ("cmp",type) + rbx + ", " + rax);
        add_instr("sete %al");
        is_compare = true;
        break;
    
    case Linear::Binary::NEQ: 
        add_instr( instr_ ("cmp",type) + rbx + ", " + rax);
        add_instr("setne %al");
        is_compare = true;
        break;
    
    case Linear::Binary::LT: 
        add_instr( instr_ ("cmp",type) + rbx + ", " + rax);
        add_instr("setl %al");
        is_compare = true;
        break;
    
    case Linear::Binary::LE: 
        add_instr( instr_ ("cmp",type) + rbx + ", " + rax);
        add_instr("setle %al");
        is_compare = true;
        break;
    
    case Linear::Binary::GT: 
        add_instr( instr_ ("cmp",type) + rbx + ", " + rax);
        add_instr("setg %al");
        is_compare = true;
        break;
    
    case Linear::Binary::GE: 
        add_instr( instr_ ("cmp",type) + rbx + ", " + rax);
        add_instr("setge %al");
        is_compare = true;
        break;
    
    case Linear::Binary::AND: 
        add_instr( instr_ ("and",type) + rbx + ", " + rax);
        break;

    case Linear::Binary::OR: 
        add_instr( instr_ ("or",type) + rbx + ", " + rax);
        break;
    
    default:
        assert(false);
        break;
    }

    if (is_compare) {
        add_instr ( instr_("movzb",type) + "%al, " + rax);
    }

    store (rax, instr.dist);
}
void CodeGenerator::visit(Linear::Unary& instr) {
    auto type = instr.dist->type;
    std::string reg = reg_("%rax",type);

    // load to register
    load (instr.operands[0], reg);
    
    switch (instr.op)
    {
    case Linear::Unary::Minus:
        add_instr( instr_ ("neg", type) + reg);
        break;
    case Linear::Unary::Not:
        add_instr( instr_("cmp",type) + "$0, " + reg);
        add_instr( "sete %al" );
        add_instr (instr_ ("movzb",type) + "%al, " + reg);
        break;

    default:
        assert(false);
        break;
    }
    
    store (reg, instr.dist);
}
void CodeGenerator::visit(Linear::Assign& instr) {
    load (instr.operands[0], "%rax");
    store ("%rax", instr.dist);
}
void CodeGenerator::visit(Linear::Helper& instr) {
    assert(false);
}
void CodeGenerator::visit(Linear::Push_Scope& instr) {
    push_scope();
}
void CodeGenerator::visit(Linear::Pop_Scope& instr) {
    pop_scope();
}
void CodeGenerator::visit(Linear::Declare& instr) {
    put (instr.location->id, instr.location->type);
}
void CodeGenerator::visit(Linear::Label& instr) {
    add_instr(instr.label + ":");
}
void CodeGenerator::visit(Linear::Method_Call& instr) {}
void CodeGenerator::visit(Linear::Return& instr) {
    if (instr.return_value) {
        load (instr.return_value, "%rax");
    }
    add_instr ("jmp .L_" + method_name + "_epilogue");
}
void CodeGenerator::visit(Linear::Jump& instr) {
    assert(false);
}
void CodeGenerator::visit(Linear::J_Cond& instr) {
    auto type = instr.condition->type;
    std::string reg = reg_("%rax",type);
    std::string cmp = instr_("cmp", type);

    load(instr.condition, reg);

    add_instr (cmp + "$0, " + reg);
    add_instr ("jne " + instr.label);
}
void CodeGenerator::visit(Linear::J_UnCond& instr) {
    add_instr("jmp " + instr.label);
}


std::string CodeGenerator::code() {
    std::string x86 = "";
    for (auto u : asm_code) {
        if (u.back() != ':') x86 += "     ";
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