#include <imgui-SFML.h>

sf::Vector2f operator+(const sf::Vector2f& vec, int scal);

bool operator<(const sf::Vector2u& vec1, const sf::Vector2u& vec2);

bool operator<(const sf::Vector2i& vec1, const sf::Vector2i& vec2);

bool operator>(const sf::Vector2i& vec1, const sf::Vector2i& vec2);

bool operator<(const sf::Vector2i& vec1, const sf::Vector2f& vec2);

bool operator>(const sf::Vector2i& vec1, const sf::Vector2f& vec2);
