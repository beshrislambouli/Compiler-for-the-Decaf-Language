#pragma once 

#include <iostream>
#include <fstream>


class CodeGenerator {
public:
    int Generate(std::ifstream& fin, std::ofstream& fout);
};