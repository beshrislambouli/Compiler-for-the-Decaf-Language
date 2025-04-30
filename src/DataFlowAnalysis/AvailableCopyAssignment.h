#pragma once
#include "CFG.h"
#include "DataFlowAnalysis.h"

namespace AvailableCopyAssignment {

using Var = std::string;

class Assign {
public:
    Var dist= "";
    Var src = "";
    int bit = -1;
    std::vector <int> idxs;
    Assign (int idx, Var dist, Var src, int bit) : dist(dist), src(src) , bit (bit) {
        idxs.push_back (idx);
    }
};

class AvailableCopyAssignment {
public:
    CFG& cfg;
    std::vector<std::vector<bool>> IN;
    std::vector<std::vector<bool>> OUT;
    std::vector<std::vector<bool>> GEN;
    std::vector<std::vector<bool>> KILL;

    std::vector<Assign> Assigns;
    
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
        if (instr->get_dist() == "" ) return ;
    
        Var dist = instr->get_dist();
        for (auto& assign : Assigns) {
            if ( dist == assign.dist || dist == assign.src ) {
                KILL [assign.bit] = true;
                GEN  [assign.bit] = false;
            }
        }

        if (is_instance_of (instr, Linear::Assign)) {
            auto assign_ptr = dynamic_cast<Linear::Assign*>(instr.get());
            Var src = assign_ptr->operands[0]->id;
            for (auto& assign : Assigns) {
                if ( dist == assign.dist && src == assign.src ) {
                    KILL [assign.bit] = false;
                    GEN  [assign.bit] = true ;
                }
            }
        }
    }

    void Build_Defs() {
        int assign_counter = 0 ;
        std::set <std::pair<Var,Var>> already;
        for (int i = 0 ; i < cfg.method->instrs.size() ; i ++ ) {
            auto& instr = cfg.method->instrs[i];
            if ( !is_instance_of (instr, Linear::Assign) ) continue;

            auto assign_ptr = dynamic_cast<Linear::Assign*>(instr.get());
            std::pair asn = std::make_pair (assign_ptr->dist->id, assign_ptr->operands[0]->id);
            
            if ( already.find (asn) == already.end () ) {
                already.insert (asn);
                Assigns.push_back (Assign(i,assign_ptr->dist->id, assign_ptr->operands[0]->id, assign_counter++));
            } else {
                for (auto& assign : Assigns) {
                    if (assign .dist == asn.first && assign.src == asn.second) {
                        assign.idxs.push_back (i);
                    }
                }
            }
        }

        for (int i = 0 ; i < cfg.BBs.size() ; i ++ ) {
            std::vector <bool> tmp(assign_counter, false);
            IN  .push_back (tmp);
            OUT .push_back (tmp);
            GEN .push_back (tmp);
            KILL.push_back (tmp);
        }
    }

    void print () {
        cfg.print();
        for (auto& u : Assigns) {
            std::cout << "Assignment: " << u.dist << " = " << u.src << " BIT: " << u.bit << std::endl;
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