#pragma once
#include "CFG.h"
#include "DataFlowAnalysis.h"

namespace ReachingDefinitions {

using Def = int; // instruction index that has the def
using Use = int; // instruction index that has the use
using Var = std::string;


class Reaching_Definitions {
public:
    CFG& cfg;
    std::vector<std::vector<bool>> IN;
    std::vector<std::vector<bool>> OUT;
    std::vector<std::vector<bool>> GEN;
    std::vector<std::vector<bool>> KILL;

    // helpers
    std::map <Var, std::vector<Def> > Var_to_Defs;
    std::map <Def, int> Def_to_bit;
    std::map <Def, std::vector<Use> > Def_To_Uses;

    Reaching_Definitions(CFG& cfg) : cfg(cfg) {
        Build();
        DataFlowAnalysis::DataFlowAnalysis DFS (  
                DataFlowAnalysis::DataFlowAnalysis::Reaching_Definitions_t,
                cfg,
                IN,
                OUT,
                GEN,
                KILL
            );
    }

    
    void Build(){
        Build_Defs(); // collect all defs and assign an id;

        for (auto& BB : cfg.BBs) {
            for (int i = 0 ; i < BB.instrs.size() ; i ++ ) {
                Process_Instr (BB.instrs [i], GEN[BB.id], KILL[BB.id]);
            }
        }
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
    
    std::map <Def, std::vector<Use>> Def_Use_Chains () {
        
        for (auto u: Def_to_bit) {
            Def def = u.first;
            std::vector<Use> tmp;
            Def_To_Uses [def] = tmp;
        }


        for (auto& BB : cfg.BBs ) {
            
            // init reaching value is the IN to this block
            std::vector <bool> Cur_Reaching;
            std::vector <bool> place_holder (IN [BB.id].size());
            for (auto u : IN [BB.id] ) Cur_Reaching .push_back (u);

            for (int i = 0 ; i < BB.instrs.size () ; i ++ ) {
                auto& instr = cfg.method->instrs [BB.instrs[i]];
                
                // std::cout << "Cur_Reaching" << std::endl;
                // for (auto u: Cur_Reaching) std::cout << u ? "1" : "0" ;
                // std::cout << std::endl;
                
                // Linear::PrettyPrinter printer;
                // instr->accept(printer);

                std::vector<Var> vars = instr->get_operands();
                // for each use, get all its defs, and push this use to the reaching ones
                // note: this way if a use doesn't have a def it won't be in a chain
                // this is ok because it's either a global -> will not rename, or a non-defined local -> undefined
                for (auto& var : vars) {
                    // std::cout << "VAR: " << var << std::endl;
                    for (auto& def : Var_to_Defs [var] ) {
                        // std::cout << "DEF: " << def << " " ;
                        if ( Cur_Reaching [Def_to_bit [def]] == false ) continue;

                        Def_To_Uses [def]. push_back (BB.instrs[i]);

                    }
                    // std::cout << std::endl;
                }
                // update the reaching defs
                Process_Instr (BB.instrs[i], Cur_Reaching, place_holder);
            }

        }

        return Def_To_Uses;
    }


    void print () {
        cfg.print();
        for (auto& u : Def_to_bit) {
            std::cerr << "Def: " << u.first << " BIT: " << u.second << std::endl;
        }
        for (int i = 0 ; i < cfg.BBs.size () ; i ++ ) {
            std::cerr << "------------" << std::endl;
            std::cerr << "Block: " << i << std::endl;

            std::cerr << "IN" << std::endl;
            for (auto u: IN[i]) std::cerr << u ? "1" : "0" ;
            std::cerr << std::endl;

            std::cerr << "OUT" << std::endl;
            for (auto u: OUT[i]) std::cerr << u ? "1" : "0" ;
            std::cerr << std::endl;

            std::cerr << "GEN" << std::endl;
            for (auto u: GEN[i]) std::cerr << u ? "1" : "0" ;
            std::cerr << std::endl;

            std::cerr << "KILL" << std::endl;
            for (auto u:KILL[i]) std::cerr << u ? "1" : "0" ;
            std::cerr << std::endl;

            std::cerr << "------------" << std::endl;
        }
    }
};
}