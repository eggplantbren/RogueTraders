#ifndef RogueTraders_Inputs_hpp
#define RogueTraders_Inputs_hpp

#include <iostream>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

namespace RogueTraders
{

// Forward declaration
class Population;

class Inputs
{
    public:

        // Stuff about goods
        int num_goods;
        std::vector<double> total_supplies;

        // People's preferences parameterised by Cobb-Douglas powers
        int num_people;
        std::vector<std::vector<double>> powers;

    public:
        // Basic constructor and loader
        Inputs();

        // A static instance which is const
        static const Inputs instance;
};

/* IMPLEMENTATIONS FOLLOW */

const Inputs Inputs::instance;

Inputs::Inputs()
{
    const std::string filename = "inputs.yaml";

    // Powers for preferences. Load from YAML file
    YAML::Node yaml;
    try
    {
        yaml = YAML::LoadFile(filename);
    }
    catch(...)
    {
        std::cerr << "Error reading or parsing file " << filename << '.';
        std::cerr << std::endl;
        return;
    }

    // Read data.
    num_goods = yaml["num_goods"].as<int>();
    total_supplies = yaml["total_supplies"].as<std::vector<double>>();
    num_people = yaml["num_people"].as<int>();

    // Consistency check
    assert(num_people == (int)yaml["people"].size());

    for(int i=0; i<num_people; ++i)
    {
        const auto& person = yaml["people"][i];
        powers.emplace_back(person["powers"].as<std::vector<double>>());

        // Consistency check
        assert((int)powers.back().size() == num_goods);
    }
}

} // namespace

#endif

