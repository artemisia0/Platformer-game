#include <Point.hpp>


void Point::setPosition(sf::Vector2f v) {
	mPosition = v;
}

sf::Vector2f Point::position() {
	return mPosition;
}
