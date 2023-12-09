#include <MagicBall.hpp>
#include <Util.hpp>
#include <Character.hpp>
#include <GameState.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

#include <cassert>


MagicBall::MagicBall(Context* cntx,
					 sf::Vector2f movement,
					 Character* character,
					 MagicType type)
	: Body(cntx, BodyType::None)
	, mMovement(movement)
	, mElapsedTime(sf::Time::Zero)
	, mDelay(sf::seconds(0.7))
	, mCharacter(character)
	, mMagicType(type) {
	Body::setPosition(mCharacter->position());
	Body::setSize(sf::Vector2f(8, 8));
	mSprite.setTexture(*context()->textures->resource(TextureID::Terrain));
	if (mMagicType == MagicType::First) {
		mSprite.setTextureRect(sf::IntRect(0, 32*13, 32, 32));
	} else if (mMagicType == MagicType::Second) {
		mSprite.setTextureRect(sf::IntRect(0, 32*14, 32, 32));
	} else if (mMagicType == MagicType::Third) {
		mSprite.setTextureRect(sf::IntRect(0, 32*15, 32, 32));
	} else {
		assert(false);
	}
	centerOrigin(&mSprite);
}

void MagicBall::setDelay(sf::Time delay) {
	mDelay = delay;
}

void MagicBall::handleSignal(const Signal& signal) {
	if (mElapsedTime > mDelay) {
		Body::handleSignal(signal);
	}
}

void MagicBall::handleEvent(const sf::Event& event) {
	if (mElapsedTime > mDelay) {
		Body::handleEvent(event);
	}
}

void MagicBall::update(sf::Time dt) {
	mElapsedTime += dt;

	if (mElapsedTime > mDelay) {
		Body::update(dt);
		Body::setVelocity(mMovement*dt.asSeconds());
		mSprite.setPosition(Body::position() + Body::size()/2.f);
		mSprite.rotate(2000*dt.asSeconds());

		if (Body* body = bodyThatIAlmostCollideWith(mCharacter->ID()); body) {
			Character* character = dynamic_cast<Character*>(body);
			if (character) {
				int damage = 0;
				if (mMagicType == MagicType::First) {
					damage = 2;
				} else if (mMagicType == MagicType::Second) {
					damage = 5;
				} else if (mMagicType == MagicType::Third) {
					damage = 10;
				} else {
					assert(false);
				}
				character->health -= damage;
			}
			markForRemoval();
		}
		if (Body::haveJustCollided()) {  // Collided with terrain.
			markForRemoval();
		}
	}
	else {
		setPosition(mCharacter->position());
	}
}

void MagicBall::draw(sf::Time dt) {
	if (mElapsedTime > mDelay) {
		Body::draw(dt);
		context()->renderTarget->draw(mSprite);
	}
}
