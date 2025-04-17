
#pragma once
#include "Linear.h"
#include "DecafAST.h"
#include <iostream>

class Utils {
public:
    std::vector<std::unique_ptr<Linear::Instr>> instrs;
    std::unique_ptr<Linear::Operand> ret;

    
};