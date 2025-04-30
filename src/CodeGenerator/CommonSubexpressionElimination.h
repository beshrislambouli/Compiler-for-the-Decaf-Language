#pragma once
#include <cassert>
#include <memory>
#include <iostream>
#include <set>
#include "Linear.h"
#include "CFG.h"
#include "AvailableExpressions.h"
#include "RegisterAllocator.h"

namespace CommonSubexpressionElimination {

using Var = std::string ; 

class CommonSubexpressionElimination {
    CFG& cfg;
    std::set<Var> Ignore;
public:
    CommonSubexpressionElimination (std::vector<Var> Globals, CFG& cfg) : cfg(cfg) {
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

    bool apply () {
        AvailableExpressions::AvailableExpressions AE (cfg);

        bool changed = false ;

        for (auto& BB : cfg.BBs) {
            std::vector <bool> live = AE.IN [BB.id];
            std::vector <bool> place_holder (live.size());
            if (live.size() == 0 ) continue;

            for (int i = 0 ; i < BB.instrs.size () ; AE.Process_Instr (BB.instrs[i], live, place_holder) ,i ++ ) {
                auto& instr = cfg.method -> instrs [BB.instrs [i]];
                if (!is_instance_of (instr, Linear::Binary)) continue;

                auto binary_ptr = dynamic_cast<Linear::Binary*> (instr.get());
                Var operand1 = binary_ptr->operands[0]->id;
                Var operand2 = binary_ptr->operands[1]->id;
                auto op = binary_ptr -> op;
                
                if (Ignore.find (operand1) != Ignore.end() || Ignore.find (operand2) != Ignore.end() ) continue; 

                for (auto& expr : AE.Exprs) {
                    if (expr.operand1 == operand1 && expr.operand2 == operand2 && expr.op == op ) {
                        if (live [expr.bit]) {
                            // replace
                            auto assign_instr = std::make_unique <Linear::Assign> ();

                            assign_instr -> dist = binary_ptr->dist->get_copy();
                            assign_instr -> operands .push_back (
                                std::make_unique <Linear::Var> (
                                    assign_instr->dist->type,
                                    expr.dist
                                )
                            );

                            cfg.method -> instrs [BB.instrs [i]] = std::move(assign_instr);

                            changed = true;
                        } else {
                            // rename
                            rename (binary_ptr->dist->id, expr.dist);
                        }
                    }
                }
            }
        }
        return changed ;
    }

    void rename (Var from, Var to ) {
        for (auto& instr : cfg.method->instrs) {
            Register_Allocator::Edit_Instr edit_instr_use (
                Register_Allocator::Edit_Instr::Type::USE,
                from,
                to
            );
            Register_Allocator::Edit_Instr edit_instr_def (
                Register_Allocator::Edit_Instr::Type::DEF,
                from,
                to
            );

            instr ->accept (edit_instr_use);
            instr ->accept (edit_instr_def);

            if (is_instance_of (instr, Linear::Declare)) {
                Linear::Declare* declare_ptr = dynamic_cast<Linear::Declare*>(instr.get());
                if (declare_ptr->location->id == from) {
                    declare_ptr->location->id = to;
                }
            }
        }
    }
};

}