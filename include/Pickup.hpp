#pragma once

#include <Body.hpp>

#include <SFML/Graphics/Sprite.hpp>

#include <functional>


enum class PickupID {
	None,
	Block,
};

class Pickup : public sf::Sprite, public Body {
public:
	Pickup(Context*,
		   PickupID id,
		   int val,
		   sf::Vector2f pos=sf::Vector2f(),
		   std::function<void()> callback=std::function<void()>());

	APP_ELEMENT;

	void setPosition(sf::Vector2f);
	void setCallback(std::function<void()>);

	PickupID ID();
	int genericEnumValue();

	bool equals(Pickup&);


private:
	sf::Texture* texture(PickupID);
	sf::IntRect textureRect(PickupID, int);


private:
	PickupID mID;
	int mGenericEnumValue;  // It shall be type-casted to some enum.
	std::function<void()> mCallback;
};
