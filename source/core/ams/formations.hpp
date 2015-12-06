#pragma once
#include <ams/unit.hpp>
#include <algorithm>

namespace ams
{

/**
             /|\
 (spacing)    | (direction)
   <-->       |
U  U  U  U (origin)  U  U  U  U
U  U  U  U  U  U  U  U  U  U  U
U  U  U  U  U  U  U  U  U  U  U
U  U  U  U  U  U  U  U  U  U  U
<-----------(width)----------->
*/
struct box_formation
{
    vec2f direction;
    vec2f origin;
    float width;
    float spacing;
};

void assign_formation(const box_formation& formation, units& group)
{
    auto down = -normalize(formation.direction) * formation.spacing;
    auto right = vec2f{-down[1], down[0]};
    int w_count = formation.width / formation.spacing;
    auto lu_corner = formation.origin - right * w_count * 0.5f;
    std::vector<bool> selected(group.size(), false);
    for (int y = 0; std::count(selected.begin(), selected.end(), false) > 0; ++y)
    {
        for (int x = 0; x != w_count; ++x)
        {
            auto goal = lu_corner + down * y + right * x;
            float distance = std::numeric_limits<float>::max();
            std::size_t found = 0;
            for (std::size_t i = 0; i != group.size(); ++i)
                if (not selected[i])
                {
                    float distance2 = length_squared(group[i].position - goal);
                    if (distance2 >= distance)
                        continue;
                    distance = distance2;
                    found = i;
                }
            if (distance == std::numeric_limits<float>::max())
                return;
            group[found].target_position = goal;
            selected[found] = true;
        }
    }
}

}
