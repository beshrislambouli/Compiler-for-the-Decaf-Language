#pragma once
#include <cassert>
#include <memory>
#include <iostream>
#include <set>
#include "Linear.h"
#include "CFG.h"

namespace CSE {
class Set_Bit {
public:
    std::vector <bool> bit;
    std::map <std::string,int> id_to_bit_loc;

    Set_Bit (std::set<std::string> ids) {
        for (auto id : ids) {
            id_to_bit_loc [id] = bit.size () ;
            bit .push_back (false);
        }
    }

    void fill_bit (bool val) {
        for (int i = 0 ; i < bit.size () ; i ++ ) {
            bit [i] = val;
        }
    }

    bool exist (std::string id) {
        return id_to_bit_loc.find (id) != id_to_bit_loc.end();
    }

    bool get_bit (std::string id) {
        assert (exist (id));
        return bit [id_to_bit_loc[id]];
    }
    
    void on_if_exist (std::string id) {
        if (!exist (id)) return ;
        bit [id_to_bit_loc[id]] = true;
    }

    void equal_bit (std::vector <bool> other_bit) {
        assert (bit.size () == other.size());

        for (int i = 0 ; i < bit.size () ; i ++ ) {
            bit [i] = other_bit [i];
        }
    }

    std::vector <bool> Union_bit (std::vector <bool> bit_1, std::vector <bool> bit_2) {
        assert (bit_1.size() == bit_2.size ());
        std::vector <bool> bit_ret;

        for (int i = 0 ; i < bit_1.size() ; i ++ ) {
            bit_ret .push_back ( bit_1 [i] | bit_2 [i] );
        }

        return bit_ret;
    }

    std::vector <bool> Intersect_bit (std::vector <bool> bit_1, std::vector <bool> bit_2) {
        assert (bit_1.size() == bit_2.size ());
        std::vector <bool> bit_ret;

        for (int i = 0 ; i < bit_1.size() ; i ++ ) {
            bit_ret .push_back ( bit_1 [i] & bit_2 [i] );
        }

        return bit_ret;
    }

    std::vector <bool> Diff_bit (std::vector <bool> bit_1, std::vector <bool> bit_2) {
        assert (bit_1.size() == bit_2.size ());
        std::vector <bool> bit_ret;

        for (int i = 0 ; i < bit_1.size() ; i ++ ) {
            if ( bit_1 [i] == true && bit_2 [i] == false ) {
                bit_ret .push_back (true);
            } else {
                bit_ret .push_back (false);
            }
        }

        return bit_ret;
    }
};

class Common_Subexpression_Elimination {
    CFG cfg;
    std::unique_ptr<Linear::Method>& method;
public:
    Common_Subexpression_Elimination(std::unique_ptr<Linear::Method>& method) 
    : method(method)
    , cfg (method)
    {}

    std::vector <Set_Bit> IN;
    std::vector <Set_Bit> OUT;
    std::vector <Set_Bit> GEN;
    std::vector <Set_Bit> KILL;
    std::set <std::string> exprs;

    bool apply () {
        Globa_Available_Expressions () ;

        int n_bb = cfg.BBs.size () ;

        std::set < std::string > exprs_tmp;
        std::vector < std::unique_ptr < Linear::Declare >> new_declares ;
        std::vector < std::pair < int , std::unique_ptr<Linear::Assign> > > new_assigns_instead_of_id;
        std::vector < std::pair < int , std::unique_ptr<Linear::Assign> > > new_assigns_after_id;
        for ( int i = 0 ; i < n_bb ; i ++ ) {
            auto& instr = method -> instrs [cfg.BBs[i].instr];

            if ( ! is_instance_of (instr, Linear::Binary) ) continue;
            
            auto binary_ptr = dynamic_cast<Linear::Binary*>(instr.get());
            std::string instr_expr = binary_ptr->hash();
            if ( exprs .find (instr_expr) == exprs.end () ) continue;

            std::string instr_expr_tmp = "CSE_TMP_" + instr_expr ;

            if ( IN[i] .get_bit (instr_expr) ) { // expr is available 
                auto instr_assign = std::make_unique <Linear::Assign> ();
                instr_assign -> dist = binary_ptr->dist ->get_copy();
                instr_assign -> operands .push_back (std::make_unique<Linear::Var>(binary_ptr->dist->type, instr_expr_tmp)) ;

                new_assigns_instead_of_id .push_back ( std::make_pair (cfg.BBs[i].instr, std::move (instr_assign)) );
            } else { // expr is not available
                auto instr_assign = std::make_unique <Linear::Assign> ();
                instr_assign -> dist = std::make_unique<Linear::Var>(binary_ptr->dist->type, instr_expr_tmp);
                instr_assign -> operands .push_back (binary_ptr->dist ->get_copy());

                if ( exprs_tmp .find (instr_expr_tmp) == exprs_tmp .end () ) {
                    auto instr_declare = std::make_unique<Linear::Declare>();
                    instr_declare -> location = instr_assign -> dist ->get_copy ();

                    new_declares .push_back (std::move(instr_declare));
                    exprs_tmp .insert (instr_expr_tmp);
                }

                new_assigns_after_id .push_back ( std::make_pair (cfg.BBs[i].instr, std::move(instr_assign)) );
            }
        }

        // ORDER MATTER HERE !!!

        // new_assigns_instead_of_id
        for (auto& u : new_assigns_instead_of_id ) {
            int idx = u.first ;
            auto& assign = u.second;
            
            method -> instrs [idx] = std::move (assign);
        }

        // new_assigns_after_id
        sort (new_assigns_after_id.begin(),new_assigns_after_id.end());
        reverse (new_assigns_after_id.begin(),new_assigns_after_id.end());
        for (auto& u : new_assigns_after_id ) {
            int idx = u.first;
            auto& assign = u.second;  

             method -> instrs .insert (method->instrs.begin() + idx + 1 , std::move (assign)); // NOTE: destroyed the order
        }

        // new declared 
        for (auto& decl : new_declares ) {
            method -> instrs .insert (method->instrs.begin() +1 , std::move (decl)); // declare after the push_scope
        }

        return new_assigns_instead_of_id.size () > 0 ;
    }

    void Globa_Available_Expressions () {
        int n_bb = cfg.BBs.size ();
        if (n_bb == 0) return ;

        fill_set_bit();
        int n_ids = IN[0].bit.size ();
        if (n_ids == 0) return ;

        // fill GEN
        for (int i = 0 ; i < n_bb ; i ++ ) {
            auto& instr = method->instrs[cfg.BBs [i].instr];
            if ( ! is_instance_of(instr,Linear::Binary) ) continue;

            auto binary_ptr = dynamic_cast<Linear::Binary*>(instr.get());
            std::string instr_expr = binary_ptr->hash();

            GEN [i].on_if_exist (instr_expr);
        }
        
        // fill kill, stupid, will be fixed
        for (int i = 0 ; i < n_bb ; i ++ ) {
            auto& instr = method->instrs[cfg.BBs [i].instr];
            if ( ! is_instance_of(instr,Linear::Statement) ) continue;

            auto stmt_ptr = dynamic_cast<Linear::Statement*>(instr.get());
            std::string instr_dist = stmt_ptr->dist->id;

            for (auto& expr : exprs ) {
                std::string operand_1 = "";
                std::string operand_2 = "";
                int num_ = 0;
                for (int j = 0 ; j < expr.size () ; j ++ ) {
                    if (expr [j] == '_' ) {
                        num_ ++ ;
                        continue;
                    }
                    if (num_ == 0) {
                        operand_1 += expr [j];
                    }
                    if (num_ == 2) {
                        operand_2 += expr [j];
                    }
                }

                if ( instr_dist == operand_1 || instr_dist == operand_2 ) {
                    KILL [i].on_if_exist (expr);
                }
            }
        }

        // for all nodes n in N OUT[n] = E;
        for (int i = 0 ; i < n_bb ; i ++ ) {
            OUT [i].fill_bit (true);
        }

        // OUT[Entry] = GEN[Entry]; 
        int entry = cfg.BBs[0].id;
        OUT[entry].equal_bit (GEN[entry].bit);

        // Changed = N - { Entry };
        std::set<int> blocks_need_recompute ;
        for (int i = 0 ; i < n_bb ; i ++ ) {
            if ( i == entry ) continue;
            blocks_need_recompute .insert (i);
        }

        // while (Changed != emptyset)
        while ( !blocks_need_recompute.empty() ) {
            // choose a node n in Changed;
            // Changed = Changed - { n };
            int node = *(blocks_need_recompute.begin());
            blocks_need_recompute.erase(blocks_need_recompute.begin()) ;

            // IN[n] = E;
            // for all nodes p in predecessors(n) 
            // IN[n] = IN[n] intersect OUT[p];
            IN [node].fill_bit(true);
            for (auto parent : cfg.BBs[node].parents ) {
                IN [node] .equal_bit ( IN [node].Intersect_bit ( IN[node].bit, OUT[parent].bit ) ) ;
            }

            
            // save the old bits first
            std::vector<bool> out_node_old;
            for (int i = 0 ; i < OUT[node].bit.size() ; i ++ ) {
                out_node_old.push_back ( OUT[node].bit[i] ) ;
            }

            // OUT[n] = GEN[n] U (IN[n] - KILL[n]);
            OUT [node] .equal_bit ( OUT [node].Union_bit ( GEN [node].bit,  OUT [node].Diff_bit ( IN [node].bit, KILL [node].bit ) ) );


            // check if OUT [node] changed
            bool changed = false;
            for (int i = 0 ; i < OUT[node].bit.size() ; i ++ ) {
                if (out_node_old [i] != OUT[node].bit[i]) changed = true;
            }

            // if (OUT[n] changed)
            //  for all nodes s in successors(n)
            //  Changed = Changed U { s };
            if (changed) {
                for (auto child : cfg.BBs[node].children) {
                    blocks_need_recompute.insert (child);
                }
            }
        }

        // cfg.print();

        // for (int i = 0 ; i < n_bb ; i ++ ) {
        //     std::cout << "------------" << std::endl;
        //     std::cout << "BLOCK " << i << std::endl;
        //     std::cout << "IN:" << std::endl;
        //     for (int j = 0 ; j < n_ids ; j ++ ) {
        //         std::cout << IN [i].bit[j] ? "1" : "0" ;
        //     }
        //     std::cout << std::endl;
        //     std::cout << "OUT:" << std::endl;
        //     for (int j = 0 ; j < n_ids ; j ++ ) {
        //         std::cout << OUT [i].bit[j] ? "1" : "0" ;
        //     }
        //     std::cout << std::endl;
        //     std::cout << "------------" << std::endl;
        // }
    }

    void fill_set_bit () {
        // get the (1) binary exprs between (2)local (3)vars/literals
        std::set <std::string> locals;
        for (auto& instr : method -> instrs ) {

            // collect locals for (2)
            if ( is_instance_of (instr, Linear::Declare) ) {
                auto declare_ptr = dynamic_cast<Linear::Declare*>(instr.get());
                locals .insert (declare_ptr->location->id);
            }

            //(1) binary
            if ( ! is_instance_of(instr,Linear::Binary) ) continue;

            auto binary_ptr = dynamic_cast<Linear::Binary*>(instr.get());

            // (3) non arr
            if ( is_instance_of (binary_ptr->operands[0],Linear::Arr) || is_instance_of (binary_ptr->operands[1],Linear::Arr) ) continue;

            // (2) non globals 
            if ( is_instance_of (binary_ptr->operands[0],Linear::Var) && locals.find (binary_ptr->operands[0]->id) == locals.end () ) continue; 
            if ( is_instance_of (binary_ptr->operands[1],Linear::Var) && locals.find (binary_ptr->operands[1]->id) == locals.end () ) continue; 

            exprs .insert(binary_ptr->hash());
        }

        for (int i = 0 ; i < cfg.BBs.size () ; i ++ ) {
            IN. push_back  (Set_Bit(exprs));
            OUT.push_back  (Set_Bit(exprs));
            GEN.push_back  (Set_Bit(exprs));
            KILL.push_back (Set_Bit(exprs));
        }
    }
};
}