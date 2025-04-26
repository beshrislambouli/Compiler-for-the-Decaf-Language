#pragma once
#include <cassert>
#include <memory>
#include <iostream>
#include <map>
#include "Linear.h"
#include "LinearPrinter.h"

#define is_instance_of(uptr, Type) (dynamic_cast<Type*>((uptr).get()) != nullptr)

class Basic_Block {
public:
    int id;
    std::vector<int> instrs;
    std::vector<int> parents;
    std::vector<int> children;

    Basic_Block (int id, std::vector<int> instrs) : id(id) , instrs(instrs) {}

    void add_parent (int BB_id) {
        parents.push_back(BB_id);
    }

    void add_child (int BB_id) {
        children.push_back(BB_id);
    }
};


class CFG {
    std::unique_ptr<Linear::Method>& method;
public:
    std::vector<Basic_Block> BBs;
    CFG(std::unique_ptr<Linear::Method>& method);
    void add_directed_edge (int bb_from, int bb_to);
    void print();
};