#include <iostream>
#include "RogueTraders/Population.hpp"

int main()
{
    DNest4::RNG rng(time(0));
    RogueTraders::Population pop(rng);

    std::cout << pop << std::endl;
    for(int i=0; i<1000000; ++i)
        pop.try_trade(rng);
    std::cout << pop << std::endl;

    return 0;
}

