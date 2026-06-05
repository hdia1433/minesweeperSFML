#include "mineTile.hpp"
#include "sfVectorOperators.hpp"

MineTile::MineTile(): location(0, 0), size(0, 0), arrLoc(0, 0), state(TileState::none)
{
}

MineTile::MineTile(const sf::Vector2f& location, const sf::Vector2f& size, const sf::Vector2i& arrLoc):
    location(location),
    size(size),
    arrLoc(arrLoc),
    state(TileState::none)
{
}

void MineTile::setOnPressed(std::function<void(int, int)> onPressed)
{
    this->onPressed = onPressed;
}

void MineTile::render(sf::RenderWindow& window)
{
    sf::RectangleShape outerRect(size);
    outerRect.setPosition(location);
    outerRect.setOutlineThickness(2);
    outerRect.setOutlineColor(sf::Color(0, 0, 0));
    outerRect.setFillColor(sf::Color(51, 52, 70));

    sf::RectangleShape innerRect(size * .8f);
    innerRect.setPosition(location + ((size * .2f) / 2.f));
    innerRect.setOutlineThickness(2);
    innerRect.setOutlineColor(sf::Color(0, 0, 0));
    innerRect.setFillColor(sf::Color(51, 52, 70));

    sf::ConvexShape bottomTrap(4);
    bottomTrap.setPoint(0, {outerRect.getPosition().x, outerRect.getPosition().y + outerRect.getSize().y});
    bottomTrap.setPoint(1, outerRect.getPosition() + outerRect.getSize());
    bottomTrap.setPoint(2, innerRect.getPosition() + innerRect.getSize());
    bottomTrap.setPoint(3, {innerRect.getPosition().x, innerRect.getPosition().y + innerRect.getSize().y});
    bottomTrap.setOutlineColor(sf::Color(0, 0, 0));
    bottomTrap.setFillColor(sf::Color(31, 32, 36));

    sf::ConvexShape rightTrap(4);
    rightTrap.setPoint(0, {outerRect.getPosition().x + outerRect.getSize().x, outerRect.getPosition().y});
    rightTrap.setPoint(1, outerRect.getPosition() + outerRect.getSize());
    rightTrap.setPoint(2, innerRect.getPosition() + innerRect.getSize());
    rightTrap.setPoint(3, {innerRect.getPosition().x + innerRect.getSize().x, innerRect.getPosition().y});
    rightTrap.setOutlineColor(sf::Color(0, 0, 0));
    rightTrap.setFillColor(sf::Color(31, 32, 36));

    sf::VertexArray line(sf::PrimitiveType::Lines, 2);
    line[0] = sf::Vertex(outerRect.getPosition(), sf::Color::Black);
    line[1] = sf::Vertex(innerRect.getPosition(), sf::Color::Black);

    window.draw(outerRect);
    window.draw(bottomTrap);
    window.draw(rightTrap);
    window.draw(line);
    window.draw(innerRect);
}

void MineTile::resize(const sf::Vector2f& boardLoc, const sf::Vector2f& boardSize)
{
}

void MineTile::handleInput(const sf::Event& event)
{
    if (auto click = event.getIf<sf::Event::MouseButtonReleased>())
    {
        if (click->position > location && click->position < location + size)
        {
            if (TileState::none == state && sf::Mouse::Button::Left == click->button)
            {
                onPressed(arrLoc.x, arrLoc.y);
            }
            else if (sf::Mouse::Button::Right == click->button)
            {
                switch (state)
                {
                    case TileState::none:
                        state = TileState::flagged;
                        break;
                    case TileState::flagged:
                        state = TileState::questioned;
                        break;
                    case TileState::questioned:
                        state = TileState::none;
                        break;
                }
            }
        }
    }
}

void MineTile::drawFlag(sf::RenderWindow& window)
{
}
