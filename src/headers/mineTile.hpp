#include <functional>
#include <imgui-SFML.h>
#include <imgui.h>

enum class TileState
{
    none,
    flagged,
    questioned
};

class MineTile
{
  private:
    sf::Vector2f location;
    sf::Vector2f size;
    sf::Vector2i arrLoc;
    TileState state;

    std::function<void(int, int)> onPressed;

  public:
    MineTile();

    MineTile(const sf::Vector2f& location, const sf::Vector2f& size, const sf::Vector2i& arrLoc);

    void setOnPressed(std::function<void(int, int)> onPressed);

    void render(sf::RenderWindow& window);

    void resize(const sf::Vector2f& boardLoc, const sf::Vector2f& boardSize);

    void handleInput(const sf::Event& event);

  private:
    void drawFlag(sf::RenderWindow& window);
};
