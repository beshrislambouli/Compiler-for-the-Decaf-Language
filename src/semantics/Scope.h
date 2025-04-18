#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <variant>
#include <optional>
#include "DecafAST.h"

using T_t = AST::Type::Type_t;


class Method {
public:
    T_t type=T_t::Null_Type; 
    bool is_import=false;
    std::vector<T_t> parameters; 

    Method () {}
    Method(T_t type, bool is_import, const std::vector<T_t>& parameters) 
    : type(type)
    , is_import(is_import)
    , parameters(parameters) 
    {}
};

class Var {
public:
    T_t type=T_t::Null_Type;
    
    Var (){}
    Var (T_t type) : type(type) {}
};

class Arr {
public:
    T_t type=T_t::Null_Type;
    std::string size="-1";

    Arr (){}
    Arr (T_t type, std::string size) : type(type) , size(size) {}
};
 
using Value = std::variant<Method, Var, Arr>;

class Scope {
public:
    std::unordered_map < std::string, Value > hash_table;
    bool is_loop=false;
    std::string current_method="";

    Scope () {}
    Scope (bool is_loop) : is_loop(is_loop) {}
    Scope (std::string current_method) : current_method(current_method) {}

    bool in_scope(std::string& id) {
        return hash_table.find(id) != hash_table.end();
    }


    // put
    void put_method(std::string id, T_t type, bool is_import, std::vector<T_t>& parameters) {

        if ( in_scope (id) ) std::cout << "ERROR: Scope put" << std::endl;

        hash_table [id] = Method(type,is_import,parameters);
    }

    void put_var(std::string id, T_t type) {
        
        if ( in_scope (id) ) std::cout << "ERROR: Scope put" << std::endl;

        hash_table [id] = Var(type);
    }

    void put_arr(std::string id, T_t type, std::string size) {
        
        if ( in_scope (id) ) std::cout << "ERROR: Scope put" << std::endl;

        hash_table [id] = Arr(type,size);
    }

    // get
    std::optional<Value> get (std::string id) {
        if ( !in_scope (id) ) return std::nullopt;
        return hash_table [id];
    }
};

class Scope_Stack {
    std::vector <Scope> stack;

public:
    //helper var
    bool is_extern_arg_for_import_method=false;


    // stack
    void push_new_scope () {
        stack.push_back (Scope());
    }

    void push_loop_scope () {
        stack.push_back(Scope(true));
    }

    void push_method_scope (std::string id) {
        stack.push_back(Scope(id));
    }

    void pop() {
        if (stack.size () == 0 ) {
            std::cout << "ERROR: POP Scope_Stack" << std::endl;
            return;
        }
        stack.pop_back();
    }

    bool is_loop() {
        for (int i = stack.size () - 1 ;  i >= 0 ; i -- ) {
            if (stack[i].is_loop) {
                return true;
            }
        }
        return false;
    }

    std::optional<std::string> get_current_method() {
        for (int i = stack.size () - 1 ;  i >= 0 ; i -- ) {
            if (stack[i].current_method != "" ) {
                return stack[i].current_method;
            }
        }
        std::cout << "ERROR: Scope_Stack : get_current_method" << std::endl;
        return std::nullopt;
    }


    // scope

    // put
    void put_method(std::string id, T_t type, bool is_import, std::vector<T_t>& parameters) {

        stack.back().put_method(id, type, is_import, parameters);
    }

    void put_var(std::string id, T_t type) {

        stack.back().put_var(id, type);
    }

    void put_arr(std::string id, T_t type, std::string size) {
        
        stack.back().put_arr(id, type, size);
    }


    // get
    std::optional <Value> get (std::string id) {
        for (int i = stack.size() -1 ; i >= 0 ; i -- ) {
            auto ans = stack[i].get (id);
            if ( ans.has_value() ) {
                return ans.value();
            }
        }
        return std::nullopt;
    }

    bool in_current_scope (std::string id) {
        return stack.back().get(id).has_value();
    }

    bool is_declared (std::string id) {
        return get(id).has_value();
    }

    std::optional<T_t> get_type (std::string id) {
        if (!is_declared(id)) return std::nullopt;
        Value ans = get(id).value();
        
        if (std::holds_alternative<Method>(ans)) {
            return std::get<Method>(ans).type;
        }
        if (std::holds_alternative<Var>(ans)) {
            return std::get<Var>(ans).type;
        }
        if (std::holds_alternative<Arr>(ans)) {
            return std::get<Arr>(ans).type;
        }

        std::cout << "ERROR: Scope_Stack : get_type " << std::endl;
        return std::nullopt;
    }

    bool is_array (std::string id) {
        if (!is_declared(id)) return false;

        if (std::holds_alternative<Arr>(get(id).value())) {
            return true;
        }

        return false;
    }
    std::string get_array_size(std::string id) {
        if (!is_array(id)) return "-1";
        Value ans = get(id).value();
        if (std::holds_alternative<Arr>(ans)) {
            return std::get<Arr>(ans).size;
        }
        return "-1";
    }

    bool is_var (std::string id) {
        if (!is_declared(id)) return false;

        if (std::holds_alternative<Var>(get(id).value())) {
            return true;
        }

        return false;
    }

    bool is_method (std::string id) {
        if (!is_declared(id)) return false;

        if (std::holds_alternative<Method>(get(id).value())) {
            return true;
        }

        return false;
    }

    bool is_import (std::string id) {
        if (!is_declared(id)) return false;

        Value ans = get(id).value();
        if (std::holds_alternative<Method>(ans)) {
            if (std::get<Method>(ans).is_import){
                return true;
            }
        }

        return false;
    }

    std::vector<T_t> get_method_parameters (std::string id) {
        if (!is_method(id)) {
            std::cout << "ERROR: Scope_Stack : get_method_parameters" << std::endl;
            return {};
        }
        return std::get<Method>(get(id).value()).parameters; // TODO: recheck this
    }


    std::string declare_method(std::string id, T_t type_t, bool is_import, std::vector<T_t> parameters, int row, int col, std::string AST_Node_Type) {
        std::stringstream error;
        if (in_current_scope(id)) {
            error << "Error: " << "Line: " << row << " " << "Col: " << col << " " << AST_Node_Type << " " << id << " already in scope" << std::endl;
        }
        put_method(id, type_t,is_import,parameters);
        return error.str();
    }

    std::string declare_var(std::string id, T_t type_t, int row, int col, std::string AST_Node_Type) {
        std::stringstream error;
        if (in_current_scope(id)) {
            error << "Error: " << "Line: " << row << " " << "Col: " << col << " " << AST_Node_Type << " " << id << " already in scope" << std::endl;
        }
        put_var(id, type_t);
        return error.str();
    }

    std::string declare_arr(std::string id, T_t type_t, std::string size, int row, int col, std::string AST_Node_Type) {
        std::stringstream error;
        if (in_current_scope(id)) {
            error << "Error: " << "Line: " << row << " " << "Col: " << col << " " << AST_Node_Type << " " << id << " already in scope" << std::endl;
        }
        put_arr(id, type_t, size);
        return error.str();
    }

};

