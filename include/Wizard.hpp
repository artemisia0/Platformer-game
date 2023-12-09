#pragma once

#include <Character.hpp>
#include <BehaviourState.hpp>
#include <MovementState.hpp>


class Wizard : public Character {
public:
	Wizard(Context*);
	virtual ~Wizard() = default;

	APP_ELEMENT;


private:
	void idleBehaviour(sf::Time);
	void targetingBehaviour(sf::Time);
	void move(sf::Time);
	

private:
	BehaviourState mBehaviourState;
	MovementState mMovementState;
	bool mIsJumping;
};
