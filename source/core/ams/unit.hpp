#pragma once
#include <ams/math.hpp>
#include <vector>

namespace ams
{

struct unit
{
    float radius;
    vec2f position;
    vec2f target_position;

    unit() { }
    unit(float radius, vec2f position) : radius(radius), position(position), target_position(position) { }
};

using units = std::vector<unit>;

}
