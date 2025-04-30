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
    std::set <Move> moves;

    // graph 
    int deg = 0;
    std::set <int> adj;
    std::set <int> del;

    // color
    int alias = -1;
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
    bool is_adj (int node) {
        return adj.find (node) != adj.end () ;
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
        // PreColor(REG);

        Build_Interference ();
        // for (auto& web : webs ) {
        //     web.print ();
        // }
        Color ();
    }

    // webs
    std::set <int> precolored;
    std::set <int> simplifyWorklist;
    std::set <int> freezeWorklist;
    std::set <int> spillWorklist;
    std::set <int> coalescedNodes;
    std::set <int> coloredNodes;
    std::set <int> spilledNodes;
    std::vector <int> selectStack;
    
    // moves
    std::set<int> coalescedMoves;
    std::set<int> constrainedMoves;
    std::set<int> frozenMoves;
    std::set<int> worklistMoves;
    std::set<int> activeMoves;

    void printAll() {
    std::cout << "precolored: ";
    for (const int& x : precolored) std::cout << x << " ";
    std::cout << std::endl;

    std::cout << "simplifyWorklist: ";
    for (const int& x : simplifyWorklist) std::cout << x << " ";
    std::cout << std::endl;

    std::cout << "freezeWorklist: ";
    for (const int& x : freezeWorklist) std::cout << x << " ";
    std::cout << std::endl;

    std::cout << "spillWorklist: ";
    for (const int& x : spillWorklist) std::cout << x << " ";
    std::cout << std::endl;

    std::cout << "coalescedNodes: ";
    for (const int& x : coalescedNodes) std::cout << x << " ";
    std::cout << std::endl;

    std::cout << "coloredNodes: ";
    for (const int& x : coloredNodes) std::cout << x << " ";
    std::cout << std::endl;

    std::cout << "spilledNodes: ";
    for (const int& x : spilledNodes) std::cout << x << " ";
    std::cout << std::endl;

    std::cout << "selectStack: ";
    for (const int& x : selectStack) std::cout << x << " ";
    std::cout << std::endl;

    std::cout << "coalescedMoves: ";
    for (const int& x : coalescedMoves) std::cout << x << " ";
    std::cout << std::endl;

    std::cout << "constrainedMoves: ";
    for (const int& x : constrainedMoves) std::cout << x << " ";
    std::cout << std::endl;

    std::cout << "frozenMoves: ";
    for (const int& x : frozenMoves) std::cout << x << " ";
    std::cout << std::endl;

    std::cout << "worklistMoves: ";
    for (const int& x : worklistMoves) std::cout << x << " ";
    std::cout << std::endl;

    std::cout << "activeMoves: ";
    for (const int& x : activeMoves) std::cout << x << " ";
    std::cout << std::endl;

    std::cout << "------------" << std::endl;
}

    void Color () {
        MakeWorklist ();

        while (true) {
            // printAll();
            if (simplifyWorklist.size()) Simplify();
            else if (worklistMoves.size()) Coalesce();
            else if (freezeWorklist.size()) Freeze();
            else if (spillWorklist.size()) SelectSpill();
            else break;
        }
        // std::cout << "DONE" << std::endl;
        // printAll();
        AssignColors();

    }

    void Simplify() {
        int node = * (simplifyWorklist.begin());
        simplifyWorklist.erase (node);
        
        selectStack .push_back (node);
        
        auto ADJ = Adjacent (node);
        for (auto u : ADJ) {
            DecrementDegree (u);
        }
    }

    void Coalesce() {
        int m = *(worklistMoves.begin());

        auto p = dist_src (m);
        int x = p.first;
        int y = p.second;

        x = GetAlias (x);
        y = GetAlias (y);

        int u,v;
        if ( in (precolored,y) ) {
            u = y ;
            v = x ;
        } else {
            u = x ;
            v = y ;
        }

        worklistMoves.erase (m);
        if (u == v) {
            coalescedMoves .insert (m);
            AddWorkList (u);
        } else if ( in (precolored,u) || in (precolored,v) || (webs [u].is_adj (v) || webs [v].is_adj (u)) ) {
            constrainedMoves .insert (m);
            AddWorkList (u);
            AddWorkList (v);
        } else if ( ( in (precolored,u) && OK (v,u) ) || ( !in (precolored,u) && Conservative (u,v) )) {
            coalescedMoves .insert (m);
            Combine (u,v);
            AddWorkList(u);
        } else {
            activeMoves .insert (m);
        }
    }

    void Combine (int u, int v) {
        if ( in (freezeWorklist, v) ) {
            freezeWorklist .erase (v);
        } else {
            spillWorklist .erase (v);
        }
        coalescedNodes .insert (v);
        webs [v].alias = u;

        for (auto m : webs [v].moves) {
            webs [u].moves .insert (m);
        }

        auto ADJ_v = Adjacent (v);
        for (auto t : ADJ_v) {
            AddEdge (t,u);
            DecrementDegree(t);
        }
        if (webs [u].deg >= K && in(freezeWorklist,u) ) {
            freezeWorklist .erase (u);
            spillWorklist  .insert(u);
        }
    }

    void Freeze () {
        int node = *(freezeWorklist.begin());
        freezeWorklist.erase (node);
        simplifyWorklist .insert (node);
        FreezeMoves (node);
    }

    void FreezeMoves (int node) {
        auto node_moves = NodeMoves(node);

        for (auto m : node_moves) {
            auto p = dist_src (m);
            int x = p.first ;
            int y = p.second;

            int v;
            if (GetAlias(y) == GetAlias (node)) {
                v = GetAlias(x);
            } else {
                v = GetAlias (y);
            }

            activeMoves .erase (m);
            frozenMoves .insert(m);

            auto tmp = NodeMoves(v);
            if (tmp.size() == 0 && webs [v].deg < K ) {
                freezeWorklist   .erase (v);
                simplifyWorklist .insert(v);
            }
        }
    }

    void SelectSpill() {
        int node = *(spillWorklist.begin());
        spillWorklist.erase (node);
        simplifyWorklist.insert (node);
        FreezeMoves (node);
    }

    void AssignColors () {
        while (selectStack.size ()) {
            int node = selectStack.back();
            selectStack.pop_back();

            std::set <int> okColors;
            for (int i = 0 ; i < K ; i ++ ) okColors .insert (i);

            for (auto w : webs [node].adj) {
                auto alias = GetAlias (w);
                if ( in(precolored,alias) || in (coloredNodes,alias) ) {
                    okColors .erase (webs [alias].color);
                }
            }
            if ( okColors.size() == 0 ) {
                spilledNodes.insert (node);
                webs [node].spilled = true;
            } else {
                coloredNodes.insert (node);
                webs [node].color = * (okColors.begin());
            }
        }
        for (auto node : coalescedNodes) {
            webs [node].color = webs [GetAlias(node)].color;
        }
    }

    bool Conservative (int u , int v) {
        int k = 0 ;
        
        auto ADJ = Adjacent (u);
        for (auto node : ADJ) {
            if ( webs [node].deg >= K ) k ++ ;
        }
        ADJ = Adjacent (v);
        for (auto node : ADJ) {
            if ( webs [node].deg >= K ) k ++ ;
        }
        return k < K;
    }

    bool OK (int v, int r) {
        auto ADJ = Adjacent (v);

        for (auto t : ADJ ) {
            // std::cout << "V: " << v << " t " << t << " " << in(precolored,t) << " " << webs[t].color << " " << webs[r].color << std::endl;
            // if (in(precolored,t) && webs[t].color == webs[r].color) return false;
            if ( webs [t].deg < K || in (precolored,t) || (webs [t].is_adj (r) || webs[r].is_adj (t) )) continue;
            return false;
        }

        return true; 
    }
    void AddWorkList(int node) {
        if ( !in(precolored,node) && !MoveRelated (node) && webs [node].deg < K ) {
            freezeWorklist .erase (node);
            simplifyWorklist .insert (node);
        }
    }

    int GetAlias (int node) {
        if ( in (coalescedNodes, node) ) {
            return GetAlias (webs[node].alias);
        } else {
            return node;
        }
    }

    void DecrementDegree (int node) {
        webs [node] .deg -- ;
        if (webs [node].deg == K-1 ) {
            auto ADJ = Adjacent (node);
            ADJ .insert (node);
            EnableMoves (ADJ);
            spillWorklist .erase (node);
            if (MoveRelated (node)) {
                freezeWorklist .insert (node);
            } else {
                simplifyWorklist.insert (node);
            }
        }
    }

    void EnableMoves (std::set<int> nodes) {
        for (auto node : nodes) {
            auto node_moves = NodeMoves (node);
            for (auto m : node_moves) {
                if ( in (activeMoves, m) ) {
                    activeMoves .erase (m);
                    worklistMoves .insert (m);
                }
            }
        }
    }

    void MakeWorklist () {
        for (int web_id = 0 ; web_id < webs.size () ; web_id ++ ) {
            if (webs [web_id].color != -1) continue;
            if (webs [web_id].deg >= K ) {
                spillWorklist  .insert (web_id);
            } else if ( MoveRelated (web_id) ){
                freezeWorklist .insert (web_id);
            } else {
                simplifyWorklist .insert (web_id);
            }
        }
    }

    // adjList[n] \ (selectStack ∪ coalescedNodes)
    std::set<int> Adjacent (int node) {
        std::set <int> ret;
        for (auto u : webs[node].adj) {
            if ( ! in (coalescedNodes,u) ) {
                bool here = false;
                for (auto v : selectStack) if (v == u) here = true;
                if (!here) {
                    ret .insert (u);
                }
            }
        }
        return ret;
    }

    // moveList[n] ∩ (activeMoves ∪ worklistMoves)
    std::set<int> NodeMoves (int node ) {
        std::set <int> ret;
        for (auto m : webs [node].moves){
            if ( in (activeMoves,m) ||  in(worklistMoves,m) ) {
                ret .insert (m);
            }
        }
        return ret;
    }

    // NodeMoves(n) ̸= {}
    bool MoveRelated (int node) {
        std::set<int> tmp = NodeMoves(node);
        return tmp.size () != 0;
    }

    void PreColor(std::vector<std::string> REG) {
        std::string param_reg [] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};

        for (auto& web : webs ) {
            if (! web.is_arg ) continue;
            
            for (int c = 0 ; c < REG.size () ; c ++ ) {
                if ( REG [c] == param_reg [web.arg_num]) {
                    web.color = c;
                }
            }
        }
    }

    void AddEdge (int web1_id, int web2_id) {
        if (web1_id == web2_id) return ;
        if ( !in (precolored, web1_id) ) {
            webs [web1_id] .add_adj (web2_id);
            webs [web1_id] .deg = webs [web1_id] .adj.size() ;
        }
        if ( !in (precolored, web2_id) ) {
            webs [web2_id] .add_adj (web1_id); 
            webs [web2_id] .deg = webs [web2_id] .adj.size() ;
        }
    }


    // void Color (int k) {

    //     // classify nodes
    //     std::set <int> web_less_k;
    //     std::set <int> web_more_k;
    //     for (int i = 0 ; i < webs.size() ; i ++ ) {
    //         if (webs [i].color != -1) continue;
            
    //         if (webs[i].adj.size () >= k){
    //             web_more_k .insert (i);
    //         } else {
    //             web_less_k .insert (i);
    //         }
    //     }
        
    //     // fill stack 
    //     std::vector <int> stack;
    //     while (web_less_k.size () > 0 || web_more_k .size () > 0 ) {
    //         if (web_less_k.size()>0){
    //             int node = * (web_less_k.begin());
    //             web_less_k .erase (node);

    //             stack.push_back (node);
                
    //             for (auto adj_id : webs[node].adj) {
    //                 auto& adj_web = webs [adj_id];

    //                 adj_web.del_adj (node);
    //                 if (adj_web.adj.size () == k-1) {
    //                     web_more_k .erase (adj_id);
    //                     web_less_k .insert(adj_id);
    //                 }
    //             }
    //         } else if (web_more_k.size()>0) {
    //             int to_spill_id = -1;
    //             double min_cost = 1e9;
    //             for (auto web_id : web_more_k) {
    //                 double cur_cost = webs [web_id].get_cost() / (double)(webs [web_id].adj.size());
    //                 if (cur_cost < min_cost) {
    //                     min_cost = cur_cost;
    //                     to_spill_id = web_id;
    //                 }
    //             }
    //             assert (to_spill_id != -1);

    //             web_more_k.erase (to_spill_id);
    //             web_less_k.insert(to_spill_id);
    //         }
    //     }

    //     // return the del to adj
    //     for (auto& web : webs) {
    //         web.restore ();
    //     }

    //     // color
    //     while (stack.size()) {
    //         int node = stack.back () ;
    //         auto& web = webs [node];
    //         stack.pop_back();

    //         std::set<int> taken_colors;
    //         for (auto u : web.adj) {
    //             taken_colors .insert (webs[u].color);
    //         }

    //         bool to_spill = true;
    //         for (int c = 0 ; c < k ; c ++ ) {
    //             if ( taken_colors.find (c) == taken_colors.end () ) {
    //                 web. color = c;
    //                 to_spill = false;
    //                 break;
    //             }
    //         }

    //         web. spilled = to_spill;
    //     }

    // }


    void Build_Interference () {
        Liveness::Liveness liveness(cfg);

        for (auto& BB: cfg.BBs) {
            std::vector<bool> live = liveness.OUT[BB.id];

            for (int i = BB.instrs.size() - 1 ; i >= 0 ; liveness.Process_Instr (BB, i, live), i --) {
                auto& instr = cfg.method->instrs [BB.instrs[i]];
                Var dist = instr -> get_dist();
                if (dist == "" || !is_V_Reg (dist) ) continue;

                int web1_id = V_Reg_Web (dist);
                
                // add moves
                if ( is_instance_of (instr, Linear::Assign) ) {
                    Linear::Assign* assign_ptr = dynamic_cast <Linear::Assign*> (instr.get());
                    Var src = assign_ptr->operands[0]->id;
                    if (src != "" && is_V_Reg (src)) {
                        int web2_id = V_Reg_Web (src);

                        webs [web1_id] .moves .insert (BB.instrs[i]);
                        webs [web2_id] .moves .insert (BB.instrs[i]);
                        worklistMoves  .insert (BB.instrs[i]);
                    }
                }


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
            // std::string prefix = "FUNC_ARG_";
            // if (web.original_id.compare(0, prefix.size(), prefix) == 0) {
            //     size_t n_pos = web.original_id.rfind("_N");
                
            //     web .is_arg = true;
            //     web .arg_num= std::stoi(web.original_id.substr(n_pos+2));
            //     web .new_id += "_ARG_" + std::to_string (web.arg_num);
            //     precolored .insert (counter-1);
            // }


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

    std::pair<int,int> dist_src (int m) {
        auto& instr = cfg.method->instrs [m];
        if (! is_instance_of (instr, Linear::Assign) ) std::cout << "ERROR" << std::endl;
        Linear::Assign* assign_ptr = dynamic_cast <Linear::Assign*> (instr.get());
        Var x_id = assign_ptr -> get_dist();
        Var y_id = (assign_ptr -> get_operands())[0];
        int x = V_Reg_Web (x_id);
        int y = V_Reg_Web (y_id);
        return std::make_pair (x,y);
    }
    
};
}