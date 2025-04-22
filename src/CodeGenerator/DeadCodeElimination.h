#pragma once
#include <cassert>
#include <memory>
#include <iostream>
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
        fill_set_bit();

        
        int n_bb = cfg.BBs.size ();
        if (n_bb == 0) return ;
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


        // cfg.print();
        
        // for (int i = 0 ; i < n_bb ; i ++ ) {
        //     std::cout << "------------" << std::endl;
        //     std::cout << "BLOCK " << i << std::endl;
        //     std::cout << "DEF:" << std::endl;
        //     for (int j = 0 ; j < n_vars ; j ++ ) {
        //         std::cout << DEF [i].bit[j] ? "1" : "0" ;
        //     }
        //     std::cout << std::endl;
        //     std::cout << "USE:" << std::endl;
        //     for (int j = 0 ; j < n_vars ; j ++ ) {
        //         std::cout << USE [i].bit[j] ? "1" : "0" ;
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

        // init the set bit with zeros for vars
        for (int i = 0 ; i < cfg.BBs.size () ; i ++ ) {
            IN. push_back (Set_Bit(vars));
            OUT.push_back (Set_Bit(vars));
            USE.push_back (Set_Bit(vars));
            DEF.push_back (Set_Bit(vars));
        }
    }
    
};