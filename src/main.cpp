#include <iostream>
#include <fstream>
#include <memory>
#include "cli.h"

namespace decaf {

    class DecafCompiler {
    public:
        static void main(int argc, char** argv) {
            std::cout << "WORKED" << std::endl;
            // CommandLineInterface::parse(argc, argv, {});
            // std::ifstream inputStream;
            // std::ofstream outputStream;

            // if (!CommandLineInterface::infile.empty()) {
            //     inputStream.open(CommandLineInterface::infile);
            // }
            // if (!CommandLineInterface::outfile.empty()) {
            //     outputStream.open(CommandLineInterface::outfile);
            // }

            // switch (CommandLineInterface::target) {
            //     case CompilerAction::SCAN:
            //         break;
            //     case CompilerAction::PARSE:
            //         break;
            //     case CompilerAction::INTER:
            //         break;
            //     case CompilerAction::ASSEMBLY:
            //         break;
            //     default:
            //         break;
            // }
        }
    };
}

int main(int argc, char** argv) {
    decaf::DecafCompiler::main(argc, argv);
    return 0;
}
