#pragma once
#include "Linear.h"
#include <iostream>
#include "CFG.h"
#include <vector>
#include "ReachingDefinitions.h"

namespace Register_Allocator {
    using Def = int; // instruction index that has the def
    using Use = int; // instruction index that has the use
    using Var = std::string;

class RegisterAllocator {
    CFG& cfg;

public:
    RegisterAllocator (CFG& cfg) : cfg(cfg) {
        ReachingDefinitions::Reaching_Definitions r_d (cfg);
        std::map <Def, std::vector<Use> >& Chains = r_d.Def_Use_Chains();

        cfg.print();
        for (auto& chain : Chains) {
            std::cout << "Def : " << chain.first << std::endl;
            for (auto use : chain.second ) {
                std::cout << use << " " ;
            }
            std::cout << std::endl;
        }
    }
};
}