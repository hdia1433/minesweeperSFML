#include "sfVectorOperators.hpp"

sf::Vector2f operator+(const sf::Vector2f& vec, int scal)
{
    return {vec.x + scal, vec.y + scal};
}

bool operator<(const sf::Vector2u& vec1, const sf::Vector2u& vec2)
{
    return vec1.x < vec2.x && vec1.y < vec2.y;
}

bool operator<(const sf::Vector2i& vec1, const sf::Vector2f& vec2)
{
    return vec1.x < vec2.x && vec1.y < vec2.y;
}

bool operator>(const sf::Vector2i& vec1, const sf::Vector2f& vec2)
{
    return vec1.x > vec2.x && vec1.y > vec2.y;
}
