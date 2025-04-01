#include <iostream>
#include <fstream>
#include "DecafLexer.h"
#include "DecafParser.h"
#include "antlr4-runtime.h"

class Semantics {
public:
    int check (std::ifstream& fin, std::ofstream& fout);
};