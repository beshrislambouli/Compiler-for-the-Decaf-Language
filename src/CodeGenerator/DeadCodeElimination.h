#pragma once
#include <cassert>
#include <memory>
#include <iostream>
#include <set>
#include <algorithm>
#include "Linear.h"
#include "CFG.h"
#include "Liveness.h"


namespace DCE {

using Var = std::string;

class Dead_Code_Elimination {
    CFG& cfg;
    std::set<Var> Ignore;
public:
    Dead_Code_Elimination(std::vector<Var> Globals, CFG& cfg) 
    : cfg(cfg)
    {
        // (a) ignore the globals
        for (auto var : Globals) {
            Ignore .insert (var);
        }

        // (b) ignore the local arrays
        for (auto& instr : cfg.method -> instrs) {
            if ( ! is_instance_of (instr, Linear::Declare) ) continue;

            auto declare_ptr = dynamic_cast<Linear::Declare*>(instr.get());
            if ( is_instance_of (declare_ptr->location, Linear::Arr) ) {
                Ignore .insert (declare_ptr->location->id);
            }
        }
        
    }

    bool apply(){
        
        Liveness::Liveness liveness(cfg);

        std::vector<int> instr_to_del;
        for (auto& BB: cfg.BBs) {
            std::vector<bool> live = liveness.OUT[BB.id];


            for (int i = BB.instrs.size() - 1 ; i >= 0 ; liveness.Process_Instr (BB, i, live), i --) {
                auto& instr = cfg.method->instrs [BB.instrs[i]];

                 // don't remove instrs with side effects
                if ( is_instance_of (instr, Linear::Method_Call) ) continue;

                // don't remove globals/local arrays
                Var dist = instr->get_dist();
                if (dist == "" || Ignore.find(dist) != Ignore.end()) continue; 

                int bit = liveness.Var_to_bit[dist];
                if ( live [bit] == false) {
                    instr_to_del .push_back (BB.instrs[i]);
                }

            }
        }

        std::sort    (instr_to_del.begin(), instr_to_del.end());
        std::reverse (instr_to_del.begin(), instr_to_del.end());

        for (auto idx : instr_to_del ) cfg.method -> instrs.erase (cfg.method->instrs.begin() + idx) ;

        return instr_to_del.size () > 0 ;
    }

    void show (std::vector<bool>& v, std::string xx) {
        std::cerr << "-------------" << std::endl;
        std::cerr << xx << std::endl;
        for (auto u : v ) std::cerr << u ? "1" : "0";
        std::cerr << std::endl;
        std::cerr << "--------------" << std::endl;
    }
};
}