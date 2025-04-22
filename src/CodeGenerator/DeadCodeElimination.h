#pragma once
#include <cassert>
#include <memory>
#include <iostream>
#include "Linear.h"
#include "CFG.h"

class Dead_Code_Elimination {
    CFG cfg;
    std::unique_ptr<Linear::Method>& method;
public:
    Dead_Code_Elimination(std::unique_ptr<Linear::Method>& method) 
    : method(method) 
    , cfg(method)
    {}


    void apply(){
        cfg.print();
    };
};