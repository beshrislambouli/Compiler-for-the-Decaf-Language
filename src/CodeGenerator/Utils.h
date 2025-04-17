
#pragma once
#include "Linear.h"
#include "DecafAST.h"
#include <iostream>

class Utils {
public:
    std::string method_name;
    std::vector<std::unique_ptr<Linear::Instr>> instrs;
    std::unique_ptr<Linear::Operand> ret;

    // the first one for continue and the second one for break
    std::vector<std::pair<std::string,std::string>> loop_labels;

    void push_instr(std::unique_ptr<Linear::Instr>&& instr) {
        instrs.push_back(std::move(instr));
    }

    void push_scope () {
        instrs.push_back (std::make_unique<Linear::Push_Scope>());
    }

    void pop_scope () {
        instrs.push_back (std::make_unique<Linear::Pop_Scope>());
    }

    void push_loop_labels(std::string continue_label, std::string break_label) {
        loop_labels.push_back(std::make_pair(continue_label,break_label));
    }
    void pop_loop_labels() {
        loop_labels.pop_back();
    }

    std::string continue_label(){
        return loop_labels.back().first;
    }
    std::string break_label(){
        return loop_labels.back().second;
    }

    void declare (Linear::Type type, std::string id) {
        auto instr_declare = std::make_unique<Linear::Declare>();

        auto var = std::make_unique<Linear::Var>();
        var->type = type;
        var ->id = id;

        instr_declare->location = std::move(var);

        instrs.push_back(std::move(instr_declare));
    }

    void declare (Linear::Type type, std::string id, std::string size) {
        auto instr_declare = std::make_unique<Linear::Declare>();
        
        auto arr = std::make_unique<Linear::Arr>();
        arr->type = type;
        arr->id = id;
        
        auto index = std::make_unique<Linear::Literal>();
        index->type = Linear::Int;
        index->id= size;

        arr -> index = std::move(index);

        instr_declare -> location = std::move(arr);

        instrs.push_back(std::move(instr_declare));
    }

    void assign (std::unique_ptr<Linear::Location>&& dist, std::unique_ptr<Linear::Operand>&& operand) {
        auto instr_assign = std::make_unique<Linear::Assign>();

        instr_assign->dist = std::move(dist);
        instr_assign->operands.push_back(std::move(operand));

        instrs.push_back(std::move(instr_assign));
    }

    void binary (std::unique_ptr<Linear::Location>&& dist, std::unique_ptr<Linear::Operand>&& operand1, std::unique_ptr<Linear::Operand>&& operand2, Linear::Binary::Op op) {
        auto instr_binary = std::make_unique<Linear::Binary>();

        instr_binary->dist = std::move(dist);
        instr_binary->operands.push_back(std::move(operand1));
        instr_binary->operands.push_back(std::move(operand2));
        instr_binary->op = op;

        instrs.push_back(std::move(instr_binary));
    }

    void label (std::string label) {
        push_instr(std::make_unique<Linear::Label>(label));
    }

    int label_num = 0;
    std::string get_label() {
        return "_" + method_name + "_L" + std::to_string(label_num++);
    }

    int tmp_num = 0;
    std::string get_tmp() {
        return "_" + method_name + "_TMP" + std::to_string(tmp_num++); 
    }
};