#pragma once

#include <SFML/System/Vector2.hpp>


class Point {
public:
	void setPosition(sf::Vector2f);
	sf::Vector2f position();


private:
	sf::Vector2f mPosition;
};
