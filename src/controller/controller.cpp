#include "controller.hpp"
#include <stdexcept>

#include "sfVectorOperators.hpp"

GameBoard computeGameBoard(const sf::Vector2u& windowSize)
{
    float gameBoardSize = windowSize.y - 140;

    return GameBoard({(windowSize.x - gameBoardSize) / 2, 120}, {gameBoardSize, gameBoardSize});
}

Controller::Controller():
    window(sf::VideoMode({720, 720}), "Minesweeper", sf::Style::Close),
    gameBoard(computeGameBoard(window.getSize()))
{
    window.setFramerateLimit(60);

    gameBoard.setRestart([this](){
            restartGameBoard();
            });
}

void Controller::start()
{
    gameBoard.ready();

    if (!ImGui::SFML::Init(window))
    {
        throw std::runtime_error("Couldn't load window");
    }

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (event->is<sf::Event::Resized>())
            {
                sf::Vector2u windowSize = window.getSize();

                if (windowSize.x < 260 || windowSize.y < 260)
                {
                    window.setSize({std::max<uint>(windowSize.x, 260), std::max<uint>(windowSize.y, 260)});
                }

                auto& resized = *event->getIf<sf::Event::Resized>();

                sf::FloatRect visibleArea({0.f, 0.f}, {(float)resized.size.x, (float)resized.size.y});
                window.setView(sf::View(visibleArea));

                gameBoard.resize(windowSize);
            }
            else
            {
                gameBoard.handleInput(*event);
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        gameBoard.update();

        window.clear();

        gameBoard.render(window);

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    system("printf '\033[2A\033[0J'");
}

void Controller::restartGameBoard()
{
    gameBoard = GameBoard(computeGameBoard(window.getSize()));
    gameBoard.setRestart([this](){
            restartGameBoard();
            });
    gameBoard.ready();
}
