#include "CFG.h"
#include "ReachingDefinitions.h"

CFG::CFG(std::unique_ptr<Linear::Method>& method) : method(method) {
    
    std::vector <int> cur_block_instrs;
    std::map < std::string, int> label_to_block;
    for (int instr_id = 0 ; instr_id < method->instrs.size () ; instr_id ++ ) {
        int cur_block_id = BBs.size () ;
        cur_block_instrs.push_back (instr_id);

        // save the block of each label
        if (is_instance_of(method->instrs[instr_id], Linear::Label) ) {
            auto label_ptr = dynamic_cast<Linear::Label*>(method->instrs[instr_id].get());
            label_to_block [label_ptr->label] = cur_block_id;
        }

        // should end the block here
        if ( instr_id == method->instrs.size () - 1 || 
             is_instance_of(method->instrs[instr_id]  , Linear::Jump) ||
             is_instance_of(method->instrs[instr_id+1], Linear::Label) )     
        {
            BBs.push_back (Basic_Block (cur_block_id, cur_block_instrs)) ;
            while (cur_block_instrs.size()) cur_block_instrs.pop_back();
        }

    }


    for (int bb_id = 0 ; bb_id < BBs.size () ; bb_id ++ ) {
        auto& last_instr = method->instrs [BBs[bb_id].instrs.back()] ;
        
        // fall-through is an option unless the last instr is an uncond jump
        if ( bb_id + 1 < BBs.size () && !is_instance_of (last_instr, Linear::J_UnCond) ) {
            add_directed_edge (bb_id, bb_id+1) ;
        }

        // if the last instr jumps, connect with the jump to label's block
        if (is_instance_of (last_instr, Linear::Jump)) {
            auto j_ptr = dynamic_cast<Linear::Jump*>(last_instr.get());
            add_directed_edge (bb_id, label_to_block [j_ptr->label]) ;
        }
    }

}



void CFG::add_directed_edge (int bb_from, int bb_to) {
    assert (bb_from >= 0 && bb_from < BBs.size());
    assert (bb_to >= 0 && bb_to < BBs.size());

    BBs [bb_from] .add_child (bb_to);
    BBs [bb_to]   .add_parent(bb_from);
}

void CFG::print () {
    for (int i = 0 ; i < BBs.size () ; i ++ ) {
        std::cerr << "----------------" << std::endl;
        std::cerr << "Block: " << i << std::endl;

        std::cerr << "Instrs: " << std::endl;
        for (int j = 0 ; j < BBs[i].instrs.size() ; j ++ ) {
            auto& instr = method->instrs [BBs[i].instrs[j]];
            std::cerr << BBs[i].instrs[j] << " " ;
            Linear::PrettyPrinter printer;
            instr -> accept (printer); 
        }
        std::cerr << std::endl;

        std::cerr << "Parents: " << std::endl;
        for (int j = 0 ; j < BBs[i].parents.size() ; j ++ ) {
            std::cerr << BBs[i].parents[j] << " " ;
        }
        std::cerr << std::endl;

        std::cerr << "Children: " << std::endl;
        for (int j = 0 ; j < BBs[i].children.size() ; j ++ ) {
            std::cerr << BBs[i].children[j] << " " ;
        }
        std::cerr << std::endl;

        std::cerr << "----------------" << std::endl;
    }
}