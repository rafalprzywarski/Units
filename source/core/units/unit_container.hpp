#pragma once
#include <units/math.hpp>

namespace units
{

template <typename unit_type>
class unit_container
{
public:
    bool put(const unit_type& u)
    {
        if (collides_with(u.position, u.radius))
            return false;

        units.push_back(u);
        return true;
    }
    bool collides_with(const vec2f& position, float radius) const
    {
        for (auto& u : units)
            if (unit_collides_with(u, position, radius))
                return true;
        return false;
    }
private:
    std::vector<unit_type> units;

    bool unit_collides_with(const unit_type& u, const vec2f& position, float radius) const
    {
        return length_squared(position - u.position) < (radius + u.radius) * (radius + u.radius);
    }
};

}
