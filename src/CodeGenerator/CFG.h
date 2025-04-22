#pragma once
#include <cassert>
#include <memory>
#include <iostream>
#include <map>
#include "Linear.h"
#include "CFG.h"

#define is_instance_of(uptr, Type) (dynamic_cast<Type*>((uptr).get()) != nullptr)

class Basic_Block {
public:
    int id;
    int instr; 
    std::vector<int> parents;
    std::vector<int> children;

    Basic_Block (int id, int instr) : id(id) , instr(instr) {}

    void add_parent (int BB_id) {
        parents.push_back(BB_id);
    }

    void add_child (int BB_id) {
        children.push_back(BB_id);
    }
};


class CFG {
public:
    std::vector<Basic_Block> BBs;
    std::map < std::string, int > label_to_BB;

    CFG(std::unique_ptr<Linear::Method>& method) {
        
        for (int instr_id = 0 ; instr_id < method->instrs.size() ; instr_id ++ ) {
            auto& instr = method->instrs [instr_id];
            if ( is_instance_of (instr, Linear::Helper ) ) continue;

            int bb_id = BBs.size ();
            if ( is_instance_of (instr, Linear::Label) ) {
                
                auto label_ptr = dynamic_cast<Linear::Label*>(instr.get());
                label_to_BB [label_ptr->label] = bb_id;
            }

            BBs.push_back ( Basic_Block ( bb_id, instr_id ) );
        }


        for (int bb_id = 0 ; bb_id < BBs.size () ; bb_id ++ ) {
            auto& instr = method->instrs [BBs [bb_id] .instr];

            if (bb_id + 1 < BBs.size () && !is_instance_of (instr, Linear::J_UnCond)) {
                add_directed_edge (bb_id, bb_id + 1 );
            }
            
            if ( is_instance_of (instr, Linear::Jump) ) {

                auto j_ptr = dynamic_cast<Linear::Jump*>(instr.get());
                add_directed_edge( bb_id, label_to_BB [j_ptr->label] );
            }
        }

    }

    void add_directed_edge (int bb_from, int bb_to) {
        assert (bb_from >= 0 && bb_from < BBs.size());
        assert (bb_to >= 0 && bb_to < BBs.size());

        BBs [bb_from] .add_child (bb_to);
        BBs [bb_to]   .add_parent(bb_from);
    }

    void print () {
        for (int i = 0 ; i < BBs.size () ; i ++ ) {
            std::cout << "---------------------" << std::endl;
            std::cout << "BASIC BLOCK: " << BBs[i].id << " INSTR: " << BBs[i].instr << std::endl;
            std::cout << "PARENTS: " ;
            for (int j = 0 ; j < BBs[i].parents.size () ; j ++ ) {
                std::cout << BBs[i].parents[j] << " " ;
            }
            std::cout << std::endl;
            std::cout << "CHILDREN: " ;
            for (int j = 0 ; j < BBs[i].children.size () ; j ++ ) {
                std::cout << BBs[i].children[j] << " " ;
            }
            std::cout << std::endl;
            std::cout << "---------------------" << std::endl;
        }
    }
};