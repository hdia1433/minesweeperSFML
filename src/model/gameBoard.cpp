#include "gameBoard.hpp"
#include "cstdlib"
#include <algorithm>
#include <ctime>
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

    srand(time(0));
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

    sf::RectangleShape inRect(size - 20);
    inRect.setPosition(location + 10);
    inRect.setOutlineThickness(2);
    inRect.setOutlineColor(sf::Color::Black);
    inRect.setFillColor(sf::Color::Transparent);

    window.draw(inRect);

    for (uint i = 1; i <= 9; i++)
    {
        sf::RectangleShape vLine({2, inRect.getSize().y});
        vLine.setPosition({inRect.getPosition().x - 1 + (inRect.getSize().x / 10) * i, inRect.getPosition().y});
        vLine.setFillColor(sf::Color::Black);

        window.draw(vLine);

        sf::RectangleShape hLine({inRect.getSize().x, 2});
        hLine.setPosition({inRect.getPosition().x, inRect.getPosition().y - 1 + (inRect.getSize().y / 10) * i});
        hLine.setFillColor(sf::Color::Black);

        window.draw(hLine);
    }

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
    if (!generated)
    {
        generated = true;
        for (uint i = 0; i < 10; i++)
        {
            sf::Vector2i bombLoc({rand() % 10, rand() % 10});

            while ((bombLoc > sf::Vector2i{row - 1, col - 1} && bombLoc < sf::Vector2i{row + 1, col + 1}) ||
                   hiddenTiles[row][col] == 9)
            {
                sf::Vector2i bombLoc({rand() % 10, rand() % 10});
            }

            hiddenTiles[bombLoc.x][bombLoc.y] = 9;

            for (int inRow = std::max(bombLoc.x - 1, 0); inRow <= std::min(bombLoc.x + 1, (int)hiddenTiles.size());
                 inRow++)
            {
                for (int inCol = std::max(bombLoc.y - 1, 0);
                     inCol <= std::min(bombLoc.y + 1, (int)hiddenTiles[inRow].size()); inCol++)
                {
                    if ((inRow == row && inCol == col) || hiddenTiles[inRow][inCol] == 9)
                    {
                        continue;
                    }

                    hiddenTiles[inRow][inCol]++;
                }
            }
        }
    }

    tiles[row][col].reset();

    if (0 == hiddenTiles[row][col])
    {
        for (int inRow = std::max(row - 1, 0); inRow <= std::min<int>(row + 1, tiles.size()); inRow++)
        {
            for (int inCol = std::max(col - 1, 0); inCol <= std::min<int>(col + 1, tiles[row].size()); inCol++)
            {
                if (tiles[inRow][inCol])
                {
                    tilePressed(inRow, inCol);
                }
            }
        }
    }
}
