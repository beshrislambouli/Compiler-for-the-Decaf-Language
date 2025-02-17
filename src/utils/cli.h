#ifndef CLI_H
#define CLI_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

namespace decaf {

    enum class CompilerAction { DEFAULT, SCAN, PARSE, INTER, ASSEMBLY };

    class CommandLineInterface {
    public:
        static void printUsage(const std::string& message);
        static void parse(int argc, char** argv, const std::vector<std::string>& optNames);

        static std::string infile;
        static std::string outfile;
        static CompilerAction target;
        static bool debug;
        static std::vector<std::string> extras;
        static std::vector<bool> opts;
    };

}

#endif
