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
    bool disabled;

    std::function<void(int, int)> onPressed;

    std::function<void()> flagPlaced;
    std::function<void()> flagRemoved;

  public:
    MineTile();

    MineTile(const sf::Vector2f& location, const sf::Vector2f& size, const sf::Vector2i& arrLoc);

    const sf::Vector2f getPosition() const;

    void setDisabled(bool disabled);

    void setOnPressed(std::function<void(int, int)> onPressed);

    void setFlagPlaced(std::function<void()> flagPlaced);

    void setFlagRemoved(std::function<void()> flagRemoved);

    void render(sf::RenderWindow& window);

    void resize(const sf::Vector2f& boardLoc, const sf::Vector2f& boardSize);

    void handleInput(const sf::Event& event);

  private:
    void drawFlag(sf::RenderWindow& window);
};
