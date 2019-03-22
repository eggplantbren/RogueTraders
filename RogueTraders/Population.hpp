#ifndef RogueTraders_Population_hpp
#define RogueTraders_Population_hpp

#include <iomanip>
#include "Options.hpp"
#include <ostream>
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

        // Getter
        const std::vector<std::vector<double>>& get_quantities() const;
};


// Output operator
std::ostream& operator << (std::ostream& out, const Population& pop);



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

const std::vector<std::vector<double>>& Population::get_quantities() const
{
    return quantities;
}


std::ostream& operator << (std::ostream& out, const Population& pop)
{
    const auto& quantities = pop.get_quantities();

    out << std::setprecision(8);
    for(int i=0; i<num_goods; ++i)
    {
        for(int j=0; j<num_people; ++j)
           out << std::setw(11) << quantities[i][j] << ' ';
        out << '\n';
    }

    return out;
}

} // namespace

#endif

