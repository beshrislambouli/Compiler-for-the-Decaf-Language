#pragma once
#include "CFG.h"
#include "DataFlowAnalysis.h"

namespace Liveness{

using Var = std::string;

class Liveness {
public:
    CFG& cfg;
    std::vector<std::vector<bool>> IN;
    std::vector<std::vector<bool>> OUT;
    std::vector<std::vector<bool>> GEN; // USE
    std::vector<std::vector<bool>> KILL; // DEF

    //helpers
    std::map <Var, int> Var_to_bit;


    Liveness (CFG& cfg) : cfg(cfg) {
        Build ();
        DataFlowAnalysis::DataFlowAnalysis DFS (  
                DataFlowAnalysis::DataFlowAnalysis::Liveness_t,
                cfg,
                IN,
                OUT,
                GEN,
                KILL
            );
    }

    void Build() {
        Build_Vars(); // collect all ids and assign a bit

        for (auto& BB : cfg.BBs) {
            Process_Block_Before_Index (BB.id, -1, GEN[BB.id], KILL[BB.id]);
        }


    }

    void Process_Block_Before_Index(int BB_id, int index, std::vector<bool>& GEN, std::vector<bool>& KILL) {
        auto& BB = cfg.BBs[BB_id];
        // process all instrs in bb before index
        for (int i = BB.instrs.size () - 1 ; i > index ; i -- ) {
            // get all the vars that get defined in the block before this instr
            // to not include their uses as GEN 
            std::set<Var> defined_in_block;
            for (int j = 0 ; j < i ; j ++) {
                int instr_id = BB.instrs[j];
                auto& instr = cfg.method->instrs [instr_id];
                std::string dist = instr->get_dist();
                if (dist != "") {
                    defined_in_block.insert (dist);
                }
                
            }

            int instr_id = BB.instrs[i];
            auto& instr = cfg.method->instrs [instr_id];


            Var dist = instr->get_dist();
            std::vector<Var> operands = instr->get_operands();
            
            // KILL: set of variables defined in block
            KILL [Var_to_bit[dist]] = true;

            // GEN: set of variables with upwards exposed uses in block
            for (auto& operand : operands) {
                // this means that it's a use for a def not in this block
                if ( defined_in_block .find (operand) == defined_in_block.end() ) {
                    GEN [Var_to_bit[operand]] = true;
                }
            }
        }
    }

    void Process_Instr (Basic_Block& BB, int idx_in_bb, std::vector<bool>& live) {
        auto& instr = cfg.method->instrs [BB.instrs[idx_in_bb]];

        Var dist = instr->get_dist();
        std::vector<Var> operands = instr->get_operands();

        // ORDER MATTER HERE
        live [Var_to_bit[dist]] = false;
        for (auto& operand : operands) {
            live [Var_to_bit[operand]] = true;
        }
    } 

    void Build_Vars() {
        
        // collect all ids and assign a bit for them
        std::set<std::string> ids;
        for (auto& instr : cfg.method->instrs) {
            std::string dist = instr->get_dist();
            std::vector<std::string> operands = instr->get_operands();

            if (ids .find (dist) == ids.end()) {
                Var_to_bit [dist] = ids.size () ;
                ids .insert (dist);
            }
            

            for (auto& operand : operands) {
                if (ids .find (operand) == ids.end()) {
                    Var_to_bit [operand] = ids.size () ;
                    ids .insert (operand);
                }
            }
        }

        for (int i = 0 ; i < cfg.BBs.size () ; i ++) {
            std::vector <bool> tmp (ids.size(), false);
            IN  .push_back (tmp);
            OUT .push_back (tmp);
            GEN .push_back (tmp);
            KILL.push_back (tmp);
        }

    }

    void print () {
        cfg.print();
        for (auto& u : Var_to_bit) {
            std::cout << "Var: " << u.first << " BIT: " << u.second << std::endl;
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
