#pragma once
#include "Linear.h"
#include <iostream>
#include <algorithm>
#include <cmath>
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
    std::set <Def> defs;
    std::set <Use> uses;

    // coalescing
    Var coalesced_to = "";
    std::set <Move> moves;

    // graph 
    int deg = 0;
    std::set <int> adj;
    std::set <int> del;

    // color
    int color = -1;
    bool spilled = false;
    bool across_call = false;
    bool is_param = false;


    Web (Var original_id) : original_id(original_id) {}

    void add_def ( Def def ) {
        defs .insert (def);
    }
    void add_use ( Use use ) {
        uses. insert (use);
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
    double spill_cost (std::vector <int>& nested_fors) {
        double cost = 0.0;
        for (auto def : defs ) {
            cost += pow (10, std:: max ( 0 , std::min (nested_fors[def],17) ) ) ;
        }
        for (auto use : uses ) {
            cost += pow (10, std:: max ( 0 , std::min (nested_fors[use],17) ) ) ;
        }
        return cost;
    }
    bool is_adj (int node) {
        return adj.find (node) != adj.end () ;
    }

    void print () {
        std::cerr << "old_id: " << original_id << " new_id: " << new_id << std::endl;
        std::cerr << "COLOR: " << color << " SPILLED: " << spilled << std::endl; 

        std::cerr << "DEFS: " << std::endl;
        for (auto def : defs ) std::cerr << def << " " ;
        std::cerr << std::endl;

        std::cerr << "USES: " << std::endl;
        for (auto use : uses ) std::cerr << use << " " ;
        std::cerr << std::endl;

        std::cerr << "Adj: " << std::endl;
        for (auto web : adj ) std::cerr << web << " " ;
        std::cerr << std::endl;

        std::cerr << "---------------" << std::endl;
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
    int ret = false;
    int to_literal = false;
    
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
            for (int i = 0 ; i < instr.operands.size () ; i ++ ) {
                instr.operands[i]->accept(*this);
                if (ret && to_literal) {
                    instr.operands[i] = std::make_unique<Linear::Literal>(instr.operands[i]->type,new_id);
                }
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
            // return location is a var from linear builder so no need to check for arr
            if (instr.return_location) {
                instr.return_location -> accept (*this);
            }
        }
        if (type == Type::USE) {
            for (int i = 0 ; i < instr.args.size () ; i ++ ) {
                instr.args[i]->accept(*this);
                if (ret && to_literal) {
                    instr.args[i] = std::make_unique<Linear::Literal>(instr.args[i]->type,new_id);
                }
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
                if (ret && to_literal) {
                    instr.return_value = std::make_unique<Linear::Literal>(instr.return_value->type,new_id);
                }
            }
        }
    }
    void visit(Linear::J_Cond& instr) override {
        if (type == Type::DEF) {
            return ;
        }
        if (type == Type::USE) {
            instr.condition -> accept (*this);
            if (ret && to_literal) {
                instr.condition = std::make_unique<Linear::Literal>(instr.condition->type,new_id);
            }
        }
    }

    void visit(Linear::Var& instr) override {
        ret = 0 ;
        if (instr.is_array_var) return;

        if ( instr.id == original_id ) {
            instr.id = new_id;
            ret = 1; 
        }
    }
    void visit(Linear::Arr& instr) override {
        // we don't change names of arrays, so we only go to index
        instr.index -> accept(*this);
        if (ret && to_literal) {
            instr.index = std::make_unique<Linear::Literal>(instr.index->type,new_id);
        }
        ret = 0;
    }

    // NOTE: only instrs with def/uses will be called
    void visit(Linear::Program& program) override { ret = 0 ; }
    void visit(Linear::Method& method) override { ret = 0 ; }
    void visit(Linear::Operand& instr) override { ret = 0 ; }
    void visit(Linear::Literal& instr) override { ret = 0 ; }
    void visit(Linear::Location& instr) override { ret = 0 ; }
    void visit(Linear::Instr& instr) override { ret = 0 ; }
    void visit(Linear::Statement& instr) override { ret = 0 ; }
    void visit(Linear::Helper& instr) override { ret = 0 ; }
    void visit(Linear::Push_Scope& instr) override { ret = 0 ; }
    void visit(Linear::Pop_Scope& instr) override { ret = 0 ; }
    void visit(Linear::Declare& instr) override { ret = 0 ; }
    void visit(Linear::Label& instr) override { ret = 0 ; }
    void visit(Linear::Jump& instr) override { ret = 0 ; }
    void visit(Linear::J_UnCond& instr) override { ret = 0 ; }
};

class RegisterAllocator {
public:
    CFG& cfg;
    std::vector<Web> webs;
    int K;

    RegisterAllocator (std::vector<Var> Globals, CFG& cfg, std::vector<std::string> REG) : cfg(cfg), K (REG.size()) {

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
        Color ();
    }

    // webs
    std::set <int> simplifyWorklist;
    std::set <int> spillWorklist;
    std::set <int> coloredNodes;
    std::vector <int> selectStack;
    

    void Color () {

        for (int web_id = 0 ; web_id < webs.size () ; web_id ++ ) {
            if (webs [web_id].color != -1) continue;
            if (webs [web_id].deg >= K ) {
                spillWorklist  .insert (web_id);
            } else {
                simplifyWorklist .insert (web_id);
            }
        }

        fill_nested_fors ();

        while (true) {
            if (simplifyWorklist.size()) {
                int node = * (simplifyWorklist.begin());
                simplifyWorklist.erase (node);

                selectStack .push_back (node);

                auto ADJ = Adjacent (node);
                for (auto u : ADJ) {
                    DecrementDegree (u);
                }
            }
            else if (spillWorklist.size()) {
                int node;
                double min_cost = 1e9;
                for (auto u : spillWorklist) {
                    if ( ( webs [u].spill_cost(nested_fors) / (1 + Adjacent(u).size()) ) < min_cost) {
                        min_cost = ( webs [u].spill_cost(nested_fors) / (1 + Adjacent(u).size()) );
                        node = u;
                    }
                }

                spillWorklist.erase (node);
                simplifyWorklist.insert (node);
            }
            else break;
        }
        AssignColors();
    }


    void FillAcrossCall() {
        Liveness::Liveness liveness(cfg);

        for (auto& BB: cfg.BBs) {
            std::vector<bool> live = liveness.OUT[BB.id];

            for (int i = BB.instrs.size() - 1 ; i >= 0 ; liveness.Process_Instr (BB, i, live), i --) {
                auto& instr = cfg.method->instrs [BB.instrs[i]];
                if ( !is_instance_of(instr,Linear::Method_Call) ) continue;

                for (auto u : liveness.Var_to_bit) {
                    auto var = u.first;
                    auto bit = u.second;

                    if ( !live[bit] ) continue;

                    for (auto& web : webs ) {
                        if (web .new_id == var) {
                            web .across_call = true;
                        }
                    }
                }
            }
        }
        for (auto& web : webs) {
            for (auto use : web.uses) {
                if ( is_instance_of (cfg.method->instrs[use], Linear::Method_Call) ) {
                    web .is_param = true;
                }
            }
        }
    }

    void AssignColors () {
        FillAcrossCall ();
        while (selectStack.size ()) {
            int node = selectStack.back();
            selectStack.pop_back();

            std::set <int> okColors;
            for (int i = 0 ; i < K ; i ++ ) okColors .insert (i);

            std::set <int> extra ;
            for (int i = 0 ; i < K + 5 ; i ++ ) extra .insert (i);

            for (auto w : webs [node].adj) {
                if ( in (coloredNodes,w) ) {
                    okColors .erase (webs [w].color);
                    extra    .erase (webs [w].color);
                }
            }

            if ( okColors.size() == 0 ) {

                bool allowed_extra = 1;
                if ( webs[node] .across_call || webs [node].is_param ) allowed_extra = 0 ;

                if (allowed_extra && extra.size()) {
                    coloredNodes.insert (node);
                    webs [node].color = * (extra.begin());
                } else {
                    webs [node].spilled = true;
                }
                
            } else {
                coloredNodes.insert (node);
                webs [node].color = * (okColors.begin());
            }
        }
    }

    void DecrementDegree (int node) {
        webs [node] .deg -- ;
        if (webs [node].deg == K-1 ) {
            spillWorklist .erase (node);
            simplifyWorklist.insert (node);
        }
    }

    std::set<int> Adjacent (int node) {
        std::set <int> ret;
        for (auto u : webs[node].adj) {
            bool here = false;
            for (auto v : selectStack) if (v == u) here = true;
            if (!here) {
                ret .insert (u);
            }
        }
        return ret;
    }

    void AddEdge (int web1_id, int web2_id) {
        if (web1_id == web2_id) return ;
        webs [web1_id] .add_adj (web2_id);
        webs [web1_id] .deg = webs [web1_id] .adj.size() ;
        webs [web2_id] .add_adj (web1_id); 
        webs [web2_id] .deg = webs [web2_id] .adj.size() ;
    }

    void Build_Interference () {
        Liveness::Liveness liveness(cfg);

        for (auto& BB: cfg.BBs) {
            std::vector<bool> live = liveness.OUT[BB.id];

            for (int i = BB.instrs.size() - 1 ; i >= 0 ; liveness.Process_Instr (BB, i, live), i --) {
                auto& instr = cfg.method->instrs [BB.instrs[i]];
                Var dist = instr -> get_dist();
                if (dist == "" || !is_V_Reg (dist) ) continue;

                int web1_id = V_Reg_Web (dist);
                
                // add adj
                for (auto& u : liveness.Var_to_bit) {
                    Var var = u.first;
                    int bit = u.second;

                    if (var == "" || !is_V_Reg (var) || live [bit] == false) continue;

                    int web2_id = V_Reg_Web (var);
                    AddEdge (web1_id, web2_id);
                }
            }
        }
    }

    void Build_Webs (std::set<Var>& Ignore) {
        ReachingDefinitions::Reaching_Definitions r_d (cfg);
        std::map <Def, std::vector<Use>> Chains = r_d.Def_Use_Chains();


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

    bool in (std::set<int>& s, int id) {
        return s.find (id) != s.end ();
    }

    std::vector <int> nested_fors;
    void fill_nested_fors () {
        int crnt = 0 ;

        std::string for_body   = "for_body"  ;
        std::string while_body = "while_body";

        std::string for_end   = "for_end";
        std::string while_end = "while_end";

        for (auto& instr : cfg.method->instrs) {
            if ( is_instance_of (instr, Linear::Label) ) {
                Linear::Label* label_ptr = dynamic_cast<Linear::Label*>(instr.get());
                std::string label = label_ptr->label;
                crnt += ( !label.compare(0, for_body.size(), for_body) || !label.compare(0, while_body.size(), while_body));
                crnt -= ( !label.compare(0, for_end.size(), for_end) || !label.compare(0, while_end.size(), while_end));
            }
            nested_fors.push_back (crnt);
        }
    }
    
};
}