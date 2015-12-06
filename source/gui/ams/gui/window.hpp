#pragma once
#include <memory>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace ams
{
namespace gui
{

std::unique_ptr<sf::RenderWindow> create_window();

template <typename unit_container>
void draw_units(const unit_container& group, sf::RenderWindow& window)
{
    sf::CircleShape shape(1, 16);
    shape.setFillColor(sf::Color(0, 0, 0, 0));
    float thickness = 2;
    shape.setOutlineThickness(thickness);
    shape.setOutlineColor(sf::Color(150, 250, 100));

    for (auto& u : group)
    {
        shape.setRadius(u.radius - thickness);
        shape.setPosition(u.position[0] - u.radius, u.position[1] - u.radius);
        window.draw(shape);
    }
}

void main_loop(sf::RenderWindow& window, std::function<void(float, float)> on_click, std::function<void()> on_update, std::function<void()> on_render);

}
}
