#include "gameBoard.hpp"
#include "cstdlib"
#include "imgui.h"
#include <algorithm>
#include <ctime>
#include <functional>
#include <memory>
#include <string>
#include <unistd.h>

GameBoard::GameBoard(const sf::Vector2f& location, const sf::Vector2f& size):
    location(location),
    size(size),
    tiles(makeTileMap<10>(*this)),
    hiddenTiles({}),
    generated(false),
    flagsLeft(10),
    playState(PlayState::playing)
{
    for (auto& row : tiles)
    {
        for (std::unique_ptr<MineTile>& tile : row)
        {
            tile->setOnPressed([this](int row, int col) { tilePressed(row, col); });
            tile->setFlagPlaced([this]() { flagPlaced(); });
            tile->setFlagRemoved([this]() { flagRemoved(); });
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

void GameBoard::setRestart(std::function<void()> restart)
{
    this->restart = restart;
}

void GameBoard::ready()
{
    for (auto& row : tiles)
    {
        for (std::unique_ptr<MineTile>& tile : row)
        {
            tile->setOnPressed([this](int row, int col) { tilePressed(row, col); });
            tile->setFlagPlaced([this]() { flagPlaced(); });
            tile->setFlagRemoved([this]() { flagRemoved(); });
        }
    }
}

void GameBoard::update()
{
    for(uint row = 0; row < hiddenTiles.size(); row++)
    {
        for(uint col = 0; col < hiddenTiles[row].size(); col++)
        {
            if(hiddenTiles[row][col] != 9 && tiles[row][col])
            {
                return;
            }
        }
    }

    for(auto& row: tiles)
    {
        for(std::unique_ptr<MineTile>& tile: row)
        {
            if(tile)
            {
                tile->setDisabled(true);
            }
        }
    }

    playState = PlayState::won;
}

void GameBoard::render(sf::RenderWindow& window)
{
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(window.getSize().x, 100), ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("##hidden_id", nullptr, flags);

    ImGui::SetWindowFontScale(2.25);

    ImGui::Dummy(ImVec2(0, 10));

    float windowWidth = ImGui::GetWindowWidth();
    std::string buf = "Flags left to place:";
    float textWidth = ImGui::CalcTextSize(buf.c_str()).x;
    ImGui::SetCursorPosX((windowWidth - textWidth) / 2);
    ImGui::Text("%s", buf.c_str());

    buf = std::to_string(flagsLeft);
    textWidth = ImGui::CalcTextSize(buf.c_str()).x;
    ImGui::SetCursorPosX((windowWidth - textWidth) / 2);
    ImGui::Text("%s", buf.c_str());
    ImGui::End();

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

    for (uint row = 0; row < hiddenTiles.size(); row++)
    {
        for (uint col = 0; col < hiddenTiles[row].size(); col++)
        {
            int tile = hiddenTiles[row][col];

            sf::Vector2f tilePos(inRect.getPosition() +
                                 sf::Vector2f({(inRect.getSize().x / 10) * row, (inRect.getSize().y / 10) * col}));

            if (tile > 8)
            {
                sf::CircleShape outMine((inRect.getSize().y / 10 - 10) / 2);
                outMine.setPosition(tilePos + 5);
                outMine.setFillColor(sf::Color(90, 90, 90));

                sf::CircleShape inMine((inRect.getSize().y / 10 - 30) / 2);
                inMine.setPosition(tilePos + 15);
                inMine.setFillColor(sf::Color::Red);

                window.draw(outMine);
                window.draw(inMine);
            }
            else if (tile > 0)
            {
                sf::Font font(std::filesystem::path("fonts/mine-sweeper.ttf"));
                sf::Text num(font, std::to_string(tile), (inRect.getSize().x / 10) / 2);
                sf::FloatRect bounds = num.getLocalBounds();
                num.setOrigin(bounds.position + bounds.size / 2.f);
                num.setPosition(tilePos + inRect.getSize() / 10.f / 2.f);

                window.draw(num);
            }
        }
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

    if(playState != PlayState::playing)
    {
        sf::RectangleShape darkenRect({(float)window.getSize().x, (float)window.getSize().y});
        darkenRect.setPosition({0, 0});
        darkenRect.setFillColor(sf::Color(0, 0, 0, 100));

        window.draw(darkenRect);

        ImVec2 winSize(100, 100);
        ImGui::SetNextWindowSize(winSize, ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2((window.getSize().x / 2.f) - (winSize.x / 2), (window.getSize().y / 2.f) - (winSize.y / 2)), ImGuiCond_Once);
        std::string title;
        if(PlayState::won == playState)
        {
            title = "You won!";
        }
        else 
        {
            title = "You lost.";
        }

        ImGui::Begin(title.c_str());
        float windowWidth = ImGui::GetWindowWidth();
        float buttonWidth = 120;
        ImGui::SetCursorPosX((windowWidth - buttonWidth) / 2);
        if (ImGui::Button("Restart", ImVec2(buttonWidth, 0)))
        {
            restart();
        }
        ImGui::SetCursorPosX((windowWidth - buttonWidth) / 2);
        if(ImGui::Button("Quit", ImVec2(buttonWidth, 0)))
        {
            window.close();
        }
        ImGui::End();
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

            while ((bombLoc >= sf::Vector2i{row - 1, col - 1} && bombLoc <= sf::Vector2i{row + 1, col + 1}) ||
                   hiddenTiles[bombLoc.x][bombLoc.y] == 9)
            {
                bombLoc = {rand() % 10, rand() % 10};
            }

            hiddenTiles[bombLoc.x][bombLoc.y] = 9;

            for (int inRow = std::max(bombLoc.x - 1, 0); inRow <= std::min(bombLoc.x + 1, (int)hiddenTiles.size() - 1);
                 inRow++)
            {
                for (int inCol = std::max(bombLoc.y - 1, 0);
                     inCol <= std::min(bombLoc.y + 1, (int)hiddenTiles[inRow].size() - 1); inCol++)
                {
                    if ((inRow == bombLoc.x && inCol == bombLoc.y) || hiddenTiles[inRow][inCol] == 9)
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
        for (int inRow = std::max(row - 1, 0); inRow <= std::min<int>(row + 1, tiles.size() - 1); inRow++)
        {
            for (int inCol = std::max(col - 1, 0); inCol <= std::min<int>(col + 1, tiles[row].size() - 1); inCol++)
            {
                if (tiles[inRow][inCol])
                {
                    tilePressed(inRow, inCol);
                }
            }
        }
    }
    else if(9 == hiddenTiles[row][col])
    {
        for(auto& row: tiles)
        {
            for(std::unique_ptr<MineTile>& tile: row)
            {
                if(tile)
                {
                    tile->setDisabled(true);
                }
            }
        }

        playState = PlayState::lost;
    }
}

void GameBoard::flagPlaced()
{
    flagsLeft--;
}

void GameBoard::flagRemoved()
{
    flagsLeft++;
}
