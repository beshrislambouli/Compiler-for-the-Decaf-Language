#include <iostream>
#include <fstream>
#include <memory>
#include "cli.h"
#include "scanner.h"
#include "parser.h"
#include "semantics.h"
#include "CodeGenerator.h"

namespace decaf {

    class DecafCompiler {
    public:
        static int main(int argc, char** argv) {
            std::vector<std::string> optNames = {"cp", "cse", "cf", "dce", "regalloc"};
            CommandLineInterface::parse(argc, argv, optNames);
        
            std::ifstream inputStream = input (CommandLineInterface::infile);
            std::ofstream outputStream= output(CommandLineInterface::outfile);
            
            if (CommandLineInterface::target == CompilerAction::DEFAULT) {
                CommandLineInterface::target = CompilerAction::ASSEMBLY;
            }

            if (CommandLineInterface::target == CompilerAction::SCAN) {
                Scanner scanner;
                return scanner.scan(inputStream,outputStream);
            } else if (CommandLineInterface::target == CompilerAction::PARSE) {
                Parser parser;
                return parser.parse(inputStream,outputStream); 
            } else if (CommandLineInterface::target == CompilerAction::INTER) {
                Semantics semantics;
                return semantics.check(inputStream,outputStream);
            } else if (CommandLineInterface::target == CompilerAction::ASSEMBLY) {
                std::set <std::string> opts ;
                for (int i = 0 ; i < optNames .size () ; i ++ ) {
                    if (CommandLineInterface::opts [i]) {
                        opts .insert (optNames [i]);
                    }
                }
                CodeGenerator code_generator;
                return code_generator.Generate(inputStream,outputStream, opts);
            }
            return 1;
        }

        static std::ifstream input(std::string infile) {
            std::ifstream inputStream;
            // Check if infile is empty and throw an error
            if (CommandLineInterface::infile.empty()) {
                throw std::runtime_error("Input file is required.");
            } else {
                inputStream.open(CommandLineInterface::infile);
                if (!inputStream.is_open()) {
                    throw std::runtime_error("Failed to open input file: " + CommandLineInterface::infile);
                }
            }
            return inputStream;
        }

        static std::ofstream output(std::string outfile) {
            std::ofstream outputStream;
            // If outfile is empty, default to stdout using ofstream
            if (CommandLineInterface::outfile.empty()) {
                outputStream.copyfmt(std::cout);  // Share the formatting settings with stdout
                outputStream.clear(std::cout.rdstate());  // Copy the state of std::cout
                outputStream.basic_ios<char>::rdbuf(std::cout.rdbuf());  // Use the same buffer as std::cout
            } else {
                outputStream.open(CommandLineInterface::outfile);
                if (!outputStream.is_open()) {
                    throw std::runtime_error("Failed to open output file: " + CommandLineInterface::outfile);
                }
            }
            return outputStream;
        }
    };
}

int main(int argc, char** argv) {
    return decaf::DecafCompiler::main(argc, argv);
}
