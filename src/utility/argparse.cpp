#include <iostream>

#include <cxxopts.hpp>

#include "argparse.h"


std::optional<ArgValues> ParseArgs(int argc, char *argv[]) {
    cxxopts::Options options(
        "MapGenerator",
        "Simple map generator using WFC algorithms to create a simple map .txt file.");

    options.add_options()
        ("f,filename", "Output map file name.",
            cxxopts::value<std::string>())
        ("s,size", "Map size. Options are: 'small', 'medium', 'large'. Default: 'small'.",
            cxxopts::value<std::string>()->default_value("small"))
        ("c,count", "How many maps should be created. Default: 1.",
            cxxopts::value<int>()->default_value("1"))
        ("seed", "Override seed. All maps will use this same seed if --count > 1.",
            cxxopts::value<int>())
        ("v,verbose", "Perform all logging. Default: off.",
            cxxopts::value<bool>()->default_value("false"))
        ("h,help", "Print usage.");

    cxxopts::ParseResult result;
    result = options.parse(argc, argv);

    if(result.count("help")) {
        std::cout << options.help() << std::endl;
        return std::nullopt;
    }

    ArgValues argValues = {
        .mapNamePrefix = "",
        .mapSize = MapSize::MapSize_Small,
        .mapCount = 1,
        .seed = -1,
        .verboseLogging = false};

    if(result.count("filename")) {
        argValues.mapNamePrefix = result["filename"].as<std::string>();
    }

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

    if(result.count("seed")) {
        argValues.seed = result["seed"].as<int>();
    }

    if(result.count("verbose")) {
        argValues.verboseLogging = result["verbose"].as<bool>();
    }

    return argValues;
}
