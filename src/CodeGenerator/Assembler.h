#pragma once
#include "CodeGenerator.h"
#include <map>

class Frame {
public:
    std::map<std::string,std::string> loc;

    bool is_def(std::string id) {
        return loc.find(id) != loc.end();
    }

    std::string get(std::string id) {
        return loc[id];
    }
    void put(std::string id, std::string loc_){
        loc [id] = loc_;
    }
};
class Assembler {
    std::vector<Frame> frame_segs;
public:
    std::string asm_code = "";
    std::string ret;

    void add_instr(std::string instr){
        asm_code += instr + '\n';
    }

    void push_scope() {
        frame_segs.push_back(Frame());
    }
    void pop_scope() {
        frame_segs.pop_back();
    }

    std::string get(std::string id){
        for (int i = frame_segs.size () - 1 ; i >= 0 ; i -- ) {
            if (frame_segs[i].is_def(id)){
                return frame_segs[i].get(id);
            }
        }
        std::cout << "ERROR get" << std::endl;
        exit(1);
    }

    int stack_size = 0;
    std::string get_next_loc(Linear::Type type) {
        if (type == Linear::Int) {
            stack_size += 4;
        } else {
            stack_size += 8;
        }
        return std::to_string(stack_size) + "(%rbp)";
    }

    void declare(std::string id, Linear::Type type){
        frame_segs.back().put(id,get_next_loc(type));
    }
    void declare (std::string id, Linear::Type type, std::string size){

    }
};