#pragma once
#include "CFG.h"
#include "DataFlowAnalysis.h"

namespace AvailableCopyAssignment {

using Def = int; // instruction index that has the assignment
using Var = std::string;


class AvailableCopyAssignment {
public:
    CFG& cfg;
    std::vector<std::vector<bool>> IN;
    std::vector<std::vector<bool>> OUT;
    std::vector<std::vector<bool>> GEN;
    std::vector<std::vector<bool>> KILL;

    //helpers 
    std::map <Var, std::vector<Def> > Var_to_Defs;
    std::map <Def, int> Def_to_bit;
    
    AvailableCopyAssignment(CFG& cfg) : cfg(cfg) {
        Build () ;
        DataFlowAnalysis::DataFlowAnalysis DFS (  
                DataFlowAnalysis::DataFlowAnalysis::Available_Expressions_t,
                cfg,
                IN,
                OUT,
                GEN,
                KILL
            );
    }

    void Build () {
        Build_Defs();
        if (IN[0].size()==0) return;

        for (auto& BB : cfg.BBs) {
            for (int i = 0 ; i < BB.instrs.size () ; i ++ ) {
                Process_Instr (BB.instrs [i], GEN[BB.id], KILL[BB.id]);

                // std::cout << BB.instrs [i] << std::endl;
                // std::cout << "GEN" << std::endl;
                // for (auto u: GEN[BB.id]) std::cout << u ? "1" : "0" ;
                // std::cout << std::endl;

                // std::cout << "KILL" << std::endl;
                // for (auto u:KILL[BB.id]) std::cout << u ? "1" : "0" ;
                // std::cout << std::endl;
                // std::cout << "--------------" << std::endl;

            }
        }
    }

    void Process_Instr (int def_id, std::vector<bool>& GEN, std::vector<bool>& KILL) {
        auto& instr = cfg.method->instrs [def_id];

        if (instr->get_dist() != "") {
            Var dist = instr->get_dist();
            
            for (auto u : Def_to_bit) {
                Def def = u.first;
                int bit = u.second;

                Linear::Assign* assign_ptr = dynamic_cast <Linear::Assign*>(cfg.method->instrs [def].get());
                if (assign_ptr->get_dist() == dist || assign_ptr->operands[0]->id == dist) {
                    KILL [bit] = true;
                    GEN  [bit] = false;
                }
            }
        }

        if (is_instance_of (instr, Linear::Assign)) {
            GEN [Def_to_bit[def_id]] = true;
            KILL[Def_to_bit[def_id]] = false;
        }
    }

    void Build_Defs() {
        Def def_counter = 0 ;
        for (int i = 0 ; i < cfg.method->instrs.size() ; i ++ ) {
            auto& instr = cfg.method->instrs[i];
            if ( !is_instance_of (instr, Linear::Assign) ) continue;

            Var dist = instr->get_dist();
            if (Var_to_Defs.find (dist) == Var_to_Defs.end ()) {
                std::vector <Def> tmp;
                Var_to_Defs [dist] = tmp;
            }

            Var_to_Defs [dist] .push_back (i);
            Def_to_bit  [i] = def_counter ++ ;
        }
        for (int i = 0 ; i < cfg.BBs.size() ; i ++ ) {
            std::vector <bool> tmp(def_counter, false);
            IN  .push_back (tmp);
            OUT .push_back (tmp);
            GEN .push_back (tmp);
            KILL.push_back (tmp);
        }
    }

    void print () {
        cfg.print();
        for (auto& u : Def_to_bit) {
            std::cout << "Assignment: " << u.first << " BIT: " << u.second << std::endl;
        }
        for (int i = 0 ; i < cfg.BBs.size () ; i ++ ) {
            std::cout << "------------" << std::endl;
            std::cout << "Block: " << i << std::endl;

            std::cout << "IN" << std::endl;
            for (auto u: IN[i]) std::cout << u ? "1" : "0" ;
            std::cout << std::endl;

            std::cout << "OUT" << std::endl;
            for (auto u: OUT[i]) std::cout << u ? "1" : "0" ;
            std::cout << std::endl;

            std::cout << "GEN" << std::endl;
            for (auto u: GEN[i]) std::cout << u ? "1" : "0" ;
            std::cout << std::endl;

            std::cout << "KILL" << std::endl;
            for (auto u:KILL[i]) std::cout << u ? "1" : "0" ;
            std::cout << std::endl;

            std::cout << "------------" << std::endl;
        }
    }
};

}