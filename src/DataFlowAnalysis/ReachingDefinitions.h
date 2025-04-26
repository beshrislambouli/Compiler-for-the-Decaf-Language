#include "CFG.h"
#include "DataFlowAnalysis.h"

namespace ReachingDefinitions {

using Def = int; // instruction index that has the def
using Var = std::string;


class Reaching_Definitions {
    CFG& cfg;
    std::vector<std::vector<bool>> IN;
    std::vector<std::vector<bool>> OUT;
    std::vector<std::vector<bool>> GEN;
    std::vector<std::vector<bool>> KILL;

    // helpers
    std::map <Var, std::vector<Def> > Var_to_Defs;
    std::map <Def, int> Def_to_bit;
public:
    Reaching_Definitions(CFG& cfg) : cfg(cfg) {
        Build();
    }

    
    void Build(){
        Build_Defs(); // collect all defs and assign an id;

        for (auto& BB : cfg.BBs) {
            for (int i = 0 ; i < BB.instrs.size() ; i ++ ) {
                Process_Instr (BB.instrs [i], GEN[BB.id], KILL[BB.id]);
            }
        }

        
        DataFlowAnalysis::DataFlowAnalysis DFS (  DataFlowAnalysis::DataFlowAnalysis::Reaching_Definitions_t,
                                cfg,
                                IN,
                                OUT,
                                GEN,
                                KILL
                                );
    }

    void Process_Instr (int def_id, std::vector<bool>& GEN, std::vector<bool>& KILL) {
        auto& instr = cfg.method->instrs [def_id];
        std::string dist = instr->get_dist();
        if (dist == "") return ;

        for (auto def : Var_to_Defs [dist]) {
            int bit = Def_to_bit [def] ;
            if (def == def_id) {
                GEN [bit] = true;
                KILL[bit] = false;
            } else {
                GEN [bit] = false;
                KILL[bit] = true;
            }
        }
    }

    void Build_Defs(){
        Def def_counter = 0 ;
        for (int i = 0 ; i < cfg.method->instrs.size() ; i ++ ) {
            auto& instr = cfg.method->instrs [i];

            std::string dist = instr->get_dist();
            if (dist == "") continue;
            if (Var_to_Defs.find (dist) == Var_to_Defs.end()) {
                std::vector<Def> tmp;
                Var_to_Defs [dist] = tmp;
            }

            Var_to_Defs [dist] .push_back (i);
            Def_to_bit  [i] = def_counter ++;
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
            std::cout << "Def: " << u.first << " BIT: " << u.second << std::endl;
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