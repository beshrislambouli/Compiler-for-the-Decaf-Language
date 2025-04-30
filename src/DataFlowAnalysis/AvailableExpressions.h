#pragma once
#include "CFG.h"
#include "DataFlowAnalysis.h"

namespace AvailableExpressions {

using Var = std::string;

class Expr {
public:
    Var dist = "";
    Var operand1 = "";
    Var operand2 = "";
    Linear::Binary::Op op ;
    int bit = -1;

    Expr (Var dist, Var operand1, Var operand2, Linear::Binary::Op op, int bit ) 
    : dist(dist)
    , operand1 (operand1)
    , operand2 (operand2)
    , op (op)
    , bit (bit)
    {}
};

class AvailableExpressions {
public:
    CFG& cfg;
    std::vector<std::vector<bool>> IN;
    std::vector<std::vector<bool>> OUT;
    std::vector<std::vector<bool>> GEN;
    std::vector<std::vector<bool>> KILL;

    std::vector<Expr> Exprs;

    AvailableExpressions (CFG& cfg) : cfg(cfg) {
        Build ();
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
        Build_Exprs ();
        if ( IN [0].size () == 0 ) return ;
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


    void Process_Instr (int expr_id, std::vector<bool>& GEN, std::vector<bool>& KILL) {
        auto& instr = cfg.method -> instrs [expr_id];

        if (instr->get_dist() != "" ) {
            Var dist = instr->get_dist();

            for (auto& expr : Exprs) {
                if (dist == expr.operand1 || dist == expr.operand2) {
                    KILL [expr.bit] = true ;
                    GEN  [expr.bit] = false;
                }
            }
        }

        if (is_instance_of (instr,Linear::Binary)) {
            auto binary_ptr = dynamic_cast<Linear::Binary*> (instr.get());
            Var operand1 = binary_ptr->operands[0]->id;
            Var operand2 = binary_ptr->operands[1]->id;
            auto op = binary_ptr -> op;

            for (auto& expr : Exprs) {
                if (expr.operand1 == operand1 && expr.operand2 == operand2 && expr.op == op ) {
                    KILL [expr.bit] = false;
                    GEN  [expr.bit] = true ;
                }
            }
        }

    }
    
    void Build_Exprs() {
        static int AE_N = 0;
        AE_N ++;
        int expr_counter = 0;
        std::set <std::pair< std::pair<Var,Var>, Linear::Binary::Op> > already ;
        for (int i = 0 ; i < cfg.method->instrs.size() ; i ++ ) {
            auto &instr = cfg.method->instrs [i];
            if ( ! is_instance_of (instr,Linear::Binary) ) continue ;

            auto binary_ptr = dynamic_cast<Linear::Binary*> (instr.get());
            std::pair< std::pair<Var,Var>, Linear::Binary::Op> expr = { {binary_ptr->operands[0]->id, binary_ptr->operands[1]->id} , binary_ptr->op } ;

            if (already.find (expr) == already.end () ) {
                already .insert (expr);

                Exprs .push_back (
                    Expr (
                        "EXPR_" + cfg.method->id + "_" + std::to_string(expr_counter) + "_AE_N_" + std::to_string(AE_N),
                        binary_ptr->operands[0]->id,
                        binary_ptr->operands[1]->id,
                        binary_ptr->op,
                        expr_counter
                    )
                );
                expr_counter ++ ;
            }

        }

        for (int i = 0 ; i < cfg.BBs.size() ; i ++ ) {
            std::vector <bool> tmp(expr_counter, false);
            IN  .push_back (tmp);
            OUT .push_back (tmp);
            GEN .push_back (tmp);
            KILL.push_back (tmp);
        }
    }


    void print () {
        cfg.print();
        for (auto& u : Exprs) {
            std::cout << "Expr: " << u.dist << " = " << u.operand1 << " op " << u.operand2 << " BIT: " << u.bit << std::endl;
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

};