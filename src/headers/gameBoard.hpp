#include <imgui-SFML.h>
#include <imgui.h>
#include <memory>

#include "mineTile.hpp"
#include "sfVectorOperators.hpp"

class GameBoard
{
  private:
    sf::Vector2f location;
    sf::Vector2f size;
    std::array<std::array<std::unique_ptr<MineTile>, 10>, 10> tiles;
    std::array<std::array<int, 10>, 10> hiddenTiles;
    bool generated;
    int flagsLeft;

  public:
    GameBoard(const sf::Vector2f& location, const sf::Vector2f& size);

    const sf::Vector2f& getLocation() const;

    const sf::Vector2f& getSize() const;

    void update();

    void render(sf::RenderWindow& window);

    void resize(const sf::Vector2u& windowSize);

    void handleInput(const sf::Event& event);

    void tilePressed(int row, int col);

    void flagPlaced();

    void flagRemoved();
};

template <int size> std::array<std::array<std::unique_ptr<MineTile>, size>, size> makeTileMap(GameBoard& gameBoard)
{
    std::array<std::array<std::unique_ptr<MineTile>, size>, size> tileMap = {};

    int tileSize = (gameBoard.getSize().x - 20) / size;

    for (uint row = 0; row < tileMap.size(); row++)
    {
        for (uint col = 0; col < tileMap[row].size(); col++)
        {
            tileMap[row][col] = std::make_unique<MineTile>(
                MineTile(gameBoard.getLocation() + 10 + sf::Vector2f(tileSize * (int)row, tileSize * (int)col),
                         {(float)tileSize, (float)tileSize}, {(int)row, (int)col}));
        }
    }

    return tileMap;
}
