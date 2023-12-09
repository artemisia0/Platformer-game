#include <Lizard.hpp>
#include <Signal.hpp>
#include <Player.hpp>
#include <Util.hpp>

#include <cstdlib>


Lizard::Lizard(Context* cntx)
	: Character(cntx, BodyType::NPC)
	, mBehaviourState(BehaviourState::Idle)
	, mMovementState(MovementState::None)
	, mIsJumping(false) {
	mAnimation = defaultAnimation(TextureID::Lizard);
	health = 10;
}

void Lizard::handleSignal(const Signal& signal) {
	Character::handleSignal(signal);
}

void Lizard::handleEvent(const sf::Event& event) {
	Character::handleEvent(event);
}

void Lizard::update(sf::Time dt) {
	if (mBehaviourState == BehaviourState::Idle) {
		idleBehaviour(dt);
	} else if (mBehaviourState == BehaviourState::Targeting) {
		targetingBehaviour(dt);
	}
	move(dt);
	Character::update(dt);
}

void Lizard::idleBehaviour(sf::Time dt) {
	if (rand()%800 == 0) {
		mMovementState = MovementState::Right;
	} else if (rand()%800 == 0) {
		mMovementState = MovementState::Left;
	} else if (rand()%400 == 0) {
		mMovementState = MovementState::None;
	}
	if (rand()%600 == 0) {
		mIsJumping = true;
	} else if (rand()%300 == 0) {
		mIsJumping = false;
	}
	// Checking if lizard can figure out where player is.
	if (distance(context()->player->position(), position()) < 300) {
		mBehaviourState = BehaviourState::Targeting;
	}
}

void Lizard::move(sf::Time dt) {
	auto signalData = SignalData::None;
	if (mMovementState == MovementState::Right) {
		signalData = SignalData::WalkRight;
	} else if (mMovementState == MovementState::Left) {
		signalData = SignalData::WalkLeft;
	}
	context()->signalQueue->push_back(
		Signal{SignalID::CharacterAction,
				signalData,
				mID});
	if (mIsJumping) {
		context()->signalQueue->push_back(
			Signal{SignalID::CharacterAction,
				SignalData::Jump,
				mID});
	}
}

void Lizard::targetingBehaviour(sf::Time dt) {
	const auto playerPos = context()->player->position();
	const auto pos = position();
	if (playerPos.x < pos.x) {
		mMovementState = MovementState::Left;
	} else if (playerPos.x > pos.x) {
		mMovementState = MovementState::Right;
	}
	if ((playerPos.y < pos.y and rand()%200 == 0)
		or rand()%1000 == 0) {
		mIsJumping = true;
	}
	if (rand()%500 == 0) {
		mIsJumping = false;
	}
	if (distance(playerPos, position()) < 40) {
		context()->signalQueue->push_back(
			Signal{SignalID::CharacterAction,
				(mLastDirection ? SignalData::AttackByKnifeRight
				 : SignalData::AttackByKnifeLeft),
					mID});
	}
	if (std::abs(playerPos.y-pos.y) < 100 and rand()%50 == 0) {
		context()->signalQueue->push_back(
			Signal{SignalID::CharacterAction,
				(mLastDirection ? SignalData::AttackByMagicRight
				 : SignalData::AttackByMagicLeft),
					mID});
	}
	if (distance(playerPos, position()) > 600) {
		mBehaviourState = BehaviourState::Idle;
	}
}

void Lizard::draw(sf::Time dt) {
	Character::draw(dt);
}
