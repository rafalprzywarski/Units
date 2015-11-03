#pragma once
#include <units/math.hpp>
#include <vector>

namespace units
{

struct unit
{
    float radius;
    vec2f position;
};

using units = std::vector<unit>;

}
