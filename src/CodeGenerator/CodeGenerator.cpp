#include "CodeGenerator.h"
#include "semantics.h"
#include "LinearBuilder.h"

int CodeGenerator::Generate(std::ifstream& fin, std::ofstream& fout) {

    Semantics semantics;
    int valid_semantics = semantics.check(fin,fout);
    if (valid_semantics != 0 ) return valid_semantics;


    LinearBuilder linear_builder;
    std::unique_ptr<Linear::Program> linear_program = linear_builder.build (std::move(semantics.AST));

    for (auto& method : linear_program->methods) {
        std::cout << method->type << " " << method->id << std::endl;
        for (auto& location : method->params) {
            std::cout << location->type << " " << location->id << std::endl;
        }
        std::cout << "----" << std::endl;;
    }
    return 1;
} 