#include <units/unit.hpp>
#include <units/formations.hpp>
#include <units/gui/window.hpp>
#include <memory>

struct walking_unit
{
    float radius = 0;
    units::vec2f position;
    units::vec2f target_position;

    walking_unit() = default;
    walking_unit(float radius, units::vec2f position) : radius(radius), position(position), target_position(position) { }
};

using walking_units = std::vector<walking_unit>;

units::units create_units_grid(units::vec2f origin, units::vec2i size, float radius, float spacing)
{
    units::units group;
    for (int i = 0; i < size[0]; ++i)
        for (int j = 0; j < size[1]; ++j)
            group.push_back(units::unit{
                radius,
                origin +
                units::vec2f{
                    spacing + radius + i * (radius * 2 + spacing),
                    spacing + radius + j * (radius * 2 + spacing)
                }});
    return group;
}

bool collides(const units::unit& u, const units::units& group)
{
    for (auto& other : group)
        if (&u != &other && cml::length(u.position - other.position) < (u.radius + other.radius))
            return true;
    return false;
}

void update_units(units::units& group)
{
    for (auto& u : group)
    {
        auto old_position = u.position;
        auto direction = u.target_position - u.position;
        if (length(direction) == 0)
            continue;
        u.position += normalize(direction) * (2 + std::rand() % 10 * 0.2f);
        if (!collides(u, group))
            continue;

        u.position = old_position;

        u.position[0] += std::rand() % 5 - 2;
        u.position[1] += std::rand() % 5 - 2;
        if (collides(u, group))
            u.position = old_position;
    }
}

void set_units_target(units::units& group, units::vec2f target)
{
    for (auto& u : group)
        u.target_position = target;
}

int main()
{
    auto window = units::gui::create_window();
    auto group = create_units_grid({5, 5}, {30, 30}, 5, 7);

    units::gui::main_loop(*window, [&](float world_x, float world_y)
    {
        units::box_formation formation{{1, 0}, {world_x, world_y}, 1000, 25};
        assign_formation(formation, group);
    }, [&] { update_units(group); }, [&] { units::gui::draw_units(group, *window); } );
}
