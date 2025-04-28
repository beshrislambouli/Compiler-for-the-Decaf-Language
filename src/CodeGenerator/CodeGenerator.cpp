#include "CodeGenerator.h"

#define is_instance_of(uptr, Type) (dynamic_cast<Type*>((uptr).get()) != nullptr)


int CodeGenerator::Generate(std::ifstream& fin, std::ofstream& fout) {

    Semantics semantics;
    int valid_semantics = semantics.check(fin,fout);
    if (valid_semantics != 0 ) return valid_semantics;


    LinearBuilder linear_builder;
    std::unique_ptr<Linear::Program> linear_program = linear_builder.build (std::move(semantics.AST));

    Linear::PrettyPrinter printer;
    // linear_program -> accept (printer);

    // RENAMING 
    // (1) rename so that no two ids have the same name any where
    Preprocess preprocess;
    linear_program -> accept (preprocess);

    // (2) rename according to webs

    // (a) collect globals so i ignore them
    std::vector <std::string> globals;
    for (auto& dec : linear_program ->globals) {
        globals .push_back (dec->location->id);
    }

    // (b) use webs to rename each method
    for (auto& method : linear_program ->methods) {
        CFG cfg (method);
        Register_Allocator::RegisterAllocator reg (globals, cfg, REG);
        for (auto& web : reg.webs) {
            if (!web.spilled) {
                method->var_to_color [web.new_id] = web.color;
            }
        }
        // linear_program -> accept (printer);

        // EDIT THE DECLARES

        // First, collect any id used anywhere (you don't want to delete decls for those vars)
        // vars that are not V_reg are those that have a use before any def
        std::set<std::string> ids;
        for (auto& instr : method->instrs) {
            std::string dist = instr->get_dist();
            std::vector<std::string> operands = instr->get_operands();

            ids .insert (dist);
            for (auto& operand : operands) {
                ids .insert (operand);
            }
        } 

        // Now collect all the defs for unused vars anymore, record their type so that the new
        // V_reg get the correct type
        std::vector<int> declare_to_del;
        std::map <std::string, Linear::Type> original_id_to_type;
        for (int i = 0 ; i < cfg.method->instrs.size () ; i ++ ) {
            auto& instr = cfg.method->instrs[i];
            if ( !is_instance_of (instr, Linear::Declare) ) continue;

            auto declare_ptr = dynamic_cast<Linear::Declare*>(instr.get());
            if ( !is_instance_of (declare_ptr->location, Linear::Var) ) ;
            std::string original_id =  declare_ptr->location->id;
            Linear::Type type = declare_ptr->location->type;
            
            // while all local vars should be renamed in theory
            // you still need this is because there might be a declare to a var that get used before a def -> not in webs
            // changing the declare name would make a use of a var without a declare
            if ( ids.find (original_id) != ids.end() ) continue;

            declare_to_del .push_back (i);
            original_id_to_type [original_id] = type;
        }

        std::sort    (declare_to_del.begin(), declare_to_del.end());
        std::reverse (declare_to_del.begin(), declare_to_del.end());
        
        // del old declared
        for (auto idx : declare_to_del) method->instrs.erase (method->instrs.begin() + idx) ;
        
        // add the webs' ones
        for (auto& web : reg.webs) {
            
            auto var = std::make_unique <Linear::Var>();
            var ->id = web.new_id;
            var ->type = original_id_to_type [web.original_id];

            auto declare = std::make_unique <Linear::Declare>();
            declare->location = std::move(var);

            method -> instrs .insert (method->instrs.begin() +1 , std::move (declare)); // declare after the push_scope
        }
    }

    // linear_program -> accept (printer);

    linear_program -> accept (*this);
    fout << code();
    return 0;
} 



void CodeGenerator::visit(Linear::Program& program) {
    push_scope();

    add_instr(".text");

    if (program.globals.size()){
        add_instr(".data");
        for (auto& dec : program.globals) {
            dec -> accept(*this);
        }
        add_instr(".text");
    }

    // error_falloff msg
    add_instr(".L_falloff_error_msg:");
    std::string error_msg = "     .string \"Runtime Error: Control fell off end of non-void function.\\n\"";
    add_instr(error_msg);
    add_instr("     .align 4");

    int place_holder_index = asm_code.size();
    add_comment("no_need_to_alloc_string");

    add_instr(".globl main");
    for (auto& method : program.methods) {
        cur_method_var_to_color = method->var_to_color;
        method -> accept(*this);
    }

    // add error_falloff method
    add_instr("error_falloff:");
    add_instr("movq $1, %rax"); // sys call write
    add_instr("movq $2, %rdi"); // fd: stderr
    add_instr("leaq .L_falloff_error_msg(%rip), %rsi"); // msg
    add_instr("movq $58, %rdx"); // lenght of the msg
    add_instr("syscall");

    add_instr("movq $231, %rax"); // sys call exit_group
    add_instr("movq $-1, %rdi"); // exit code
    add_instr("syscall");
    add_instr("hlt");


    if (used_strings.size () > 0 ) {
        std::string declare_strings = "";
        for (auto u : used_strings) {
            std::string label = u.first;
            std::string content = u.second;

            declare_strings += label + ":\n";
            declare_strings += "     .string \"" + content + "\"\n";
            declare_strings += "     .align 4\n";
        }
        asm_code[place_holder_index] = declare_strings;
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

    add_instr("pushq %r12");
    add_instr("pushq %r13");
    add_instr("pushq %r14");
    add_instr("pushq %r15");
    
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
            add_instr( instr_("mov",type) + src + ", " + reg_("%rax",type) );
            add_instr( instr_("mov",type) + reg_("%rax",type) + ", " + loc );
        }
        
    }

    // already pushed the scope
    for (int i = 1 ; i < method.instrs.size () ; i ++ ) {
        method.instrs[i] -> accept(*this);
    }

    stack_offset = abs(stack_offset);
    if ( stack_offset % 16 != 0 ){
        stack_offset += (16 - (stack_offset % 16) ) % 16;
    }
    if (stack_offset > 0 ) {
        asm_code [place_holder_index] = "subq $" + std::to_string(stack_offset) + ", %rsp";
    }
    
    

    if (method.type != Linear::Type::Void) {
        add_instr("call error_falloff");
    }
    
    add_instr(".L_" + method_name + "_epilogue:");

    add_instr("popq %r15");
    add_instr("popq %r14");
    add_instr("popq %r13");
    add_instr("popq %r12");

    add_instr("movq %rbp, %rsp");
    add_instr("popq %rbp");
    if ( method_name == "main" ) add_instr("movq $0, %rax");
    add_instr("ret");
    
}

void CodeGenerator::visit(Linear::Operand& instr) {
    assert(false);
}
void CodeGenerator::visit(Linear::Literal& instr) {
    if (instr.is_string) {
        std::string label = ".STR_" + std::to_string(string_label++);
        used_strings.push_back(std::make_pair(label,instr.id));

        ret = "$" + label;
    } else {
        ret = "$" + instr.id;
    }
    
}
void CodeGenerator::visit(Linear::Location& instr) {
    assert(false);
}
void CodeGenerator::visit(Linear::Var& instr) {
    if (instr.is_array_var) {
        Info info = get(instr.id);

        if (info.is_global) {
            ret = "$" + info.id;
        } else {
            add_instr("leaq " + get_loc(instr.id) + ", " + "%rbx" );
            ret = "%rbx";
        }
    } else if (cur_method_var_to_color.find (instr.id) != cur_method_var_to_color.end ()) {
        ret = reg_ (REG[cur_method_var_to_color[instr.id]],instr.type);
    } else {
        ret = get_loc(instr.id);
    }
    
}
void CodeGenerator::visit(Linear::Arr& instr) {

    // load the index to %rax
    load (instr.index, "%rbx");
    if (instr.index->type != Linear::Type::Long ) add_instr("movslq %ebx, %rbx");

    Info info = get(instr.id);
    if (info.is_global) {
        ret = info.id + "(,%rbx," + std::to_string(type_size(info.type)) + ")";
    } else {
        ret = std::to_string(info.offset) + "(%rbp,%rbx," + std::to_string(type_size(info.type)) + ")";
    }
}
void CodeGenerator::visit(Linear::Instr& instr) {
    assert(false);
}
void CodeGenerator::visit(Linear::Statement& instr) {
    assert(false);
}
void CodeGenerator::visit(Linear::Binary& instr) {
    auto type = instr.operands[0]->type;
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
        type = instr.dist->type;
        add_instr ( instr_("movzb",type) + "%al, " + reg_("%rax",type));
    }

    store (rax, instr.dist);
}
void CodeGenerator::visit(Linear::Unary& instr) {
    auto type = instr.dist->type;
    std::string rax = reg_("%rax",type);
    std::string rbx = reg_("%rbx",type);

    
    switch (instr.op)
    {
    case Linear::Unary::Minus:
        // load to register
        load (instr.operands[0], rax);
        add_instr( instr_ ("neg", type) + rax);
        break;

    case Linear::Unary::Not:
        // load to register
        load (instr.operands[0], rax);

        add_instr( instr_("cmp",type) + "$0, " + rax);
        add_instr( "sete %al" );
        add_instr (instr_ ("movzb",type) + "%al, " + rax);
        break;
    
    case Linear::Unary::LONG_CAST:
        load (instr.operands[0], "%eax"); // instr.operands[0] has to be int from linear builder
        add_instr("movslq %eax, %rax");
        break;

    default:
        assert(false);
        break;
    }
    
    store (rax, instr.dist);
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
    if ( is_instance_of (instr.location, Linear::Var)) {

        put (instr.location->id, instr.location->type);

    } else if ( is_instance_of (instr.location, Linear::Arr) ){

        Linear::Arr* arr = dynamic_cast<Linear::Arr*>(instr.location.get());
        put (arr->id, arr->type, arr->index->id);

    } else {
        assert(false);
    }
    
}

void CodeGenerator::visit(Linear::Label& instr) {
    add_instr(instr.label + ":");
}
void CodeGenerator::visit(Linear::Method_Call& instr) {
    std::string param_reg [] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};

    // save caller-saved regs, NOTE: pay attention to the alignment
    // add_instr("pushq %r10");
    // add_instr("pushq %r11");
    // for (int i = 0 ; i < 6 ; i ++ ) {
    //     add_instr("pushq " + param_reg[i]);
    // }

    // 16-alignment before call
    if (instr.args.size() > 6) {
        int to_align = ( 16 - ( ( instr.args.size() - 6 ) * 8 ) % 16 ) % 16 ;
        add_instr("subq $" + std::to_string(to_align) + ", " + "%rsp" );
    }

    // push +6 args to stack
    for (int i = instr.args.size() - 1 ; i >= 6 ; i -- ) {
        load (instr.args[i], "%rax");
        add_instr ("pushq %rax");
    }

    // load args
    for (int i = std::min ((int)instr.args.size()-1,5) ; i >= 0 ; i -- ) {
        load (instr.args[i], param_reg[i]);
    }
    
    // System V ABI: to use printf
    add_instr("movq $0, %rax");

    add_instr("call " + instr.id) ;

    // clean up
    if (instr.args.size() > 6) {
        int to_align = ( 16 - ( ( instr.args.size() - 6 ) * 8 ) % 16 ) % 16 ;
        to_align += ( instr.args.size() - 6 ) * 8 ;
        add_instr("addq $" + std::to_string(to_align) + ", " + "%rsp" );
    }

    // for (int i = 5 ; i >= 0 ; i -- ) {
    //     add_instr("popq " + param_reg[i]);
    // }
    // add_instr("popq %r11");
    // add_instr("popq %r10");

    // store return value
    if (instr.return_location){
        store ("%rax", instr.return_location);
    }
}
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

    add_instr (cmp + "$" + instr.jump_on + ", " + reg);
    add_instr ("je " + instr.label);
}
void CodeGenerator::visit(Linear::J_UnCond& instr) {
    add_instr("jmp " + instr.label);
}


std::string CodeGenerator::code() {
    std::string x86 = "";
    for (auto u : asm_code) {
        if (u.back() != ':' && u[0] != '.' ) x86 += "     ";
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
void CodeGenerator::load (std::unique_ptr<Linear::Location>& src_operand, std::string dist_reg) {
    auto type = src_operand->type;
    add_instr( instr_("mov",type) + query(src_operand) + ", " + reg_(dist_reg,type) );
}
void CodeGenerator::store(std::string src_reg, std::unique_ptr<Linear::Location>& dist_loc) {
    auto type = dist_loc->type;
    add_instr( instr_("mov",type) + reg_(src_reg,type) + ", " + query(dist_loc) );
}
void CodeGenerator::store(std::string src_reg, std::unique_ptr<Linear::Operand>& dist_loc) {
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

    if (is_global()) {
        add_instr(".comm " + id + ", " + std::to_string (type_size(type)) + ", " + std::to_string (type_size(type)));
    }
}
void CodeGenerator::put(std::string id, Linear::Type type, std::string size_){
    int size = std::stoi(size_);
    assert ( size > 0 );

    int offset = 0;
    for (int i = 0 ; i < size ; i ++ ) {
        offset = stack_alloc(type);
    }

    symbol_table_stack.back().put(id,offset,type);

    if (is_global()) {
        add_instr(".comm " + id + ", " + std::to_string ( size * type_size(type)) + ", 8");
    }
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
    assert(exis(id));
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