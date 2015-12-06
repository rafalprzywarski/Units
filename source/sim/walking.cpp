#include <ams/unit.hpp>
#include <ams/formations.hpp>
#include <ams/gui/window.hpp>
#include <memory>

struct walking_unit
{
    float radius = 0;
    float foot_distance = 0;
    float speed = 1;
    ams::vec2f feet_positions[2];
    ams::vec2f current_foot_target;
    ams::vec2f target_position;
    bool is_walking = false;
    unsigned current_foot = 0;
    unsigned ticks = 0;

    walking_unit() = default;
    walking_unit(float radius, float foot_distance, ams::vec2f position) : radius(radius), foot_distance(foot_distance), feet_positions{position - ams::vec2f{foot_distance, 0}, position + ams::vec2f{foot_distance, 0}}, target_position(position) { }

    ams::vec2f get_position() const { return 0.5f * (feet_positions[0] + feet_positions[1]); }
};

using walking_units = std::vector<walking_unit>;

bool has_reached_targed(walking_unit& u)
{
    return length(u.target_position - u.get_position()) < 0.5;
}

ams::vec2f get_foot_target_position(walking_unit& u)
{
    auto sign = float(u.current_foot) * 2 - 1;
    auto direction = normalize(u.target_position - u.get_position());
    auto direction_normal = ams::cross_product(direction);
    return u.target_position + sign * u.foot_distance * direction_normal;
}

void update_unit(walking_unit& u)
{
    if (has_reached_targed(u))
    {
        u.is_walking = false;
        return;
    }

    if (!u.is_walking)
    {
        u.is_walking = true;
        u.current_foot = 0;
        u.ticks = 20;
    }

    if (u.ticks == 0)
    {
        u.ticks = 40;
        u.current_foot = 1 - u.current_foot;
    }

    u.ticks--;

    ams::vec2f direction = get_foot_target_position(u) - u.feet_positions[u.current_foot];
    float distance = length(direction);
    u.feet_positions[u.current_foot] += direction * std::min(1.0f, u.speed / distance);
}

void set_target_position(walking_unit& u, ams::vec2f target_position)
{
    u.target_position = target_position;
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
        draw_circle(shape, u.get_position(), u.radius, window);
        float foot_radius = FOOT_RATIO * u.radius;
        shape.setOutlineColor((u.is_walking && u.current_foot == 0) ? SELECTED_COLOR : COLOR);
        draw_circle(shape, u.feet_positions[0], foot_radius, window);
        shape.setOutlineColor((u.is_walking && u.current_foot == 1) ? SELECTED_COLOR : COLOR);
        draw_circle(shape, u.feet_positions[1], foot_radius, window);
    }
}

int main()
{
    auto window = ams::gui::create_window();
    walking_unit unit{30, 12, {0, 0}};

    ams::gui::main_loop(*window, [&](float world_x, float world_y)
    {
        set_target_position(unit, {world_x, world_y});
    }, [&] { update_unit(unit); }, [&] { draw_walking_units(walking_units{unit}, *window); } );
}
