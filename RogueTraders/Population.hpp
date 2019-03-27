#ifndef RogueTraders_Population_hpp
#define RogueTraders_Population_hpp

#include <cassert>
#include <iomanip>
#include <iostream>
#include "Options.hpp"
#include <ostream>
#include <DNest4/code/RNG.h>
#include <yaml-cpp/yaml.h>
#include <vector>

namespace RogueTraders
{

/* The state of a population. */
class Population
{
    private:

        // { Quantity of good i owned by person j }
        std::vector<std::vector<double>> quantities;

        // Utility values of each person
        std::vector<double> person_utilities;

        // Compute one utility
        void compute_utility(int person);

        // Compute the utilities
        void compute_utilities();

    public:

        // Constructor. Uses an RNG to generate an initial allocation
        Population(DNest4::RNG& rng);

        // Try to do a trade. Accept if both parties are better off.
        // Returns true if it was accepted.
        bool try_trade(DNest4::RNG& rng);

        // Getters
        const std::vector<std::vector<double>>& get_quantities() const;
        const std::vector<double>& get_person_utilities() const;
};


// Output operator
std::ostream& operator << (std::ostream& out, const Population& pop);


/* IMPLEMENTATIONS FOLLOW */

Population::Population(DNest4::RNG& rng)
:quantities(num_goods, std::vector<double>(num_people))
,person_utilities(num_people)
{
    for(int i=0; i<num_goods; ++i)
    {
        // Generate from an exponential, accumulate total
        double tot = 0.0;
        for(int j=0; j<num_people; ++j)
        {
            quantities[i][j] = -log(1.0 - rng.rand());
            tot += quantities[i][j];
        }

        // Factor for normalisation
        double factor = total_supplies[i]/tot;
        for(int j=0; j<num_people; ++j)
            quantities[i][j] *= factor;
    }

    compute_utilities();
}

void Population::compute_utility(int person)
{
    assert(person >= 0 && person < num_people);
    person_utilities[person] = 0.0;

    // Powers for preferences. Load from YAML file preferences.yaml
    static std::vector<std::vector<double>> powers;
    if(powers.size() == 0)
    {

        YAML::Node yaml;
        try
        {
            yaml = YAML::LoadFile("preferences.yaml");
        }
        catch(...)
        {
            std::cerr << "Error reading or parsing preferences.yaml.";
            std::cerr << std::endl;
        }

        // Read preference parameters from the yaml node.
        int num_people = yaml["people"].size();
        for(int i=0; i<num_people; ++i)
        {
            const auto& person = yaml["people"][i];
            powers.emplace_back(person["powers"].as<std::vector<double>>());
        }
    }


    for(int i=0; i<num_goods; ++i)
        person_utilities[person] += pow(quantities[i][person],
                                        powers[person][i]);
}

void Population::compute_utilities()
{
    for(int i=0; i<num_people; ++i)
        compute_utility(i);
}

const std::vector<std::vector<double>>& Population::get_quantities() const
{
    return quantities;
}

const std::vector<double>& Population::get_person_utilities() const
{
    return person_utilities;
}

bool Population::try_trade(DNest4::RNG& rng)
{
    // Choose two people to trade between
    int person1 = rng.rand_int(num_people);
    int person2;
    do
    {
        person2 = rng.rand_int(num_people);
    }while(person2 == person1);

    // Which goods are they trading?
    int good1 = rng.rand_int(num_goods);
    int good2 = rng.rand_int(num_goods);

    // Backup utilities and quantities
    double utility1 = person_utilities[person1];
    double utility2 = person_utilities[person2];
    double quantity11 = quantities[good1][person1];
    double quantity12 = quantities[good1][person2];
    double quantity21 = quantities[good2][person1];
    double quantity22 = quantities[good2][person2];

    // How much of each good is being given up?
    double trade1 = quantity11*pow(rng.rand(), 10.0*rng.rand());
    double trade2 = quantity22*pow(rng.rand(), 10.0*rng.rand());

    // Do the trade.
    // Loss
    quantities[good1][person1] -= trade1;
    quantities[good2][person2] -= trade2;

    // Gain
    quantities[good2][person1] += trade2;
    quantities[good1][person2] += trade1;

    // Updated utilities
    compute_utility(person1);
    compute_utility(person2);

    // Accept or reject the trade
    bool accepted = true;
/*
    std::cout << utility1 << ' ' << person_utilities[person1] << " ";
    std::cout << (person_utilities[person1] > utility1) << "    ";
    std::cout << utility2 << ' ' << person_utilities[person2] << " ";
    std::cout << (person_utilities[person2] > utility2) << "    ";
    std::cout << std::endl;
*/
    if((person_utilities[person1] < utility1) ||
       (person_utilities[person2] < utility2))
    {
        // Reject the trade
        person_utilities[person1] = utility1;
        person_utilities[person2] = utility2;
        quantities[good1][person1] = quantity11;
        quantities[good1][person2] = quantity12;
        quantities[good2][person1] = quantity21;
        quantities[good2][person2] = quantity22;
        accepted = false;
    }

    return accepted;
}

std::ostream& operator << (std::ostream& out, const Population& pop)
{
    const auto& quantities = pop.get_quantities();
    const auto& person_utilities = pop.get_person_utilities();

    out << std::setprecision(12);

    out << "quantities:\n";
    for(int i=0; i<num_goods; ++i)
        for(int j=0; j<num_people; ++j)
            out << "    - " << quantities[i][j] << '\n';
    out << "utilities:\n";
    for(int j=0; j<num_people; ++j)
        out << "    - " << person_utilities[j] << '\n';

    return out;
}

} // namespace

#endif

