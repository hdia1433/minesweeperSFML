#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include "gameBoard.hpp"

class Controller
{
  private:
    sf::RenderWindow window;
    sf::Clock deltaClock;

    GameBoard gameBoard;

  public:
    Controller();

    void start();

    void restartGameBoard();
};
