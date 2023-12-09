#pragma once

#include <AppElement.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include <vector>


class ParallaxLayer : public AppElement,
					  public sf::Sprite {
public:
	ParallaxLayer(Context*);

	APP_ELEMENT;

	void setVelocityFactor(sf::Vector2f);
	void setVelocity(sf::Vector2f);


private:
	sf::Vector2f mVelocityFactor;
	sf::Vector2f mVelocity;
};


class Parallax : public AppElement,
				 public std::vector<ParallaxLayer> {
public:
	Parallax(Context*);

	APP_ELEMENT;
};
