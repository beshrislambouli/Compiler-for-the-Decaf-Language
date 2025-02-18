#include <iostream>
#include <fstream>
#include "DecafLexer.h"
#include "DecafParser.h"
#include "antlr4-runtime.h"

class Parser {
public:
    int parse (std::ifstream& fin, std::ofstream& fout);
};