#include "CFG.h"
#include <vector>
#include <set>

namespace DataFlowAnalysis {

class DataFlowAnalysis {
    CFG& cfg;
    std::vector<std::vector<bool>>& IN  ;
    std::vector<std::vector<bool>>& OUT ;
    std::vector<std::vector<bool>>& GEN ;
    std::vector<std::vector<bool>>& KILL;
public:
    enum Type {
        Reaching_Definitions_t,
        Available_Expressions_t,
        Liveness_t,
    };

    DataFlowAnalysis(   Type type, CFG& cfg, 
                        std::vector<std::vector<bool>>& IN ,
                        std::vector<std::vector<bool>>& OUT,
                        std::vector<std::vector<bool>>& GEN, 
                        std::vector<std::vector<bool>>& KILL
                    ) : cfg(cfg), IN (IN), OUT(OUT), GEN(GEN), KILL(KILL)
    {
                        
        int N = cfg.BBs.size();
        if (N == 0) return;
        int S = GEN [0].size();
        // init IN, OUT
        init (S, type) ;

        // init todo
        std::set<int> todo;
        for (int i = 0 ; i < N ; i ++ ) todo .insert (i);
        if ( type == Reaching_Definitions_t || type == Available_Expressions_t) todo.erase (0);
        if ( type == Liveness_t ) todo.erase(N-1);
        while (todo.size()) {
            int n =  *(todo.begin());
            todo.erase(todo.begin());
            if ( type == Reaching_Definitions_t ) {
                fill (IN[n] ,false);
                for (auto& parent : cfg.BBs[n].parents) {
                    Union (IN[n], OUT[parent], IN [n]) ;
                }

                std::vector<bool> old (S);
                equal (old, OUT [n]);
                std::vector<bool> diff = Diff(IN[n], KILL[n]);
                Union ( GEN [n], diff, OUT[n]);

                if (changed (old, OUT[n])) {
                    for (auto& child : cfg.BBs[n].children) {
                        todo.insert (child);
                    }
                }

            }
            if ( type == Available_Expressions_t) {
                fill (IN[n] ,true);
                for (auto& parent : cfg.BBs[n].parents) {
                    Intersect (IN[n], OUT[parent], IN [n]) ;
                }

                std::vector<bool> old (S);
                equal (old, OUT [n]);
                std::vector<bool> diff = Diff(IN[n], KILL[n]);
                Union ( GEN [n], diff, OUT[n]);

                if (changed (old, OUT[n])) {
                    for (auto& child : cfg.BBs[n].children) {
                        todo.insert (child);
                    }
                }

            }
            if ( type == Liveness_t) {
                fill (OUT[n],false);
                for (auto& child : cfg.BBs[n].children) {
                    Union (OUT[n], IN [child], OUT [n]) ;
                }

                std::vector<bool> old (S);
                equal (old, IN [n]);
                std::vector<bool> diff = Diff(OUT[n], KILL[n]);
                Union ( GEN [n], diff, IN[n]);

                if (changed (old, IN[n])) {
                    for (auto& parent : cfg.BBs[n].parents) {
                        todo.insert (parent);
                    }
                }
            }
        }

    }


    void init (int S, Type type) {
        int N = cfg.BBs.size () ;
        // init with correct values for type
        int entry = 0;
        int exit = N-1;
        switch (type)
        {
        case Reaching_Definitions_t:

            // for all nodes n in N OUT[n] = emptyset; 
            for (int i = 0 ; i < N ; i ++ ) {
                fill (OUT[i],false); 
            }

            // IN[Entry] = emptyset; 
            fill (IN[entry],false);

            // OUT[Entry] = GEN[Entry];
            for (int i = 0 ; i < S ; i ++ ) {
                OUT [entry][i] = GEN[entry][i];
            }

            break;
        
        case Available_Expressions_t:

            // for all nodes n in N OUT[n] = E; 
            for (int i = 0 ; i < N ; i ++ ) {
                fill (OUT[i],true); 
            }

            // IN[Entry] = emptyset; 
            fill (IN[entry],false);

            // OUT[Entry] = GEN[Entry];
            for (int i = 0 ; i < S ; i ++ ) {
                OUT [entry][i] = GEN[entry][i];
            }

            break;

        case Liveness_t:
            
            // for all nodes n in N - {exit} IN[n] = emptyset;
            for (int i = 0 ; i < N ; i ++ ) {
                fill (IN[i],false); 
            }

            // OUT[Exit] = emptyset; 
            fill (OUT[exit],false);

            // IN[Exit] = use[Exit];
            for (int i = 0 ; i < S ; i ++ ) {
                IN [exit][i] = GEN[exit][i];
            }

            break;
        
        default:
            break;
        }

    }

    void Union (std::vector<bool>& l, std::vector<bool>& r, std::vector<bool>& res) {
        assert ( l.size() == r.size() && l.size () == res.size () ) ;
        for (int i = 0 ; i < res.size() ; i ++ ) {
            res [i] = l [i] | r [i] ;
        }
    }

    void Intersect (std::vector<bool>& l, std::vector<bool>& r, std::vector<bool>& res) {
        assert ( l.size() == r.size() && l.size () == res.size () ) ;
        for (int i = 0 ; i < res.size() ; i ++ ) {
            res [i] = l [i] & r [i] ;
        }
    }

    std::vector<bool> Diff (std::vector<bool>& l, std::vector<bool>& r) {
        std::vector<bool> res;
        assert ( l.size() == r.size() ) ;
        for (int i = 0 ; i < l.size() ; i ++ ) {
            res .push_back( l [i] == true && r [i] == false ) ;
        }
        return res;
    }

    void fill (std::vector<bool>& v, bool val) {
        for (int i = 0 ; i < v .size () ; i ++ ) {
            v [i] = val;
        }
    }
    
    void equal(std::vector<bool>& to, std::vector<bool>& from) {
        assert (from .size() == to.size () );
        for (int i = 0 ; i < to.size () ; i ++ ) {
            to [i] = from [i];
        }
    }

    bool changed(std::vector<bool>& from, std::vector<bool>& to) {
        assert (from .size() == to.size () );
        for (int i = 0 ; i < to.size () ; i ++ ) {
            if ( to [i] != from [i] ) {
                return true;
            }
        }
        return false;
    }
};
}