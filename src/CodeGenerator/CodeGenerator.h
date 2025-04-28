#pragma once 

#include <iostream>
#include <fstream>
#include <cassert>
#include "semantics.h"
#include "Linear.h"
#include "LinearBuilder.h"
#include "LinearPrinter.h"
#include "DeadCodeElimination.h"
// #include "Common_Subexpression_Elimination.h"
#include "preprocess.h"
#include "RegisterAllocator.h"
#include "CFG.h"
#include "ReachingDefinitions.h"
class Info;
class Symbol_Table;

class CodeGenerator : public Linear::Visitor {
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

private:
    int stack_offset = 0;
    int string_label = 0;
    std::string method_name = "";
    std::vector<std::string> asm_code;
    std::vector<Symbol_Table> symbol_table_stack;
    std::vector<std::pair<std::string,std::string>> used_strings; // (label, content)

     // code
    std::string code();
    void add_instr(std::string instr);
    void add_comment(std::string instr);
    void load (std::unique_ptr<Linear::Operand>& src_operand, std::string dist_reg);
    void load (std::unique_ptr<Linear::Location>& src_operand, std::string dist_reg);
    void store(std::string src_reg, std::unique_ptr<Linear::Location>& dist_loc);
    void store(std::string src_reg, std::unique_ptr<Linear::Operand>& dist_loc);

    // REG
    std::vector<std::string> REG = {
        "%rdi", "%rsi", "%rdx", 
        // "%rcx", "%r8", "%r9",
        "%r10", "%r11", 
        "%r12", "%r13", "%r14", "%r15"
    };
    std::map <std::string, int> cur_method_var_to_color;

    //symbol table
    void push_scope();
    void pop_scope ();
    bool is_global ();
    Info get(std::string id);
    std::string get_loc(std::string id);
    void put(std::string id, Linear::Type type);
    void put(std::string id, Linear::Type type, std::string size);

    //helpers
    int type_size(Linear::Type type);
    std::string type_suffix(Linear::Type type);
    int stack_alloc(Linear::Type type);
    std::string instr_ (std::string id,  Linear::Type type);
    std::string reg_   (std::string reg, Linear::Type type);

    std::string ret;
    std::string query(std::unique_ptr<Linear::Operand>& operand);
    std::string query(std::unique_ptr<Linear::Location>& operand);
};

class Info {
public:
    int offset = 0;
    Linear::Type type;
    std::string id = "";
    bool is_global = false;
 
    Info(){}

    Info (int offset, Linear::Type type, std::string id, bool is_global)
    : offset(offset)
    , type(type) 
    , id (id)
    , is_global(is_global)
    {}
};

class Symbol_Table { // to use the visit functions
    std::map<std::string,Info> table;
public:
    bool is_global;
    Symbol_Table(bool is_global) : is_global(is_global) {}

    bool exist(std::string id) ;
    Info get(std::string id);
    void put(std::string id, int offset, Linear::Type type);
};
