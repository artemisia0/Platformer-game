#include <Character.hpp>
#include <Signal.hpp>
#include <GameState.hpp>
#include <MagicBall.hpp>
#include <Util.hpp>
#include <SoundPlayer.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>

#include <cassert>
#include <cmath>


static int globalID = 1;  // Useful when calculating an unique ID.

std::unique_ptr<Animation> Character::defaultAnimation(TextureID textureID) {
	auto animation = std::make_unique<Animation>(context());
	// Settings a player animation.
	// AnimationItem is a set of frames.
	// Animation is a set of AnimationItem's.

	AnimationItem walkLeft(context());
	sf::Sprite sprite(*context()->textures->resource(textureID));
	for (int i = 1; i != 9; ++i) {
		sprite.setTextureRect(sf::IntRect(64*i, 64*9, 64, 64));
		walkLeft.push_back(sprite);
	}

	AnimationItem walkRight(context());
	for (int i = 1; i != 9; ++i) {
		sprite.setTextureRect(sf::IntRect(64*i, 64*11, 64, 64));
		walkRight.push_back(sprite);
	}

	AnimationItem attackByKnifeLeft(context());
	for (int i = 0; i != 6; ++i) {
		sprite.setTextureRect(sf::IntRect(64*i, 64*13, 64, 64));
		attackByKnifeLeft.push_back(sprite);
	}

	AnimationItem attackByKnifeRight(context());
	for (int i = 0; i != 6; ++i) {
		sprite.setTextureRect(sf::IntRect(64*i, 64*15, 64, 64));
		attackByKnifeRight.push_back(sprite);
	}

	AnimationItem attackByMagicLeft(context());
	for (int i = 0; i != 13; ++i) {
		sprite.setTextureRect(sf::IntRect(64*i, 64*17, 64, 64));
		attackByMagicLeft.push_back(sprite);
	}

	AnimationItem attackByMagicRight(context());
	for (int i = 0; i != 13; ++i) {
		sprite.setTextureRect(sf::IntRect(64*i, 64*19, 64, 64));
		attackByMagicRight.push_back(sprite);
	}

	animation->push_back(walkLeft);
	animation->push_back(walkRight);
	animation->push_back(attackByKnifeLeft);
	animation->push_back(attackByKnifeRight);
	animation->push_back(attackByMagicLeft);
	animation->push_back(attackByMagicRight);
	return animation;
}

Character::Character(Context* cntx, BodyType type)
	: Body(cntx, type)
	, mLastDirection(false)
	, mID(++globalID)
	, mMagicType(MagicType::First) {
	setSize(sf::Vector2f(24, 48));
	// Every character has player's animation by default.
	// It shall be overriden.
	mAnimation = defaultAnimation(TextureID::Player);
}

void Character::setMagicType(MagicType magicType) {
	mMagicType = magicType;
}

void Character::handleSignal(const Signal& signal) {
	Body::handleSignal(signal);
	if (mAnimation) {
		mAnimation->handleSignal(signal);
	}

	if (signal.extra != mID) {
		return;
	}
	if (signal.ID == SignalID::CharacterAction) {
		if (signal.data == SignalData::Jump
			and not mAnimation->isPlaying()) {
			jumpAction();
		} else if (signal.data == SignalData::Down
				   and not mAnimation->isPlaying()) {
			downAction();
		} else if (signal.data == SignalData::WalkLeft
				   and not mAnimation->isPlaying()) {
			walkLeftAction();
		} else if (signal.data == SignalData::WalkRight
				   and not mAnimation->isPlaying()) {
			walkRightAction();
		} else if (signal.data == SignalData::AttackByKnifeLeft
				   and not mAnimation->isPlaying()) {
			attackByKnifeLeftAction();
		} else if (signal.data == SignalData::AttackByKnifeRight
				   and not mAnimation->isPlaying()) {
			attackByKnifeRightAction();
		} else if (signal.data == SignalData::AttackByMagicLeft
				   and not mAnimation->isPlaying()) {
			attackByMagicLeftAction();
		} else if (signal.data == SignalData::AttackByMagicRight
				   and not mAnimation->isPlaying()) {
			attackByMagicRightAction();
		}
	}
}

void Character::handleEvent(const sf::Event& event) {
	Body::handleEvent(event);
	if (mAnimation) {
		mAnimation->handleEvent(event);
	}
}

void Character::update(sf::Time dt) {
	Body::update(dt);
	if (mAnimation) {
		mAnimation->update(dt*std::abs(velocity().x/50.f));
	}
	if (health <= 0) {
		markForRemoval();
	}
	processDamageQueryQueue(dt);
}

void Character::draw(sf::Time dt) {
	Body::draw(dt);
	if (mAnimation) {
		mAnimation->draw(dt);
		
		auto sprite = mAnimation->sprite();
		auto pos = position();
		pos.x -= 20;
		pos.y -= 15;
		sprite.setPosition(pos);
		context()->renderTarget->draw(sprite);

		sf::Text healthText(
			std::to_string(health),
			*context()->fonts->resource(FontID::Menu),
			14);
		healthText.setPosition(sf::Vector2f(position().x+12,
											position().y-12));
		healthText.setFillColor(sf::Color(200, 200, 200));
		centerOrigin(&healthText);
		context()->renderTarget->draw(healthText);
	}
}

void Character::jumpAction() {
	if (not standingOnGround()) {
		return;
	}
	setVelocity(sf::Vector2f(0, -500));
	if (type() == BodyType::Player) {
		context()->soundPlayer->play(SoundID::Jump);
	}
}

void Character::downAction() {
}

void Character::walkLeftAction() {
	if (mAnimation) {
		mAnimation->setActiveItemIndex(0);
		setVelocity(sf::Vector2f(-6, 0));
		mLastDirection = false;
	}
}

void Character::walkRightAction() {
	if (mAnimation) {
		mAnimation->setActiveItemIndex(1);
		setVelocity(sf::Vector2f(6, 0));
		mLastDirection = true;
	}
}

void Character::attackByKnifeLeftAction() {
	if (mAnimation) {
		mAnimation->playItemWithIndex(2);
	}
	auto oldSize = size();
	auto oldPosition = position();

	setSize(sf::Vector2f(48, size().y));
	setPosition(sf::Vector2f(position().x-32, position().y));
	if (Body* body = bodyThatIAlmostCollideWith(mID); body) {
		Character* character = dynamic_cast<Character*>(body);
		if (character) {
			mDamageQueryQueue.push_back(
				DamageQuery{character, knifeDamage, sf::seconds(0.4)});
		}
	}

	setSize(oldSize);
	setPosition(oldPosition);

	if (true) {
		context()->soundPlayer->play(SoundID::AttackByKnife);
	}
}

void Character::attackByKnifeRightAction() {
	if (mAnimation) {
		mAnimation->playItemWithIndex(3);
	}
	auto oldSize = size();
	auto oldPosition = position();

	setSize(sf::Vector2f(48, size().y));
	setPosition(sf::Vector2f(position().x+32, position().y));
	if (Body* body = bodyThatIAlmostCollideWith(mID); body) {
		Character* character = dynamic_cast<Character*>(body);
		if (character) {
			mDamageQueryQueue.push_back(
				DamageQuery{character, knifeDamage, sf::seconds(0.4)});
		}
	}

	setSize(oldSize);
	setPosition(oldPosition);

	if (true) {
		context()->soundPlayer->play(SoundID::AttackByKnife);
	}
}

void Character::attackByMagicLeftAction() {
	if (mAnimation) {
		mAnimation->playItemWithIndex(4);
	}
	assert(context() and context()->gameState
		   and context()->gameState->bodies());
	auto magicBall = std::make_unique<MagicBall>(
			context(),
			sf::Vector2f(-1000.f, -600.f),
			this,
			mMagicType);
	context()->gameState->bodies()->push_back(std::move(magicBall));

	if (true) {
		context()->soundPlayer->play(SoundID::AttackByMagic);
	}
}

void Character::attackByMagicRightAction() {
	if (mAnimation) {
		mAnimation->playItemWithIndex(5);
	}
	assert(context() and context()->gameState
		   and context()->gameState->bodies());
	context()->gameState->bodies()->push_back(
		std::make_unique<MagicBall>(
			context(),
			sf::Vector2f(1000.f, -600.f),
			this,
			mMagicType));

	if (true) {
		context()->soundPlayer->play(SoundID::AttackByMagic);
	}
}

int Character::ID() {
	return mID;
}

void Character::processDamageQueryQueue(sf::Time dt) {
	std::vector<std::vector<DamageQuery>::iterator> queriesToRemove;
	for (int i = 0, lim = int(mDamageQueryQueue.size()); i != lim; ++i) {
		auto& query = mDamageQueryQueue.at(i);
		query.elapsedTime += dt;
		if (query.elapsedTime > query.delay) {
			query.character->health -= query.damage;
			queriesToRemove.push_back(mDamageQueryQueue.begin()+i);
		}
	}
	for (auto iter : queriesToRemove) {
		mDamageQueryQueue.erase(iter);
	}
}
