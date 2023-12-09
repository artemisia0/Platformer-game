#pragma once

#include <Character.hpp>
#include <BehaviourState.hpp>
#include <MovementState.hpp>


class Lizard : public Character {
public:
	Lizard(Context*);
	virtual ~Lizard() = default;

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
