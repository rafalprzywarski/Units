#pragma once
#include <units/math.hpp>

namespace units
{

template <typename unit_type>
class unit_container
{
public:
    void put(const unit_type& u)
    {
        this->u = u;
    }
    bool collides_with(const vec2f& position, float radius) const
    {
        return length_squared(position - u.position) < (radius + u.radius) * (radius + u.radius);
    }
private:
    unit_type u;
};

}
