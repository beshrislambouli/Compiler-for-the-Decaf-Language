#include "CodeGenerator.h"
#include "semantics.h"
#include "LinearBuilder.h"
#include "LinearPrinter.h"

int CodeGenerator::Generate(std::ifstream& fin, std::ofstream& fout) {

    Semantics semantics;
    int valid_semantics = semantics.check(fin,fout);
    if (valid_semantics != 0 ) return valid_semantics;


    LinearBuilder linear_builder;
    std::unique_ptr<Linear::Program> linear_program = linear_builder.build (std::move(semantics.AST));

    Linear::PrettyPrinter printer;
    linear_program -> accept (printer); 
    return 0;
} 