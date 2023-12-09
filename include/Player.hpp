#pragma once

#include <Character.hpp>
#include <Inventory.hpp>


class Player : public Character {
public:
	Player(Context*);

	APP_ELEMENT;

	Inventory* inventory();

	sf::Vector2f previousPosition();
	int coins();
	void setCoins(int);


private:
	void handleKeyboardInput();
	std::unique_ptr<Inventory> defaultInventory();


private:
	std::unique_ptr<Inventory> mInventory;
	sf::Vector2f mPreviousPosition;
	int mCoins;
	sf::Time mTimeSinceLastWalkSound;
	sf::Time mWalkSoundPeriod;
};
