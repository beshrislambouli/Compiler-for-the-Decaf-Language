#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include "DecafAST.h"

using T_t = AST::Type::Type_t;


class Scope {
    std::unordered_map < std::string, T_t > hash_table;
public:
    void put (std::string id, T_t t) {
        if (hash_table.find(id) != hash_table.end()) {
            std::cout << "ERROR: Scope put" << std::endl;
        }
        hash_table [id] = t;
    }
    T_t get (std::string id) {
        if (hash_table.find(id) == hash_table.end ()) {
            return T_t::Null_Type;
        }
        return hash_table [id];
    }
};

class Scope_Stack {
    std::vector <Scope> stack;
public:
    void add_new_scope () {
        stack.push_back (Scope{});
    }

    void put (std::string id, T_t t) {
        stack .back ().put (id,t);
    }

    T_t get (std::string id) {
        T_t ans = stack.back().get (id);
        if ( ans == T_t::Null_Type ) {
            std::cout << "ERROR: Scope_Stack get" << std::endl;
        }
        return ans;
    }

    bool in_current_scope (std::string id) {
        return stack.back().get(id) != T_t::Null_Type;
    }

    void pop() {
        if (stack.size () == 0 ) {
            std::cout << "ERROR: POP Scope_Stack" << std::endl;
            return;
        }
        stack.pop_back();
    }
};

