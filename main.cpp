#include <iostream>
#include "RogueTraders/Population.hpp"

using namespace RogueTraders;

int main()
{
    DNest4::RNG rng(time(0));
    std::cout << std::setprecision(12);

    constexpr int reps = 1000;
    constexpr int steps = 100000;

    for(int rep=0; rep<reps; ++rep)
    {
        Population pop(rng);

        for(int i=0; i<steps; ++i)
            pop.try_trade(rng);

        // Output assets of person 0
        const auto& qs = pop.get_quantities();
        for(int good=0; good < Inputs::instance.num_goods; ++good)
            std::cout << qs[good][0] << ' ';
        std::cout << std::endl;
    }

    return 0;
}

