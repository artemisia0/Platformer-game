#pragma once

#include <AppElement.hpp>
#include <BodyType.hpp>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <deque>


class Body : public AppElement {
public:
	Body(Context*, BodyType type);
	virtual ~Body() = default;
	
	APP_ELEMENT;

	void setVelocity(sf::Vector2f);
	void setPosition(sf::Vector2f);
	void setSize(sf::Vector2f);

	bool collides(Body&);
	bool almostCollides(Body&);
	sf::Vector2f velocity();
	sf::Vector2f position();
	sf::Vector2f size();
	sf::FloatRect bounds();
	bool equals(Body&);
	bool standingOnGround();
	void markForRemoval();
	bool markedForRemoval();
	bool haveJustCollided();
	BodyType type();
	Body* bodyThatIAlmostCollideWith(int badID);


private:
	void moveOnXAxis(sf::Time);
	void moveOnYAxis(sf::Time);
	bool collides();  // Collides other body or terrain.


private:
	std::deque<sf::Vector2f> mVelocityQueue;
	sf::Vector2f mPosition;
	sf::Vector2f mSize;
	bool mMarkedForRemoval;
	bool mHaveJustCollided;
	BodyType mType;
};
