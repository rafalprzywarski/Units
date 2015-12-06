#include <ams/unit.hpp>
#include <ams/formations.hpp>
#include <ams/gui/window.hpp>
#include <memory>

struct walking_unit
{
    float radius = 0;
    ams::vec2f position;
    ams::vec2f target_position;

    walking_unit() = default;
    walking_unit(float radius, ams::vec2f position) : radius(radius), position(position), target_position(position) { }
};

using walking_units = std::vector<walking_unit>;

void update_unit(walking_unit& u)
{
    auto direction = u.target_position - u.position;
    auto distance = length(direction);
    if (distance < 0.5)
        return;
    u.position += direction * std::min(1.0f, (2 + std::rand() % 10 * 0.2f) / distance);
}

int main()
{
    auto window = ams::gui::create_window();
    walking_unit unit{10, {0, 0}};

    ams::gui::main_loop(*window, [&](float world_x, float world_y)
    {
        unit.target_position = {world_x, world_y};
    }, [&] { update_unit(unit); }, [&] { ams::gui::draw_units(walking_units{unit}, *window); } );
}
