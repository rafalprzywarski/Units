#include <ams/unit.hpp>
#include <ams/formations.hpp>
#include <ams/gui/window.hpp>
#include "walking_unit.hpp"
#include <memory>

namespace ams
{

void draw_circle(sf::CircleShape shape, ams::vec2f pos, float radius, sf::RenderWindow& window)
{
    shape.setRadius(radius);
    shape.setPosition(pos[0] - radius, pos[1] - radius);
    window.draw(shape);
}

template <typename unit_container>
void draw_walking_units(const unit_container& group, sf::RenderWindow& window)
{
    auto COLOR = sf::Color(150, 250, 100);
    auto SELECTED_COLOR = sf::Color(250, 150, 100);
    const auto FOOT_RATIO = 0.2f;
    sf::CircleShape shape(1, 16);
    shape.setPointCount(32);
    shape.setFillColor(sf::Color(0, 0, 0, 0));
    shape.setOutlineThickness(2);
    shape.setOutlineColor(COLOR);

    for (auto& u : group)
    {
        draw_circle(shape, get_position(u), u.radius, window);
        float foot_radius = FOOT_RATIO * u.radius;
        shape.setOutlineColor((u.is_walking && u.current_foot == 0) ? SELECTED_COLOR : COLOR);
        draw_circle(shape, u.feet_positions[0], foot_radius, window);
        shape.setOutlineColor((u.is_walking && u.current_foot == 1) ? SELECTED_COLOR : COLOR);
        draw_circle(shape, u.feet_positions[1], foot_radius, window);
    }
}

}

int main()
{
    auto window = ams::gui::create_window();
    ams::walking_unit unit{30, 12, {0, 0}};

    ams::gui::main_loop(*window, [&](float world_x, float world_y)
    {
        set_target_position(unit, {world_x, world_y});
    }, [&] { update_unit(unit); }, [&] { ams::draw_walking_units(ams::walking_units{unit}, *window); } );
}
