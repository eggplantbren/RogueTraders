#ifndef RogueTraders_Options_hpp
#define RogueTraders_Options_hpp

#include <vector>

namespace RogueTraders
{

// 2, 2, and {1.0, 1.0} will make it an Edgeworth box
constexpr int num_goods = 2;
constexpr int num_people = 2;
const std::vector<double> total_supplies = {1.0, 1.0};

} // namespace

#endif

