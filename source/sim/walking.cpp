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
    float foot_distance = 0;
    float speed = 1;
    ams::vec2f left_foot_position, right_foot_position;
    ams::vec2f target_position;
    walking_unit_state state = walking_unit_state::STANDING;
    unsigned ticks = 0;

    walking_unit() = default;
    walking_unit(float radius, float foot_distance, ams::vec2f position) : radius(radius), foot_distance(foot_distance), left_foot_position(position - ams::vec2f{foot_distance, 0}), right_foot_position(position + ams::vec2f{foot_distance, 0}), target_position(position) { }

    ams::vec2f get_position() const { return 0.5f * (left_foot_position + right_foot_position); }
};

using walking_units = std::vector<walking_unit>;

bool has_reached_targed(walking_unit& u)
{
    return length(u.target_position - u.get_position()) < 0.5;
}

walking_unit_state other_leg(walking_unit_state state)
{
    return (state == walking_unit_state::MOVING_RIGHT_LEG) ? walking_unit_state::MOVING_LEFT_LEG : walking_unit_state::MOVING_RIGHT_LEG;
}

ams::vec2f get_foot_target_position(walking_unit& u, float sign)
{
    auto direction = normalize(u.target_position - u.get_position());
    auto direction_normal = ams::cross_product(direction);
    return u.target_position - sign * u.foot_distance * direction_normal;
}

ams::vec2f get_left_foot_target_position(walking_unit& u)
{
    return get_foot_target_position(u, -1);
}

ams::vec2f get_right_foot_target_position(walking_unit& u)
{
    return get_foot_target_position(u, 1);
}

void update_unit(walking_unit& u)
{
    if (has_reached_targed(u))
    {
        u.state = walking_unit_state::STANDING;
        return;
    }

    if (u.state == walking_unit_state::STANDING)
    {
        u.state = walking_unit_state::MOVING_RIGHT_LEG;
        u.ticks = 20;
    }

    if (u.ticks == 0)
    {
        u.ticks = 40;
        u.state = other_leg(u.state);
    }

    u.ticks--;

    if (u.state == walking_unit_state::MOVING_LEFT_LEG)
    {
        ams::vec2f direction = get_left_foot_target_position(u) - u.left_foot_position;
        float distance = length(direction);
        u.left_foot_position += direction * std::min(1.0f, u.speed / distance);
    }
    else
    {
        ams::vec2f direction = get_right_foot_target_position(u) - u.right_foot_position;
        float distance = length(direction);
        u.right_foot_position += direction * std::min(1.0f, u.speed / distance);
    }
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
        shape.setOutlineColor(u.state == walking_unit_state::MOVING_LEFT_LEG ? SELECTED_COLOR : COLOR);
        draw_circle(shape, u.left_foot_position, foot_radius, window);
        shape.setOutlineColor(u.state == walking_unit_state::MOVING_RIGHT_LEG ? SELECTED_COLOR : COLOR);
        draw_circle(shape, u.right_foot_position, foot_radius, window);
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
