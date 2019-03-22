#include <iostream>
#include "RogueTraders/Population.hpp"

int main()
{
    DNest4::RNG rng(time(0));
    RogueTraders::Population pop(rng);

    return 0;
}

