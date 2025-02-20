#include <iostream>
#include <fstream>
#include "DecafLexer.h"
#include "antlr4-runtime.h"


class Scanner {
public:
    int scan (std::ifstream& fin, std::ofstream& fout);
};