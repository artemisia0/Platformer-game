#pragma once

#include <Body.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>


class Character;

enum class MagicType {
	First,
	Second,
	Third,
};

class MagicBall : public Body {
public:
	MagicBall(Context*,
			  sf::Vector2f movement,
			  Character*,
			  MagicType type=MagicType::First);

	APP_ELEMENT;

	void setDelay(sf::Time);


private:
	sf::Vector2f mMovement;
	sf::Sprite mSprite;
	sf::Time mElapsedTime;
	sf::Time mDelay;
	Character* mCharacter;
	MagicType mMagicType;
};
