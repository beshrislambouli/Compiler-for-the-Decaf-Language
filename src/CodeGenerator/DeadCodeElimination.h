#pragma once
#include <cassert>
#include <memory>
#include <iostream>
#include <set>
#include "Linear.h"
#include "CFG.h"

class Set_Bit {
public:
    int n;
    std::vector <bool> bit;
    std::vector <std::string> vars;
    std::map <std::string, int> var_to_bit_loc;
    
    Set_Bit (std::vector <std::string> vars) : n(vars.size()), vars(vars) {
        for (int i = 0 ; i < n ; i ++ ) {
            var_to_bit_loc [vars[i]] = bit.size();
            bit.push_back (false);
        }
    }

    bool exist (std::string var) {
        return var_to_bit_loc.find (var) != var_to_bit_loc.end () ;
    }

    bool get_var_bit(std::string var) {
        assert (exist(var));
        return bit [var_to_bit_loc[var]];
    }

    void on_if_exist (std::string var) {
        if (!exist(var)) return ;
        bit [var_to_bit_loc[var]] = true;
    }

    void empty_bit () {
        for (int i = 0 ; i < bit.size () ; i ++ ) {
            bit [i] = false;
        }
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

class Dead_Code_Elimination {
    CFG cfg;
    std::unique_ptr<Linear::Method>& method;
public:
    Dead_Code_Elimination(std::unique_ptr<Linear::Method>& method) 
    : method(method) 
    , cfg(method)
    {}


    std::vector <Set_Bit> IN;
    std::vector <Set_Bit> OUT;
    std::vector <Set_Bit> USE;
    std::vector <Set_Bit> DEF;
    void apply(){
        
        Global_Liveness();

        int n_bb = cfg.BBs.size() ;

        std::vector<int> instr_to_del;
        for (int i = 0 ; i < n_bb ; i ++ ) {

            auto& instr = method->instrs[cfg.BBs [i].instr];
            if ( is_instance_of (instr, Linear::Method_Call ) ) continue; // don't delete instr with side effects

            std::string dist = instr->get_dist();
            if ( OUT[i].exist (dist) && OUT[i].get_var_bit(dist) == false) {
                instr_to_del .push_back (cfg.BBs [i].instr);
            }
        }

        sort (instr_to_del.begin(),instr_to_del.end());
        reverse (instr_to_del.begin(), instr_to_del.end());
        for (int i = 0 ; i < instr_to_del.size() ; i ++ ) {
            method->instrs.erase (method->instrs.begin() + instr_to_del [i]) ;
        }
    }

    void Global_Liveness () {
        int n_bb = cfg.BBs.size ();
        if (n_bb == 0) return ;

        fill_set_bit();
        int n_vars = IN[0].bit.size ();
        if (n_vars == 0) return ;

        // init the use and def
        for (int i = 0 ; i < n_bb ; i ++ ) {

            auto& instr = method->instrs[cfg.BBs [i].instr]; // this instr will not be a Helper instr

            std::string dist = instr->get_dist();
            std::vector<std::string> operands = instr->get_operands();

            DEF [i].on_if_exist (dist);
            for (auto& operand : operands) {
                USE [i].on_if_exist(operand);
            }
        }

        // IN[Exit] = use[Exit];
        int exit_block = cfg.BBs.back().id;
        IN [exit_block].equal_bit (USE[exit_block].bit);

        //Changed = N - { Exit };
        std::set<int> blocks_need_recompute ;
        for (int i = 0 ; i < n_bb ; i ++ ) {
            if ( i == exit_block ) continue;
            blocks_need_recompute .insert (i);
        }

        // while (Changed != emptyset)
        while ( !blocks_need_recompute.empty() ) {
            // choose a node n in Changed;
            // Changed = Changed - { n };
            int node = *(blocks_need_recompute.begin());
            blocks_need_recompute.erase(blocks_need_recompute.begin()) ;

            // OUT[n] = emptyset;
            // for all nodes s in successors(n)
            // OUT[n] = OUT[n] U IN[p];
            OUT [node].empty_bit();
            for (auto child : cfg.BBs[node].children ) {
                OUT [node] .equal_bit ( OUT [node].Union_bit ( OUT[node].bit, IN[child].bit ) ) ;
            }

            
            // save the old bits first
            std::vector<bool> in_node_old;
            for (int i = 0 ; i < IN[node].bit.size() ; i ++ ) {
                in_node_old.push_back ( IN[node].bit[i] ) ;
            }

            // IN[n] = use[n] U (out[n] - def[n]);
            IN [node] .equal_bit ( IN [node].Union_bit ( USE [node].bit,  IN [node].Diff_bit ( OUT [node].bit, DEF [node].bit ) ) );


            // check if IN [node] changed
            bool changed = false;
            for (int i = 0 ; i < IN[node].bit.size() ; i ++ ) {
                if (in_node_old [i] != IN[node].bit[i]) changed = true;
            }

            // if (IN[n] changed)
            //  for all nodes p in predecessors(n)
            //  Changed = Changed U { p };
            if (changed) {
                for (auto parent : cfg.BBs[node].parents) {
                    blocks_need_recompute.insert (parent);
                }
            }
        }



        // cfg.print();
    
        // for (int i = 0 ; i < n_bb ; i ++ ) {
        //     std::cout << "------------" << std::endl;
        //     std::cout << "BLOCK " << i << std::endl;
        //     std::cout << "IN:" << std::endl;
        //     for (int j = 0 ; j < n_vars ; j ++ ) {
        //         std::cout << IN [i].bit[j] ? "1" : "0" ;
        //     }
        //     std::cout << std::endl;
        //     std::cout << "OUT:" << std::endl;
        //     for (int j = 0 ; j < n_vars ; j ++ ) {
        //         std::cout << OUT [i].bit[j] ? "1" : "0" ;
        //     }
        //     std::cout << std::endl;
        //     std::cout << "------------" << std::endl;
        // }

    }

    void fill_set_bit() {
        // get the local vars
        std::vector<std::string> vars;
        for (auto& instr : method -> instrs ) {
            if ( ! is_instance_of (instr, Linear::Declare) ) continue;

            auto declare_ptr = dynamic_cast<Linear::Declare*>(instr.get());

            if ( is_instance_of (declare_ptr->location, Linear::Var) ) {
                vars .push_back (declare_ptr->location->id);
            }
        }
        for (auto& param : method->params) {
            vars .push_back (param->id);
        }

        // init the set bit with zeros for vars
        for (int i = 0 ; i < cfg.BBs.size () ; i ++ ) {
            IN. push_back (Set_Bit(vars));
            OUT.push_back (Set_Bit(vars));
            USE.push_back (Set_Bit(vars));
            DEF.push_back (Set_Bit(vars));
        }
    }
    
};