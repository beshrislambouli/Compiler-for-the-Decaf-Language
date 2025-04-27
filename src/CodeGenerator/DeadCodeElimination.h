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


            for (int i = BB.instrs.size() - 1 ; i >= 0 ; Process_Instr (liveness, BB, i, live), i --) {
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

            // // show(live_out,"LIVE OUT");
            // for (int i = 0 ; i < BB.instrs.size () ; i ++ ) {
            //     int instr_id = BB.instrs[i];
            //     auto& instr = cfg.method->instrs[instr_id];

            //     if ( is_instance_of (instr, Linear::Method_Call) ) continue; // don't remove instrs with side effects

            //     Var dist = instr->get_dist();
            //     if (dist == "" || Ignore.find(dist) != Ignore.end()) continue; // don't remove globals/local arrays

            //     // I want to get what is live out the block - what got killed after this instr
            //     std::vector<bool> KILL (live.size(), false);
            //     std::vector<bool> place_holder (live.size());
            //     liveness.Process_Block_Before_Index(BB.id, i, place_holder, KILL);
            //     // show(live,"LIVE after " + std::to_string (instr_id));

            //     int bit = liveness.Var_to_bit[dist];
            //     if ( live [liveness.Var_to_bit[dist]] == false) {
            //         // std::cout << "WILL DELETE " << instr_id << std::endl;
            //         instr_to_del .push_back (instr_id);
            //     }
            // }
        }

        std::sort    (instr_to_del.begin(), instr_to_del.end());
        std::reverse (instr_to_del.begin(), instr_to_del.end());

        for (auto idx : instr_to_del ) cfg.method -> instrs.erase (cfg.method->instrs.begin() + idx) ;

        return instr_to_del.size () > 0 ;
    }

    void Process_Instr (Liveness::Liveness& liveness, Basic_Block& BB, int idx_in_bb, std::vector<bool>& live) {
        auto& instr = cfg.method->instrs [BB.instrs[idx_in_bb]];

        Var dist = instr->get_dist();
        std::vector<Var> operands = instr->get_operands();

        // ORDER MATTER HERE
        live [liveness.Var_to_bit[dist]] = false;
        for (auto& operand : operands) {
            live [liveness.Var_to_bit[operand]] = true;
        }
    } 

    void show (std::vector<bool>& v, std::string xx) {
        std::cout << "-------------" << std::endl;
        std::cout << xx << std::endl;
        for (auto u : v ) std::cout << u ? "1" : "0";
        std::cout << std::endl;
        std::cout << "--------------" << std::endl;
    }
};
}