#include <iostream>
#include "RogueTraders/Population.hpp"

int main()
{
    DNest4::RNG rng(time(0));
    RogueTraders::Population pop(rng);
    std::cout << std::setprecision(12);

    std::cout << pop << std::endl;

    for(int i=0; i<1000000; ++i)
    {
        pop.try_trade(rng);

/*
        const auto& qs = pop.get_quantities();
        if(i%10000 == 0)
        {
            for(int a=0; a < 3; ++a)
            {
                for(int b=0; b < 3; ++b)
                    std::cout << qs[a][b] << ' ';
            }
            std::cout << std::endl;
        }
*/
    }
    std::cout << pop << std::endl;


    return 0;
}

