#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <units/unit.hpp>
#include <memory>

units::units create_units_grid(int width, int height, float radius, float spacing)
{
    units::units group;
    for (int i = 0; i < width; ++i)
        for (int j = 0; j < height; ++j)
            group.push_back(units::unit{
                radius,
                {
                    spacing + radius + i * (radius * 2 + spacing),
                    spacing + radius + j * (radius * 2 + spacing)
                }});
    return group;
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
    auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "units", sf::Style::Close | sf::Style::Titlebar, settings);
    window->setVerticalSyncEnabled(true);
    return window;
}

template <typename F>
void main_loop(sf::RenderWindow& window, F step)
{
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        step();

        window.display();
    }
}

int main()
{
    auto window = create_window();
    auto group = create_units_grid(6, 8, 7, 7);

    main_loop(*window, [&]
    {
        draw_units(group, *window);
    });

}
