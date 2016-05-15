#pragma once
#include <ams/math.hpp>
#include <vector>

namespace ams
{

enum class walking_unit_state
{
    standing,
    walking
};

struct walking_unit
{
    struct model
    {
        float max_speed = 1.5;
        float radius = 1;
        float foot_distance = 0.25;
        unsigned max_ticks = 30;
        model(float radius, float foot_distance) : radius(radius), foot_distance(foot_distance) { }
    } model_;
    struct physical_state
    {
        ams::vec2f feet_positions[2];
        float speed{};
        physical_state(vec2f left, vec2f right) : feet_positions{left, right} {}
    } physical;
    struct logical_state
    {
        walking_unit_state state = walking_unit_state::standing;
        unsigned ticks = 0;
        ams::vec2f target_position;
        unsigned current_foot = 0;
        logical_state(ams::vec2f target_position) : target_position(target_position) { }
    } logical;

    walking_unit() = default;
    walking_unit(model model, ams::vec2f position) : model_(model), physical{position - ams::vec2f{model.foot_distance, 0}, position + ams::vec2f{model.foot_distance, 0}}, logical(position) { }
};

using walking_units = std::vector<walking_unit>;

inline vec2f get_position(const walking_unit& u)
{
    return (u.physical.feet_positions[0] + u.physical.feet_positions[1]) / 2;
}

inline bool has_reached_targed(const walking_unit& u)
{
    return length(u.logical.target_position - get_position(u)) < 0.005;
}

inline vec2f get_foot_target_position(walking_unit& u)
{
    auto sign = float(u.logical.current_foot) * 2 - 1;
    auto direction = normalize(u.logical.target_position - get_position(u));
    auto direction_normal = ams::cross_product(direction);
    return u.logical.target_position - sign * u.model_.foot_distance * direction_normal;
}

inline void start_first_step(walking_unit& u)
{
    u.logical.current_foot = 1;
    u.logical.ticks = u.model_.max_ticks;
    u.physical.speed = u.model_.max_speed / 2;
}

inline void start_next_step(walking_unit& u)
{
    u.logical.ticks = u.model_.max_ticks;
    u.physical.speed = u.model_.max_speed;
    u.logical.current_foot = 1 - u.logical.current_foot;
}

inline void step_tick(walking_unit& u)
{
    u.logical.ticks--;

    ams::vec2f current_foot_target = get_foot_target_position(u);
    ams::vec2f direction = current_foot_target - u.physical.feet_positions[u.logical.current_foot];
    float distance = length(direction);
    if (u.physical.speed > distance)
        u.physical.feet_positions[u.logical.current_foot] = current_foot_target;
    else
        u.physical.feet_positions[u.logical.current_foot] += direction * u.physical.speed / distance;
}

inline walking_unit_state standing_tick(walking_unit& u)
{
    return walking_unit_state::standing;
}

inline walking_unit_state walking_tick(walking_unit& u)
{
    step_tick(u);

    if (has_reached_targed(u))
        return walking_unit_state::standing;

    if (u.logical.ticks == 0)
        start_next_step(u);

    return walking_unit_state::walking;
}

inline walking_unit_state walking_set_target_position(walking_unit& u, ams::vec2f target_position)
{
    u.logical.target_position = target_position;

    return walking_unit_state::walking;
}

inline walking_unit_state standing_set_target_position(walking_unit& u, ams::vec2f target_position)
{
    u.logical.target_position = target_position;

    if (has_reached_targed(u))
        return walking_unit_state::standing;

    start_first_step(u);

    return walking_unit_state::walking;
}

inline void set_target_position(walking_unit& u, ams::vec2f target_position)
{
    if (u.logical.state == walking_unit_state::walking)
        u.logical.state = walking_set_target_position(u, target_position);
    else
        u.logical.state = standing_set_target_position(u, target_position);
}

inline void tick(walking_unit& u)
{
    if (u.logical.state == walking_unit_state::walking)
        u.logical.state =  walking_tick(u);
    else
        u.logical.state =  standing_tick(u);
}

inline void update_unit(walking_unit& u)
{
    tick(u);
}

}
