#pragma once
#include "Linear.h"
#include <iostream>
#include <algorithm>
#include "CFG.h"
#include <vector>
#include "ReachingDefinitions.h"
#include "Liveness.h"

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
    std::set <int> adj;
    std::set <int> del;

    // color
    int color = -1;
    bool spilled = false;

    // for precoloring
    bool is_arg = false;
    int arg_num = -1;

    Web (Var original_id) : original_id(original_id) {}

    void add_def ( Def def ) {
        defs .push_back (def);
    }
    void add_use ( Use use ) {
        uses. push_back (use);
    }
    void add_adj (int adj_web) {
        adj .insert (adj_web);
    }
    void del_adj (int adj_web) {
        adj .erase (adj_web);
        del .insert(adj_web);
    }
    void restore () {
        for (auto u : del ) {
            adj.insert (u);
        }
        del.clear();
    }
    double get_cost (){
        return defs.size() + uses.size();
    }

    void print () {
        std::cout << "old_id: " << original_id << " new_id: " << new_id << std::endl;
        std::cout << "COLOR: " << color << " SPILLED: " << spilled << std::endl; 

        std::cout << "DEFS: " << std::endl;
        for (auto def : defs ) std::cout << def << " " ;
        std::cout << std::endl;

        std::cout << "USES: " << std::endl;
        for (auto use : uses ) std::cout << use << " " ;
        std::cout << std::endl;

        std::cout << "Adj: " << std::endl;
        for (auto web : adj ) std::cout << web << " " ;
        std::cout << std::endl;

        std::cout << "---------------" << std::endl;
    }
};

class Edit_Instr : public Linear::Visitor {   
public:
    enum Type {
        DEF,
        USE,
    };
    Type type;
    Var original_id;
    Var new_id;
    Edit_Instr (Type type, Var original_id, Var new_id) 
    : type(type) 
    , original_id (original_id)
    , new_id (new_id)
    {}
    
    // the main idea is to dive into the operand only if it's relevant to the type,
    // operand shouldn't think about DEF/USE cause it doesn't have the context

    void edit_stmt (Linear::Statement& instr) {
        if (type == Type::DEF) {
            // this if is important since you don't want to visit an arr
            // that visits an index -> get changed in DEF
            if ( is_instance_of (instr.dist, Linear::Var)) {
                instr.dist -> accept (*this);
            }
        }
        if (type == Type::USE) {
            for (auto& operand : instr.operands) {
                operand -> accept (*this);
            }

            // need to edit c in a[b[c]] = 5;
            // this if is important since you don't want to visit a def var in use
            if (is_instance_of (instr.dist, Linear::Arr)) {
                instr.dist ->accept (*this);
            }
        }
    }
    
    void visit(Linear::Binary& instr) override {
        edit_stmt (instr);
    }
    void visit(Linear::Unary& instr) override {
        edit_stmt (instr);
    }
    void visit(Linear::Assign& instr) override {
        edit_stmt (instr);
    }
    void visit(Linear::Method_Call& instr) override {
        if (type == Type::DEF) {
            // return location is an var from linear builder so need to check for arr
            if (instr.return_location) {
                instr.return_location -> accept (*this);
            }
        }
        if (type == Type::USE) {
            for (auto& arg : instr.args) {
                arg -> accept (*this);
            }
        }
    }
    void visit(Linear::Return& instr) override {
        if (type == Type::DEF) {
            return ;
        }
        if (type == Type::USE) {
            if (instr.return_value) {
                instr.return_value -> accept (*this);
            }
        }
    }
    void visit(Linear::J_Cond& instr) override {
        if (type == Type::DEF) {
            return ;
        }
        if (type == Type::USE) {
            instr.condition -> accept (*this);
        }
    }

    void visit(Linear::Var& instr) override {
        if (instr.is_array_var) return;

        if ( instr.id == original_id ) {
            instr.id = new_id;
        }
    }
    void visit(Linear::Arr& instr) override {
        // we don't change names of arrays, so we only go to index
        instr.index -> accept(*this);
    }

    // NOTE: only instrs with def/uses will be called
    void visit(Linear::Program& program) override {}
    void visit(Linear::Method& method) override {}
    void visit(Linear::Operand& instr) override {}
    void visit(Linear::Literal& instr) override {}
    void visit(Linear::Location& instr) override {}
    void visit(Linear::Instr& instr) override {}
    void visit(Linear::Statement& instr) override {}
    void visit(Linear::Helper& instr) override {}
    void visit(Linear::Push_Scope& instr) override {}
    void visit(Linear::Pop_Scope& instr) override {}
    void visit(Linear::Declare& instr) override {}
    void visit(Linear::Label& instr) override {}
    void visit(Linear::Jump& instr) override {}
    void visit(Linear::J_UnCond& instr) override {}
};

class RegisterAllocator {
public:
    CFG& cfg;
    std::vector<Web> webs;

    RegisterAllocator (std::vector<Var> Globals, CFG& cfg) : cfg(cfg) {

        std::set<Var> Ignore;
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
        
        
        Build_Webs (Ignore);
        Build_Interference ();
        Color (6);
    }


    void Color (int k) {

        // classify nodes
        std::set <int> web_less_k;
        std::set <int> web_more_k;
        for (int i = 0 ; i < webs.size() ; i ++ ) {
            if (webs[i].adj.size () >= k){
                web_more_k .insert (i);
            } else {
                web_less_k .insert (i);
            }
        }
        
        // fill stack 
        std::vector <int> stack;
        while (web_less_k.size () > 0 || web_more_k .size () > 0 ) {
            if (web_less_k.size()>0){
                int node = * (web_less_k.begin());
                web_less_k .erase (node);

                stack.push_back (node);
                
                for (auto adj_id : webs[node].adj) {
                    auto& adj_web = webs [adj_id];

                    adj_web.del_adj (node);
                    if (adj_web.adj.size () == k-1) {
                        web_more_k .erase (adj_id);
                        web_less_k .insert(adj_id);
                    }
                }
            } else if (web_more_k.size()>0) {
                int to_spill_id = -1;
                double min_cost = 1e9;
                for (auto web_id : web_more_k) {
                    double cur_cost = webs [web_id].get_cost() / (double)(webs [web_id].adj.size());
                    if (cur_cost < min_cost) {
                        min_cost = cur_cost;
                        to_spill_id = web_id;
                    }
                }
                assert (to_spill_id != -1);

                web_more_k.erase (to_spill_id);
                web_less_k.insert(to_spill_id);
            }
        }

        // return the del to adj
        for (auto& web : webs) {
            web.restore ();
        }

        // color
        while (stack.size()) {
            int node = stack.back () ;
            auto& web = webs [node];
            stack.pop_back();

            std::set<int> taken_colors;
            for (auto u : web.adj) {
                taken_colors .insert (webs[u].color);
            }

            bool to_spill = true;
            for (int c = 0 ; c < k ; c ++ ) {
                if ( taken_colors.find (c) == taken_colors.end () ) {
                    web. color = c;
                    to_spill = false;
                    break;
                }
            }

            web. spilled = to_spill;
        }

    }


    void Build_Interference () {
        Liveness::Liveness liveness(cfg);
        ReachingDefinitions::Reaching_Definitions reaching_definitions(cfg);

        for (auto& BB: cfg.BBs) {
            std::vector<bool> live = liveness.OUT[BB.id];

            for (int i = BB.instrs.size() - 1 ; i >= 0 ; liveness.Process_Instr (BB, i, live), i --) {
                auto& instr = cfg.method->instrs [BB.instrs[i]];
                Var dist = instr -> get_dist();
                if (dist == "" || !is_V_Reg (dist) ) continue;

                // two webs interfere if at a def of web1
                // web2 has a reaching def to this point and the var is will be alive after this point
                // DEF_WEB_2 -> DEF_WEB_1 -> USE_WEB_2
                
                // First, I will build the reaching def until this point
                std::vector <bool> Cur_Reaching = reaching_definitions.IN[BB.id];
                std::vector <bool> place_holder (Cur_Reaching.size());
                for (int j = 0 ; j < i ; j ++ ) {
                    reaching_definitions.Process_Instr (BB.instrs[j], Cur_Reaching, place_holder);
                }

                // (a) DEF_WEB_1
                int web1_id = V_Reg_Web (dist);
                for (int web2_id = 0 ; web2_id < webs.size() ; web2_id ++) {
                    if (web1_id == web2_id) continue;

                    // (b) USE_WEB_2
                    Var var = webs[web2_id].new_id;
                    if (live [liveness.Var_to_bit[var]] == false) continue;

                    // (c) DEF_WEB_2
                    for (auto def : webs[web2_id].defs) {
                        if (Cur_Reaching [reaching_definitions.Def_to_bit[def]] == true) {
                            add_edge (web1_id, web2_id);
                            break;
                        }
                    }
                }
                // for (auto& u : liveness.Var_to_bit) {
                //     Var var = u.first;
                //     int bit = u.second;

                //     if (var == "" || !is_V_Reg (var) || live [bit] == false) continue;

                //     int web2_id = V_Reg_Web (var);
                //     add_edge (web1_id, web2_id);
                // }
            }
        }
    }

    void add_edge (int web1_id, int web2_id) {
        if (web1_id == web2_id) return ;
        webs [web1_id] . add_adj (web2_id);
        webs [web2_id] . add_adj (web1_id);
    }

    void Build_Webs (std::set<Var>& Ignore) {
        ReachingDefinitions::Reaching_Definitions r_d (cfg);
        std::map <Def, std::vector<Use>> Chains = r_d.Def_Use_Chains();

        // for (auto chain : Chains) {
        //     std::cout << "DEF: " << chain.first << std::endl;
        //     for (auto use : chain.second) {
        //         std::cout << use << " " ;
        //     }
        //     std::cout << std::endl;
        // }
        // fill the local webs (which are chains for now) preparing to merging
        std::vector <Web> cur_webs ;
        std::vector <bool>got_merged;
        for (auto& chain : Chains ) {
            Def def = chain.first;
            Var var = cfg.method->instrs [def]->get_dist();
            if ( Ignore .find (var) != Ignore .end () ) continue;

            // Create a web for this var   
            cur_webs   .push_back (Web (var));
            got_merged .push_back (false); 

            // Add the chain's def and uses
            cur_webs.back() .add_def (def);
            for (auto use : chain.second) {
                cur_webs.back() .add_use (use);
            }
        }

        // merge
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

        // move to global webs
        for (int i = 0 ; i < cur_webs.size () ; i ++ ) {
            if (got_merged [i]) continue;
            webs .push_back (std::move(cur_webs[i]));
        }

        Install_Webs ();
    }
    void Install_Webs () {
        int counter = 0 ;
        for (auto& web : webs) {
            web. new_id = "V_Reg_" + std::to_string (counter++);
            // web.print();

            // check if arg and add the arg num
            std::string prefix = "FUNC_ARG_";
            if (web.original_id.compare(0, prefix.size(), prefix) == 0) {
                size_t n_pos = web.original_id.rfind("_N");
                
                web .is_arg = true;
                web .arg_num= std::stoi(web.original_id.substr(n_pos+2));
                web .new_id += "_ARG_" + std::to_string (web.arg_num);
            }


            for (auto& def : web.defs) {
                Edit_Instr edit_instr (Edit_Instr::Type::DEF, web.original_id, web.new_id);

                auto& instr = cfg.method->instrs[def];
                instr -> accept (edit_instr);
            }
            for (auto& use : web.uses) {
                Edit_Instr edit_instr (Edit_Instr::Type::USE, web.original_id, web.new_id);

                auto& instr = cfg.method->instrs[use];
                instr -> accept (edit_instr);
            }
        }
        // note: you still need to edit declares, but this should happen in the very end because it will break the cfg 
        // because you will need to add/del insts -> miss the order
    }



    // helpers
    bool is_V_Reg (Var id) {
        for (auto& web : webs ) {
            if (web.new_id == id) {
                return true;
            } 
        }
        return false;
    }
    int V_Reg_Web (Var id) {
        assert (is_V_Reg(id));
        int idx = 0;
        for (auto& web : webs){
            if (web.new_id == id) {
                return idx;
            }
            idx ++;
        }
        assert (false);
    }
};
}