#pragma once
#include <ams/math.hpp>
#include <vector>

namespace ams
{

struct walking_unit
{
    float radius = 0;
    float foot_distance = 0;
    float max_speed = 1.5;
    float speed = max_speed;
    ams::vec2f feet_positions[2];
    ams::vec2f current_foot_target;
    ams::vec2f target_position;
    bool is_walking = false;
    unsigned current_foot = 0;
    unsigned max_ticks = 30;
    unsigned ticks = 0;

    walking_unit() = default;
    walking_unit(float radius, float foot_distance, ams::vec2f position) : radius(radius), foot_distance(foot_distance), feet_positions{position - ams::vec2f{foot_distance, 0}, position + ams::vec2f{foot_distance, 0}}, target_position(position) { }
};

using walking_units = std::vector<walking_unit>;

inline void set_target_position(walking_unit& u, ams::vec2f target_position)
{
    u.target_position = target_position;
}

inline vec2f get_position(const walking_unit& u)
{
    return (u.feet_positions[0] + u.feet_positions[1]) / 2;
}

inline bool has_reached_targed(const walking_unit& u)
{
    return length(u.target_position - get_position(u)) < 0.005;
}

inline vec2f get_foot_target_position(walking_unit& u)
{
    auto sign = float(u.current_foot) * 2 - 1;
    auto direction = normalize(u.target_position - get_position(u));
    auto direction_normal = ams::cross_product(direction);
    return u.target_position - sign * u.foot_distance * direction_normal;
}

inline void update_unit(walking_unit& u)
{
    if (has_reached_targed(u))
    {
        u.is_walking = false;
        return;
    }

    if (!u.is_walking)
    {
        u.is_walking = true;
        u.current_foot = 1;
        u.ticks = u.max_ticks / 2;
        u.speed = u.max_speed;
        u.current_foot_target = get_foot_target_position(u);
    }

    if (u.ticks == 0)
    {
        u.ticks = u.max_ticks;
        u.speed = u.max_speed;
        u.current_foot = 1 - u.current_foot;
        u.current_foot_target = get_foot_target_position(u);
    }

    u.ticks--;

    ams::vec2f direction = u.current_foot_target - u.feet_positions[u.current_foot];
    float distance = length(direction);
    if (u.speed > distance)
    {
        u.ticks = 0;
        u.feet_positions[u.current_foot] = u.current_foot_target;
    }
    else
    {
        u.feet_positions[u.current_foot] += direction * u.speed / distance;
    }
}

}
