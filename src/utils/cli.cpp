#include "cli.h"
#include <cstdlib>
#include <cstring>
#include <algorithm>

namespace decaf {

    std::string CommandLineInterface::infile = "";
    std::string CommandLineInterface::outfile = "";
    CompilerAction CommandLineInterface::target = CompilerAction::DEFAULT;
    bool CommandLineInterface::debug = false;
    std::vector<std::string> CommandLineInterface::extras;
    std::vector<bool> CommandLineInterface::opts;

    void CommandLineInterface::printUsage(const std::string& message) {
        if (!message.empty()) {
            std::cerr << message << std::endl;
        }
        std::cerr << "Usage: run.sh [options] <filename>\n"
                  << "Summary of options:\n"
                  << "  -t <stage>              --target <stage>           compile to the given stage\n"
                  << "  -o <outfile>            --output <outfile>         write output to <outfile>\n"
                  << "  -O <(opt|-opt|all)...>  --opt <(opt|-opt|all)...>  perform the listed optimizations\n"
                  << "  -d                      --debug                    print debugging information\n"
                  << "  -h                      --help                     print help information\n"
                  << "\n"
                  << "Long description of options:\n"
                  << "  -t <stage>          <stage> is one of \"scan\", \"parse\", \"inter\", or \"assembly\".\n"
                  << "  --target <stage>    Compilation will proceed to the given stage and halt there.\n"
                  << "  -d                  Print debugging information. If this option is not given,\n"
                  << "  --debug             there will be no output to the screen on successful\n"
                  << "                      compilation.\n"
                  << "  -O <optspec>        Perform the listed optimizations. <optspec> is a comma-\n"
                  << "  --opt <optspec>     separated list of optimization names, or the special symbol\n"
                  << "                      \"all\", meaning all possible optimizations. You may\n"
                  << "                      explicitly disable an optimization by prefixing its name\n"
                  << "                      with '-'.\n"
                  << "  -o <outfile>        Write output to <outfile>. If this option is not given,\n"
                  << "  --output <outfile>  output will be written to a file with the same base name as\n"
                  << "                      the input file and the extension changed according to the\n"
                  << "                      final stage executed.\n";
        std::exit(1);
    }

    void CommandLineInterface::parse(int argc, char** argv, const std::vector<std::string>& optNames) {
        std::string targetStr = "";
        opts = std::vector<bool>(optNames.size(), false);

        if (argc == 1) {
            printUsage("No arguments given.");
        }

        for (int i = 1; i < argc; ++i) {
            if (std::string(argv[i]) == "--debug" || std::string(argv[i]) == "-d") {
                debug = true;
            } else if (std::string(argv[i]) == "--help" || std::string(argv[i]) == "-h") {
                printUsage("");
            } else if (std::string(argv[i]) == "--output" || std::string(argv[i]) == "-o") {
                if (i < argc - 1) {
                    outfile = argv[++i];
                } else {
                    printUsage("No output file specified with option " + std::string(argv[i]));
                }
            } else if (std::string(argv[i]) == "--target" || std::string(argv[i]) == "-t") {
                if (i < argc - 1) {
                    targetStr = argv[++i];
                } else {
                    printUsage("No target specified with option " + std::string(argv[i]));
                }
            } else if (std::string(argv[i]) == "--opt" || std::string(argv[i]) == "-O") {
                if (i < argc - 1) {
                    std::string optsList = argv[++i];
                    size_t pos = 0;
                    while ((pos = optsList.find(',')) != std::string::npos) {
                        std::string opt = optsList.substr(0, pos);
                        if (opt == "all") {
                            std::fill(opts.begin(), opts.end(), true);
                        } else {
                            auto it = std::find(optNames.begin(), optNames.end(), opt);
                            if (it != optNames.end()) {
                                opts[std::distance(optNames.begin(), it)] = true;
                            }
                        }
                        optsList.erase(0, pos + 1);
                    }
                } else {
                    printUsage("No optimizations specified with option " + std::string(argv[i]));
                }
            } else {
                extras.push_back(argv[i]);
            }
        }

        if (!targetStr.empty()) {
            std::transform(targetStr.begin(), targetStr.end(), targetStr.begin(), ::tolower);
            if (targetStr == "scan") target = CompilerAction::SCAN;
            else if (targetStr == "parse") target = CompilerAction::PARSE;
            else if (targetStr == "inter") target = CompilerAction::INTER;
            else if (targetStr == "assembly") target = CompilerAction::ASSEMBLY;
            else {
                printUsage("Invalid target: " + targetStr);
            }
        }

        // grab infile
        for (auto& extra : extras) {
            if (extra[0] != '-') {
                infile = extra;
                break;
            }
        }
    }
}
