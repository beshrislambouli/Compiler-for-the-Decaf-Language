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
        
        for (auto& BB : cfg.BBs) {
            std::vector<bool> live = ACA.IN [BB.id];
            std::vector<bool> place_holder (live.size());
            if (live.size() == 0 ) continue;

            for (int i = 0 ; i < BB.instrs.size () ; ACA.Process_Instr(BB.instrs[i], live, place_holder), i ++) {
                auto& instr = cfg.method->instrs[BB.instrs[i]];

                // std::cout << "INSTR: " << BB.instrs [i] << std::endl;
                // for (auto u : live) std::cout << u ? "1" : "0";
                // std::cout << std::endl;

                if (instr->get_operands().size() == 0 ) continue;

                std::vector<Var> operands = instr->get_operands ();
                // std::cout << "VAR" << std::endl; ;
                for (auto var : operands) {
                    // std::cout << std::endl;
                    // std::cout << var << " " ;
                    if (Ignore.find (var) != Ignore.end() ) {
                        // std::cout << "ignored, " ;
                        continue;
                    }
                    bool to_literal = 0;
                    std::set<Var> candidates;
                    for (auto& assign: ACA.Assigns) {
                        if (assign.dist == var && live [assign.bit] == true ) {
                            candidates.insert (get_src(assign.idxs[0]));
                            if ( isLiteral (assign.idxs[0]) ) to_literal = true;
                        }
                    }
                    if (candidates.size() != 1)  {
                        // std::cout << "too many candidats, ";
                        continue;
                    }
                    if (Ignore.find (*(candidates.begin())) != Ignore.end() ) {
                        // std::cout << "ignored candidate, " ;
                        continue;
                    }
                    // std::cout << "CHANGED to " << *(candidates.begin()) << ", " << std::endl;
                    Register_Allocator::Edit_Instr edit_instr ( Register_Allocator::Edit_Instr::Type::USE,
                                                                var,
                                                                *(candidates.begin())                                                             
                                                                );
                    
                    edit_instr .to_literal = to_literal;

                    instr->accept(edit_instr);
                    changed = true;
                }
            }
        }


        return changed;
    }

    bool isLiteral (int id) {
        Linear::Assign* assign_ptr = dynamic_cast<Linear::Assign*>(cfg.method->instrs[id].get());
        return is_instance_of (assign_ptr->operands[0],Linear::Literal);
    }
    Var get_src (int id) {
        Linear::Assign* assign_ptr = dynamic_cast<Linear::Assign*>(cfg.method->instrs[id].get());
        return assign_ptr->operands[0]->id;
    }
};


} // namespace CopyPropagation
