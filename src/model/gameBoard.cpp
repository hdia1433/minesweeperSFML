#include "gameBoard.hpp"
#include <memory>

GameBoard::GameBoard(const sf::Vector2f& location, const sf::Vector2f& size):
    location(location),
    size(size),
    tiles(makeTileMap<10>(*this)),
    hiddenTiles({}),
    generated(false)
{
    for (auto& row : tiles)
    {
        for (std::unique_ptr<MineTile>& tile : row)
        {
            tile->setOnPressed([this](int row, int col) { tilePressed(row, col); });
        }
    }
}

const sf::Vector2f& GameBoard::getLocation() const
{
    return location;
}

const sf::Vector2f& GameBoard::getSize() const
{
    return size;
}

void GameBoard::render(sf::RenderWindow& window)
{
    sf::RectangleShape rect(size);
    rect.setPosition(location);
    rect.setOutlineThickness(2);
    rect.setOutlineColor(sf::Color(51, 52, 70));
    rect.setFillColor(sf::Color(31, 32, 36));

    window.draw(rect);

    for (auto& row : tiles)
    {
        for (std::unique_ptr<MineTile>& tile : row)
        {
            if (tile)
            {
                tile->render(window);
            }
        }
    }
}

void GameBoard::resize(const sf::Vector2u& windowSize)
{
    float gameBoardSize = windowSize.y - 140;

    float xMargin = (windowSize.x - gameBoardSize) / 2;
    float yMargin = xMargin < 20 ? (((float)windowSize.y - (float)windowSize.x - 40 - 150) / 2) + 150 : 120;

    if (xMargin < 20)
    {
        xMargin = 20;
        gameBoardSize = windowSize.x - 40;
    }

    location = {xMargin, yMargin};
    size = {gameBoardSize, gameBoardSize};

    for (auto& row : tiles)
    {
        for (std::unique_ptr<MineTile>& tile : row)
        {
            if (tile)
            {
                tile->resize(location, size);
            }
        }
    }
}

void GameBoard::handleInput(const sf::Event& event)
{
    for (auto& row : tiles)
    {
        for (std::unique_ptr<MineTile>& tile : row)
        {
            if (tile)
            {
                tile->handleInput(event);
            }
        }
    }
}

void GameBoard::tilePressed(int row, int col)
{
    tiles[row][col].reset();
}
