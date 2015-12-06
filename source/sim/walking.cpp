#include <ams/unit.hpp>
#include <ams/formations.hpp>
#include <ams/gui/window.hpp>
#include <memory>

struct walking_unit
{
    float radius = 0;
    float feet_spacing = 0;
    float speed = 3;
    ams::vec2f position;
    ams::vec2f target_position;
    ams::vec2f direction{0, 1};

    walking_unit() = default;
    walking_unit(float radius, float feet_spacing, ams::vec2f position) : radius(radius), feet_spacing(feet_spacing), position(position), target_position(position) { }
};

using walking_units = std::vector<walking_unit>;

void update_unit(walking_unit& u)
{
    auto direction = u.target_position - u.position;
    auto distance = length(direction);
    if (distance < 0.5)
        return;
    u.direction = direction / distance;
    u.position += direction * std::min(1.0f, u.speed / distance);
}

void draw_circle(sf::CircleShape shape, ams::vec2f pos, float radius, sf::RenderWindow& window)
{
    shape.setRadius(radius);
    shape.setPosition(pos[0] - radius, pos[1] - radius);
    window.draw(shape);
}

template <typename unit_container>
void draw_walking_units(const unit_container& group, sf::RenderWindow& window)
{
    const auto FOOT_RATIO = 0.2f;
    sf::CircleShape shape(1, 16);
    shape.setPointCount(32);
    shape.setFillColor(sf::Color(0, 0, 0, 0));
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color(150, 250, 100));

    for (auto& u : group)
    {
        draw_circle(shape, u.position, u.radius, window);
        float foot_radius = FOOT_RATIO * u.radius;
        auto left_foot = ams::cross_product(u.direction) * (u.feet_spacing * 0.5f);
        auto right_foot = -left_foot;
        draw_circle(shape, u.position + left_foot, foot_radius, window);
        draw_circle(shape, u.position + right_foot, foot_radius, window);
    }
}

int main()
{
    auto window = ams::gui::create_window();
    walking_unit unit{30, 23, {0, 0}};

    ams::gui::main_loop(*window, [&](float world_x, float world_y)
    {
        unit.target_position = {world_x, world_y};
    }, [&] { update_unit(unit); }, [&] { draw_walking_units(walking_units{unit}, *window); } );
}
