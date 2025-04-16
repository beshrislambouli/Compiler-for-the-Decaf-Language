#include "CodeGenerator.h"
#include "semantics.h"
#include "LinearBuilder.h"

int CodeGenerator::Generate(std::ifstream& fin, std::ofstream& fout) {

    Semantics semantics;
    int valid_semantics = semantics.check(fin,fout);
    if (valid_semantics != 0 ) return valid_semantics;


    LinearBuilder linear_builder;
    semantics.AST -> accept (linear_builder);
    return 1;
} 