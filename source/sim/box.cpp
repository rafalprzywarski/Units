#include <ams/unit.hpp>
#include <ams/formations.hpp>
#include <ams/gui/window.hpp>
#include <memory>

ams::units create_units_grid(ams::vec2f origin, ams::vec2i size, float radius, float spacing)
{
    ams::units group;
    for (int i = 0; i < size[0]; ++i)
        for (int j = 0; j < size[1]; ++j)
            group.push_back(ams::unit{
                radius,
                origin + ams::vec2f{i, j} * (radius * 2 + spacing) + ams::vec2f{1, 1} * (spacing + radius)});
    return group;
}

bool collides(const ams::unit& u, const ams::units& group)
{
    for (auto& other : group)
        if (&u != &other && cml::length(u.position - other.position) < (u.radius + other.radius))
            return true;
    return false;
}

void update_units(ams::units& group)
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

void set_units_target(ams::units& group, ams::vec2f target)
{
    for (auto& u : group)
        u.target_position = target;
}

int main()
{
    auto window = ams::gui::create_window();
    auto group = create_units_grid({5, 5}, {30, 30}, 5, 7);
    ams::gui::main_loop(*window, [&](float world_x, float world_y)
    {
        ams::box_formation formation{{1, 0}, {world_x, world_y}, 1000, 25};
        assign_formation(formation, group);
    }, [&] { update_units(group); }, [&] { ams::gui::draw_units(group, *window); } );
}
