#include "mineTile.hpp"
#include "sfVectorOperators.hpp"
#include <functional>

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

const sf::Vector2f MineTile::getPosition() const
{
    return location;
}

void MineTile::setOnPressed(std::function<void(int, int)> onPressed)
{
    this->onPressed = onPressed;
}

void MineTile::setFlagPlaced(std::function<void()> flagPlaced)
{
    this->flagPlaced = flagPlaced;
}

void MineTile::setFlagRemoved(std::function<void()> flagRemoved)
{
    this->flagRemoved = flagRemoved;
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

    if (TileState::flagged == state)
    {
        drawFlag(window);
    }
    else if (TileState::questioned == state)
    {
        sf::Font font(std::filesystem::path("fonts/mine-sweeper.ttf"));
        sf::Text question(font, "?", size.y / 2);
        sf::FloatRect bounds = question.getLocalBounds();

        question.setOrigin(bounds.position + bounds.size / 2.f);
        question.setPosition(location + size / 2.f);

        window.draw(question);
    }
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
                        flagPlaced();
                        break;
                    case TileState::flagged:
                        state = TileState::questioned;
                        flagRemoved();
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
    sf::RectangleShape poleRect({2, size.y * .5f});
    poleRect.setPosition({location.x + (size.x * .5f) - 1, location.y + (size.y / 4)});
    poleRect.setOutlineColor(sf::Color(0, 0, 0));
    poleRect.setFillColor(sf::Color(0, 0, 0));

    sf::RectangleShape baseRect({size.x / 3, 2});
    baseRect.setPosition({location.x + (size.x / 3), location.y + (size.y * (3.f / 4))});
    baseRect.setOutlineColor(sf::Color(0, 0, 0));
    baseRect.setFillColor(sf::Color(0, 0, 0));

    sf::ConvexShape flag(3);
    flag.setPoint(0, {location.x + (size.x / 2), location.y + (size.y / 4)});
    flag.setPoint(1, {location.x + size.y * (3.f / 4), location.y + size.y * (3.f / 8)});
    flag.setPoint(2, {location.x + size.x / 2, location.y + size.y / 2});
    flag.setFillColor(sf::Color::Red);
    flag.setOutlineColor(sf::Color::Red);

    window.draw(poleRect);
    window.draw(baseRect);
    window.draw(flag);
}
