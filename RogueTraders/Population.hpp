#ifndef RogueTraders_Population_hpp
#define RogueTraders_Population_hpp

#include "Options.hpp"
#include <DNest4/code/RNG.h>
#include <vector>

namespace RogueTraders
{

/* The state of a population. */
class Population
{
    private:

        // { Quantity of good i owned by person j }
        std::vector<std::vector<double>> quantities;

    public:

        // Constructor. Specify the dimensions.
        Population(DNest4::RNG& rng);
};


/* IMPLEMENTATIONS FOLLOW */

Population::Population(DNest4::RNG& rng)
:quantities(num_goods, std::vector<double>(num_people))
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
}



} // namespace

#endif

