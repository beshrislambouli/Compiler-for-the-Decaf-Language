#include "CodeGenerator.h"
#include "semantics.h"

int CodeGenerator::Generate(std::ifstream& fin, std::ofstream& fout) {
    Semantics semantics;
    return semantics.check (fin,fout);
} 