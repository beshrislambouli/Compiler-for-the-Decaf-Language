
#pragma once
#include "Linear.h"
#include "DecafAST.h"
#include <iostream>

class Utils {
public:
    std::vector<std::unique_ptr<Linear::Instr>> instrs;
    std::unique_ptr<Linear::Operand> ret;

    void push_scope () {
        instrs.push_back (std::make_unique<Linear::Push_Scope>());
    }

    void pop_scope () {
        instrs.push_back (std::make_unique<Linear::Pop_Scope>());
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
};