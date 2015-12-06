#include "window.hpp"
#include <iostream>

namespace units
{
namespace gui
{

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

std::unique_ptr<sf::RenderWindow> create_window()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;
    auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "units", sf::Style::Default, settings);
    window->setVerticalSyncEnabled(true);
    return window;
}

template <typename F, typename EH>
void main_loop(sf::RenderWindow& window, EH handler, F step)
{
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            handler(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        step();

        window.display();
    }
}

void handle_event(const sf::Event& event, sf::RenderWindow& window, view_controller& main_view, std::function<void(float, float)> on_click)
{
    if (event.type == sf::Event::MouseWheelScrolled)
    {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
            main_view.pan_y(event.mouseWheelScroll.delta * 5);
        else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
            main_view.pan_x(event.mouseWheelScroll.delta * 5);
    }
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f world_position = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        on_click(world_position.x, world_position.y);
    }
}

void main_loop(sf::RenderWindow& window, std::function<void(float, float)> on_click, std::function<void()> on_update, std::function<void()> on_render)
{
    view_controller main_view;

    sf::Clock frameClock;
    main_loop(window, [&](const sf::Event& event)
    {
        handle_event(event, window, main_view, on_click);
    },
    [&]
    {
        std::cout << "frame time: " << frameClock.restart().asMicroseconds() << " us" << std::endl;
        sf::Clock stepClock;
        on_update();
        std::cout << "update: " << stepClock.restart().asMicroseconds() << " us" << std::endl;

        window.setView(main_view.get_view(window.getSize()));
        on_render();

        std::cout << "render: " << stepClock.restart().asMicroseconds() << " us" << std::endl;
    });
}

}
}
