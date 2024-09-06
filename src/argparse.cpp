#include <iostream>

#include <cxxopts.hpp>

#include "argparse.h"

std::optional<ArgValues> parse_args(int argc, char *argv[]) {
    cxxopts::Options options(
        "MapGenerator",
        "Simple map generator using WFC algorithms to create a simple map .txt file.");

    options.add_options()
        ("f,filename", "Output map file name.", cxxopts::value<std::string>())
        ("s,size", "Map size. Options are: 'small', 'medium', 'large'. Default: 'small'.", cxxopts::value<std::string>())
        ("c,count", "How many maps should be created. Default: 1.", cxxopts::value<int>())
        ("v,verbose", "Perform all logging. Default: off.", cxxopts::value<bool>()->default_value("false"))
        // ("input_files", "Input file(s) to concatenate", cxxopts::value<std::vector<std::string>>());
        ("h,help", "Print usage.");

    cxxopts::ParseResult result;
    result = options.parse(argc, argv);

    if(result.count("help")) {
        std::cout << options.help() << std::endl;
        return std::nullopt;
    }

    ArgValues argValues = {};

    std::string mapNamePrefix;
    if(result.count("filename")) {
        argValues.mapNamePrefix = result["filename"].as<std::string>();
    }

    argValues.mapSize = MapSize::MapSize_Small;
    if(result.count("size")) {
        std::string sizeString = result["size"].as<std::string>();
        auto itr = MapDefinitions::stringSizeMappings.find(sizeString);
        if(itr != MapDefinitions::stringSizeMappings.end()) {
            argValues.mapSize = itr->second;
        }
    }

    if(result.count("count")) {
        argValues.mapCount = result["count"].as<int>();
    }

    if(result.count("verbose")) {
        argValues.verboseLogging = result["verbose"].as<bool>();
    }

    return argValues;
}