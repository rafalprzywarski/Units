#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <units/unit.hpp>
#include <memory>

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
        u.position[0] += std::rand() % 5 - 2;
        u.position[1] += std::rand() % 5 - 2;
        if (collides(u, group))
            u.position = old_position;
    }
}

void draw_units(const units::units& group, sf::RenderWindow& window)
{
    sf::CircleShape shape(1, 16);
    shape.setFillColor(sf::Color(0, 0, 0, 0));
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color(150, 250, 100));

    for (auto& u : group)
    {
        auto r = u.radius - 1;
        auto r2 = 0.5f * r;
        shape.setRadius(r);
        shape.setPosition(u.position[0] - r2, u.position[1] - r2);
        window.draw(shape);
    }
}

std::unique_ptr<sf::RenderWindow> create_window()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;
    auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "units", sf::Style::Default, settings);
    window->setVerticalSyncEnabled(true);
    return window;
}

class view_controller
{
public:
    sf::View get_view(sf::Vector2u window_size)
    {
        return {pan, sf::Vector2f(window_size)};
    }
    void pan_x(float d)
    {
        pan.x -= d;
    }
    void pan_y(float d)
    {
        pan.y -= d;
    }
private:
    sf::Vector2f pan{0, 0};
};

template <typename F>
void main_loop(sf::RenderWindow& window, F step)
{
    view_controller main_view;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                    main_view.pan_y(event.mouseWheelScroll.delta * 5);
                else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
                    main_view.pan_x(event.mouseWheelScroll.delta * 5);
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.setView(main_view.get_view(window.getSize()));

        step();

        window.display();
    }
}

int main()
{
    auto window = create_window();
    auto group = create_units_grid({5, 5}, {10, 10}, 5, 7);

    main_loop(*window, [&]
    {
        update_units(group);
        draw_units(group, *window);
    });

}
