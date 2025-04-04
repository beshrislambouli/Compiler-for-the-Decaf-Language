#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "DecafAST.h"

using T_t = AST::Type::Type_t;


class Scope {
public:
    std::unordered_map < std::string, T_t > hash_table;
    bool is_loop;


    Scope(bool is_loop) : is_loop(is_loop) {}

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

    // stack
    void add_new_scope (bool is_loop = false) {
        stack.push_back (Scope{is_loop});
    }

    void pop() {
        if (stack.size () == 0 ) {
            std::cout << "ERROR: POP Scope_Stack" << std::endl;
            return;
        }
        stack.pop_back();
    }

    bool is_loop() {
        return stack.back().is_loop;
    }


    // scope
    void put (std::string id, T_t t) {
        stack .back ().put (id,t);
    }

    T_t get (std::string id) {
        for (int i = stack.size() -1 ; i >= 0 ; i -- ) {
            T_t ans = stack[i].get (id);
            if ( ans != T_t::Null_Type ) {
                return ans;
            }
        }
        return T_t::Null_Type;
    }

    bool in_current_scope (std::string id) {
        return stack.back().get(id) != T_t::Null_Type;
    }

    bool declared (std::string id) {
        return get(id) != T_t::Null_Type;
    }

    

    std::string Declare(std::string id, T_t type_t, int row, int col, std::string AST_Node_Type) {
        std::stringstream error;
        if (in_current_scope(id)) {
            error << "Error: " << "Line: " << row << " " << "Col: " << col << " " << AST_Node_Type << " " << id << " already in scope" << std::endl;
        }
        put(id, type_t);
        return error.str();
    }

};

