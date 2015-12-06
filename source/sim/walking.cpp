#include <ams/unit.hpp>
#include <ams/formations.hpp>
#include <ams/gui/window.hpp>
#include <memory>

struct walking_unit
{
    float radius = 0;
    float feet_spacing = 0;
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
    u.position += direction * std::min(1.0f, (2 + std::rand() % 10 * 0.2f) / distance);
}

template <typename unit_container>
void draw_walking_units(const unit_container& group, sf::RenderWindow& window)
{
    const auto FOOT_RATIO = 0.2f;
    sf::CircleShape shape(1, 16);
    shape.setPointCount(32);
    shape.setFillColor(sf::Color(0, 0, 0, 0));
    float thickness = 2;
    shape.setOutlineThickness(thickness);
    shape.setOutlineColor(sf::Color(150, 250, 100));

    for (auto& u : group)
    {
        shape.setRadius(u.radius);
        shape.setPosition(u.position[0] - u.radius, u.position[1] - u.radius);
        window.draw(shape);
        float foot_radius = FOOT_RATIO * u.radius;
        shape.setRadius(foot_radius);
        auto left_foot = ams::cross_product(u.direction) * (u.feet_spacing * 0.5f);
        auto right_foot = -left_foot;
        shape.setPosition(u.position[0] + left_foot[0] - foot_radius, u.position[1] + left_foot[1] - foot_radius);
        window.draw(shape);
        shape.setPosition(u.position[0] + right_foot[0] - foot_radius, u.position[1] + right_foot[1] - foot_radius);
        window.draw(shape);
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
