#include <Pickup.hpp>
#include <Block.hpp>
#include <Util.hpp>
#include <Player.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

#include <cassert>


Pickup::Pickup(Context* cntx,
			   PickupID id,
			   int val,
			   sf::Vector2f pos,
			   std::function<void()> callback)
	: Body(cntx, BodyType::Pickup)
	, mID(id)
	, mGenericEnumValue(val)
	, mCallback(callback) {
	assert(context() and context()->textures);
	
	setPosition(pos + sf::Vector2f(TILE, TILE)/2.f);
	setTexture(*texture(mID));
	setTextureRect(textureRect(mID, val));
	setScale(0.5, 0.5);
	centerOrigin(this);
	setVelocity(sf::Vector2f((rand()%150)*(rand()%3 - 1),
							 -rand()%600));
	Body::setPosition(getPosition());
	Body::setSize(sf::Vector2f(TILE, TILE)/2.f);
}

void Pickup::handleSignal(const Signal& signal) {
	Body::handleSignal(signal);
}

void Pickup::handleEvent(const sf::Event& event) {
	Body::handleEvent(event);
}

void Pickup::update(sf::Time dt) {
	Body::setVelocity(sf::Vector2f(0.f, 1000.f)*dt.asSeconds());
	Body::update(dt);

	setPosition(Body::position());

	if (context()->player->collides(*this)
		and context()->player->inventory()->hasFreeSpaceFor(
			mID, mGenericEnumValue)) {
		markForRemoval();
		mCallback();
	}
}

void Pickup::draw(sf::Time dt) {
	Body::draw(dt);
	context()->renderTarget->draw(*this);
}

PickupID Pickup::ID() {
	return mID;
}

int Pickup::genericEnumValue() {
	return mGenericEnumValue;
}

sf::Texture* Pickup::texture(PickupID ID) {
	TextureID textureID(TextureID::None);
	if (ID == PickupID::Block) {
		textureID = TextureID::Terrain;
	}
	return context()->textures->resource(textureID);
}

sf::IntRect Pickup::textureRect(PickupID ID, int val) {
	if (ID == PickupID::Block) {
		Block block = static_cast<Block>(val);
		assert(block != Block::None);
		if (block == Block::Dirt) {
			return sf::IntRect(0, 0, 32, 32);
		}
		if (block == Block::DirtWithGrass) {
			return sf::IntRect(0, 32*1, 32, 32);
		}
		if (block == Block::Stone) {
			return sf::IntRect(0, 32*2, 32, 32);
		}
		if (block == Block::Gold) {
			return sf::IntRect(0, 32*3, 32, 32);
		}
		if (block == Block::Diamond) {
			return sf::IntRect(0, 32*4, 32, 32);
		}
		if (block == Block::Sand) {
			return sf::IntRect(0, 32*5, 32, 32);
		}
		if (block == Block::SandWithGrass) {
			return sf::IntRect(0, 32*6, 32, 32);
		}
		if (block == Block::Mud) {
			return sf::IntRect(0, 32*7, 32, 32);
		}
		if (block == Block::MudWithSnow) {
			return sf::IntRect(0, 32*8, 32, 32);
		}
		if (block == Block::Torch) {
			return sf::IntRect(0, 32*19, 32, 32);
		}
		if (block == Block::Magic1) {
			return sf::IntRect(0, 32*13, 32, 32);
		}
		if (block == Block::Magic2) {
			return sf::IntRect(0, 32*14, 32, 32);
		}
		if (block == Block::Magic3) {
			return sf::IntRect(0, 32*15, 32, 32);
		}
		if (block == Block::Token) {
			return sf::IntRect(0, 32*18, 32, 32);
		}
		if (block == Block::Workbench) {
			return sf::IntRect(0, 32*16, 32, 32);
		}
		if (block == Block::HealthBox) {
			return sf::IntRect(0, 32*17, 32, 32);
		}
	}
	assert(false);
	return sf::IntRect();
}

void Pickup::setPosition(sf::Vector2f v) {
	sf::Sprite::setPosition(v);
}

void Pickup::setCallback(std::function<void()> callback) {
	mCallback = callback;
}

bool Pickup::equals(Pickup& pickup) {
	return mID == pickup.mID
		and mGenericEnumValue == pickup.mGenericEnumValue;
}
