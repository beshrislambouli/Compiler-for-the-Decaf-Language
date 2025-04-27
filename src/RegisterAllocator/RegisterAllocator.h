#pragma once
#include "Linear.h"
#include <iostream>
#include "CFG.h"
#include <vector>
#include "ReachingDefinitions.h"

namespace Register_Allocator {
    using Def = int; // instruction index that has the def
    using Use = int; // instruction index that has the use
    using Move= int; // instruction index that has the move
    using Var = std::string;
    
class Web {
public:
    // old and new var name
    Var original_id;
    Var new_id = "";

    // web's defs/uses for interfering
    // note there might be redundancy here but it doesn't really matter
    std::vector <Def> defs;
    std::vector <Use> uses;

    // coalescing
    Var coalesced_to = "";
    std::vector <Move> moves;

    // graph 
    std::vector <int> adj;
    std::vector <int> del;

    // color
    int color = -1;
    bool spilled = false;

    Web (Var original_id) : original_id(original_id) {}

    void add_def ( Def def ) {
        defs .push_back (def);
    }
    void add_use ( Use use ) {
        uses. push_back (use);
    }
};


class RegisterAllocator {
    CFG& cfg;
    std::vector<Web> webs;

public:
    RegisterAllocator (CFG& cfg) : cfg(cfg) {
        Build_Webs ();
    }

    void Build_Webs () {
        ReachingDefinitions::Reaching_Definitions r_d (cfg);
        std::map <Def, std::vector<Use>> Chains = r_d.Def_Use_Chains();

        // fill the local webs (which are chains for now) preparing to merging
        std::vector <Web> cur_webs ;
        std::vector <bool>got_merged;
        for (auto& chain : Chains ) {
            Def def = chain.first;
            // get the dist from the instr at Def as the original_id         
            cur_webs   .push_back (Web (cfg.method->instrs [def]->get_dist()));
            got_merged .push_back (false); 

            cur_webs.back() .add_def (def);
            for (auto use : chain.second) {
                cur_webs.back() .add_use (use);
            }
        }


        for (int i = 0 ; i < cur_webs.size () ; i ++ ) {
            if (got_merged [i]) continue;

            for (int j = i + 1 ; j < cur_webs.size () ; j ++ ) {
                if (got_merged [j]) continue;

                if (cur_webs [i].original_id != cur_webs [j].original_id) continue;
                
                bool merge = false;
                for (auto use_i : cur_webs [i].uses) {
                    for (auto use_j : cur_webs [j].uses) {
                        if (use_i == use_j) {
                            merge = true;
                            break;
                        }
                    }
                }
                if (!merge) continue;

                got_merged [j] = true; 
                for (auto def_j : cur_webs [j].defs) {
                    cur_webs [i] .add_def (def_j);
                }
                for (auto use_j : cur_webs [j].uses) {
                    cur_webs [i] .add_use (use_j);
                }
            }
        }

        for (int i = 0 ; i < cur_webs.size () ; i ++ ) {
            if (got_merged [i]) continue;
            webs .push_back (std::move(cur_webs[i]));
        }
    }
};
}