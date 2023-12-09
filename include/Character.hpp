#pragma once

#include <Body.hpp>
#include <Animation.hpp>
#include <Inventory.hpp>
#include <CharacterProperties.hpp>
#include <MagicBall.hpp>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>


class Character;

class DamageQuery {
public:
	Character* character = nullptr;
	int damage = 0;
	sf::Time delay = sf::Time::Zero;
	sf::Time elapsedTime = sf::Time::Zero;
};

class Character : public Body,
				  public CharacterProperties,
				  private sf::NonCopyable {
public:
	Character(Context*, BodyType type);
	virtual ~Character() = default;

	APP_ELEMENT;

	Animation* animation();
	int ID();

	void setMagicType(MagicType);


protected:
	std::unique_ptr<Animation> defaultAnimation(TextureID);

	void jumpAction();
	void downAction();
	void walkLeftAction();
	void walkRightAction();
	void attackByKnifeLeftAction();
	void attackByKnifeRightAction();
	void attackByMagicLeftAction();
	void attackByMagicRightAction();
	void processDamageQueryQueue(sf::Time);


protected:
	std::unique_ptr<Animation> mAnimation;
	bool mLastDirection;  // true is right and false is left.
	int mID;  // Every character has its own unique identifier.
	std::vector<DamageQuery> mDamageQueryQueue;
	MagicType mMagicType;
};
