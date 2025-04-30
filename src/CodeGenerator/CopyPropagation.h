#pragma once
#include "CFG.h"
#include "AvailableCopyAssignment.h"
#include "RegisterAllocator.h"

namespace CopyPropagation {

using Var = std::string ;    

class CopyPropagation {
    CFG& cfg;
    std::set<Var> Ignore;
public:
    CopyPropagation(std::vector<Var> Globals, CFG& cfg) : cfg(cfg) {

        // (a) ignore the globals
        for (auto var : Globals) {
            Ignore .insert (var);
        }

        // (b) ignore the local arrays
        // (c) ignore the args since they are needed for reg alloc
        for (auto& instr : cfg.method -> instrs) {
            if ( ! is_instance_of (instr, Linear::Declare) ) continue;

            auto declare_ptr = dynamic_cast<Linear::Declare*>(instr.get());
            if ( is_instance_of (declare_ptr->location, Linear::Arr) ) {
                Ignore .insert (declare_ptr->location->id);
            }

            std::string prefix = "FUNC_ARG_";
            if (declare_ptr->location->id.compare(0, prefix.size(), prefix) == 0) {
                Ignore .insert (declare_ptr->location->id);
            }
        }

    }

    bool apply () {
        AvailableCopyAssignment::AvailableCopyAssignment ACA (cfg);
        // ACA.print();
        bool changed = false;

        for (auto& BB: cfg.BBs) {
            std::vector<bool> live = ACA.IN[BB.id];
            std::vector<bool> place_holder (live.size());
            if (live.size () == 0 ) continue;
            
            for (int i = 0 ; i < BB.instrs.size () ; ACA.Process_Instr(BB.instrs[i],live,place_holder), i ++) {
                // std::cout << "INSTR: " << BB.instrs [i] << std::endl;
                // std::cout << "live: ";
                // for (auto u: live) std::cout << u ? "1" : "0" ;
                // std::cout << std::endl;

                auto& instr = cfg.method ->instrs [BB.instrs[i]];
                
                auto uses = instr->get_operands();
                for (auto use : uses) {
                    // std::cout << "USE: " << use << " DEF: ";
                    if (Ignore.find (use) != Ignore.end () ) {
                        // std::cout << " IGNORED" << std::endl;
                        continue;
                    }
                    std::set <Var> new_id;
                    for (auto def : ACA.Var_to_Defs [use]) {
                        // std::cout << def << " " ;
                        if (live [ACA.Def_to_bit[def]] == false) continue;
                        if (cfg.method->instrs[def]->get_operands().size () == 0 ) continue ;
                        new_id . insert ((cfg.method->instrs[def]->get_operands())[0]);
                    }

                    if (new_id.size () != 1 ) {
                        // std::cout << " NOPE" << std::endl;
                        continue;
                    }
                    if (Ignore.find (*(new_id.begin())) != Ignore.end () ) {
                        // std::cout << " IGNORE the new id" << std::endl;
                        continue;
                    }
                    // std::cout << " will rename " << use << " to " << *(new_id.begin()) << std::endl;
                    Register_Allocator::Edit_Instr edit_instr (Register_Allocator::Edit_Instr::Type::USE, use, *(new_id.begin()) );
                    instr -> accept (edit_instr);
                    changed = true;
                    
                }
                // std::cout << "------------" << std::endl;
            }
        }
        return changed;
    }
};


} // namespace CopyPropagation
