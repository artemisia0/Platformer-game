#include <Player.hpp>
#include <Signal.hpp>
#include <Block.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <SoundPlayer.hpp>

#include <cmath>
#include <cassert>


Player::Player(Context* cntx)
	: Character(cntx, BodyType::Player)
	, mPreviousPosition(0.f, 0.f)
	, mCoins(0)
	, mTimeSinceLastWalkSound(sf::Time::Zero)
	, mWalkSoundPeriod(sf::seconds(0.3)) {
	mAnimation = defaultAnimation(TextureID::Player);
	mInventory = defaultInventory();
	setPosition(sf::Vector2f(10000, -6400));
	mPreviousPosition = position();
	setSize(sf::Vector2f(24, 48));

	knifeDamage = 100;
	health = 100;
}

std::unique_ptr<Inventory> Player::defaultInventory() {
	auto inventory = std::make_unique<Inventory>(context());
	for (int i = 0; i != 16; ++i) {
		inventory->push_back(Slot(context()));
	}
	return inventory;
}

Inventory* Player::inventory() {
	return mInventory.get();
}

void Player::handleSignal(const Signal& signal) {
	Character::handleSignal(signal);
	mInventory->handleSignal(signal);
}

void Player::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::W) {
			context()->signalQueue->push_back(
				Signal{SignalID::CharacterAction, SignalData::Jump, mID});
		}
		if (event.key.code == sf::Keyboard::N) {
			context()->signalQueue->push_back(
				Signal{SignalID::CharacterAction,
					(not mLastDirection ? SignalData::AttackByKnifeLeft
					 : SignalData::AttackByKnifeRight), mID});
			}
		if (event.key.code == sf::Keyboard::M) {
			context()->signalQueue->push_back(
				Signal{SignalID::CharacterAction,
					   (not mLastDirection ? SignalData::AttackByMagicLeft
						: SignalData::AttackByMagicRight), mID});
		}
	}

	Character::handleEvent(event);
	mInventory->handleEvent(event);
}

void Player::update(sf::Time dt)  {
	mTimeSinceLastWalkSound += dt;
	const auto velocity = (position()-previousPosition())*1.5f;
	mWalkSoundPeriod =
		std::max(sf::seconds(1.0/std::max(0.01f,
										  float(std::abs(velocity.x)))),
				 sf::seconds(0.16));
	while (mTimeSinceLastWalkSound > mWalkSoundPeriod*1.5f) {
		mTimeSinceLastWalkSound -= mWalkSoundPeriod;
	}

	if (mInventory
		and mInventory->activePickup()
		and mInventory->activePickup()->ID() == PickupID::Block) {
		switch (mInventory->activePickup()->genericEnumValue()) {
		case static_cast<int>(Block::Magic2):
			setMagicType(MagicType::Second);
			break;
		case static_cast<int>(Block::Magic3):
			setMagicType(MagicType::Third);
			break;
		default:
			setMagicType(MagicType::First);
			break;
		};
	}
	
	mPreviousPosition = position();
	handleKeyboardInput();  // Yeah, it should be in 'update' method.
	Character::update(dt);
	mInventory->update(dt);
}

void Player::draw(sf::Time dt) {
	Character::draw(dt);
	mInventory->draw(dt);
}

void Player::handleKeyboardInput() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		context()->signalQueue->push_back(
			Signal{SignalID::CharacterAction, SignalData::WalkLeft, mID});
		if (mTimeSinceLastWalkSound > mWalkSoundPeriod) {
			if (standingOnGround()) {
				context()->soundPlayer->play(SoundID::Walk);
			}
			mTimeSinceLastWalkSound -= mWalkSoundPeriod;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		context()->signalQueue->push_back(
			Signal{SignalID::CharacterAction, SignalData::WalkRight, mID});
		if (mTimeSinceLastWalkSound > mWalkSoundPeriod) {
			if (standingOnGround()) {
				context()->soundPlayer->play(SoundID::Walk);
			}
			mTimeSinceLastWalkSound -= mWalkSoundPeriod;
		}
	}
}

sf::Vector2f Player::previousPosition() {
	return mPreviousPosition;
}

int Player::coins() {
	return mCoins;
}

void Player::setCoins(int coins) {
	mCoins = coins;
}
