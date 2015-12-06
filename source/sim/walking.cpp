#include <ams/unit.hpp>
#include <ams/formations.hpp>
#include <ams/gui/window.hpp>
#include <memory>

enum class walking_unit_state
{
    STANDING, MOVING_LEFT_LEG, MOVING_RIGHT_LEG
};

struct walking_unit
{
    float radius = 0;
    float feet_spacing = 0;
    float speed = 3;
    ams::vec2f left_foot_position, right_foot_position;
    ams::vec2f target_position;
    walking_unit_state state = walking_unit_state::STANDING;

    walking_unit() = default;
    walking_unit(float radius, float feet_spacing, ams::vec2f position) : radius(radius), feet_spacing(feet_spacing), left_foot_position(position - ams::vec2f{feet_spacing * 0.5f, 0}), right_foot_position(position + ams::vec2f{feet_spacing * 0.5f, 0}), target_position(position) { }

    ams::vec2f get_position() const { return 0.5f * (left_foot_position + right_foot_position); }
};

using walking_units = std::vector<walking_unit>;

bool has_reached_targed(walking_unit& u)
{
    return length(u.target_position - u.get_position()) < 0.5;
}

void update_unit(walking_unit& u)
{
    if (has_reached_targed(u))
    {
        u.state = walking_unit_state::STANDING;
        return;
    }
    ams::vec2f direction = u.target_position - u.get_position();
    auto distance = length(direction);
    ams::vec2f position = u.get_position() + direction * std::min(1.0f, u.speed / distance);
    direction = direction / distance;
    u.left_foot_position = position + ams::cross_product(direction) * u.feet_spacing * 0.5f;
    u.right_foot_position = position - ams::cross_product(direction) * u.feet_spacing * 0.5f;
    u.state = walking_unit_state::MOVING_RIGHT_LEG;
}

void set_target_position(walking_unit& u, ams::vec2f target_position)
{
    if (u.state == walking_unit_state::STANDING)
    {
        u.target_position = target_position;
    }
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
        draw_circle(shape, u.get_position(), u.radius, window);
        float foot_radius = FOOT_RATIO * u.radius;
        draw_circle(shape, u.left_foot_position, foot_radius, window);
        draw_circle(shape, u.right_foot_position, foot_radius, window);
    }
}

int main()
{
    auto window = ams::gui::create_window();
    walking_unit unit{30, 23, {0, 0}};

    ams::gui::main_loop(*window, [&](float world_x, float world_y)
    {
        set_target_position(unit, {world_x, world_y});
    }, [&] { update_unit(unit); }, [&] { draw_walking_units(walking_units{unit}, *window); } );
}
